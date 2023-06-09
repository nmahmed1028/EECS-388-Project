#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "eecs388_lib.h"
#include "metal/i2c.h"

struct metal_i2c *i2c;
uint8_t bufWrite[5];
uint8_t bufRead[1];

//The entire setup sequence
void set_up_I2C()
{
    uint8_t oldMode;
    uint8_t newMode;
    _Bool success;

    bufWrite[0] = PCA9685_MODE1;
    bufWrite[1] = MODE1_RESTART;
    printf("%d\n",bufWrite[0]);
    
    i2c = metal_i2c_get_device(0);

    if(i2c == NULL){
        printf("Connection Unsuccessful\n");
    }
    else{
        printf("Connection Successful\n");
    }
    
    //Setup Sequence
    metal_i2c_init(i2c,I2C_BAUDRATE,METAL_I2C_MASTER);
    success = metal_i2c_write(i2c,PCA9685_I2C_ADDRESS,2,bufWrite,METAL_I2C_STOP_DISABLE);//reset
    delay(100);
    printf("resetting PCA9685 control 1\n");

    //Initial Read of control 1
    bufWrite[0] = PCA9685_MODE1;//Address
    success = metal_i2c_transfer(i2c,PCA9685_I2C_ADDRESS,bufWrite,1,bufRead,1);//initial read
    printf("Read success: %d and control value is: %d\n", success, bufWrite[0]);
    
    //Configuring Control 1
    oldMode = bufRead[0];
    newMode = (oldMode & ~MODE1_RESTART) | MODE1_SLEEP;
    printf("sleep setting is %d\n", newMode);
    bufWrite[0] = PCA9685_MODE1;//address
    bufWrite[1] = newMode;//writing to register
    success = metal_i2c_write(i2c,PCA9685_I2C_ADDRESS,2,bufWrite,METAL_I2C_STOP_DISABLE);//sleep
    bufWrite[0] = PCA9685_PRESCALE;//Setting PWM prescale
    bufWrite[1] = 0x79;
    success = metal_i2c_write(i2c,PCA9685_I2C_ADDRESS,2,bufWrite,METAL_I2C_STOP_DISABLE);//sets prescale
    bufWrite[0] = PCA9685_MODE1;
    bufWrite[1] = 0x01 | MODE1_AI | MODE1_RESTART;
    printf("on setting is %d\n", bufWrite[1]);
    success = metal_i2c_write(i2c,PCA9685_I2C_ADDRESS,2,bufWrite,METAL_I2C_STOP_DISABLE);//awake
    delay(100);
    printf("Setting the control register\n");
    bufWrite[0] = PCA9685_MODE1;
    success = metal_i2c_transfer(i2c,PCA9685_I2C_ADDRESS,bufWrite,1,bufRead,1);//initial read
    printf("Set register is %d\n",bufRead[0]);
} 

//Group effort
void breakup(int bigNum, uint8_t* low, uint8_t* high){
// & with 0xFF to cut off 0's 
   int h = bigNum >> 8 & 0xFF; //shifts digis of bigNum by 8 to get high 8 bit val
   int l = (uint8_t)bigNum & 0xFF; //convert nuber to uint_8 
   *low = l; //dereferences low pointer, assigns value of l to it
   *high = h; //dereferences high pointer, assigns value of h to it
}


// Olufewa Alonge
void steering(int angle)
{ 
  printf("steering\n");
  int servoCycle = getServoCycle(angle);    
  uint8_t a = 0;    
  uint8_t b = 0;    
  uint8_t readVal = 0;    
  breakup(servoCycle, &a, &b);    
  _Bool success;    
  bufWrite[0] = PCA9685_LED1_ON_L;    
  //bufWrite[0] = PCA9685_LED0_ON_L;
  bufWrite[1] = 0;    
  bufWrite[2] = 0;    
  bufWrite[3] = a;    
  bufWrite[4] = b;    
  //success = metal_i2c_transfer(i2c,PCA9685_I2C_ADDRESS,bufWrite,5,bufRead,1); //initial read    
  readVal = bufRead[0];     
  printf("running steer %d\n", angle);
}

