#include <Arduino.h>
#include <DHT.h>
#include "State.h"
#include "Peripherals.h"
#include "config.h"


DHT   dht(DHT_PIN , DHT_TYPE);

//map I and v to 0 t0 30A and 0 to 250v
    //use own map fnf Mapfloat fn

    
float mapFloat(long x, long inMin, long inMax, float outMin, float outMax) {
  return (x - inMin) * (outMax - outMin) / (float)(inMax - inMin) + outMin;
}


void sample_sensor(void)
{
    int raw_current = analogRead(CURRENT_PIN); // 0 to 4095  //  0 to 30A
    int raw_voltage = analogRead(VOLTAGE_PIN); // 0 to 4095  //0 to 250v
    
    //map voltage and current
    voltage = mapFloat(raw_voltage, 0 ,4095 , 0 , 250);
    if(bayStatus == "CHARGING")
    {
    current = mapFloat(raw_current, 0 ,4095 , 0 , 32);
    }
    else
    {
      current = 0;
    }
    //read current and 5 values array

    //calculate power
    power = voltage * current;
    
    //to read temperature 
    float t = dht.readTemperature(DHT_PIN);
    
    if(!(isnan(t))) temperature = t;


}
float recentAvgCurrent(void)
{
   float sum =0;
   //read recent 5 current value
   for(int i=0;i<5;i++)
   {
      sum = sum + current;

   }
   return sum/5;
}



bool plugin_flag_once = 1;
bool plugout_flag_once = 1;

void plug_status(void)
{
   bool pluginReading = digitalRead(BTN_PLUGIN);
   // detect the sw is pressed
   if ( pluginReading ==LOW && plugin_flag_once)
   {
      //session time
      sessionStartMs =millis();
   // plug in switch is pressed 
   plugin_flag_once=0;
   // change bay_status FREE to charging
   if(bayStatus == "FREE")
   {
    bayStatus = "CHARGING";  
    Serial.println("Bay2 plugin detected,Bay is charging");
    //turn on the relay
      digitalWrite(RELAY_PIN, HIGH);

   }
   //update leds
   }
   if (pluginReading ==HIGH)
   {
      plugin_flag_once =1;
   }

   // plug out switch is pressed
   bool plugoutReading = digitalRead(BTN_PLUGOUT);
   // detect the sw is pressed
   if ( plugoutReading ==LOW && plugout_flag_once)
   {
   // plug in switch is pressed 
   plugout_flag_once=0;
   // change bay_status charging TO FREE
    if(bayStatus == "CHARGING")
    {
     bayStatus = "FREE";  
     Serial.println("Bay2 plugout detected,Bay is free");
     digitalWrite(RELAY_PIN, LOW);
    }
   //update leds
   }
   if (plugoutReading ==HIGH)
   {
      plugout_flag_once =1;
   } 
}
 /*void update_led_status(void)
 {
   //if bay status is FREE turn on green led
     if( bayStatus == "FREE")
    {
      digitalWrite(LED_GREEN,HIGH);
       digitalWrite(LED_YELLOW,LOW);

    }
    else{

    
   //if bay status is charging turn on yellow led
   digitalWrite(LED_GREEN,LOW);
       digitalWrite(LED_YELLOW,HIGH);


    }

 }*/