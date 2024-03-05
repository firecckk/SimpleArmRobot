using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using TMPro;

public class ArmPannel : MonoBehaviour
{
    [SerializeField] private TextMeshProUGUI textField1;
    [SerializeField] private TextMeshProUGUI textField2;
    [SerializeField] private TextMeshProUGUI textField3;
    [SerializeField] private TextMeshProUGUI textField4;
    [SerializeField] private TextMeshProUGUI textField5;
    [SerializeField] private TextMeshProUGUI textField6;
    
    [SerializeField] private Slider slider1;
    [SerializeField] private Slider slider2;
    [SerializeField] private Slider slider3;
    [SerializeField] private Slider slider4;
    [SerializeField] private Slider slider5;
    [SerializeField] private Slider slider6;

    // Start is called before the first frame update
    void Start()
    {
        slider1.onValueChanged.AddListener((v) =>
        {
            textField1.text = "Joint 0: " + v.ToString("0");
        });
        slider2.onValueChanged.AddListener((v) =>
        {
            textField2.text = "Joint 1: " + v.ToString("0");
        });
        slider3.onValueChanged.AddListener((v) =>
        {
            textField3.text = "Joint 2: " + v.ToString("0");
        });
        slider4.onValueChanged.AddListener((v) =>
        {
            textField4.text = "Joint 3: " + v.ToString("0");
        });
        slider5.onValueChanged.AddListener((v) =>
        {
            textField5.text = "Joint 4: " + v.ToString("0");
        });
        slider6.onValueChanged.AddListener((v) =>
        {
            textField6.text = "Joint 5: " + v.ToString("0");
        });

    }

    // Update is called once per frame
    void Update()
    {
        
    }

    public void OnEndEdit(int value)
    {

    }
}
