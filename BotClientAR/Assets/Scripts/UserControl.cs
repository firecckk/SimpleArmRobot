using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using uPLibrary.Networking.M2Mqtt;
using uPLibrary.Networking.M2Mqtt.Messages;
using M2MqttUnity;


namespace M2MqttUnity.Client
{
    public class UserControl : M2MqttUnityClient
    {
        [Tooltip("Left joystick to control movement")]
        [SerializeField] public FixedJoystick joystick;
        [SerializeField] public Camera player;
        //[SerializeField] public InputField addressInputField;

        //string topic = "emqx/control";
        string topic = "M2MQTT_Unity/test";

        int position_scale_rate = 1;

        Vector3 base_orientation, base_position;
        bool resetPosition=true;

        Vector3 move, orientation, position;
        bool grip = false;

        bool requireUpdate_orientation = false,
            requireUpdate_position = false,
            requireUpdate_move = false,
            requireUpdate_gripper = false;

        private List<string> eventMessages = new List<string>();

        //public void SetBrokerAddress(string brokerAddress)
        //{
        //    if (addressInputField)
        //    {
        //        this.brokerAddress = brokerAddress;
        //    }
        //}

        public void toggleGripper()
        {
            this.grip = this.grip ? false : true;
            requireUpdate_gripper = true;
        }

        // helper function: round vector to 2 decimals
        Vector3 roundVector(ref Vector3 v)
        {
            v.x = Mathf.Round(v.x * 100f) / 100f;
            v.y = Mathf.Round(v.y * 100f) / 100f;
            v.z = Mathf.Round(v.z * 100f) / 100f;
            return v;
        }

