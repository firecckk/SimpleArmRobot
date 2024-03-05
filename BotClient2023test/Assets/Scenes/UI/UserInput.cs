using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.InputSystem;

public class UserInput
{
    //public float move_x { get; set;}
    //public float move_y { get; set;}
    //public float delta_yaw { get; set; }
    //public float pitch { get; set; }

    //private static UserInput _instance=null;

    private static int fps = 24;
    private static float timer=0f;

    public static bool available()
    {
        timer += Time.deltaTime;
        if (timer > 1f/fps)
        {
            //Debug.Log(timer);
            timer = timer - 1f/fps;
            return true;
        } else
        {
            return false;
        }

    }

    public static Pack_control getInput()
    {
        Gamepad gamepad = Gamepad.current;
        Pack_control pack;
        if (gamepad != null)
        {
            Vector2 stickL = gamepad.leftStick.ReadValue(); //stickL.x will be -1.0..1.0 (for full left to full right) //stickL.y will be -1.0..1.0 (for full down to full up)
            pack = new Pack_control(stickL.x, stickL.y, 0, 0);
            return pack;
        }
        return null;
    }
}
