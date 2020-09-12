#include "freeram.h"

#include "mpu.h"
#include "I2Cdev.h"

int ret;

const byte numChars = 20;
char string_from_godot[numChars] = "start   ,255,9.99\0";
    
boolean newData = false;

unsigned long startMillis = millis(); //some global variables available anywhere in the program
unsigned long currentMillis;

unsigned long startMillis2 = millis(); //some global variables available anywhere in the program
unsigned long currentMillis2;

const unsigned long period = 10;  //the value is a number of milliseconds
int i = 0;

int led = 3;           // the pin that the LED is attached to
int brightness = 0;    // how bright the LED is


void setup() {

  // declare pin 9 to be an output:
  pinMode(led, OUTPUT);

  Fastwire::setup(400, 0);

  // Open Serial port with:
  // baud rate: 9600
  // Data size: 8 bits
  // Parity: None
  // Stop Bits: 1 bit
  Serial.begin(115200, SERIAL_8N1);

  ret = mympu_open(200);
  Serial.print("MPU init: "); Serial.println(ret);
  Serial.print("Free mem: "); Serial.println(freeRam());

  delay(100);
  
  }

unsigned int c = 0; //cumulative number of successful MPU/DMP reads
unsigned int np = 0; //cumulative number of MPU/DMP reads that brought no packet back
unsigned int err_c = 0; //cumulative number of MPU/DMP reads that brought corrupted packet
unsigned int err_o = 0; //cumulative number of MPU/DMP reads that had overflow bit set



void loop() {


  currentMillis = millis();  //get the current "time" (actually the number of milliseconds since the program started)
  if (currentMillis - startMillis >= period)  //test whether the period has elapsed
  {
    startMillis = startMillis + period;  //IMPORTANT to save the start time of the current LED state.

    // set the brightness of pin 9 from the 2nd value string_from_godot <bye,250,8.70>
    String brightness_S(string_from_godot);
    brightness = brightness_S.substring(9,12).toInt();
    analogWrite(led, brightness);


    
    ret = mympu_update();

    switch (ret) {
      case 0: c++; break;
      case 1: np++; return;
      case 2: err_o++; return;
      case 3: err_c++; return;
      default:
        Serial.print("READ ERROR!  ");
        Serial.println(ret);
        return;
    }

    //quaternarions q_w,q_x,q_y,q_z
    char q_w_s[7], q_x_s[7], q_y_s[7], q_z_s[7];

    dtostrf(mympu.q[0], 5, 3, q_w_s);
    dtostrf(mympu.q[1], 5, 3, q_x_s);
    dtostrf(mympu.q[2], 5, 3, q_y_s);
    dtostrf(mympu.q[3], 5, 3, q_z_s);




    // string_from_godot <bye  ,250,8.70>
    recvWithStartEndMarkers();

    char myConcatenation[60];
    sprintf(myConcatenation, "<%s,%s,%s,%s,%s>\n", q_w_s, q_x_s, q_y_s, q_z_s, string_from_godot);

    Serial.write(myConcatenation);

//    i++;
  }

//  currentMillis2 = millis();  //get the current "time" (actually the number of milliseconds since the program started)
//  if (currentMillis2 - startMillis2 >= 1000)  //test whether the period has elapsed
//  {
//    startMillis2 = startMillis2 + 1000;  //IMPORTANT to save the start time of the current LED state.
//    char myConcatenation2[60];
//    sprintf(myConcatenation2, "nombre d'occurrences par seconde : %i", i);
//    Serial.write(myConcatenation2);
//
//    i = 0;
//  }

}


void recvWithStartEndMarkers()
{
  static boolean recvInProgress = false;
  static byte ndx = 0;
  char startMarker = '<';
  char endMarker = '>';
  char rc;

  while (Serial.available() > 0 ) {

    rc = Serial.read();

    if (recvInProgress == true) {
      if (rc != endMarker) {
        string_from_godot[ndx] = rc;
        ndx++;
        if (ndx >= numChars) {
          ndx = numChars - 1;
        }
      }
      else {
        string_from_godot[ndx] = '\0'; // terminate the string
        recvInProgress = false;
        ndx = 0;
        newData = true;
      }
    }

    else if (rc == startMarker) {
      recvInProgress = true;
    }
  }




  //
  //
  //  //if the string string_from_godot changed then change send it to arduino
  //  if(strcmp (tempChars,string_from_godot) != 0 ){
  //    memset(string_from_godot,'\0',sizeof (string_from_godot));
  //    strcpy(tempChars, string_from_godot);
  //  }
  //Serial.flush();
}
