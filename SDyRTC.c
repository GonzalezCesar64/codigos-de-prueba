//lo escribo yo cesar
//sy cesar
#include <SD.h>
#include <SPI.h>
#include <Wire.h>
#include "RTClib.h"

RTC_DS1307 RTC;
int estadoPir;
int sensorPir = 7;
int rele = 8;
String ano;
String dia;
String mes;
String hora;
String minutos;                // variables que me sirven para guardar la hora
String segundos;
String NOMBREARCHIVO;          // variable para asignar el nombre del archivo
String imprimir;
String fecha;

const int chipSelect = 4;

bool banderaSirenaEncendida = true;    // variable buleana

void setup() {
  // put your setup code here, to run once:

  Serial.begin(9600);
  RTC.begin();
  if ((! RTC.isrunning()))
  {
    Serial.print("RTC NO ESTA ACTIVADO");
    //ESTO ES PARA AJUSTAR LA HORA LA PRIMERA VEZ
    RTC.adjust(DateTime(__DATE__, __TIME__));                      // esto solo lo pnemos la primera vez que mandamos el codigo, es para ajustar la hora
  }
  Serial.print("Iniciando Tarjeta SD");
  if (!SD.begin(chipSelect)) {
    Serial.print("no se pudo inicializar la SD o no est conectada");
  }
  else {
    Serial.print("SD inicializada");
  }

  pinMode(sensorPir, INPUT_PULLUP);
}

void loop() {
  estadoPir = digitalRead(sensorPir);

  DateTime now = RTC.now();      // guardo todo el tiomepo en una variable llamada now.

  // luego a esa variable la voy desglozando por fecha(now.year(),now.month(),now.day())
  // lo mismo hago con la hora,(now.hour(),now.minute(),now.second())
  // esos datos los guardamos en las variable de tipo string que hicmos antes

  ano = now.year();
  mes = now.month();
  dia = now.day();
  hora = now.hour();
  minutos = now.minute();
  segundos = now.second();
  fecha = ano + "/" + mes + "/" + dia;   // armo la fecha juntando las variables con los datos
  hora = hora + ":" + minutos + ":" + "segundos"; // armo la hora juntando las variables con los datos
  //////////////////////////////////////////////
  NOMBREARCHIVO =  "SIRENA.txt";     // asi se va a llamar el nombre del archivo donde vamos a guardar los datos en la memoria SD siempre lleva txt
  ///////////////////////////////////////////////

  //if(sensorPir == HIGH){
  //ENCIENDO EL RELE
  //IMPRIMO QUE ESTA ENCENDIDA LA ALARMA

  if (digitalRead(sensorPir) == LOW && banderaSirenaEncendida == true) // si el sensor pir detecta movimiento entra en el if con el valor booleano que le pusimos 
  //con el profe, pq el sensor detecta por unos 3 segundos, por 3 segundos va a estar mandando true el digitalread(sensorPir), si pasa eso la hora se va 
  // a imprimir 3 segundundos seguidos o por el tiempo en el que el sensor detecte movimiento, entra en el condicional if cuando el sensor detecta movimiento
  // y la bandera sea true(al principio la declaramos true), cuando entra, la bandera vale 0 para que no vuelva a entrar
  {
    Serial.println("sirena encendida"); //imprime esto en el serial
    digitalRead(rele  ) //prende el rele
    Serial.println("rele activado"); //imprime esto en el serial
    delay(100);
    banderaSirenaEncendida = false; // declara la bandera false para que no vuelva a entrar el if
    //////////////////////////////////////
    imprimir = fecha + " " + hora + " " + minutos + " " + segundos + " " + "se activo la sirena, Y VINO EL PUTO DE MICHE";
    // esto es lo que se imprime en la memoria, la fecha, la hora, y un texto.
    //////////////////////////////////////



    File dataFile = SD.open(NOMBREARCHIVO, FILE_WRITE); //crea el archivo Nombrearchivo que declaramos que sea "SIRENA.txt" 
    if (dataFile) {
      dataFile.println(imprimir);  // guardo en la memoria SD lo que haya en la variable IMPRIMIR(fecha,hora,y texto)
      dataFile.close();// lo cerramos, SIEMPRE HACERLO
      Serial.println(imprimir); // lo imprimimos en el serial  
    }
    else {
      Serial.println("error al intentar escribir en el archivo");
    }
  }

  if (digitalRead(sensorPir) == HIGH)
  {
    banderaSirenaEncendida = true;
    //Serial.println("sirena apagada");
  }
}
