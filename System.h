#include <SoftwareSerial.h>
#include "Connection.h"
#include "UltrasonicSensor.h"
#include "PIRSensor.h"
#include <ArduinoJson.h>

class System{

  private:

    //variables que definen los objetos que usa la clase System
    BluetoothConnection conn; //Objeto de conexion Bluetooth
    UltrasonicSensor us1; // Objeto que representa un sensor ultrasónico
    UltrasonicSensor us2; // Objeto que representa el segundo sensor ultrasónico
    PIRSensor ps1; //Objeto que representa el sensor PIR


    //variables de la clase System necesarias para el proceso
    int statePIR=0; //mantiene el estado del sensor PIR (para detectar un movimiento, ya que suele detectar antes que los ultrasónicos)
    int stateUS1=0; //mantiene el estado del sensor ultrasónico 1
    int stateUS2=0; //mantiene el estado del sensor ultrasónico 2 
    int cycle; // variable que indica el ciclo, cada X ciclos se restablece el estado del PIR

    int dataPIR=0;
    float dataUS1;
    float dataUS2;


  public:

 

     //metodo de inicializacion del sistema, inicializa los diferente componentes llamando a sus respectivos metodos
     void setUpSystem(){

      statePIR=0;
      stateUS1=0;
      stateUS2=0;
      cycle=0;
      
      //SetUp Connection
      conn.configuration();
      
      //SetUp Ultrasonico
      us1.setUpDistanceSensor(42,30);
      us2.setUpDistanceSensor(47,41);
      
      //SetUp Pir
      ps1.setUpPIR(4);
      conn.configuration();
    }


     //Metodo utilizado en el proceso de pruebas para ver el comportamiento del sistema
     void processData(){
        Serial.println("SENSOR ULTRASONICO 1");
        if(us1.deteccionUltrasonico()){
          Serial.println("\t#MOVIMIENTO: 1");
        }else{
          Serial.println("\t#MOVIMIENTO: 0");
        }
        
        Serial.println("\nSENSOR ULTRASONICO 2");
        if(us2.deteccionUltrasonico()){
          Serial.println("\t#MOVIMIENTO: 1");
        }else{
          Serial.println("\t#MOVIMIENTO: 0");
        }        
        Serial.println("\nSENSOR PIR 1");
        Serial.print("\t#MOVIMIENTO: ");
        Serial.println(ps1.getPIRState());
        
     }

     //metodo que recibe los estados de los sensores y estudia si existe movimiento
     int checkState(){
         int toret=0;//variable de retorno, vale 1 si se considera movimiento, 0 si no

        dataPIR=ps1.getPIRState();
        dataUS1=us1.distance;
        dataUS2=us2.distance;
        int mus1=us1.deteccionUltrasonico();
        int mus2=us2.deteccionUltrasonico();
        
        if(dataPIR==1){
          //establecemos los estados si el sensor PIR detecta algo, si detecta es probable en las tres próximas lecturas los demás detecten algo tambien
          cycle==0;
          statePIR=dataPIR;
          stateUS1=mus1;
          stateUS2=mus2;
        }
        //el estado lo mantenemos durante 3 lecturas, después de esto se reseta hasta que el PIR lo vuelva activar
        else if(cycle==3){
          cycle=0;
          statePIR=0;
          stateUS1=0;
          stateUS2=0;
        }

        //compruebo el estado de los sensores en anteriores momentos 
        if(statePIR || stateUS1 || stateUS2){
          
            //ha habido detección en instantes previos es más probable que haya detección ahora
              if(mus1 || mus2){
                //Si el sensor PIR detecto algo anteriormente, independiemente de lo que detecte ahora, si alguno de los ultrasonicos lo hacen se considera movimiento
                toret=1;
                //resetamos el estado
                statePIR=0;
                stateUS1=0;
                stateUS2=0;
              }
            
          
        }
        if(toret==0 && mus1 && mus2 && dataPIR){
            //si los tres sensores detectan algo se considera movimiento, probabilidad de fallo == probabilidad de que pase un objeto grande
            toret=1;
            cycle=0;
            //resetamos el estado
            statePIR=0;
            stateUS1=0;
            stateUS2=0;
        }
        
        cycle++;
        return toret;
     }


     //Este método se encarga de recoger la información e invocar al objeto de conexión para su envío
     void completeProcess(){
        
        int mov=checkState();
        

        conn.sendSequence(dataUS1,dataUS2,dataPIR,mov);

     }

    
    
};
