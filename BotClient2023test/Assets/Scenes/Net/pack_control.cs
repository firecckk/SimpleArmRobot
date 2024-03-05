using UnityEngine;

[System.Serializable]
public class Pack_control
{
    public string topic = "M2MQTT_Unity/control";
    public float v_x;
    public float v_y;
    public float delta_yaw;
    public float pitch;

    public Pack_control(float v_x, float v_y, float delta_yaw, float pitch)
    {
        this.v_x = v_x;
        this.v_y = v_y;
        this.delta_yaw = delta_yaw;
        this.pitch = pitch;
    }

    public Pack_control CreateFromJSON(string jsonString)
    {
        return JsonUtility.FromJson<Pack_control>(jsonString);
    }

    public string SaveToString()
    {
        return JsonUtility.ToJson(this);
    }
}
