/*      ***  Temporizador de cierre automático, para una puerta de garaje  ***
  Activa un contacto durante un segundo, cuando se detecta el pulsador de puerta abierta > 'N' segundos
                                   -oOo-
 - El temporizador es ajustable a partir de 60 segundos, con incrementos de 30 segundos 
 __________________________________________________________________________________
                            >>> ATtiny 25/45/85 <<<
               
                                RESET-| U |-Vcc
                                A3/D3-|   |-D2
                                A2/D4-|   |-D1
                                  GND-|   |-D0
                                       ---
           >>> Configurado ATtiny con oscilador interno de 16 MHz <<<

                             Escrito por: J_RPM
                               http://j-rpm.com
                                Mayo de 2023
____________________________________________________________________________________
*/

#define P_ABIERTA 4                           // PIN #4: Entrada del contacto fin de carrera: Puerta abierta
#define LED_PIN 3                             // PIN #3: LED de estado
#define pinRelay 0                            // PIN #0: RELAY para la conexión de la RED
#define con_Lo 1                              // PIN #1: Configuración del temporizador LSB
#define con_Hi 2                              // PIN #2: Configuración del temporizador MSB

int segundos = 60;                            // Segundos de retardo 
int indTiempo;                                // Indicador del retardo configurado
int nSeg;                                     // Contador de segundos

//////////////////////////////////////////////////////////////////////////////////////////////////////
void setup() {
  pinMode(LED_PIN, OUTPUT);         // Salida LED
  pinMode(pinRelay, OUTPUT);        // Salida RELAY
  pinMode(P_ABIERTA, INPUT);        // Entrada contacto fin de carrera
  pinMode(con_Lo, INPUT);           // Entrada LSB para leer la configuración del umbral
  pinMode(con_Hi, INPUT);           // Entrada MSB para leer la configuración del umbral
  
  digitalWrite(pinRelay, LOW);      // Relay = OFF al inicio
  digitalWrite(P_ABIERTA, HIGH);    // Pull-Up
  digitalWrite(con_Lo, HIGH);       // Pull-Up
  digitalWrite(con_Hi, HIGH);       // Pull-Up
  cargaConfig();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
void loop() {
  if(digitalRead (P_ABIERTA) == HIGH) {
    nSeg = segundos;                  // Reinicia los segundos de retardo
    digitalWrite(LED_PIN, LOW);       // Enciende LED         
    delay(500);
    digitalWrite(LED_PIN, HIGH);      // Apaga LED     
    delay(500);

  } else {
    // PUERTA ABIERTA >>> Al superar el tiempo de retardo se cierra el relé durante 1 segundo = CERRAR LA PUERTA  
    nSeg = nSeg -1;
    if (nSeg < 0) {
      nSeg = 4;
      digitalWrite(pinRelay, HIGH);   // Relay = ON >>> CERRAR LA PUERTA 
      digitalWrite(LED_PIN, LOW);     // Enciende LED         
      delay(1000);
      digitalWrite(pinRelay, LOW);    // Relay = Off 
      digitalWrite(LED_PIN, HIGH);    // Apaga LED     

    } else {
      // Destello rápido del LED >>> descontando segundos
      for(int i = 0; i < 5; i++){
        digitalWrite(LED_PIN, LOW);     // Enciende LED         
        delay(100);
        digitalWrite(LED_PIN, HIGH);    // Apaga LED     
        delay(100);
      }
    }
  }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
void cargaConfig() {
  // Se el valor del tiempo de retardo seleccionado
  if((digitalRead (con_Hi) == HIGH) & (digitalRead (con_Lo) == HIGH)){
      indTiempo = 1;                  // 2 puentes abiertos = Temporizador al mínimo
      segundos = 60;
  } else if((digitalRead (con_Hi) == HIGH) & (digitalRead (con_Lo)==LOW)){
      indTiempo = 2;
      segundos = 90;
  } else if((digitalRead (con_Hi) == LOW) & (digitalRead (con_Lo) == HIGH)){
      indTiempo = 3;
      segundos = 120;
  } else {                            // 2 puentes cerrados = Temporizador al máximo
      indTiempo = 4;
      segundos = 150;
  }

  // Indica el retardo configurado con destellos del LED
  for (int i = 0; i < indTiempo; i++){
    digitalWrite(LED_PIN, LOW);     // Enciende LED         
    delay(300);
    digitalWrite(LED_PIN, HIGH);    // Apaga LED     
    delay(300);
  }
  delay(1000);                      // Pausa de 1 segundo
  nSeg = segundos;                  // Inicia el contador de segundos
}
////////////////////// FIN ///////////////////////////////
