# Projeto_Monitoramento_Temperatura
<h2> Descrição do Projeto </h2>
<p align="center">Projeto feito para disciplina de Redes 2 UFF 2021.2</p>
<p>O projeto visa monitorar a temperatura de ambiente, pegar essa informação e enviar para um broker e o broker enviar para o cliente que se subscreve ao tóopico</p>

<h2> Tecnologias </h2>
<p>As tecnologias utilizadas foram :
Hardware
Esp32
Sensor DHT11  
Protoboard
Resistor 
Jumpers

Software

Bibliotecas para MQTT (Paho MQTT), Wi-Fi e DHT11
Protocolo Websockets
Cliente MQTT com página HTML,CSS e JS
Broker da HiveMQ Cloud

</p>

<h2> Instruções </h2>
<p >
- Baixar o <a href="https://www.arduino.cc/en/software">Arduino IDE</a>

- Instalar a seguintes bibliotecas:
  -<a href="https://github.com/me-no-dev/AsyncTCP">Async-TCP</a> </br>
  -<a href="https://github.com/marvinroger/async-mqtt-client"> Async-MQTT-Client</a> </br>
  -DHT11 sensor library by Adafruit (pode ser instalado pela IDE do Arduino)</br>
  -Adafruit Unified Sensor by Adafruit( pode ser instalado pela IDE do Arduino)</br>
  -WiFi(já vem na IDE)</br>

Para instalação das bibliotecas pode fazer download em zip dos repositorios e depois abrir no Arduino IDE em Sketch > Incluir Biblioteca > Adicionar biblioteca ZIP.
</p>

<p>
  Após feito a configuração do ambiente, deverá definir o nome do roteador Wifi e a senha dele no código sensor-mqtt.ino
  O broker padrão é o público da HiveMQ.
  Depois bastará abrir o cliente em navegador, ativar o Esp32 e visualizar as informações sendo entregues ao cliente aberto.
</p>

<h2> Autoria </h2>
<p>
Lucas Fernandes e Lucas Costa.
Baseado no projeto <a href="https://randomnerdtutorials.com/esp32-mqtt-publish-dht11-dht22-arduino/#:~:text=The%20ESP32%20is%20publishing%20temperature,topics%20and%20receive%20the%20readings.">ESP32 MQTT – Publish DHT11/DHT22 by RandomNerdTutorials</a>
</p>
