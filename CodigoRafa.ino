#define MIN_SPEED 400
#define MAX_SPEED 15


#define enPin  24
#define dirPin 28
#define stepPin 26


String inString = "";


int speed=0;
int dir=0;
bool autom= false;


void setup() {
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);
  pinMode(enPin, OUTPUT);
  pinMode(A5, INPUT);
  digitalWrite(enPin, HIGH);
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  // send an intro:
  Serial.println("\n\nString toInt():");
  Serial.println();
}
unsigned long previousMillis = 0;        // will store last time LED was updated

// constants won't change:
const long interval = 500;           // interval at which to blink (milliseconds)


void loop() {
 while (Serial.available() > 0) {
    int inChar = Serial.read();
    inString += (char)inChar;
    // if you get a newline, print the string, then the string's value:
    if (inChar == '\n') {
      Serial.println(inString);
      inString.trim();
      if (inString.equalsIgnoreCase("S")){
        Serial.println("Iniciando Rotacion, Velocidad Minima");
        autom=false;        
        speed=MIN_SPEED;
        digitalWrite(enPin,LOW);
      }
      else if (inString.equalsIgnoreCase("S0")){
        Serial.println("Deteniendo Motor");
        autom=false;
        digitalWrite(enPin,HIGH);

      }
      else if (inString.equalsIgnoreCase("C1")){
        Serial.println("Sentido de Rotacion 1");
        dir=0;
      }
      else if (inString.equalsIgnoreCase("C2")){
        Serial.println("Sentido de Rotacion 2");
        dir=1;
      }else if (inString.startsWith("M") || inString.startsWith("m")){
        int x = inString.substring(1).toInt();
        autom=false;
        Serial.print(x);
        if (x<= 100){
          speed=map(x, 0, 100, MIN_SPEED, MAX_SPEED);
          Serial.print("% Vel: ");
          Serial.println(speed);
        }
        else {
          Serial.println("Ingrese la Velocidad en porcentajes (0 - 100)");
          
        }
      }
      else if (inString.equalsIgnoreCase("A"))
      {
        Serial.println("Iniciando Modo Automatico");
        autom=true;
      }
      // clear the string for new input:
      inString = "";
    }
  }
  if (autom){
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= interval) {
        // save the last time you blinked the LED
        previousMillis = currentMillis;
        int x=map(analogRead(A5),0,1023, MIN_SPEED, MAX_SPEED);
        if (x>(speed+10) || x<(speed-10)){
          speed= x;
          
          Serial.print("Velocidad: ");
          Serial.println(speed);
        }
      }

  }
  digitalWrite(dirPin,dir);
  digitalWrite(stepPin, HIGH);
  delayMicroseconds(speed);
  digitalWrite(stepPin, LOW);
  delayMicroseconds(speed);

  
}