
//Interfaz del sensor PIR
class PIRSensor{
  private:
    int IN;
  public:
    void setUpPIR(int pinInput);
    int getPIRState();
};
