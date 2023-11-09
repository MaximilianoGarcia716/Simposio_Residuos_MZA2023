#include <OneWire.h>                 //Se importan las librerías
#include <DallasTemperature.h>
#include <ESP8266WiFi.h>            //Importa la libreria de WiFi para el ESP8266, si se usa un chip de arduino usa WiFi.h en su lugar
#include <ThingSpeak.h>
#define Pin 2                        //Se declara el pin donde se conectará la DATA
#define ChannelNumber1 2228967               //Numero de canal de Thingspeak
#define WriteAPIKey1 "L9KWRY7K4WULSKFF"      //Contraseña API de Thingspeak
#define ssid "Tecnologia"                   //Nombre de red Wifi
#define password "Tecno2022"                //Clave del Wifi



OneWire ourWire(Pin);               //Se establece el pin declarado como bus para la comunicación OneWire
DallasTemperature sensors(&ourWire);//Se llama a la librería DallasTemperature
unsigned long SendInterval=15000;   //Intervalo de envío de datos a Thingspeak en milisegundos
unsigned long DataTimer;            //Temporizador de envío de datos
WiFiClient  client;                 //Se crea un objeto de Wifi para enviar datos por red

//Esta funcion se ejecuta solo una vez
//al iniciarse el Arduino. Aqui se colocan
//las instrucciones que tienen que ver con la configuracion.
void setup() {
  Serial.begin(9600); //Iniciamos el puerto serie
  delay(1000);
  //para comunicarse con la PC
  //a una velocidad de 9600 bauds.
  sensors.begin();                     //Se inician los sensores
  WiFi.mode(WIFI_STA);                 //Se configura el Wifi del ESP
  WiFi.begin(ssid, password);          //Se carga red y contraseña
  while (WiFi.status() != WL_CONNECTED) {//Reintentar mientras no se encuentre conectado a red
    delay(500);
    Serial.print(".");
  }
  Serial.println(".");
  ThingSpeak.begin(client);             //Iniciamos el cliente de Thingspeak
  DataTimer=millis();                   //Iniciamos el timer
  delay(500);
  Serial.println("ESP inicializado");
}

void loop() {
  int Valor_Humedad = analogRead(A0); //Lee el valor del conversor A/D del sensor de humedad
  float Humedad=(645-Valor_Humedad)*100/(645-278); //Ajustamos la escala con 645=0% (Aire humedo) y 278=100% (Agua) de humedad

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
  if((millis()-DataTimer)>SendInterval){ //Verificamos si se ha medido el intervalo de envío de datos
    ThingSpeak.setField(1, Humedad);    //Preparamos dato de humedad para enviar a thingspeak
    ThingSpeak.setField(2, Temperatura);//Preparamos dato de temperatura para enviar a thingspeak
    int x = ThingSpeak.writeFields(ChannelNumber1, WriteAPIKey1); //Enviamos los datos
    if (x == 200) {                                               //Verifica si los datos se enviaron e informa al usuario
      Serial.println("Channel updated");
    }
    else {
      Serial.println("Trouble updating channel, HTTP error code " + String(x));
    }
    DataTimer = millis();
  }
  delay(1000);                         //Se provoca una parada de 1 segundo antes de la próxima lectura
}