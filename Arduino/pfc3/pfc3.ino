#include <Arduino.h>
#include <WiFlySerial.h>
#include <SoftwareSerial.h>
#include <Streaming.h>
#include <ShiftBrite.h>
#include <Color.h>

#define BUFFSIZE 128

// LED parameters
int datapin = 5;
int latchpin = 6;
int enablepin = 10;
int clockpin = 11;

int u = 50;
// LED variables

boolean started = false;
// Indica si tenemos un N jugador en el circuito
boolean isPlayer[] = { false, false, false, false, false, false, false };
// Los colores de cada uno de los jugadores (1-7)
unsigned char colors[] = { 0,0,u, 0,u,0, 0,u,u, u,0,0, u,0,u, u,u,0, u,u,u };
// Posicion de cada uno de los jugadores
unsigned char posicion[] = { 0, 0, 0, 0, 0, 0, 0 };
// Iterara sobre los jugadores para ir mostrandolos en el circuito de leds
unsigned char cont = 0;
// Data que recibo del wifi
unsigned char data = 0;
// Numero de leds
unsigned char nLeds = 0;
// Codigo arbitrario
unsigned char code = 0;
// Tiempo en segundos
unsigned long time = millis();

// Wifi parameters

// Casa
char ssid[] = "WLAN_00";
char passphrase[] = "70726635494b6736596a5258635375434542314a";

// oficina Enric
//char ssid[] = "OSIES";
//char passphrase[] = "dsPIC30F4011";

// Wifi variables
char *ip = (char *)malloc(BUFFSIZE);
char *ssidTest = (char *) malloc(BUFFSIZE);
char *mac = (char *) malloc(BUFFSIZE);
boolean success = false;
boolean needreconnect = false;
boolean needreset = false;
boolean hasClient;
boolean connection;

// Connect the WiFly TX pin to the Arduino RX pin  (Transmit from WiFly-> Receive into Arduino)
// Connect the WiFly RX pin to the Arduino TX pin  (Transmit from Arduino-> Receive into WiFly)
// 
// Connect the WiFly GND pin to an Arduino GND pin
// Finally, connect the WiFly BATT pin to the 3.3V pin (NOT the 5v pin)

#define ARDUINO_RX_PIN  2
#define ARDUINO_TX_PIN  3

// Inicializamos las librerias del wifi y el LED
WiFlySerial wifi(ARDUINO_RX_PIN, ARDUINO_TX_PIN);
ShiftBrite shift(datapin, latchpin, enablepin, clockpin); //construct

boolean reconnect(){
  //initial the needreconnect is false;
  needreconnect = false;
  Serial.println("Start to connect to WIFI...");

  char *macTest = wifi.getMAC(mac,BUFFSIZE);
  Serial.println("MAC-Adrress:");
  Serial.println(macTest);

  success = wifi.setAuthMode( WIFLY_AUTH_WPA2_PSK);
  if(success) Serial.println("--> Auth Mode set!");
  else {
    Serial.println("--> FAILED to set Auth Mode!");
    needreconnect = true;
  }

  success = wifi.setJoinMode(  WIFLY_JOIN_AUTO );
  if(success) Serial.println("--> Join Mode set!");
  else {
    Serial.println("--> FAILED to set Join Mode!");
    needreconnect = true;  
}

  success = wifi.setChannel("9");
//  success = wifi.setChannel("13");
  if(success) Serial.println("--> Channel set!");
  else {
    Serial.println("--> FAILED to set Channel!");
    needreconnect = true;
  }
  
  success = wifi.setPassphrase(passphrase);
  if(success) Serial.println("--> Passphrase set!");
  else {
    Serial.println("--> FAILED to set Passphrase!");
    needreconnect = true;
  }
  
  success = wifi.setSSID(ssid);
  if(success) Serial.println("--> SSID set!");
  else {
    Serial.println("--> FAILED to set SSID!");
    needreconnect = true; 
 }
 //static IP because DHCP was not working
//  success = wifi.setIP("147.83.173.40");
  success = wifi.setIP("192.168.1.41");
  if(success) Serial.println("--> IP set!");
  else {
    Serial.println("--> FAILED to set IP!");
    needreconnect = true;
  }
  
  success = wifi.setDHCPMode( WIFLY_DHCP_OFF );
  if(success) Serial.println("--> DHCP Mode set!");
  else {
    Serial.println("--> FAILED to set DHCP Mode!");
    needreconnect = true;
  }
  
  success = wifi.setLocalPort(2000);
  if(success) Serial.println("--> Port set!");
  else {
    Serial.println("--> FAILED to set Port!");
    needreconnect = true;
  }
  
  success = wifi.join();
  if(success) Serial.println("--> WIFI JOINED!");
  else {
    Serial.println("--> FAILED to join WIFI!");
    needreconnect = true;
  }
  return !needreconnect;
}

