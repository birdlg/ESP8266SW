
// ***************************************
//     ESP8266 Switch for Amazon Echo
// ***************************************
// target: ES-12F ESP8266 module
// 2018.01. by Bird

#include <ESP8266WiFi.h>          //standard ESP8266 Wi-Fi library

#include <fauxmoESP.h>
#include <WeMo.h>

#define DEVICE_NAME      "Magic Bird"
#define ESSID           "AWV2-7F-RIGHT"
#define WIFI_PASSWORD   "connectme"
#define SSR             14        // pin to control SSR, active high
#define LED             16        // pin to control on-board LED, active high

//***********************************************************************************************

fauxmoESP fauxmo;                 // fauxmo object, the emulated WeMo switch

//***********************************************************************************************
void wifiInit() {
    WiFi.mode(WIFI_STA);
    Serial.printf("[WIFI] Connecting to %s ", ESSID);
    WiFi.begin(ESSID, WIFI_PASSWORD);

    while (WiFi.status() != WL_CONNECTED)       // wait while connecting
    {
        Serial.print(".");
        delay(100);
        digitalWrite(LED,~digitalRead(LED));   // blink the LED while connecting
    }
    Serial.println();

    Serial.printf("Connected. IP address: %s\n",  WiFi.localIP().toString().c_str());
    digitalWrite(LED, LOW);
}

void IO_init()   //misc I/O initialization
{
  pinMode(LED, OUTPUT);       
  pinMode(SSR, OUTPUT);   
  digitalWrite(LED, LOW);
  digitalWrite(SSR, LOW);
}

void onSetHandler(unsigned char device_id, const char * device_name, bool state)    // callback function of setting state
{
  Serial.printf("[MAIN] Device #%d (%s) state: %s\n", device_id, device_name, state ? "ON" : "OFF");
  digitalWrite(SSR, state);
  digitalWrite(LED, state);
}

bool onGetHandler(unsigned char device_id, const char * device_name)                // callback function of getting state
{
  return digitalRead(SSR);
}


void fauxmoInit()
{
  fauxmo.enable(true);                //enable fauxmo
  fauxmo.addDevice(DEVICE_NAME);      // add device
  
  fauxmo.onSetState(onSetHandler);      
  fauxmo.onGetState(onGetHandler);

}
//***********************************************************************************************

void setup() 
{
  Serial.begin(115200);
  IO_init();
  wifiInit();
  fauxmoInit();
}

void loop()
{
  fauxmo.handle();
}
