/*************************************************************
  Download latest Blynk library here:
    https://github.com/blynkkk/blynk-library/releases/latest

  Blynk is a platform with iOS and Android apps to control
  Arduino, Raspberry Pi and the likes over the Internet.
  You can easily build graphic interfaces for all your
  projects by simply dragging and dropping widgets.

    Downloads, docs, tutorials: http://www.blynk.cc
    Sketch generator:           http://examples.blynk.cc
    Blynk community:            http://community.blynk.cc
    Follow us:                  http://www.fb.com/blynkapp
                                http://twitter.com/blynk_app

  Blynk library is licensed under MIT license
  This example code is in public domain.

 *************************************************************
  This example runs directly on ESP32 chip.

  Note: This requires ESP32 support package:
    https://github.com/espressif/arduino-esp32

  Please be sure to select the right ESP32 module
  in the Tools -> Board menu!

  Change WiFi ssid, pass, and Blynk auth token to run :)
  Feel free to apply it to any other example. It's simple!
 *************************************************************/

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial


#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>


// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "CjNAmh2qPqyhveU5ca2jOWdP71xv5Xek";

// Your WiFi credentials.
// Set password to "" for open networks.
// The EE IOT network is hidden. You might not be able to see it.
// But you should be able to connect with these credentials. 
char ssid[32] = "EE-IOT-Platform-02";
char pass[32] = "g!TyA>hR2JTy";


#define LED 2
const int freq = 5000;
const int ledChannel = 0;
const int resolution = 10;
int Led_State = 0;
int Slider_State = 0;
int Time_Counter = 0;
String Message = "";

BlynkTimer timer;

void setup()
{
  // Serial Monitor
  Serial.begin(115200);
  Blynk.begin(auth, ssid, pass);

  timer.setInterval(1000L, myTimerEvent);


  ledcSetup(ledChannel, freq, resolution);
  ledcAttachPin(LED, ledChannel);
  
}

void loop()
{
  Blynk.run();
  timer.run();
}

// This function executes whenever V1 is written to.
// You don't need to call it. Blynk handles the trigger for you.
BLYNK_WRITE(V0)
{

    // param is a member variable of the Blynk ADT. It is exposed so you can read it.
    int pinValue = param.asInt(); // assigning incoming value from pin V1 to a variable

    // Because V1 is a button, pinValue will be a 0 or a 1.
    if (pinValue == 0) {
        Led_State = 0;
        ledcWrite(ledChannel, 0);
    }
    else {
        Led_State = 1;
        ledcWrite(ledChannel, Slider_State);
    }
}

BLYNK_WRITE(V1)
{
    // param is a member variable of the Blynk ADT. It is exposed so you can read it.
    int val = param.asInt(); // assigning incoming value from pin V1 to a variable
    Slider_State = val;
    if(Led_State == 1) {
    ledcWrite(ledChannel, val); // Note that this takes ledChannel as an argument, NOT the pin! Set duty = val.
    }
}

void myTimerEvent() {
  

   if (Time_Counter == 100){
        // Do thing that needs to happen every 1 second
        Time_Counter = 0; // reset time counter
        Blynk.virtualWrite(V2, millis() / 1000);
    }
    else {
        // Send serial data to Blynk terminal
        char character;
        while(Serial.available()) { // Check if serial is available every 10 ms
            character = Serial.read();
            Message.concat(character);
        }
        if (Message != "") {
            Blynk.virtualWrite(V3, Message);
            Message = ""; // Clear String
        }  
    }

  Time_Counter++;
}
