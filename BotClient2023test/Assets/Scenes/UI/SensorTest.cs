using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class SensorTest : MonoBehaviour
{
    float scale_rate=1000000f;

    Vector3 delta_t = new Vector3(0.02f, 0.02f, 0.02f);
    Vector3 last_speed = new Vector3(0f, 0f, 0f);
    float DeadZone = 0.02f;
    Rigidbody rb;

    Vector3 base_position = new Vector3(0,100,400);
    // Start is called before the first frame update
    void Start()
    {
        rb = GetComponent<Rigidbody>();
    }

    void FixedUpdate()
    {
        Quaternion orientation = SensorFusion.GetOrientation();
        //Vector3 acc = SensorFusion.GetAccelerometer();
        Vector3 acc = Input.gyro.userAcceleration;
        //Debug.Log("orientation: " + orientation.eulerAngles);
        this.transform.rotation = orientation;
        this.transform.position = this.transform.position + Vector3.Scale(last_speed, delta_t);
        last_speed = Vector3.Scale(acc,delta_t);
        last_speed = Vector3.Scale(last_speed, new Vector3(scale_rate, scale_rate, scale_rate));

        // Dead zone filter
        if (Mathf.Abs(acc.x) < DeadZone) last_speed.x = 0;
        if (Mathf.Abs(acc.y) < DeadZone) last_speed.y = 0;
        if (Mathf.Abs(acc.z) < DeadZone) last_speed.z = 0;
        //Vector3 filtered_acc = new Vector3(filtered_x, filtered_y, filtered_z);
        //rb.AddForce(filtered_x * speed, 0, filtered_x * speed, ForceMode.Impulse);
        Debug.Log("acc: " + acc);
        Debug.Log("position: " + (this.transform.position-base_position));
    }

}
