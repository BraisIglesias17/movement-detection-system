#include "PIRSensor.h"
#include <Arduino.h>

//Implementación de la interfaz del PIRSensor


//inicializacion del pin del sensor
void PIRSensor::setUpPIR(int pinInput){
  IN=pinInput;
  pinMode(IN,INPUT);
}


//metodo que recibe el estado del sensor PIR: 1 detecta movimiento, 0 no detecta movimiento
int PIRSensor::getPIRState(){
  return digitalRead(IN);
}
