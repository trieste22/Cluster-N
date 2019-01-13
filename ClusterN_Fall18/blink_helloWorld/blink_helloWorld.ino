//LED (or vibrating motor) 8-compass point output test
// includes blink circle to ensure all working
//    and sendData() fn to send array of outputs to turn on/off
// current as of 10.13.18

void blinkTest(int OUT, int t, int reps);
void sendData(int doIt[], bool cmd);


int led = 13;
int n = 2;
int ne = 3;
int e = 4;
int se = 5;
int s = 6;
int sw = 7;
int w = 8;
int nw = 9;

int outs[] = {n, ne, e, se, s, sw, w, nw};
int len = sizeof(outs)/sizeof(outs[0]);
bool go = 1;

void setup() {
  pinMode(led, OUTPUT);
//  blinkTest(led, 150,len);
//  Serial.print("Hello,world");
  
  for(int i = 0; i < len; i++){
    pinMode(outs[i], OUTPUT);
    digitalWrite(outs[i], HIGH);
    delay(25);
  }
  delay(1000);
  for(int8_t i = 0; i < len; i++){
    digitalWrite(outs[i], LOW);
  }
//  Serial.begin(9600);

}

void loop() {
  int practice[] = {1,2,3,5,7};
  int practice3[] = {n,s,e,w};
  int practice2[] = {n,ne, s,sw, w};
  if(go){
    len = sizeof(practice2)/sizeof(practice2[0]);
    sendData(practice2,len, HIGH);
    go = 0;
    delay(1000);
    sendData(practice2, len, LOW);
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

void sendData(int doIt[], int len, bool cmd){
//  Serial.println();

  for(int i = 0; i <len; i++){
    digitalWrite(doIt[i], cmd);
//    Serial.print(doIt[i]);
    delay(250);
  }
}


