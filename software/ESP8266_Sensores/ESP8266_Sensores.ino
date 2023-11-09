#include <OneWire.h>                 //Se importan las librerías
#include <DallasTemperature.h>
#define Pin 2                        //Se declara el pin donde se conectará la DATA

OneWire ourWire(Pin);                //Se establece el pin declarado como bus para la comunicación OneWire
DallasTemperature sensors(&ourWire); //Se llama a la librería DallasTemperature

//Esta funcion se ejecuta solo una vez
//al iniciarse el Arduino. Aqui se colocan
//las instrucciones que tienen que ver con la configuracion.
void setup() {
  Serial.begin(9600); //Iniciamos el puerto serie
  //para comunicarse con la PC
  //a una velocidad de 9600 bauds.
  sensors.begin();                     //Se inician los sensores
  Serial.println("ESP inicializado");
}

void loop() {
  int Valor_Humedad = analogRead(A0); //Lee el valor del conversor A/D del sensor de humedad
  float Humedad=(645-Valor_Humedad)*100/(645-278); //Ajustamos la escala con 645=0% (Aire) y 278=100% (Agua) de humedad

  sensors.requestTemperatures();       //Prepara el sensor de temperatura para la lectura
  float Temperatura = sensors.getTempCByIndex(0); //Se obtiene el dato de temperatura
  
  if(!(Valor_Humedad<200)){ //Si no se detecta sensor de humedad no imprime datos de humedad
    Serial.print("Humedad: ");
    Serial.println(Humedad); //Se lee e imprime el dato del sensor de humedad
  }
  
  if(!(Temperatura<-100)){ //Si no se detecta sensor de temperatura no imprime datos de temperatura
    Serial.print(Temperatura); //Se lee e imprime la temperatura en grados Centigrados
    Serial.println(" Grados Centigrados");
  }
  Serial.println("Still working");
  delay(1000);                         //Se provoca una parada de 1 segundo antes de la próxima lectura
}