#include "MqttClient.h"

// MQTT Broker
//const char *mqtt_broker = "192.168.31.22";
const char *mqtt_broker = "192.168.43.31";
const char *mqtt_username = "esp32";
const char *mqtt_password = "esp32client";
const int mqtt_port = 1883;
const char *topic = "M2MQTT_Unity/test";
const char *status_topic = "emqx/status";

WiFiClient espClient;
PubSubClient client(espClient);

void callback(char *topic, byte *payload, unsigned int length);

void MqttClientSetup() {
    //connecting to a mqtt broker
    client.setServer(mqtt_broker, mqtt_port);
    client.setCallback(callback);
    while (!client.connected()) {
        String client_id = "esp32-client-";
        client_id += String(WiFi.macAddress());
        Serial.printf("The client %s connects to the public MQTT broker\n", client_id.c_str());
        if (client.connect(client_id.c_str(), mqtt_username, mqtt_password)) {
            Serial.println("Public EMQX MQTT broker connected");
            client.subscribe(topic);
            Serial.println("Topic subscribed");
        } else {
            Serial.print("failed with state ");
            Serial.print(client.state());
            delay(2000);
        }
    }
}


void solveForVectors(char * str, unsigned int len) {
    char* end;
    
    while(str[0]!='\n') {
        if(str[0] == ';') break;
        char type=str[0];
        Vector3 * vector;
        switch(type) {
            case 'o': // orientation
            vector = &state::orientation;
            state::require_update = true;
            break;
            case 'p': // position
            vector = &state::position;
            state::require_update = true;
            break;
            case 'm': // movement
            vector = &state::movement;
            state::move_update = true;
            break;
            case 'd': // debug
            vector = &state::debug;
            break;
            case 'g': // grabbing
            state::grab = true;
            str++;
            continue;
        }
        str++;
        vector->x = std::strtod(str, &end);
        str = end + 1;
        vector->y = std::strtod(str, &end);
        str = end + 1;
        vector->z = std::strtod(str, &end);
        str = end;
    }
}

void callback(char *topic, byte *payload, unsigned int length) {
    //Serial.print("Message arrived in topic: ");
    //Serial.println(topic);
    //Serial.print("Message:");
    //for (int i = 0; i < length; i++) {
    //    Serial.print((char) payload[i]);
    //}
    //Serial.println();
    //Serial.println("-----------------------");
    solveForVectors((char *) payload, length);
}

void MqttClientLoop() {
    client.loop();
}