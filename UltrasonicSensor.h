#include <Arduino.h>
#include "DistanceSensor.h"

class UltrasonicSensor: public DistanceSensor{

      private:
        
        //pines de conexion
        int trigPin;
        int echoPin;
        
        // defines variables
        float duration; // variable de la duracion del viaje de la onda
        
        bool primeraVez=true;
        
        float distanciaHabitual=24;// variable que indica la distancia habitual que debe detectar el sensor cuando no hay movimiento
        float ultimosValores[10]={0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0};
        int indiceActual=0;

        void verArray(float arr[]){
        int i=0;
        String toret="#ARRAY DE VALORES: \n";
        Serial.print("\t#ARRAY DE VALORES: ");
        for(i=0;i<10;i++){
          Serial.print(arr[i]);
          if(i!=9){
            Serial.print(", ");
          }
          toret+=arr[i];
          toret+=",";
        }
        toret+="\n############\n";
        //Serial.println(toret);
      
        }

        //establece si una distancia es más grande que otra, en funcion del parametro rango, que es un porcentaje (0.5 por ejemplo)
        bool diferenciaSignificante(float v1, float v2, float rango){
          bool toret=false;
          float diferencia=v1-v2;
          if((diferencia>(v1*rango)) || (diferencia<(v1*(-rango)))){
            toret=true;
          }
          return toret;
        }

        //añade la entrada del sensor Ultrasonico al array de ultimos valores y actualiza la distancia habitual 
        void establecerValorHabitual(){
          int i=0;
          float media=distanciaHabitual;
          for(i=9;i>=0;i--){ //BUCLE PARA DESPLAZAR LOS VALORES DEL ARRAY DE ULTIMAS DISTANCIAS
            if(i==0){
              ultimosValores[i]=distance;
            }else{
              ultimosValores[i]=ultimosValores[i-1];
            }
          }
          int j=1;
          for(i=0;i<10;i++){ //BUCLE PAR ACTUALIZAR LA DISTANCIA HABITUAL ACTUAL
              //if(!diferenciaSignificante(distanciaHabitual,ultimosValores[i],0.2)){ //SI EL VALOR SE DIFERENCIA MUCHO DE LA MEDIA ACTUAL NO SE TIENE EN CUENTA
                media+=ultimosValores[i];
                j++;
              //}
          }
          media=media/j;
          distanciaHabitual=media;
        }

                //obtiene la distancia por medio del sensor ultrasonico
        void obtenerDistanciaHC(){
        
           // Establecemos el pin de emisión a 0
          digitalWrite(trigPin, LOW);
          
          // Establecemos el pin de emsión a 1 duranet 10 microsegundos
          digitalWrite(trigPin, HIGH);
          delayMicroseconds(10);
          digitalWrite(trigPin, LOW);
          // Leemos la respuesta (echo), el método pulseIn() devuelve el tiempo que tardó en regresar la señal enviada
          duration = pulseIn(echoPin, HIGH);
          // Teniendo el tiempo y sabiendo la velocidad calculamos la distancia
          distance = duration * 0.034 / 2; // Dividimos entre dos (ya que el tiempo es de ida y vuelta)
          
        }

      public:
        float distance; // variable for the distance measurement
        
        //establece los pines de entrada y salida del sensor
        void setUpDistanceSensor(int emision, int recepcion){
         
          echoPin=recepcion;
          trigPin=emision; 
          pinMode(trigPin,OUTPUT);
          pinMode(echoPin,INPUT);
        }


        //devuelve un 1 si detecta movimiento o un 0 si no, ademas llama a obtenerDistancia() y establecerValorHabitual()
        int deteccionUltrasonico(){
          int toret=0;
          obtenerDistanciaHC();
          if(primeraVez){
            primeraVez=false;
            distanciaHabitual=distance;
          }
        
          establecerValorHabitual();
          
          Serial.print("\n\t#DISTANCIA HABITUAL: ");
          Serial.println(distanciaHabitual);
          verArray(ultimosValores);
          Serial.print("\n\t#DISTANCIA: ");
          Serial.println(distance);
          
          if(diferenciaSignificante(distanciaHabitual,distance,0.2)){
            toret=1;
          }
          return toret; 
        }


};
