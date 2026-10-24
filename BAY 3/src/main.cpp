#include <Arduino.h>
#include <WiFi.h>
#include "State.h"
#include "config.h"
#include "Peripherals.h"
#include "Network.h"
#include "Telemetry.h"
#include "model.h"
#include "edge_ai.h"
#include"optimization.h"




void setup()
{

//initialise serial monitor
    Serial.begin(115200);
    dht.begin();  // initialise sesnor

    //congig esp32 with real time 
    configTime(0,0,"pool.ntp.org", "time.nist.gov");
    //config periperal pins
   pinMode(BTN_PLUGIN, INPUT_PULLUP);
   pinMode(BTN_PLUGOUT, INPUT_PULLUP);
   pinMode(RELAY_PIN , OUTPUT);
   pinMode(LED_GREEN , OUTPUT) ; 
   pinMode(LED_YELLOW , OUTPUT) ; 
   pinMode(LED_RED, OUTPUT) ; 

   //conneect board to wifi
   connectWiFi();


   

   //config mqtt server
    mqtt.setServer(MQTT_SERVER, MQTT_PORT); //mqtt server address of things board and port number




   //connect bord to the cloud
   connectMQTT();//TOKEN ,device id

}

unsigned long now;
unsigned long last_print;

void loop()
{
    if (!mqtt.connected()) {
      connectMQTT();
    }
    mqtt.loop();
     //push data every 5 sec
    now = millis();
    if((now - last_print) > 5000)
    {
        last_print = now;
         
        //read data from sensors   // voltage ,current ,temperature ,power ,baystatus
        sample_sensor();
        //run Ai to get prediction
        runEdgeAIInference();

        //decide load based on predictions

      runOptimization();
       
        
        //publish the data

        publishTelemetry() ;

        
    }
    plug_status();
    updateLeds();

    
}