boolean factoryReset(){
  needreset = false;
  //success = wifi.SendCommandSimple("factory RESET","Set Factoy Defaults");
  success = wifi.SendCommandSimple("factory RESET","Defaults");
  if(success)Serial.println("execute factory RESET");
  else {
    Serial.println("--> FAILED to execute factory RESET");
    needreset = true;
  }
  success = wifi.SendCommandSimple("reboot"," ");
  if(success)Serial.println("execute reboot");
  else {
    Serial.println("-->FAILED to reboot");
    needreset = true;
  }  
  return !needreset;
}

void setup(){

  Serial.begin(9600);
  while(!factoryReset()); // factoryReset till resetted
  while(!reconnect()); // reconnect till connected

  //Print SSID
  char *rv_ssidTest = wifi.getSSID(ssidTest,BUFFSIZE);
  Serial.println(rv_ssidTest);

  //Print IP
  char *rv_ip = wifi.getIP(ip,BUFFSIZE);
  Serial.println(rv_ip);

  free(ip);
  free(ssidTest);
  free(mac);
  
  // En vez de jugar con el serial, jugar con un led y el estado
  // amarillo -> escuchando // verde -> conectado // rojo -> error

  Serial.println("Starting wifi Listener...");
  boolean begin = wifi.begin();
  
  if(begin) Serial.println("begin listening...");
  else Serial.println("Listening error!");
  
  Serial.println("Waiting for Clients to connect...");
  connection = wifi.serveConnection();

  if(connection) Serial.println("Client connected!");
  else Serial.println("Internal error!");

}

void loop(){

  // if not connected restart link
  while (!wifi.isAssociated() ) {
    Serial.print("Device Connected");
    Serial.println(wifi.isAssociated());
    while(!reconnect()); // till connected
  }
  
  hasClient = wifi.isConnectionOpen();
  
  if (!hasClient) {
  
      //Todo Somestop Action in your codes, before the connection should be closed
      
      wifi.drain();
      success = wifi.begin();// important to serveConnection
      if(success) Serial.println("begin listening...");
      else Serial.println("Listening error!");
      
      Serial.println("Waiting for Clients to connect...");
      connection = wifi.serveConnection();

      if(connection) Serial.println("Client connected!");
      else Serial.println("Internal error!");
    
  } else {
      // Todo Your aduino application, what ever you want to do with the data
      if(wifi.available() > 0){
	 data = wifi.read();

         Serial.print("data: ");
         Serial.println(data);
         
         // CODES:
         //  0: CODE
         //  1: N LEDS
         //  2: JUGADOR ENTRA
         //  3: EMPIEZA LA CARRERA
         //  4: JUGADOR AVANZA
         //  5: JUGADOR TERMINA
         //  6: JUGADOR SALE
         
         if (code == 0) {
           if (data <= 6) {
             code = data;
           } else {
             Serial.println("ERROR: CODE > 6");
             code = 0;
           }
         } else if (code == 1) { // N LEDS
           nLeds = data;
           code = 0;
         } else if (code == 2) { // JUGADOR ENTRA
           isPlayer[data] = true;
           vaciar();
           llenarLento(data);
           vaciarLento();
           for (int i=0; i < 7; i++) {
             if (isPlayer[i]) shift.sendColor(colors[i*3], colors[i*3+1], colors[i*3+2]);
           }
           code = 0;
         } else if (code == 3) { // EMPIEZA LA CARRERA
           started = true;
           posicion[data] = 0;
           code = 0;
         } else if (code == 4) { // JUGADOR AVANZA
           posicion[data] = posicion[data] + 1;
           Serial.print("posicion: ");
           Serial.println(posicion[data]);
           code = 0;
         } else if (code == 5) { // JUGADOR TERMINA
           started = false;
           vaciar();
           llenarLento(data);
           vaciarLento();
           llenarLento(data);
           vaciarLento();
           llenarLento(data);
           code = 0;
         } else if (code == 6) { // JUGADOR SALE
           started = false;
           posicion[data] = 0;
           isPlayer[data] = false;               
           code = 0;
         }
         
      }

      if (started && millis() - time > 100) {
        time = millis();

        if (isPlayer[cont]) {
          //vaciar();
          for (int i = nLeds-1; i >= 0; i--) {
            if (i == posicion[cont]) {
              shift.sendColor(colors[cont*3], colors[cont*3+1], colors[cont*3+2]);
            }
            else {
              shift.sendColor(0, 0, 0);
            }
          }
        }
        for (int i = 1; i < 7; i++) {
          if (isPlayer[(cont+i)%7]) {
            cont = (cont+i)%7;
            break;
          }
        }
        //cont = (cont + 1) % 7;
      }

  }

}

void vaciar() {
  for (int i=0; i < nLeds; i++) {
    shift.sendColor(0, 0, 0);
  }
}

void llenar(int player) {
  for (int i=0; i < nLeds; i++) {
    shift.sendColor(colors[data*3], colors[data*3+1], colors[data*3+2]);
  }
}

void vaciarLento() {
  for (int i=0; i < nLeds; i++) {
    shift.sendColor(0, 0, 0);
    delay(100);
  }
}

void llenarLento(int player) {
  for (int i=0; i < nLeds; i++) {
    shift.sendColor(colors[data*3], colors[data*3+1], colors[data*3+2]);
    delay(100);
  }
}
