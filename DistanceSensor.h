class DistanceSensor{
  public:
    void setUpDistanceSensor(int emision, int recepcion);
    void obtenerDistanciaHC();
    int deteccionUltrasonico(); 
  private:
    bool diferenciaSignificante(float v1, float v2, float rango);
    void establecerValorHabitual();
    void verArray(float arr[]);
};
