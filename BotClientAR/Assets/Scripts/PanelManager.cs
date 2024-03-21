using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PannelManager : MonoBehaviour
{
    [Tooltip("The greeting prompt Game Object to show when onboarding begins.")]
    [SerializeField]
    GameObject m_GreetingPrompt;

    [Tooltip("The Options Button to enable once the greeting prompt is dismissed.")]
    [SerializeField]
    GameObject m_OptionsButton;

    // Start is called before the first frame update
    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
        
    }

    public void onClick_continue()
    {
        m_GreetingPrompt.SetActive(false);
        m_OptionsButton.SetActive(true);
        //m_MenuManager.enabled = true;
    }
}
