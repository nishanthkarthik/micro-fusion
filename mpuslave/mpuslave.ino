#pragma region Assemblies
#define Log(x) Serial.println(x)
#define LED_PIN 13
#include <inttypes.h>
#include "I2Cdev.h"
#include "MPU6050_6Axis_MotionApps20.h"
#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
#include "Wire.h"
#endif
#pragma endregion

#pragma region GlobalVars
MPU6050 mpu(0x68);
bool blinkState = false;
// MPU control/status vars
bool dmpReady = false;  // set true if DMP init was successful
uint8_t mpuIntStatus;   // holds actual interrupt status byte from MPU
uint8_t devStatus;      // return status after each device operation (0 = success, !0 = error)
uint16_t packetSize;    // expected DMP packet size (default is 42 bytes)
uint16_t fifoCount;     // count of all bytes currently in FIFO
uint8_t fifoBuffer[64]; // FIFO storage buffer
// orientation/motion vars
Quaternion q;           // [w, x, y, z]         quaternion container
VectorInt16 aa;         // [x, y, z]            accel sensor measurements
VectorInt16 aaReal;     // [x, y, z]            gravity-free accel sensor measurements
VectorInt16 aaWorld;    // [x, y, z]            world-frame accel sensor measurements
VectorFloat gravity;    // [x, y, z]            gravity vector
float euler[3];         // [psi, theta, phi]    Euler angle container
float ypr[3];           // [yaw, pitch, roll]   yaw/pitch/roll container and gravity vector
//Interrupt
volatile bool mpuInterrupt = false;     // indicates whether MPU interrupt pin has gone high
#pragma endregion

void dmpDataReady()
{
	mpuInterrupt = true;
}

void setup()
{
  //Configure Serial Port
    Serial.begin(115200);
  // join I2C bus (I2Cdev library doesn't do this automatically)
  #if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
  	Wire.begin();
  	//TWBR = 24; // 400kHz I2C clock (200kHz if CPU is 8MHz)
  #elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
  	Fastwire::setup(400, true);
  #endif
  	// initialize device
  	Log("Initializing I2C devices...\n");
  	mpu.initialize();
  	// verify connection
  	Log("Testing device connections...\n");
  	Log(mpu.testConnection() ? ("MPU6050 connection successful\n") : ("MPU6050 connection failed\n"));
  	// load and configure the DMP
  	Log("Initializing DMP...\n");
  	devStatus = mpu.dmpInitialize();
  	// supply your own gyro offsets here, scaled for min sensitivity
  	/*mpu.setXGyroOffset(220);
  	mpu.setYGyroOffset(76);
  	mpu.setZGyroOffset(-85);
  	mpu.setZAccelOffset(1788);*/
  	// make sure it worked (returns 0 if so)
  	if (devStatus == 0)
  	{
  		// turn on the DMP, now that it's ready
  		Log("Enabling DMP...\n");
  		mpu.setDMPEnabled(true);

  		// enable Arduino interrupt detection
  		Log("Enabling interrupt detection (Arduino external interrupt A1)...\n");
      attachInterrupt(0, dmpDataReady, RISING);
  		mpuIntStatus = mpu.getIntStatus();

      // set our DMP Ready flag so the main loop() function knows it's okay to use it
  		Log("DMP ready! Waiting for first interrupt...\n");
  		dmpReady = true;

  		// get expected DMP packet size for later comparison
  		packetSize = mpu.dmpGetFIFOPacketSize();
  	}
  	else
  	{
  		// ERROR!
  		// 1 = initial memory load failed
  		// 2 = DMP configuration updates failed
  		// (if it's going to break, usually the code will be 1)
  		Serial.print(F("DMP Initialization failed (code "));
        Serial.print(devStatus);
        Serial.println(F(")"));
  	}

  	//set LED to out
  	pinMode(LED_PIN, OUTPUT);
}

void loop()
{
    // if programming failed, don't try to do anything
  	if (!dmpReady) return;

  	// wait for MPU interrupt or extra packet(s) available
    while (!mpuInterrupt && fifoCount < packetSize);

  	// reset interrupt flag and get INT_STATUS byte
  	mpuInterrupt = false;
  	mpuIntStatus = mpu.getIntStatus();
  	// get current FIFO count
  	fifoCount = mpu.getFIFOCount();
  	// check for overflow (this should never happen unless our code is too inefficient)
  	if ((mpuIntStatus & 0x10) || fifoCount == 1024)
  	{
  		// reset so we can continue cleanly
  		mpu.resetFIFO();
  		Log("FIFO overflow!\n");

  		// otherwise, check for DMP data ready interrupt (this should happen frequently)
  	}
  	else if (mpuIntStatus & 0x02)
  	{
  		// wait for correct available data length, should be a VERY short wait
  		while (fifoCount < packetSize) fifoCount = mpu.getFIFOCount();

  		// read a packet from FIFO
  		mpu.getFIFOBytes(fifoBuffer, packetSize);

  		// track FIFO count here in case there is > 1 packet available
  		// (this lets us immediately read more without waiting for an interrupt)
  		fifoCount -= packetSize;

  		//Start reading values
  		{
  			mpu.dmpGetQuaternion(&q, fifoBuffer);			//Quaternion
  			mpu.dmpGetAccel(&aa, fifoBuffer);				//Total Acceleration with gravity
  			mpu.dmpGetGravity(&gravity, &q);				//Gravity vector
  			mpu.dmpGetLinearAccel(&aaReal, &aa, &gravity);	//Real acceleration without gravity
  			mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);
  			mpu.dmpGetEuler(euler, &q);
  			//Log("gravity # %f\t%f\t%f\n", gravity.x, gravity.y, gravity.z);
  			//Log("accel # %f"  "\t%f"  "\t%f"  "\n", (float)aa.x / 16384 * 2, (float)aa.y / 16384 * 2, (float)aa.z / 16384 * 2);
  			//Log("ypr # %f\t%f\t%f\n", ypr[0], ypr[1], ypr[2]);
  			//Log("euler # %f\t%f\t%f\n", euler[0] / 3.14159265 * 180, euler[1] / 3.14159265 * 180, euler[2] / 3.14159265 * 180);
                        Serial.print(euler[0] / 3.14159265 * 180);
                        Serial.print('\t');
                        Serial.print(euler[1] / 3.14159265 * 180);
                        Serial.print('\t');
                        Serial.print(euler[2] / 3.14159265 * 180);
                        Serial.println('\t');
  		}
  	}
  	// blink LED to indicate activity
  	blinkState = !blinkState;
  	digitalWrite(LED_PIN, blinkState);
}
