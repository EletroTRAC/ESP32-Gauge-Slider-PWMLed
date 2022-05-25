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
float adcval = 0;                    // ADC Mittelwert

const int output = 2;

const int freq = 5000;
const int ledChannel = 0;
const int resolution = 8;

const char* PARAM_INPUT = "value";

String sliderValue = "0";

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
  
  Serial.print("");
  Serial.println("Inicio.");
  WiFi.mode(WIFI_MODE_STA);
  Serial.print("Conectando-se à ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi conectado!");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/html", String(indexHTML));
  });

  server.on("/gauge.min.js", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/plain", String(gauge));
  });

  server.on("/slider", HTTP_GET, [] (AsyncWebServerRequest *request) {
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
  
  server.onNotFound([](AsyncWebServerRequest *request){
      request->send(404, "text/plain", "página não encontrada\n\n");
  });

  server.begin();
  Serial.println("HTTP Server getsarted");
}

void loop(void){}

/* ************************** */

/* Fim */