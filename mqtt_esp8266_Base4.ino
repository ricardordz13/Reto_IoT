// Autores: Ricardo Jorge Rodríguez Treviño, André Borda Ramos
// Matrículas: A00831595, A01284398
// Fecha: 01/12/2021
// Descripción: Archivo que recibe los datos obtenidos por el sensor 
// correspondientes a la humedad y temperatura cada cierto tiempo; 
// además de enviarlos a través de un Broker MQTT para su posterior 
// recepción y con ello, utilización.

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include "DHT.h"

#define DHTTYPE DHT11
#define DHTPIN D4 
DHT dht(DHTPIN, DHTTYPE);

#define NTP_OFFSET   0 //-21600            // In seconds
#define NTP_INTERVAL 60 * 1000    // In miliseconds
#define NTP_ADDRESS  "cronos.cenam.mx" //europe.pool.ntp.org
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, NTP_ADDRESS, NTP_OFFSET, NTP_INTERVAL);



// Update these with values suitable for your network.

const char* ssid = "AndroidAPb057";
const char* password = "bfte5668";
const char* mqtt_server = "broker.mqtt-dashboard.com";
const char* topico_salida = "cosa1";
const char* topico_entrada = "A01284398";


WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE	(80)
char msg[MSG_BUFFER_SIZE];
int value = 0;

unsigned long ciclo1 = 0;
unsigned long ciclo2 = 0;
unsigned long ciclo3 = 0;
unsigned long ciclo4 = 0;
unsigned long ciclo5 = 0;



// We start by connecting to a WiFi network
void setup_wifi() {

  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
  digitalWrite(D0, HIGH);
  delay(100);
  digitalWrite(D0, LOW);

                              // Switch on the LED if an 1 was received as first character
//  if ((char)payload[0] == '1') {
//    digitalWrite(D6, HIGH);   // Turn the LED on (Note that LOW is the voltage level
                              // but actually the LED is on; this is because
                              // it is active low on the ESP-01). No es cierto en D6.
//  } else {
//    digitalWrite(D6, LOW);    // Turn the LED off by making the voltage HIGH
//  }

}


//Aquí conecta o reconecta.
void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      //client.publish("outTopic", "hello world");
      client.subscribe(topico_entrada);
      client.subscribe(topico_salida);
      client.publish(topico_salida, "hello world");      
      // ... and resubscribe
      //client.subscribe("inTopic");
     
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup() {
  pinMode(D0, OUTPUT);
  pinMode(D3, INPUT);
  pinMode(D4, OUTPUT);     
  
  digitalWrite(D0, HIGH);
  digitalWrite(D4, HIGH);
  Serial.begin(115200);
  dht.begin();
  timeClient.begin();
  //timeClient.setTimeOffset(-21600);
  
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  if (!client.connected()) {
    reconnect();
  }
  digitalWrite(D0, LOW);
  digitalWrite(D4, LOW);  
}


void proceso1() {
  float t = dht.readTemperature();
 // int t = 20;
  //Serial.print("Temperatura: "); 
  //Serial.println(t);
  snprintf (msg, MSG_BUFFER_SIZE, "{\"dispositivo\":\"IoT_0\",\"tipo\":\"temperatura\",\"dato\":%.2f}", t);
  //Serial.print("Publish message: ");
  Serial.println(msg);
  //client.publish(topico_salida, msg);
  client.publish(topico_entrada, msg);   
}

void proceso2() {
  //const int analog_p = A0;
  //unsigned int analog_in = analogRead(analog_p);
  //analogWrite(D1, analog_in/4);
  //snprintf (msg, MSG_BUFFER_SIZE, "{\"dispositivo\":\"IoT_0\",\"tipo\":\"lumi\",\"dato\":%d}", ((1024-analog_in)*100)/(1024-50));
//  Serial.print("Publish message: ");
//  Serial.println(msg);
  //client.publish(topico_salida, msg);
  //client.publish(topico_entrada, msg); 
}

void proceso3() {
  timeClient.update();
  String formattedTime = timeClient.getFormattedTime();
  unsigned long epochTime = timeClient.getEpochTime();
  Serial.print(formattedTime);
  Serial.print(" ");
  Serial.println(epochTime);
}


void proceso4() {
  float h = dht.readHumidity();
 // int h = 20;
  //Serial.print("Humedad: "); 
  //Serial.println(h);
  snprintf (msg, MSG_BUFFER_SIZE, "{\"dispositivo\":\"IoT_0\",\"tipo\":\"humedad\",\"dato\":%.2f}", h);
  //Serial.print("Publish message: ");
  //Serial.println(msg);
  //client.publish(topico_salida, msg);
  client.publish(topico_entrada, msg);   
}
  
void proceso5() {
  if (digitalRead(D4) == LOW) {
    digitalWrite(D4, HIGH);
  }
  else{
    digitalWrite(D4, LOW);
  }
     
}


void loop() {

  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  unsigned long now = millis();
  if (now - ciclo1 > 5000) {ciclo1 = now;proceso1();}
  if (now - ciclo2 > 6300) {ciclo2 = now;proceso2();}
  if (now - ciclo3 > 2000) {ciclo3 = now;proceso3();}
  if (now - ciclo4 > 4300) {ciclo4 = now;proceso4();}
  if (now - ciclo5 > 10000) {ciclo5 = now;proceso5();}

}
