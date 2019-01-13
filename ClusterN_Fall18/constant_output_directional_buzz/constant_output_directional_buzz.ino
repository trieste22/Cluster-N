#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>

//vibrating motor 8-compass point output
//   blink circle on startup
//   gets compass data from Adafruit BNO055 IMU
//    (using Adafruit's sensorapi.ino)
//   constant buzz pointing user towards north (makes you queazy)
//11.27.18


//////////////////////////////////////////////////////////////////////////////////
//////////////////////   SETUP
//////////////////////////////////////////////////////////////////////////////////

//Connections
//   ===========
//   SCL > A5
//   SDA > A4
//   VDD > 3-5V DC, GROUND > common ground

//#define SERIAL_OUT

void blinkTest(int OUT, int t, int reps);
void sendData(int doIt[], int len, bool cmd);
void displaySensorDetails(void);
void buzzOutput_test(float heading, int* arr); // buzz in direction imu currently facing (for debug/imu test)
void buzzOutput_North(float imu_heading, int* arr);
void serial_println(String string);


//Set the delay between fresh samples
#define BNO055_SAMPLERATE_DELAY_MS (250) //100

Adafruit_BNO055 bno = Adafruit_BNO055(55);

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
bool test = 0;

void setup() {
  pinMode(led, OUTPUT);
  
  for(int i = 0; i < len; i++){
    pinMode(outs[i], OUTPUT);
    digitalWrite(outs[i], HIGH);
    delay(500);
    digitalWrite(outs[i], LOW);
  }

  #ifdef SERIAL_OUT
    Serial.begin(9600);
    Serial.println("Orientation Sensor Test"); Serial.println("");
  #endif

  /* Initialise the sensor */
  if(!bno.begin())
  {
    #ifdef SERIAL_OUT
      Serial.print("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
    #endif
    while(1);
  }

  delay(1000);

  #ifdef SERIAL_OUT
    displaySensorDetails(); // Display some basic sensor info
    displaySensorStatus(); // optional: display current status
  #endif
  
  bno.setExtCrystalUse(true);

}

//////////////////////////////////////////////////////////////////////////////////
//////////////////////   MAIN
//////////////////////////////////////////////////////////////////////////////////

void loop() {
  if(test){
      int practice[] = {n,ne, s,sw, w};

    len = sizeof(practice)/sizeof(practice[0]);
    sendData(practice,len, HIGH);
    test = 0;
    delay(1000);
    sendData(practice, len, LOW);
  }

  /* Get a new sensor event */
  sensors_event_t event;
  bno.getEvent(&event);

  #ifdef SERIAL_OUT
    Serial.print("X: "); // Display floating point data
    Serial.print(event.orientation.x, 4);
    displayCalStatus(); // Optional: Display calibration status
    //displaySensorStatus(); Optional: Display sensor status (debug only)
    Serial.println("");
  #endif
  
  int buzz[] = {-3,-3};
//  buzzOutput_North(event.orientation.x, buzz); // buzz to indicate true North to user
  buzzOutput_test(event.orientation.x, buzz); // debug

  delay(BNO055_SAMPLERATE_DELAY_MS); // wait specified delay before requesting next data
  digitalWrite(buzz[0], LOW); // stop buzz
  if (buzz[1] > 0){
    digitalWrite(buzz[1], LOW);
  }
}

// heading input= heading of imu at back of neck (180 from user heading)
// output buzz indicating north from user reference point
void buzzOutput_North(float imu_heading, int* arr){
  int one;
  int two = 0;
  
  float user_n = 180 - imu_heading;
  if(user_n < 0){
    user_n = 360 + user_n; // user_n = direction with respect to user (front = 0/360) of true N
  }

  if((user_n > 348.75 && user_n <= 360) || (user_n >=0 && user_n <=11.25)){ // N
    one = n;
    serial_println("1 N");
  }
  if(user_n >11.25 && user_n <= 33.75){ // NNE
    one = n;
    two = ne;
    serial_println("2 NNE");
  }
  else if(user_n > 33.75 && user_n <= 56.25){ // NE
    one = ne;
    serial_println("3 NE");
  }
  else if(user_n > 56.25 && user_n <= 78.75){ // ENE
    one = ne;
    two = e;
    serial_println("4 ENE");
  }
  else if (user_n > 78.75 && user_n <=101.25){ // E
    one = e;
    serial_println("5 E");
  }
  else if(user_n > 101.25 && user_n <= 123.75){ // ESE
    one = e;
    two = se;
    serial_println("6 ESE");
  }
  else if (user_n > 123.75 && user_n <=146.25){ // SE
    one = se;
    serial_println("7 SE");
  }
  else if(user_n > 146.25 && user_n <= 168.75){ // SSE
    one = s;
    two = se;
    serial_println("8 SSE");
  }
  else if (user_n > 168.75 && user_n <= 191.25){ // S
    one = s;
    serial_println("9 S");
  }
  else if(user_n > 191.25 && user_n <= 213.75){ // SSW
    one = s;
    two = sw;
    serial_println("10 SSW");
  }
  else if (user_n > 213.75 && user_n <= 236.25){ // SW
    one = sw;
    serial_println("11 SW");
  }
  else if(user_n > 236.25 && user_n <= 258.75){ // WSW
    one = w;
    two = sw;
    serial_println("12 WSW");
  }
  else if(user_n > 258.75 && user_n <= 281.75){ //W
    one = w;
    serial_println("13 W");
  }
  else if(user_n > 281.75 && user_n <= 303.75){ // WNW
    one = w;
    two = nw;
    serial_println("14 WNW");
  }
  else if(user_n >303.75 && user_n <= 326.25){ //NW
    one = nw;
    serial_println("15 NW");
  }
  else if(user_n > 326.25 && user_n <= 348.75){ // NNW
    one = n;
    two = nw;
    serial_println("16 NNW");
  }

  digitalWrite(one, HIGH);
  if(two > 0){
    digitalWrite(two, HIGH);
  }

  arr[0] = one;
  arr[1] = two ;
}

void serial_println(String string){ // for debugging
  #ifdef SERIAL_OUT
    Serial.println(string);
  #endif
}

void buzzOutput_test(float heading, int* arr){
  int one;
  int two = 0;
  Serial.print(heading);

  
  if((heading > 348.75 && heading <= 360) || (heading >=0 && heading <=11.25)){ // N
    one = n;
    Serial.println("N");
  }
  if(heading >11.25 && heading <= 33.75){ // NNE
    one = n;
    two = ne;
    Serial.println("NNE");
  }
  else if(heading > 33.75 && heading <= 56.25){ // NE
    one = ne;
    Serial.println("NE");
  }
  else if(heading > 56.25 && heading <= 78.75){ // ENE
    one = ne;
    two = e;
    Serial.println("ENE");
  }
  else if (heading > 78.75 && heading <=101.25){ // E
    one = e;
    Serial.println("E");
  }
  else if(heading > 101.25 && heading <= 123.75){ // ESE
    one = e;
    two = se;
    Serial.println("ENE");
  }
  else if (heading > 123.75 && heading <=146.25){ // SE
    one = se;
    Serial.println("SE");
  }
  else if(heading > 146.25 && heading <= 168.75){ // SSE
    one = s;
    two = se;
    Serial.println("SSE");
  }
  else if (heading > 168.75 && heading <= 191.25){ // S
    one = s;
    Serial.println("S");
  }
  else if(heading > 191.25 && heading <= 213.75){ // SSW
    one = s;
    two = sw;
    Serial.println("SSW");
  }
  else if (heading > 213.75 && heading <= 236.25){ // SW
    one = sw;
    Serial.println("SW");
  }
  else if(heading > 236.25 && heading <= 258.75){ // WSW
    one = w;
    two = sw;
    Serial.println("WSW");
  }
  else if(heading > 258.75 && heading <= 281.75){ //W
    one = w;
    Serial.println("W");
  }
  else if(heading > 281.75 && heading <= 303.75){ // WNW
    one = w;
    two = nw;
    Serial.println("WNW");
  }
  else if(heading >303.75 && heading <= 326.25){ //NW
    one = nw;
    Serial.println("NW");
  }
  else if(heading > 326.25 && heading <= 348.75){ // NNW
    one = n;
    two = nw;
    Serial.println("NNW");
  }

  digitalWrite(one, HIGH);
  if(two > 0){
    digitalWrite(two, HIGH);
  }

  arr[0] = one;
  arr[1] = two ;
}


//////////////////////////////////////////////////////////////////////////////////
//////////////////////   HELPERS
//////////////////////////////////////////////////////////////////////////////////

// to indicate leds/motors working on startup
void blinkTest(int OUT, int t, int reps){
  for(int i = 0; i < reps; i++){
    digitalWrite(OUT, HIGH);
    delay(t);
    digitalWrite(OUT,LOW);
    delay(t);
  }
}

// buzz/LED output - doIt[] containins outputs to buzz (ie {n, ne}), len = length(doIt[]) cmd = on or off
void sendData(int doIt[], int len, bool cmd){
  for(int i = 0; i <len; i++){
    digitalWrite(doIt[i], cmd);
  }
}


// Displays some basic information on this sensor from the unified sensor API sensor_t type (Adafruit_Sensor)
void displaySensorDetails(void)
{
  sensor_t sensor;
  bno.getSensor(&sensor);
  Serial.println("------------------------------------");
  Serial.print  ("Sensor:       "); Serial.println(sensor.name);
  Serial.print  ("Driver Ver:   "); Serial.println(sensor.version);
  Serial.print  ("Unique ID:    "); Serial.println(sensor.sensor_id);
  Serial.print  ("Max Value:    "); Serial.print(sensor.max_value); Serial.println(" xxx");
  Serial.print  ("Min Value:    "); Serial.print(sensor.min_value); Serial.println(" xxx");
  Serial.print  ("Resolution:   "); Serial.print(sensor.resolution); Serial.println(" xxx");
  Serial.println("------------------------------------");
  Serial.println("");
  delay(500);
}


//Display some basic info about the sensor status
void displaySensorStatus(void)
{
  uint8_t system_status, self_test_results, system_error; // get system status vals (for debugging)
  system_status = self_test_results = system_error = 0;
  bno.getSystemStatus(&system_status, &self_test_results, &system_error);

  Serial.println(""); // Display results over Serial
  Serial.print("System Status: 0x");
  Serial.println(system_status, HEX);
  Serial.print("Self Test:     0x");
  Serial.println(self_test_results, HEX);
  Serial.print("System Error:  0x");
  Serial.println(system_error, HEX);
  Serial.println("");
  delay(500);
}


//Display sensor calibration status
void displayCalStatus(void)
{
  uint8_t system, gyro, accel, mag; // get four calibration values - sensor reporting 0 should be ignored
  system = gyro = accel = mag = 0; // 3 means 'fully calibrated'
  bno.getCalibration(&system, &gyro, &accel, &mag);

  Serial.print("\t"); // ignore data until system calibration > 0
  if (!system)
  {
    Serial.print("! ");
  }

  Serial.print("Sys:"); // display system status
  Serial.print(system, DEC);
  Serial.print(" G:");
  Serial.print(gyro, DEC);
  Serial.print(" A:");
  Serial.print(accel, DEC);
  Serial.print(" M:");
  Serial.print(mag, DEC);
}