        // helper function, same as Arduino's map function
        public double Map(double x, double in_min, double in_max, double out_min, double out_max)
        {
            var output = (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
            if (output > out_max) output = out_max;
            else if (output < out_min) output = out_min;
            return output;
        }

        void updateJoystickInput()
        {
            Vector3 move = Vector3.forward * joystick.Vertical + Vector3.right * joystick.Horizontal; // x, 0, y
            var tmp = move.y;
            move.y = move.z;
            move.z = tmp;
            if (!(this.move == roundVector(ref move)))
            {
                requireUpdate_move = true;
                this.move = move;
            }
            //Debug.Log("move: " + move);
        }

        void updateSensorInput()
        {
            if(resetPosition)
            {
                this.base_position = player.transform.position * position_scale_rate;
                this.base_orientation = player.transform.rotation.eulerAngles;
                resetPosition = false;
            }
            Vector3 orientation = player.transform.rotation.eulerAngles - this.base_orientation; // pitch, yaw, roll
            Vector3 position = player.transform.position * position_scale_rate - this.base_position;
            //Debug.Log("orientation: " + orientation);
            //Debug.Log("position: " + position);
            if (!(roundVector(ref orientation) == this.orientation))
            {
                this.orientation = orientation;
                requireUpdate_orientation = true;
            }
            if (!(roundVector(ref position) == this.position))
            {
                this.position = position;
                requireUpdate_position = true;
            }
        }

        void Update()
        {
            base.Update(); // call ProcessMqttEvents()

            if (eventMessages.Count > 0)
            {
                foreach (string msg in eventMessages)
                {
                    ProcessMessage(msg);
                }
                eventMessages.Clear();
            }
            updateSensorInput();
            updateJoystickInput();
        }
        
        short one_quard_update = 4;
        void FixedUpdate()
        {
            // 10 fps
            if (one_quard_update == 4)
            {
                one_quard_update = 0;
            }
            else {
                one_quard_update++;
                return;
            }

            if (client != null)
            {
                Vector3 orientation_result = new Vector3(90f, 90f, 90f);
                Vector3 position_result = new Vector3(90f, 90f, 90f);
                string content = "";
                if(requireUpdate_move)
                {
                    requireUpdate_move = false;
                    content += "m" + this.move.ToString("F2").Replace("(", "").Replace(")", "").Replace(" ", ""); 
                }
                if (requireUpdate_orientation)
                {
                    requireUpdate_orientation = false;
                    // pitch, yaw, roll 
                    orientation_result = this.orientation + orientation_result;
                    orientation_result.x = 180 - (float) Map(orientation_result.x%360, 0, 180, 0, 180);
                    // yaw is determined by position of cell phone now.
                    //orientation_result.y = 180 - (float) Map(orientation_result.y%360, 0, 180, 0, 180);
                    if (this.position.z != 0)
                    {
                        orientation_result.y = Mathf.Atan(this.position.x / this.position.z) + Mathf.PI/2;
                        orientation_result.y = 180 / Mathf.PI * orientation_result.y;
                        orientation_result.y = 180 - (float) Map(orientation_result.y % 360, 0, 180, 0, 180);
                        orientation_result.z = 180 - (float) Map(orientation_result.z%360, 0, 180, 0, 180);
                        content += "o" + orientation_result.ToString("F2").Replace("(","").Replace(")", "").Replace(" ", "");
                    }
                }
                if (requireUpdate_position)
                {
                    requireUpdate_position = false;
                    // l1 l2 are length of robotic arm.
                    var l1 = 100; var l2 = 145;
                    var min_distance = Mathf.Sqrt(l1 * l1 + l2 * l2);
                    var max_distance = l1+l2;
                    // x is useless, y is height, z is depth
                    Debug.Log("position: " + this.position);
                    position_result.x = 0;
                    position_result.y = (float) Map(this.position.y, -0.3, 0.3, -245, 245);
                    position_result.z = Mathf.Sqrt(this.position.x * this.position.x + this.position.z * this.position.z);
                    position_result.z = (float) Map(position_result.z, -0.3, 0.3, -245, 245);
                    //apply anti deadzone
                    //var anti_deadzone = 50;
                    //converted_position.y += converted_position.y < anti_deadzone && converted_position.y >= 0 ? anti_deadzone : -anti_deadzone;
                    //converted_position.z += converted_position.z < anti_deadzone && converted_position.z >= 0 ? anti_deadzone : -anti_deadzone;

                    var distance = Mathf.Sqrt(position_result.y * position_result.y + position_result.z * position_result.z);
                    Debug.Log("converted position: " + position_result);
                    // in order to make sure inverse kinemtic works, min < depth < max
                    if (this.position.z == 0) { } // avoid singularity
                    else
                    {
                        if (this.position.z < 0)
                        {
                            // reset arm
                            //content += "p0,145,-100";
                            position_result.y = 145;
                            position_result.z = -100;
                        }
                        else if (distance < min_distance)
                        {
                            //if depth out of arm range, keep the orientation
                            var angle = Mathf.Atan(this.position.y / this.position.z) + Mathf.PI / 2;
                            position_result.y = 177 * Mathf.Sin(angle);
                            position_result.z = 177 * Mathf.Cos(angle);
                        }
                        else if (distance > max_distance)
                        {
                            //if depth out of arm range, keep the orientation
                            var angle = Mathf.Atan(this.position.y / this.position.z) + Mathf.PI / 2;
                            position_result.y = 245 * Mathf.Sin(angle);
                            position_result.z = 245 * Mathf.Cos(angle);
                        }
                        // distance, height
                        content += "p" + position_result.ToString("F2").Replace("(", "").Replace(")", "").Replace(" ", "");
                    }
                }
                if (requireUpdate_gripper)
                {
                    requireUpdate_gripper = false;
                    //content = this.grip ? "u" : "g"; // ungrip, grip
                    content += "g";
                    this.grip = false;
                }
                // send control command only if any value changes.
                if (content != "")
                client.Publish(this.topic, System.Text.Encoding.UTF8.GetBytes(content+";"), MqttMsgBase.QOS_LEVEL_EXACTLY_ONCE, false);
            }
        }

        protected override void DecodeMessage(string topic, byte[] message)
        {
            string msg = System.Text.Encoding.UTF8.GetString(message);
            //Debug.Log("Received: " + msg);
            StoreMessage(msg);
            if (topic == this.topic)
            {
            }
        }

        private void StoreMessage(string eventMsg)
        {
            eventMessages.Add(eventMsg);
        }

        private void ProcessMessage(string msg)
        {
            Debug.Log("Received: " + msg);
        }

        protected override void OnConnecting()
        {
            base.OnConnecting();
            Debug.Log("Connecting to broker on " + brokerAddress + ":" + brokerPort.ToString() + "...\n");
        }

        protected override void OnConnected()
        {
            base.OnConnected();
            Debug.Log("Connected to broker on " + brokerAddress + "\n");
        }

        protected override void SubscribeTopics()
        {
            //client.Subscribe(new string[] { topic }, new byte[] { MqttMsgBase.QOS_LEVEL_EXACTLY_ONCE });
        }

        protected override void UnsubscribeTopics()
        {
            client.Unsubscribe(new string[] { topic });
        }

        protected override void OnConnectionFailed(string errorMessage)
        {
            Debug.Log("Mqtt: CONNECTION FAILED! " + errorMessage);
        }

        protected override void OnDisconnected()
        {
            Debug.Log("Mqtt: Disconnected.");
        }

        protected override void OnConnectionLost()
        {
            Debug.Log("Mqtt: CONNECTION LOST!");
        }

        private void OnDestroy()
        {
            Disconnect();
        }

        protected override void Start()
        {
            base.Start();
            Debug.Log("Ready.");
        }
    }
}