//Nayyir Ahmed
void stopMotor(){
   printf("stopping motor\n");
   uint8_t low;
   uint8_t high;
   breakup(280, &low, &high); //280 is the value needed in LED0_OFF to put the wheels in neutral
   //bufWrite[0] = PCA9685_LED1_ON_L; //starting register
   bufWrite[0] = PCA9685_LED0_ON_L;
   bufWrite[1] = 0x00;
   bufWrite[2] = 0x00;
   bufWrite[3] = low;
   bufWrite[4] = high;
   printf("running stop\n");
   /*0 index: starting point 
     1-2 indices: values of LED0_ON_L AND LED0_ON_H 
     3-4 indices: values of LED0_OFF_L AND LED0_OFF_H (passing in the two 8 bit values)*/
   //metal_i2c_transfer(i2c,PCA9685_I2C_ADDRESS,bufWrite,5,bufRead,1); //bufWrite values written into LED0 to stop the car
}


//Dylan A. Davis
void driveForward(uint8_t speedFlag){
    printf("driving forward\n");
    uint8_t low; //assigning low and high as the correct data type 
    uint8_t high; 
    int speed = 0;
    switch(speedFlag){
        case 1:
            speed = 313;
            break;
        case 2:
            speed = 315;
            break;
        case 3:
            speed = 317;
            break;
    }
    breakup(speed, &low, &high);

    //bufWrite[0] = PCA9685_LED1_ON_L;// here we are writing the exact values we want into the bufWrite register we start at the given start value and add hex 4 to ensure we are at the correct starting value.
    bufWrite[0] = PCA9685_LED0_ON_L;
    bufWrite[1] = 0x00; // make the vaules of 1 and 2 to be 0 
    bufWrite[2] = 0x00;
    bufWrite[3] = low; 
    bufWrite[4] = high;

    //metal_i2c_transfer(i2c,PCA9685_I2C_ADDRESS,bufWrite,5,bufRead,1);
    printf("running forward\n");
}

//Steve Gan
void driveReverse(uint8_t speedFlag){
    printf("driving reverse\n");
    uint8_t low; //assigning low and high as the correct data type 
    uint8_t high; 
    int speed = 0;
    switch(speedFlag){
        case 1:
            speed = 267;
            break;
        case 2:
            speed = 265;
            break;
        case 3:
            speed = 263;
            break;
    }
    breakup(speed, &low, &high);

    //bufWrite[0] = PCA9685_LED1_ON_L;// here we are writing the exact values we want into the bufWrite register we start at the given start value and add hex 4 to ensure we are at the correct starting value.
    bufWrite[0] = PCA9685_LED0_ON_L;
    bufWrite[1] = 0x00; // make the vaules of 1 and 2 to be 0 
    bufWrite[2] = 0x00;
    bufWrite[3] = low; 
    bufWrite[4] = high;

    //metal_i2c_transfer(i2c,PCA9685_I2C_ADDRESS,bufWrite,5,bufRead,1);
    printf("running reverse\n");
}
//reads in string from UART, extracts them and passes them onto variables
void raspberrypi_int_handler(int devid, int * angle, int * speed, int * duration)
{
    char * str = malloc(50 * sizeof(char));// you can use this to store the received string
                                            // it is the same as char str[20] 
    int data = ser_readline(1,50,str); //reads to/from UART0, puts values into str
    printf("%s\n",str);
    sscanf(str, "angle: %d speed: %d duration: %d", angle, speed, duration);
    //scans the elements from str array and saving them to the correct pointers
    

   // Extract the values of angle, speed and duration inside this function
   // And place them into the correct variables that are passed in

    free(str); //de allocates space once the function is done
}


int main()
{
    // Initialize I2C
    set_up_I2C();
    delay(2000);

    // Calibrate Motor
    printf("Calibrate Motor.\n");
    stopMotor();
    delay(2000);

    // initialize UART channels
    ser_setup(0); // uart0 (receive from raspberry pi)
    ser_setup(1); // uart1

    printf("Setup completed.\n");
    printf("Begin the main loop.\n");
    

    int angle, speed, duration;
    // Drive loop
    while (1) {

        if (ser_isready(0)){ //checks if UART is ready
            printf("READY");
            raspberrypi_int_handler(0,&angle,&speed,&duration); //calls handler func
            printf("\n%d, %d, %d\n", angle,speed,duration);

            steering(angle);

            if(speed < 0){
                driveReverse(abs(speed));
            } else if (speed == 0){
                stopMotor();
            } else{
                driveForward(speed);
            }

            delay(duration * 1000);

        }
        // The following pseudo-code is a rough guide on how to write your code
        // It is NOT actual C code
         /*  
        
          if (is UART0 ready?)
          {
              call raspberrypi_int_handler() to get values of angle, speed and duration

              call steering(), driveForward/Reverse() and delay with the extracted values
          }
        */

    }
    return 0;
}
