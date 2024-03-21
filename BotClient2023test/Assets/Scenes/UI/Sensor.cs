using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Sensor : MonoBehaviour
{
    Gyroscope m_Gyro;
    // Start is called before the first frame update
    void Start()
    {
        m_Gyro = Input.gyro;
        m_Gyro.enabled = true;
    }

    // Update is called once per frame
    void Update()
    {
        Debug.Log("Gyro rotation rate " + m_Gyro.rotationRate);
        Debug.Log("Gyro attitude" + m_Gyro.attitude);
        Debug.Log("Gyro enabled : " + m_Gyro.enabled);
    }
}
