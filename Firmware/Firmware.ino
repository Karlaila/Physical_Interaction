
// Include Libraries
#include "Arduino.h"
#include "Flex.h"
#include "MPU6050.h"
#include "Wire.h"
#include "I2Cdev.h"


// Pin Definitions
#define FLEX5V_PIN_SIG	A0
#define FLEX5V_PIN_SIG_1  A1
#define FLEX5V_PIN_SIG_2  A2

// Global variables and defines
int16_t mpu6050Ax, mpu6050Ay, mpu6050Az;
int16_t mpu6050Gx, mpu6050Gy, mpu6050Gz;

// object initialization
Flex flex5v_1(FLEX5V_PIN_SIG);
Flex flex5v_2(FLEX5V_PIN_SIG_1);
Flex flex5v_3(FLEX5V_PIN_SIG_2);
MPU6050 mpu6050;



// define vars for testing menu
const int timeout = 10000;       //define timeout of 10 sec
char menuOption = 0;
long time0;

// values for moving average
const double ol = 0.75, ne = 0.25;

// evergy values
int dE[] = {0,0,0}, dEma[] = {0,0,0}, dEa[] = {0,0,0}, Eold[] = {0,0,0}, Enew[] = {0,0,0};
int dE_full=0;
Flex *flexes[] = {&flex5v_1, &flex5v_2, &flex5v_3};
int counter = 0;
int count = 4;


// Setup the essentials for your circuit to work. It runs first every time your circuit is powered with electricity.
void setup() 
{
    // Setup Serial which is useful for debugging
    // Use the Serial Monitor to view printed messages
    Serial.begin(9600);
    while (!Serial) ; // wait for serial port to connect. Needed for native USB
    Serial.println("start");

    mpu6050.initialize();
   
}

// Main logic of your circuit. It defines the interaction between the components you selected. After setup, it runs over and over again, in an eternal loop.
void loop() 
{
    // reading the flexes value and adding up the dEs
    for(int i = 0; i<3; i++){
       Enew[i] = flexes[i]->read();
       dE[i] += abs(Enew[i]-Eold[i]);
       Eold[i] = Enew[i];
    }
    
    counter += 1;

    if(counter >= count){
      counter = 0;
      dE_full = 0;
      // changes for flexes are around 500; can be over thousaund though
      for(int i = 0; i<3; i++){
        dEma[i] = ol*dEma[i] + ne*dE[i];
        dE_full += dEma[i];
        dE[i] = 0;
      }
    }
    
    //Serial.print(F("flex5vVal: ")); 
    Serial.print(Enew[0]);Serial.print("\t");
    Serial.print(Enew[1]);Serial.print("\t");
    Serial.print(Enew[2]);Serial.print("\t");
    Serial.print(dEma[0]);Serial.print("\t");
    Serial.print(dEma[1]);Serial.print("\t");
    Serial.print(dEma[2]);Serial.print("\t");
    //Serial.print(F("flexdE: ")); 
    //Serial.print(dE);Serial.print("\t");
    //Serial.print(F("flexdEma: ")); 
    Serial.println(dE_full);
    

    // read MPU values
    /*mpu6050.getMotion6(&mpu6050Ax, &mpu6050Ay, &mpu6050Az, &mpu6050Gx, &mpu6050Gy, &mpu6050Gz);   //read accelerometer and gyroscope raw data in three axes
    double mpu6050Temp = ((double)mpu6050.getTemperature() + 12412.0) / 340.0;
    Serial.print("a/g-\t");
    Serial.print(mpu6050Ax); Serial.print("\t");
    Serial.print(mpu6050Ay); Serial.print("\t");
    Serial.print(mpu6050Az); Serial.print("\t");
    Serial.print(mpu6050Gx); Serial.print("\t");
    Serial.print(mpu6050Gy); Serial.print("\t");
    Serial.print(mpu6050Gz); Serial.print("\t");
    Serial.print(F("Temp- "));   
    Serial.println(mpu6050Temp);*/
    delay(100);

    
}



