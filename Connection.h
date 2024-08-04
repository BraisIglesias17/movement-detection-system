#include <SoftwareSerial.h>


class Connection{
  private:
    virtual void setUpConnection();
  public:
    virtual void sendData(String message);
    virtual void sendChar(char message);
    virtual String getData();
    virtual void sendSequence(float us1, float us2, int pir, int total);
};


class BluetoothConnection: public Connection{

  private:
  SoftwareSerial BT1=SoftwareSerial(10,11); // RX | TX
  int initialized=0;
  void setUpConnection(){
    BT1.begin(9600);
  }
  public:

  //invoca al método privado de configuración
  void configuration(){

    if(initialized==0){
      setUpConnection();
      initialized++;
    }
  }

  //método de envío de una cadena en forma de String
  void sendData(String message){
    if(initialized==0){
      setUpConnection();
      initialized++;
    }

    int i=0;
    char toSend[message.length()];
    for(i=0;i<message.length();i++){
      toSend[i]=message[i];
    }
    BT1.write(toSend);
  }

  //metodo de envio básico de un character
  void sendChar(char c){
    BT1.write(c);
  }


  //metodo que encapsula el envió  de la secuencia de sensores 
  void sendSequence(float us1, float us2, int pir, int total){
            long x;
            char buff[5];
            
            //SECUENCIA DE ENVÍO
            BT1.write("****");
            delay(55);

            x=(int) total;
            String cad=(String) x;
            cad.toCharArray(buff,5);
            BT1.write("d");
            delay(55);
            BT1.write(buff[0]);
            delay(55);

            x=(int) pir;
            cad=(String) x;
            cad.toCharArray(buff,5);
            BT1.write("p");
            delay(55);
            BT1.write(buff[0]);
            
            x=us1*10;
            cad=(String) x;
            cad.toCharArray(buff,5);
            BT1.write("u");
            delay(55);
            BT1.write(buff[0]);
            delay(55);
            BT1.write(buff[1]);
            delay(55);
            BT1.write(buff[2]);
            delay(55);
            BT1.write(buff[3]);
            delay(55);

            x=us2*10;
            cad=(String) x;
            cad.toCharArray(buff,5);
            BT1.write("m");
            delay(55);
            BT1.write(buff[0]);
            delay(55);
            BT1.write(buff[1]);
            delay(55);
            BT1.write(buff[2]);
            delay(55);
            BT1.write(buff[3]);
            delay(55);
          
  }
  

  
  String getData(){
    if(initialized==0){
      setUpConnection();
      initialized++;
    }
    if(BT1.available()){
      char input='\n';
      String toret;
      char message[255];
      int i=0;
      do{
        input=BT1.read();
        message[i++]=input;
        toret+=input;
        
      }while(input!='\n');
      
      return toret;
    }
    return "none";
  }

  
};
