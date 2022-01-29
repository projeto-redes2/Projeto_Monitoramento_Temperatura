#include <WiFi.h>
extern "C" {
  #include "freertos/FreeRTOS.h"
  #include "freertos/timers.h"
}
#include <AsyncMqttClient.h>
#include "DHT.h"





#define DHTPIN 5     // Pino digital conectado ao Sensor DHT11


#define DHTTYPE DHT11   // Define o tipo de sensor


// Conecte o pino 1 (à esquerda) do sensor a +5V
// NOTA: Se estiver usando uma placa com lógica de 3,3 V como um Arduino Due conecte o pino 1
// para 3,3V em vez de 5V!
// Conecte o pino 2 do sensor a qualquer que seja o seu DHTPIN
// Conecte o pino 3 (à direita) do sensor ao GROUND (se seu sensor tiver 3 pinos)
// Conecte o pino 4 (à direita) do sensor ao GROUND e deixe o pino 3 VAZIO (se seu sensor tiver 4 pinos)
// Conecte um resistor de 10K do pino 2 (dados) ao pino 1 (alimentação) do sensor








#define WIFI_SSID "ADICIONAR NOME DO ROTEADOR"
#define WIFI_PASSWORD "ADICIONAR_SENHA_DO_ROTEADOR"

//define o o broker mqtt
#define MQTT_HOST "broker.hivemq.com"
#define MQTT_PORT 1883

// define qual vai ser o tópico para ser submetido 
#define MQTT_PUB_TEMP "lucasfr_uff_si_redes2/esp32/dht/temperature"
#define MQTT_PUB_HUM  "lucasfr_uff_si_redes2/esp32/dht/humidity"

//define as variaveis do tempo de transmissão e as variaveis das temperatura e humidade
float humidity  ;
float temperature ;
unsigned long tempoPassado = 0;
const long interval = 10000;

// cria o objeto que será o cliente a ser utilizado para receber as informações
AsyncMqttClient mqttClient;
// cria o objeto do Sensor 
DHT dht(DHTPIN, DHTTYPE);
// define os tempos de transmissão
TimerHandle_t mqttReconnectTimer;
TimerHandle_t wifiReconnectTimer;

// conecta com o Wifi
void conectarWifi() {
  Serial.println("Connecting to Wi-Fi...");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
}

// conecta com o cliente do broker mqtt
void conectarMqtt() {
  Serial.println("Connecting to MQTT...");
  mqttClient.connect();
}

void WiFiEvent(WiFiEvent_t event) {
    Serial.printf("[WiFi-event] event: %d\n", event);
    switch(event) {
    case SYSTEM_EVENT_STA_GOT_IP:
        Serial.println("WiFi connected");
        Serial.println("IP address: ");
        Serial.println(WiFi.localIP());
        connectToMqtt();
        break;
    case SYSTEM_EVENT_STA_DISCONNECTED:
        Serial.println("WiFi lost connection");
        xTimerStop(mqttReconnectTimer, 0); // garante que não nos reconectamos ao MQTT ao reconectar ao Wi-Fi
        xTimerStart(wifiReconnectTimer, 0);
        break;
    }
}

//alerta a conexão mqtt
void onMqttConnect(bool sessionPresent) {
  Serial.println("Connected to MQTT.");
  Serial.print("Session present: ");
  Serial.println(sessionPresent);
  
}

//alerta a desconexão do mqtt
void onMqttDisconnect(AsyncMqttClientDisconnectReason razão) {
  Serial.println("Disconnected from MQTT.");

  if (WiFi.isConnected()) {
    xTimerStart(mqttReconnectTimer, 0);
  }
}


//recebe o pacote mqtt a ser enviado
void onMqttPublish(uint16_t packetId) {
  Serial.println("Publish acknowledged.");
  Serial.print("  packetId: ");
  Serial.println(packetId);
}

void setup() {
  Serial.begin(115200);
  Serial.println();
  dht.begin();

  mqttReconnectTimer = xTimerCreate("mqttTimer", pdMS_TO_TICKS(2000), pdFALSE, (void*)0, reinterpret_cast<TimerCallbackFunction_t>(connectToMqtt));
  wifiReconnectTimer = xTimerCreate("wifiTimer", pdMS_TO_TICKS(2000), pdFALSE, (void*)0, reinterpret_cast<TimerCallbackFunction_t>(connectToWifi));

  WiFi.onEvent(WiFiEvent);

  mqttClient.onConnect(onMqttConnect);
  mqttClient.onDisconnect(onMqttDisconnect);
  mqttClient.onPublish(onMqttPublish);
  mqttClient.setServer(MQTT_HOST, MQTT_PORT);

  connectToWifi();
}


void loop() {
  unsigned long tempoAtual = millis();
  // Cada número X de segundos (intervalo = 10 segundos)
  // publica uma nova mensagem MQTT
  if (tempoAtual - tempoPassado >= intervalo) {
    // Salva a última vez que uma nova leitura foi publicada
    tempoPassado = tempoAtual;
    // Novas leituras do sensor DHT
    humidity = dht.readHumidity();
    // Lê a temperatura como Celsius (o padrão)
    temperature = dht.readTemperature();
    
    // Verifica se alguma leitura falhou e sai mais cedo (para tentar novamente).
    if (isnan(temperature) || isnan(humidity)) {
      Serial.println(F("Falha ao ler do sensor DHT!"));
      return;
    }
    
    // Publica uma mensagem MQTT no tópico esp32/dht/temperature
    uint16_t packetIdPub1 = mqttClient.publish(MQTT_PUB_TEMP, 1, true, String(temperature).c_str());                            
    Serial.printf("Publicando no topico %s at QoS 1, packetId: %i", MQTT_PUB_TEMP, packetIdPub1);
    Serial.printf("Mensagem: %.2f \n", temperature);

    // Publica uma mensagem MQTT no tópico esp32/dht/humidity
    uint16_t packetIdPub2 = mqttClient.publish(MQTT_PUB_HUM, 1, true, String(humidity).c_str());                            
    Serial.printf("Publicando no topico %s at QoS 1, packetId %i: ", MQTT_PUB_HUM, packetIdPub2);
    Serial.printf("Mensagem: %.2f \n", humidity);
  }
}
