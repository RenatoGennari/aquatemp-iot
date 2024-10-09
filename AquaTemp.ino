
#include <Adafruit_Sensor.h>
#include "DHT.h"
#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>  // Biblioteca para criar e interpretar JSON
#include <OneWire.h>
#include <DallasTemperature.h>

// Definições e Constantes
#define DHTPIN 4         // Pino de dados do sensor DHT11
#define DHTTYPE DHT11    // Definir o tipo de sensor como DHT11
#define LED_PIN 2        // Pino do LED
#define PINO_ONEWIRE 12  // Pino para o sensor Dallas

OneWire oneWire(PINO_ONEWIRE);
DallasTemperature sensor(&oneWire);
DeviceAddress endereco_temp;

// Variáveis Wi-Fi e MQTT
const char* ssid = "Akira_2.4G";       // Substitua pelo seu SSID
const char* password = "lucca2000";    // Substitua pela sua senha
const char* mqttServer = "broker.hivemq.com";  // Endereço do broker MQTT
const int port = 1883;
char clientId[50];
String stMac;

// Criação de objetos
WiFiClient espClient;
PubSubClient client(espClient);
DHT dht(DHTPIN, DHTTYPE);  // Inicialização do sensor DHT11

// Variáveis de leitura
float temperature_atual = 0.0;
float humidity_atual = 0.0;
float temp_agua = 0.0;

// Função para conectar ao WiFi
void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Conectando-se ao WiFi: ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi conectado!");
  Serial.println("Endereço IP: ");
  Serial.println(WiFi.localIP());
}

// Função para conectar ao servidor MQTT
void reconnect() {
  while (!client.connected()) {
    Serial.print("Conectando ao MQTT...");
    long r = random(1000);
    sprintf(clientId, "clientId-%ld", r);
    if (client.connect(clientId)) {
      Serial.println("conectado!");
      client.subscribe("led/led");
    } else {
      Serial.print("Falha, rc=");
      Serial.print(client.state());
      Serial.println(" tentando novamente em 5 segundos");
      delay(5000);
    }
  }
}

// Função para publicar dados no MQTT
void publicarTemperatura(float temp_agua, float temp_ar, float umidade) {
  // Criação de objeto JSON
  StaticJsonDocument<200> doc;
  doc["temperatura_agua"] = temp_agua;
  doc["temperatura_ar"] = temp_ar;
  doc["umidade"] = umidade;

  char buffer[200];
  serializeJson(doc, buffer);
  
  client.publish("sensor/temperatura", buffer);
}

// Callback do MQTT para controle do LED
void callback(char* topic, byte* message, unsigned int length) {
  String stMessage;
  for (int i = 0; i < length; i++) {
    stMessage += (char)message[i];
  }

  if (String(topic) == "led/led") {
    if(stMessage == "on") {
      digitalWrite(LED_PIN, HIGH);
    }
    else if(stMessage == "off") {
      digitalWrite(LED_PIN, LOW);
    }
  }
}

void setup() {
  // Inicializações
  Serial.begin(115200);
  dht.begin();  // Iniciar DHT11
  sensor.begin();  // Iniciar sensor Dallas
  randomSeed(analogRead(0));

  // Configuração do LED
  pinMode(LED_PIN, OUTPUT);

  // Conectar Wi-Fi
  setup_wifi();

  // Conectar ao servidor MQTT
  client.setServer(mqttServer, port);
  client.setCallback(callback);
}

void loop() {
  // Verificar conexão ao servidor MQTT
  if (!client.connected()) {
    reconnect();
  }
  client.loop();  // Mantém a comunicação MQTT ativa

  // Leitura do DHT11 (Temperatura do ar e Umidade)
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  // Leitura do sensor Dallas (Temperatura da água)
  sensor.requestTemperatures();
  temp_agua = sensor.getTempCByIndex(0);

  // Verificação das leituras
  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Falha ao ler DHT!!");
  } else if (humidity_atual != humidity || temperature_atual != temperature || temp_agua != 0.0) {
    humidity_atual = humidity;
    temperature_atual = temperature;

    // Publicar dados no tópico MQTT
    publicarTemperatura(temp_agua, temperature_atual, humidity_atual);

    // Exibir no Serial Monitor
    Serial.print("Temp Ar: ");
    Serial.print(temperature_atual);
    Serial.print(" °C, Umid: ");
    Serial.print(humidity_atual);
    Serial.print(" %, Temp Água: ");
    Serial.print(temp_agua);
    Serial.println(" °C");
    Serial.println("====================");
  }

  delay(2000);  // Aguarda 2 segundos entre as leituras
}
