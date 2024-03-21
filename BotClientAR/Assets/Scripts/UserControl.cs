using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
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

        //string topic = "emqx/control";
        string topic = "M2MQTT_Unity/test";

        Vector3 move, orientation, position;
        bool grip = false;

        bool requireUpdate_orientation = false,
            requireUpdate_position = false,
            requireUpdate_move = false,
            requireUpdate_gripper = false;

        private List<string> eventMessages = new List<string>();

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

        void updateJoystickInput()
        {
            Vector3 move = Vector3.forward * joystick.Vertical + Vector3.right * joystick.Horizontal; // x, 0, y
            if (!(this.move == roundVector(ref move)))
            {
                requireUpdate_move = true;
                this.move = move;
            }
            //Debug.Log("move: " + move);
        }

        void updateSensorInput()
        {
            Vector3 orientation = player.transform.rotation.eulerAngles; // pitch, yaw, roll
            Vector3 position = player.transform.position;
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

        bool half_update_rate = true;
        void FixedUpdate()
        {
            // 25 fps
            half_update_rate = half_update_rate ? false : true;
            if (half_update_rate) return;

            if (client != null)
            {
                string content = "";
                if(requireUpdate_gripper)
                {
                    requireUpdate_gripper = false;
                    content = this.grip ? "u" : "g"; // ungrip, grip
                }
                if(requireUpdate_move)
                {
                    requireUpdate_move = false;
                    content += "m" + this.move.ToString("F2").Replace("(", "").Replace(")", "").Replace(" ", ""); 
                }
                if (requireUpdate_orientation)
                {
                    requireUpdate_orientation = false;
                    // pitch, yaw, roll 
                    Vector3 result = new Vector3(90f, 90f, 90f);
                    result = this.orientation + result;
                    result.x = result.x % 360;
                    result.y = result.y % 360;
                    result.z = result.z % 360;
                    content += "o" + result.ToString("F2").Replace("(","").Replace(")", "").Replace(" ", "");
                }
                if (requireUpdate_position)
                {
                    requireUpdate_position = false;
                    // distance, height
                    content += "p" + this.position.ToString("F2").Replace("(", "").Replace(")", "").Replace(" ", "");
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