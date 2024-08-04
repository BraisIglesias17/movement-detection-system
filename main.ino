
#include "System.h"


//Objeto que gestiona todo
System sys;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  sys.setUpSystem();
  Serial.println("-- START -- ");
}

void loop() {
  
  sys.completeProcess();
  Serial.println("------------------------------------------");


} 
