#include <stdio.h>
#include <stdint.h>
#include "eecs388_lib.h"
#include "metal/i2c.h"


struct metal_i2c *i2c;
uint8_t bufWrite[9];
uint8_t bufRead[1];


//The entire setup sequence
void set_up_I2C(){
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
  int servoCycle = getServoCycle(angle);    
  uint8_t a = 0;    
  uint8_t b = 0;    
  uint8_t readVal = 0;    
  breakup(servoCycle, &a, &b);    
  _Bool success;    
  bufWrite[0] = PCA9685_LED1_ON_L;    
  bufWrite[1] = 0;    
  bufWrite[2] = 0;    
  bufWrite[3] = a;    
  bufWrite[4] = b;    
  success = metal_i2c_transfer(i2c,PCA9685_I2C_ADDRESS,bufWrite,5,bufRead,1); //initial read    
  readVal = bufRead[0];
        
}

//Nayyir Ahmed
void stopMotor(){
   uint8_t low;
   uint8_t high;
   breakup(280, low, high); //280 is the value needed in LED0_OFF to put the wheels in neutral
   bufWrite[0] = PCA9685_LED1_ON_L; //starting register
   bufWrite[1] = 0x00;
   bufWrite[2] = 0x00;
   bufWrite[3] = low;
   bufWrite[4] = high;
   /*0 index: starting point 
     1-2 indices: values of LED0_ON_L AND LED0_ON_H 
     3-4 indices: values of LED0_OFF_L AND LED0_OFF_H (passing in the two 8 bit values)*/
   metal_i2c_transfer(i2c,PCA9685_I2C_ADDRESS,bufWrite,5,bufRead,1); //bufWrite values written into LED0 to stop the car
}

//Dylan A. Davis
void driveForward(uint8_t speedFlag)
{
    uint8_t low; //assigning low and high as the correct data type 
    uint8_t high; 
    if (speedFlag = 1) // if speed flag is = to 1 then the value we passs to break up is 313
    {
        breakup(313, low, high);
    }
    else if (speedFlag = 2 )
    {
        breakup(315, low, high);
    }
    else if (speedFlag = 3)
    {
    breakup(317,low, high);
    }

    bufWrite[0] = PCA9685_LED1_ON_L;// here we are writing the exact values we want into the bufWrite register we start at the given start value and add hex 4 to ensure we are at the correct starting value.
    bufWrite[1] = 0x00; // make the vaules of 1 and 2 to be 0 
    bufWrite[2] = 0x00;
    bufWrite[3] = low; 
    bufWrite[4] = high;

    metal_i2c_transfer(i2c,PCA9685_I2C_ADDRESS,bufWrite,5,bufRead,1);
     
}

void driveReverse(uint8_t speedFlag){
    uint8_t low; //assigning low and high as the correct data type 
    uint8_t high; 
    if (speedFlag = 1) // if speed flag is = to 1 then the value we passs to break up is 267
    {
        breakup(267, low, high);
    }
    else if (speedFlag = 2 )
    {
        breakup(265, low, high);
    }
    else if (speedFlag = 3)
    {
    breakup(263,low, high);
    }

    bufWrite[0] = PCA9685_LED1_ON_L;// here we are writing the exact values we want into the bufWrite register we start at the given start value and add hex 4 to ensure we are at the correct starting value.
    bufWrite[1] = 0x00; // make the vaules of 1 and 2 to be 0 
    bufWrite[2] = 0x00;
    bufWrite[3] = low; 
    bufWrite[4] = high;

    metal_i2c_transfer(i2c,PCA9685_I2C_ADDRESS,bufWrite,5,bufRead,1);
}

int main()
{
    set_up_I2C();
    
    //Defining the breakup function
    /*
        Task 1: breaking 12 bit into two 8-bit
        Define the function created that recieves a 12 bit number,
        0 to 4096 and breaks it up into two 8 bit numbers.

        Assign these values to a referenced value handed into
        the function. 

        ex: 
        uint8_t variable1;
        uint8_t variable2;
        breakup(2000,&variable1,&variable2);
        variable1 -> low 8 bits of 2000
        variable2 -> high 8 bits of 2000


    */    
    
    //Changing Steering Heading
    /*
        Task 2: using getServoCycle(), bufWrite, bufRead, 
        breakup(), and and metal_i2c_transfer(), implement 
        the function defined above to control the servo
        by sending it an angle ranging from -45 to 45.

        Use the getServoCycle function to get the value to 
        breakup.

        ex: 
        int valToBreak = getServoCycle(45);
        >>>sets valToBreak to 355
        
        note: the motor's speed controller is either 
        LED0 or LED1 depending on where its plugged into 
        the board. If its LED1, simply add 4 to the LED0
        address

        ex: steering(0); -> driving angle forward
    */
    
    
    //Motor config/stop. This will cause a second beep upon completion
    /*
        -Task 3: using bufWrite, bufRead, breakup(), and
        and metal_i2c_transfer(), implement the funcion made
        above. This function Configure the motor by 
        writing a value of 280 to the motor.

        -include a 2 second delay after calling this function
        in order to calibrate

        -Note: the motor's speed controller is either 
        LED0 or LED1 depending on where its plugged into 
        the board. If its LED1, simply add 4 to the LED0
        address

        ex: stopMotor();
    */


    /*
    ############################################################
        ATTENTION: The following section will cause the        
        wheels to move. Confirm that the robot is              
        Propped up to avoid it driving away, as well as         
        that nothing is touching the wheels and can get 
        caught in them

        If anything goes wrong, unplug the hifive board from
        the computer to stop the motors from moving 
        
        Avoid sticking your hand inside the 
        car while its wheels are spinning
    #############################################################
    */
    

    //Motor Forward
    /*
        -Task 4: using bufWrite, bufRead, breakup(), and
        and metal_i2c_transfer(), implement the function
        made above to Drive the motor forward. The given
        speedFlag will alter the motor speed as follows:
        
        speedFlag = 1 -> value to breakup = 313 
        speedFlag = 2 -> value to breakup = 315(Optional)
        speedFlag = 3 -> value to breakup = 317(Optional)

        -note 1: the motor's speed controller is either 
        LED0 or LED1 depending on where its plugged into 
        the board. If its LED1, simply add 4 to the LED0
        address or type and replace LED1 with LED0

        ex: driveForward(1);
    */
    
    //Motor Reverse
    /*
        -Task 5: using bufWrite, bufRead, breakup(), and
        and metal_i2c_transfer(), implement the function
        made above to Drive the motor backward. The given
        speedFlag will alter the motor speed as follows:
        
        speedFlag = 1 -> value to breakup = 267 
        speedFlag = 2 -> value to breakup = 265(Optional)
        speedFlag = 3 -> value to breakup = 263(Optional)

        -note 1: the motor's speed controller is either 
        LED0 or LED1 depending on where its plugged into 
        the board. If its LED1, simply add 4 to the LED0
        address or type and replace LED1 with LED0

        ex: driveReverse(1);
    */
    
    
    //Fully Controlling the PCA9685
    /*
        -Task 6: using previously defined functions, 
        perform the following sequence of actions
        
        -Configure the motors (wait for 2 seconds)
        -Set the steering heading to 0 degrees 
        -Drive forward (wait for 2 seconds)
        -Change the steering heading to 20 degrees (wait for 2 seconds)
        -Stop the motor (wait for 2 seconds)
        -Drive forward (wait for 2 seconds)
        -Set steering heading to 0 degrees (wait for 2 seconds)
        -Stop the motor
    */

}
