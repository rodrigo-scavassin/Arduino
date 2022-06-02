// Aluno: Rodrigo Mattos Scavassin  TIA: 32055374

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
