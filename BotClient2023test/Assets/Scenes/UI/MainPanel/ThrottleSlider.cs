using UnityEngine;
using UnityEngine.UI;
using UnityEngine.EventSystems;
using TMPro;

//[RequireComponent(typeof(Slider))]
public class ThrottleSlider : MonoBehaviour //, IPointerUpHandler
{
    [SerializeField] private Slider _slider;
    [SerializeField] private TextMeshProUGUI _sliderText=null;
    private float throttleValue = 0;
    private static bool pointerOnSlider = false;

    void Start()
    {
        _slider.onValueChanged.AddListener((value) =>
        {
            _sliderText.text = value.ToString("0.00");
            throttleValue = value;
        });
    }

    void Awake()
    {
        _slider = GetComponent<Slider>();
    }

    void Update()
    {
    }

    public void OnPointerUp()
    {
        //Debug.Log("pointer is not on Slider");
        pointerOnSlider = false;
    }

    public void OnPointerDown()
    {
        //Debug.Log("pointer is on Slider");
        pointerOnSlider = true;
    }

    public static bool isPointerOnSlider()
    {
        return pointerOnSlider;
    }
}