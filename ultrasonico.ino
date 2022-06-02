// Aluno: Rodrigo Mattos Scavassin  TIA: 32055374

//Fazendo a conexão com o mqtt
#include <ESP8266WiFi.h> //inclui suporte ao NodeMCU
#include <PubSubClient.h> //inclui suporte ao MQTT no HiveMQ Cloud

// Configuracoes da rede WiFi
const char* ssid = "AndroidAP2834"; //SSID da rede WiFi
const char* password = "abih1404"; //senha da rede WiFi

const char* mqtt_server = "broker.mqtt-dashboard.com"; //URL do broker MQTT
const int mqtt_server_port = 1883; //porta do broker MQTT

// Variaveis globais e objetos
#define MSG_BUFFER_SIZE (500) //define MSG_BUFFER_SIZE como 500
WiFiClient client; //cria o objeto client
PubSubClient mqtt_client(client); //cria o objeto mqtt_client
long lastMsg = 0;

String clientID = "ESP8266Client-"; //identificacao do cliente

String topicoPrefixo = "MACK32055374"; //para o prefixo do topico, utilizar MACK seguido do TIA
String topicoTodos = topicoPrefixo + "/#"; //para retornar todos os topicos
String topico_0 = topicoPrefixo + "/hello"; //topico para teste
String mensagem_0 = "NodeMCU Connected"; //mensagem para o topico 0
String topico_1 = topicoPrefixo + "/ocupacao"; //topico para a ocupação do quarto
String mensagem_1 = ""; //mensagem para o topico 1
String mensagemTemp = ""; //armazena temporariamente as mensagens recebidas via MQTT


// Funcao para configurar a conexao WiFi
void setup_wifi() {
  //WiFi.mode(WIFI_STA); //ativa o modo de estacao
  WiFi.begin(ssid, password);
  Serial.print("Connecting to ");
  Serial.println(ssid);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println();
  Serial.print("Connected, IP address: ");
  Serial.println(WiFi.localIP());
}

// Funcao para receber mensagens
void callback(char* topic, byte* payload, unsigned int length) {
  mensagemTemp = "";
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
    mensagemTemp += (char)payload[i];
  }

  if (String(topic) == topico_1) {
    mensagem_1 = mensagemTemp;
  }

  Serial.println();

  // Switch on the LED if the first character is present
  if ((char)payload[0] != NULL) {
    digitalWrite(LED_BUILTIN, LOW); // Turn the LED on
    // Note that LOW is the voltage level but actually the LED is on;
    // this is because it is active low on the ESP-01
    delay(500);
    digitalWrite(LED_BUILTIN, HIGH); // Turn the LED off by making the voltage HIGH
  } else {
    digitalWrite(LED_BUILTIN, HIGH); // Turn the LED off by making the voltage HIGH
  }
}

