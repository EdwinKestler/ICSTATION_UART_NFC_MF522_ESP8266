#include <SoftwareSerial.h>

SoftwareSerial mySerial(5, 4); // Rx, Tx
unsigned long UInterval = 1000UL; //Variable configurable remotamente sobre el interbalo de publicacion
unsigned long last_State_Update;                                        //Variable para llevar conteo del tiempo desde la ultima publicacion


void setup()
{
  delay(2000);
  Serial.begin(115200);
  Serial.println("Serial number will be displayed here if a card is detected by the module:\n");
  delay(500);
  // set the data rate for the SoftwareSerial port
  mySerial.begin(115200);
  delay(500);
  mySerial.write(0x01); //Send the command to RFID, please refer to RFID manual
}

byte NormalModeON[] = {0x01};                           //Comando para poner al modulo en modo de aceptar commandos
byte Request [] = {0x7f,0x03,0x52,0xf7};                //Comando para inciar el pedido de datos
byte AntiColl[] = {0x7f,0x04,0xf7};                     //Comando para leer el numero de serie de la tarjeta NFC
byte Halt[] = {0x7f,0x0b,0xf7};                         //Comando para finalizar la lectura de datos de la tarjetas
byte NormalModeOFF[] = {0x7f,0x0c,0xf7};                //Comando para finalizar el modo de aceptar comandos 


void RequestCmd(){
  //Serial.println("send Request comand");
  for(byte i =0;i<sizeof(Request);i++)(mySerial.write(Request[i]));
  delay(100);
}

void AntiCollCmd(){
  //Serial.println("send AntiColl comand");
  for(byte i =0;i<sizeof(AntiColl);i++)(mySerial.write(AntiColl[i]));
  delay(100); 
}
    
void HaltCmd(){
  //Serial.println("send Halt comand");
  for(byte i =0;i<sizeof(Halt);i++)(mySerial.write(Halt[i]));
  delay(100);
}

void loop() // run over and over
{
   if(millis() - last_State_Update > 10*UInterval) {
      Serial.println("inciando lectura de tarjeta");
      last_State_Update = millis(); //Actulizar la ultima hora de envio
      Serial.println("enviar comando Request");
      RequestCmd();
      delay(200);
      Serial.println("enviar comando Anticoll");
      AntiCollCmd();
      delay(100);
      while (mySerial.available() >0){
        Serial.print(mySerial.read(),HEX); //Display the Serial Number in HEX
      }
      delay(500);
      Serial.println("enviar comando Halt");
      HaltCmd();      
      delay(500);
   }
}
