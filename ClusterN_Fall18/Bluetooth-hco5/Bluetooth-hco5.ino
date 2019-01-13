// basic functionality of serial bluetooth with hc05 module
// Vcc > Arduino 5V, Gnd > Arduino gnd, TX > Arduino RX, RX > 1k/2k v divider for Arduino TX @ 3.3V
// one led toggled with  '1' and '0' commands sent over Bluetooth serial
// NOTE: MUST disconnect Arduino RX/TX pins in order to write code over serial. Find workaround!

void blinkTest(int OUT, int t, int reps);

int ledPin = 15;
int state = 0;

void setup() {
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
  
  blinkTest(ledPin, 500, 4);
  
  Serial.begin(9600); //HC-05 default communication
  Serial.println("Hello!");
}

void loop() {
  if(Serial.available() > 0){ // if data coming in over serial
    state = Serial.read();
  }
  
  if (state == '0'){
    digitalWrite(ledPin, LOW);
    Serial.println("LED: OFF");
    state = 0;
  }

  else if (state == '1'){
    digitalWrite(ledPin, HIGH);
    Serial.println("LED:ON");
    state = 0;
  }
}

void blinkTest(int OUT, int t, int reps){
  for(int i = 0; i < reps; i++){
    digitalWrite(OUT, HIGH);
    delay(t);
    digitalWrite(OUT,LOW);
    delay(t);
  }
}
