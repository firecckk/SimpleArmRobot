using UnityEngine;
using UnityEngine.UI;
using System.Collections;
using UnityEngine.EventSystems;

public class CamZoom : MonoBehaviour, IPointerDownHandler
{
    [Header("Zoom object")]
    public RawImage obj;
    private bool zoom = false;
    public void OnPointerDown(PointerEventData eventData)
    {
        if (eventData.clickCount == 1)
        {
            float zoomf;
            eventData.clickCount = 0;
            if(zoom)
            {
                zoomf = 1.5f;
                zoom = false;
            } else
            {
                zoomf = 2.2f;
                zoom = true;
            }
            obj.rectTransform.localScale = new Vector3(zoomf, zoomf, 1f);
        }
    }

    // Start is called before the first frame update
    void Start()
    {

    }

    // Update is called once per frame
    void Update()
    {

    }
}