// Funcao para conectar no broker MQTT
void reconnect() {
  // Loop until we’re reconnected
  while (!mqtt_client.connected()) {
    Serial.print("Attempting MQTT connection…");

  // Create a random client ID
    randomSeed(micros()); //inicializa a semente do gerador de numeros aleatorios
    clientID += String(random(0xffff), HEX);

  // Attempt to connect
    if (mqtt_client.connect(clientID.c_str())) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      mqtt_client.publish(topico_0.c_str(), mensagem_0.c_str());
      // ... and resubscribe
      mqtt_client.subscribe(topicoTodos.c_str());
    } else {
      Serial.print("failed, rc=");
      Serial.print(mqtt_client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
} 


//Carrega a biblioteca do sensor ultrassonico
#include <Ultrasonic.h>

// Definindo variavel do contador de pessoas
int ocupacao = 0;
#define distanciaDeteccao 30
#define DETECTADO 1
#define tempoEspera 1000

// DECLARAÇÃO DE VARIÁVEIS
boolean sensor1Ativado;
boolean sensor2Ativado;

int estadoSensor1 = 0;
int estadoSensor2 = 0;

unsigned long controleTempo1;
unsigned long controleTempo2;

// Contador digital
 
int segE = 9;
int segD = 8;
int segC = 7;
int segB = 10;
int segA = 11;
int segF = 12;
int segG = 13;

// Acendimento do led

int LED = 2;

//Define os pinos para o trigger e echo dos sensores
int trigger1 = 3;
int echo1 = 4;
int trigger2 = 5;
int echo2 = 6;

#define Sensor1_  1
#define Sensor2_ -1
 
//Inicializa o sensor nos pinos definidos acima
Ultrasonic ultrasonic1(trigger1, echo1);
Ultrasonic ultrasonic2(trigger2, echo2);
 
void setup()
{

  setup_wifi();

  mqtt_client.setServer(mqtt_server, mqtt_server_port); //configura a conexao com o broker MQTT
  mqtt_client.setCallback(callback); //configura o recebimento de mensagens

  // Inicialização
  Serial.begin(9600);
  Serial.println("Lendo dados do sensor...");

  pinMode(segE, OUTPUT);
  pinMode(segD, OUTPUT);
  pinMode(segC, OUTPUT);
  pinMode(segB, OUTPUT);
  pinMode(segA, OUTPUT);
  pinMode(segF, OUTPUT);
  pinMode(segG, OUTPUT);

  pinMode(LED, OUTPUT);

}
 
void loop()
{
  if (!mqtt_client.connected()) {
    reconnect();
  }
  mqtt_client.loop(); //processa as mensagens e mantem a conexao com o broker MQTT
  
  //Le as informacoes do sensor, em cm e pol
  float cmMsec1, cmMsec2;
  long microsec1 = ultrasonic1.timing();
  cmMsec1 = ultrasonic1.convert(microsec1, Ultrasonic::CM);

  long microsec2 = ultrasonic2.timing();
  cmMsec2 = ultrasonic2.convert(microsec2, Ultrasonic::CM);

  // logica de teste
  if(cmMsec1 < distanciaDeteccao){
    sensor1Ativado = true;
  }else{
    sensor1Ativado = false;
  }

    // verificar se sensor é ativado
  if(cmMsec2 < distanciaDeteccao){
    sensor2Ativado = true;
  }else{
    sensor2Ativado = false;
  }

  // Cálculo da ocupacao
  if (!sensor1Ativado && !sensor2Ativado){
    estadoSensor1 = 0;
    estadoSensor2 = 0;
  }
  
  if (sensor1Ativado && !sensor2Ativado){
    estadoSensor1 += 1;
  }
  if (!sensor1Ativado && sensor2Ativado){
    estadoSensor2 += 1;
  }
  if (estadoSensor1 > 0 && estadoSensor2 > 0){
    if (estadoSensor1 > estadoSensor2){
      ocupacao += 1;
    } else{
      ocupacao -= 1;
    }
    estadoSensor1 = 0;
    estadoSensor2 = 0;
  }

  
  //Exibe informacoes no serial monitor
  Serial.print("Distancia1 em cm: ");
  Serial.println(cmMsec1);
  Serial.print("Distancia2 em cm: ");
  Serial.println(cmMsec2);
  Serial.print("OCUPAÇÃO: ");
  Serial.println(ocupacao);
  Serial.print("Estado1: ");
  Serial.println(estadoSensor1);
  Serial.print("Estado2: ");
  Serial.println(estadoSensor2);

  String ocupacaoString = String(ocupacao);
  mensagem_1 = ocupacaoString;

  //Acende o LED

  if (ocupacao > 0){
    digitalWrite(LED, HIGH);
  } else{
    digitalWrite(LED, LOW);
  }

  // mostra no contador a quantidade de pessoas
  switch(ocupacao){  
    case 1:
    acende1();
    break;

    case 2:
    acende2();
    break;

    case 3:
    acende3();
    break;

    case 4:
    acende4();
    break;

    case 5:
    acende5();
    break;

    case 6:
    acende6();
    break;

    case 7:
    acende7();
    break;

    case 8:
    acende8();
    break;

    case 9:
    acende9();
    break;

    default:
    acende0();
    break;
  }

    //Publica mensagem
  long now = millis();
  if (now - lastMsg > 2000) {
    lastMsg = now;
    mqtt_client.publish(topico_1.c_str(), mensagem_1.c_str());
  }

}
 //definição dos números no visor
void acende0()
{
  digitalWrite(segE, LOW);
  digitalWrite(segD, LOW);
  digitalWrite(segC, LOW);
  digitalWrite(segB, LOW);
  digitalWrite(segA, LOW);
  digitalWrite(segF, LOW);
  digitalWrite(segG, HIGH); 
}
void acende1()
{
  digitalWrite(segE, HIGH);
  digitalWrite(segD, HIGH);
  digitalWrite(segC, LOW);
  digitalWrite(segB, LOW);
  digitalWrite(segA, HIGH);
  digitalWrite(segF, HIGH);
  digitalWrite(segG, HIGH); 
}
void acende2()
{
  digitalWrite(segE, LOW);
  digitalWrite(segD, LOW);
  digitalWrite(segC, HIGH);
  digitalWrite(segB, LOW);
  digitalWrite(segA, LOW);
  digitalWrite(segF, HIGH);
  digitalWrite(segG, LOW); 
}
void acende3()
{
  digitalWrite(segE, HIGH);
  digitalWrite(segD, LOW);
  digitalWrite(segC, LOW);
  digitalWrite(segB, LOW);
  digitalWrite(segA, LOW);
  digitalWrite(segF, HIGH);
  digitalWrite(segG, LOW);
}
void acende4()
{
  digitalWrite(segE, HIGH);
  digitalWrite(segD, HIGH);
  digitalWrite(segC, LOW);
  digitalWrite(segB, LOW);
  digitalWrite(segA, HIGH);
  digitalWrite(segF, LOW);
  digitalWrite(segG, LOW);
}
void acende5()
{
  digitalWrite(segE, HIGH);
  digitalWrite(segD, LOW);
  digitalWrite(segC, LOW);
  digitalWrite(segB, HIGH);
  digitalWrite(segA, LOW);
  digitalWrite(segF, LOW);
  digitalWrite(segG, LOW);
}
void acende6()
{
  digitalWrite(segE, LOW);
  digitalWrite(segD, LOW);
  digitalWrite(segC, LOW);
  digitalWrite(segB, HIGH);
  digitalWrite(segA, LOW);
  digitalWrite(segF, LOW);
  digitalWrite(segG, LOW);
}
void acende7()
{
  digitalWrite(segE, HIGH);
  digitalWrite(segD, HIGH);
  digitalWrite(segC, LOW);
  digitalWrite(segB, LOW);
  digitalWrite(segA, LOW);
  digitalWrite(segF, HIGH);
  digitalWrite(segG, HIGH);
}
void acende8()
{
  digitalWrite(segE, LOW);
  digitalWrite(segD, LOW);
  digitalWrite(segC, LOW);
  digitalWrite(segB, LOW);
  digitalWrite(segA, LOW);
  digitalWrite(segF, LOW);
  digitalWrite(segG, LOW);
}
void acende9()
{
  digitalWrite(segE, HIGH);
  digitalWrite(segD, LOW);
  digitalWrite(segC, LOW);
  digitalWrite(segB, LOW);
  digitalWrite(segA, LOW);
  digitalWrite(segF, LOW);
  digitalWrite(segG, LOW);
}
