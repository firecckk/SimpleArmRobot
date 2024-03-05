using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using TMPro;

public class IndicatorHandler : MonoBehaviour
{
    [Header("UI elements")]
    public GameObject MqttPanel;
    public GameObject ArmPanel;

    public TMP_Text MqttText;
    public GameObject MqttIndicator;

    private Image mqtt_img;
    private TMP_Text mqtt_text;

    private static bool mqttIndicatorOn = false;

    private enum FocusPanel
    {
        Main, Mqtt, Arm
    }

    private FocusPanel focus = FocusPanel.Main;

    // Start is called before the first frame update
    void Start()
    {
        mqtt_img = MqttIndicator.GetComponent<Image>();
        mqtt_text = MqttText.GetComponent<TMP_Text>();
    }

    // Update is called once per frame
    void Update()
    {
        if(mqttIndicatorOn)
        {
            mqtt_img.color = UnityEngine.Color.green;
            mqtt_text.color = UnityEngine.Color.green;
        } else
        {
            mqtt_img.color = UnityEngine.Color.red;
            mqtt_text.color = UnityEngine.Color.red;
        }
    }

    public static void mqttDisconnected()
    {
        mqttIndicatorOn = false;
    }

    public static void mqttConnected()
    {
        mqttIndicatorOn = true;
    }

    public void toggleMqttPanel()
    {
        if(focus == FocusPanel.Mqtt)
        {
            focus = FocusPanel.Main;
            MqttPanel.SetActive(false);
        } else
        {
            focus = FocusPanel.Mqtt;
            MqttPanel.SetActive(true);
        }
    }

    public void toggleArmPanel()
    {
        if (focus== FocusPanel.Arm)
        {
            focus = FocusPanel.Main;
            ArmPanel.SetActive(false);
        }
        else
        {
            focus = FocusPanel.Arm;
            ArmPanel.SetActive(true);
        }
    }
}
