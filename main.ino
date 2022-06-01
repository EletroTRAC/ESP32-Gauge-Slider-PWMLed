/* 
  ETEC Pirituba
    24/05/2022
    Projeto Saulo Gauge
*/

/* ************************** */

/* ************************** */
/* Bibliotecas */
#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiManager.h>
#include <WebServer.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <driver/adc.h>

#include "index_html.h"
#include "credentials.h"

/* ************************** */

/* ************************** */
/* Declaração de objetos e variáveis */
AsyncWebServer server(80);

const int output = 2;
const int output02 = 4;

const int freq = 5000;
const int ledChannel = 0;
const int resolution = 8;
String sliderValue = "0";

const int freq02 = 5000;
const int ledChannel02 = 0;
const int resolution02 = 8;
String sliderValue02 = "0";

const char* PARAM_INPUT = "value";


const char* ssid = SSIDc;
const char* password = PASSc;

/* ************************** */

/* ************************** */

/* Funções */

String processor(const String& var){
  //Serial.println(var);
  if (var == "SLIDERVALUE"){
    return sliderValue;
  }
  return String();
}

void setup(void)
{
  Serial.begin(115200);
  ledcSetup(ledChannel, freq, resolution);
  ledcAttachPin(output, ledChannel);
  ledcWrite(ledChannel, sliderValue.toInt());

  ledcSetup(ledChannel02, freq02, resolution02);
  ledcAttachPin(output02, ledChannel02);
  ledcWrite(ledChannel02, sliderValue02.toInt());
  
    WiFiManager wm;
    // Automatically connect using saved credentials,
    // if connection fails, it starts an access point with the specified name ( "AutoConnectAP"),
    // if empty will auto generate SSID, if password is blank it will be anonymous AP (wm.autoConnect())
    // then goes into a blocking loop awaiting configuration and will return success result

    bool res;
    // res = wm.autoConnect(); // auto generated AP name from chipid
    // res = wm.autoConnect("AutoConnectAP"); // anonymous ap
    res = wm.autoConnect("AutoConnectAP","password"); // password protected ap

    if(!res) {
        Serial.println("Failed to connect");
        // ESP.restart();
    }
    else {
        //if you get here you have connected to the WiFi    
        Serial.println("connected...yeey :)");
    }

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/html", String(indexHTML));
  });

  server.on("/gauge.min.js", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/plain", String(gauge));
  });

  server.on("/page2", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/html", String(newPage));
  });

  server.on("/slider01", HTTP_GET, [] (AsyncWebServerRequest *request) {
    String inputMessage;
    // GET input1 value on <ESP_IP>/slider?value=<inputMessage>
    if (request->hasParam(PARAM_INPUT)) {
      inputMessage = request->getParam(PARAM_INPUT)->value();
      sliderValue = inputMessage;
      ledcWrite(ledChannel, sliderValue.toInt());
    }
    else {
      inputMessage = "No message sent";
    }
    Serial.println(inputMessage);
    request->send(200, "text/plain", "OK");
  });

  server.on("/slider02", HTTP_GET, [] (AsyncWebServerRequest *request) {
    String inputMessage;
    // GET input1 value on <ESP_IP>/slider?value=<inputMessage>
    if (request->hasParam(PARAM_INPUT)) {
      inputMessage = request->getParam(PARAM_INPUT)->value();
      sliderValue = inputMessage;
      ledcWrite(ledChannel02, sliderValue02.toInt());
    }
    else {
      inputMessage = "No message sent";
    }
    Serial.println(inputMessage);
    request->send(200, "text/plain", "OK");
  });
  
  server.onNotFound([](AsyncWebServerRequest *request){
      request->send(404, "text/plain", "página não encontrada\n\n");
  });

  server.begin();
  Serial.println("HTTP Server getsarted");
}

void loop(void){}

/* ************************** */

/* Fim */
