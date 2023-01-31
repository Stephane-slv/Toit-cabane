// Stéphane Nario
// Application d'ouverture automatique toit cabane
// 2020
// Merci à Thommy Desrochers et aux auteurs des librairies

#include <Arduino.h>
#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>
#include <WiFi.h>
#include <Adafruit_BME280.h>

#define SEALEVELPRESSURE_HPA (1013.25) // Pression atmosphérique au niveau de la mer

Adafruit_BME280 bme; // objet BME

float temperature, humidity, pressure, altitude; // Variables

const char *ssid = "ESP32AP";
const char *password = "MDP1234";
//------------------------------------------------------ Constantes
//const int led = 2;
const int capteurToit = 34;
const int led = 2;
//const int captToitA_Haut = 34;
//const int captToitA_Bas = 35;
//const int toitB = 3;
//const int captToitB_Haut = 32;
//const int captToitB_Bas = 33;


AsyncWebServer server(80);

//------------------------------------------------------- Variables
int etat = 0;

void setup()
{
//----------------------------------------------------Serial
  Serial.begin(115200);
  Serial.println("\n");

  bme.begin(0x76); 

  //----------------------------------------------------GPIO
  pinMode(led, OUTPUT);
  //pinMode(toitB, OUTPUT);
  //digitalWrite(toitA, LOW);
  //digitalWrite(toitB, LOW);
  pinMode(capteurToit, INPUT);
  //pinMode(captToitA_Bas, INPUT);
  //pinMode(captToitB_Haut, INPUT);
  //pinMode(captToitB_Bas, INPUT);


  //----------------------------------------------------SPIFFS
  if(!SPIFFS.begin())
  {
    Serial.println("Erreur SPIFFS...");
    return;
  }

  File root = SPIFFS.open("/");
  File file = root.openNextFile();

  while(file)
  {
    Serial.print("File: ");
    Serial.println(file.name());
    file.close();
    file = root.openNextFile();
  }

  //----------------------------------------------------WIFI

Serial.println("Création du point d'accés");
WiFi.softAP(ssid,password);
Serial.print("Adresse IP: ");
Serial.println(WiFi.softAPIP());                            // On affiche l'adresse IP du serveur

  //----------------------------------------------------SERVER
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
  {
    request->send(SPIFFS, "/index.html", "text/html");      // Le serveur envoie au client le fichier index.html
  });

  server.on("/w3.css", HTTP_GET, [](AsyncWebServerRequest *request)
  {
    request->send(SPIFFS, "/w3.css", "text/css");           // Le serveur envoie au client la bibliotheque W3.css pour l'affichage correct de la page index.html
  });

  server.on("/script.js", HTTP_GET, [](AsyncWebServerRequest *request)
  {
    request->send(SPIFFS, "/script.js", "text/javascript"); // Le serveur envoie au client le fichier javascript
  });

  server.on("/lireCapteur", HTTP_GET, [](AsyncWebServerRequest *request)
  {
    int val = digitalRead(capteurToit);       // Envoie l'état de la led (1 si allumée, 0 si éteint)
    String ValCapteurToit = String(val);
    request->send(200, "text/plain", ValCapteurToit);
  });

  server.on("/lireTemperature", HTTP_GET, [](AsyncWebServerRequest *request)
  {
    String temp = String(temperature);
    request->send(200, "text/plain", temp);
  });

  server.on("/on", HTTP_GET, [](AsyncWebServerRequest *request)
  {
    if (etat == 0){
    digitalWrite(led, HIGH);
    etat = 1;
    request->send(200);
    }
    else {
    digitalWrite(led, LOW);
    etat = 0;
    request->send(200); 
    }
  });

  //server.on("/off", HTTP_GET, [](AsyncWebServerRequest *request)
  //{
  //  digitalWrite(led, LOW);
  //  request->send(200);
  //});

  server.begin();
  Serial.println("Serveur actif!");
}

void loop()
{

//void handle_OnConnect() {
  temperature = bme.readTemperature();
  humidity = bme.readHumidity();
  pressure = bme.readPressure() / 100.0F;
  altitude = bme.readAltitude(SEALEVELPRESSURE_HPA);
// server.send(200, "text/html", SendHTML(temperature,humidity,pressure,altitude)); 
//}
Serial.print ("Température : ");
Serial.print (temperature);
Serial.println (" °C");
Serial.print ("Humididée : ");
Serial.print (humidity);
Serial.println (" %");
Serial.print ("Pression : ");
Serial.print (pressure);
Serial.println (" HPa");
Serial.print ("Altitude : ");
Serial.print (altitude);
Serial.println (" mètres");
Serial.println ("\n");
delay(5000);
}