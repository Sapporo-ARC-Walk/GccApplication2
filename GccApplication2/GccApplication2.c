//##########################################################
//##                      R O B O T I S                   ##
//## CM-700 (Atmega2561) Example code for Dynamixel.      ##
//##                                           2009.11.10 ##
//##########################################################

#include <avr/io.h>
#include <stdio.h>
#include <math.h>
#include <util/delay.h>
#include "SerialManager.h"
#include "GyroManager.h"
#include "dynamixel.h"

#define MAIN_DELAY 1

//LED
#define LED_BAT 0x01
#define LED_TxD 0x02
#define LED_RxD 0x04
#define LED_AUX 0x08
#define LED_MANAGE 0x10
#define LED_PROGRAM 0x20
#define LED_PLAY 0x40
#define LED_MAX 7

//Button
#define SW_BUTTON 0x01
#define SW_START 0x02

//Serial
#define DEFAULT_BAUDNUM		1 // 1Mbps

//Dynamixel
#define P_CW_ANGLE_LIMIT_L	6
#define P_CCW_ANGLE_LIMIT_L	8
#define P_TORQUE_ENABLE		24
#define P_GOAL_POSITION_L	30
#define P_GOAL_POSITION_H	31
#define P_GOAL_SPEED_L		32
#define P_GOAL_SPEED_H		33
#define P_PRESENT_POSITION_L	36
#define P_PRESENT_POSITION_H	37
#define P_PRESENT_LOAD_L	40
#define P_PRESENT_LOAD_H	41
#define P_PRESENT_VOLTAGE	42
#define P_MOVING	  		46
#define P_EEP_LOCK  		47
#define SERVO_MAX	  		12
#define ACT_MAX				50

//Servo Speed
#define CYCLE_TIME  495
#define VALUE_MAX   1023

//Mode
#define MODE_0				0
#define MODE_1				1
#define MODE_2				2
#define MODE_3				3
#define MODE_4				4
#define MODE_5				5
#define MODE_6				6

//ADC port for IR sensors.
#define		ADC_PORT_1	1
#define		ADC_PORT_2	2
#define		ADC_PORT_3	3
#define		ADC_PORT_4	4
#define		ADC_PORT_5	5
#define		ADC_PORT_6	6

int serCmd[SERIAL_BUFFER_SIZE] = {0};
	                    // R1  R2 R3  R4 R5 R6  L1 L2 L3  L4  L5  L6
int servoId[SERVO_MAX] = { 13, 8, 10, 5, 9, 12, 4, 6, 16, 19, 15, 11};

#define 	ANGLE_0    0
#define 	ANGLE_0_5  51
#define 	ANGLE_1    102
#define 	ANGLE_1_5  153
#define 	ANGLE_2    205
#define 	ANGLE_2_5  256
#define 	ANGLE_3    307
#define 	ANGLE_3_5  358
#define 	ANGLE_4    410
#define 	ANGLE_4_5  461
#define 	ANGLE_5    512
#define 	ANGLE_5_5  563
#define 	ANGLE_6    614
#define 	ANGLE_6_5  665
#define 	ANGLE_7    716
#define 	ANGLE_7_5  767
#define 	ANGLE_8    818
#define 	ANGLE_8_5  869
#define 	ANGLE_9    920
#define 	ANGLE_9_5  971
#define 	ANGLE_10   1023

#define 	SPEED_SLOW     50
#define 	SPEED_NORMAL   200
#define 	SPEED_MIDDLE   200
#define 	SPEED_HIGH     400

int angleList[ACT_MAX][SERVO_MAX + 1] = {
   // R1   R2   R3   R4   R5   R6   L1   L2   L3   L4   L5   L6    Speed
    { 656, 786, 500, 715, 570, 518, 348, 536, 502, 294, 449, 500,  100 },	//0 Default
    { 112, 307, 910, 706, 910, 706,  37, 342,  100 },	//1 Pre Walk
	{ 37, ANGLE_5, ANGLE_8, ANGLE_5, ANGLE_8, ANGLE_5, ANGLE_2, ANGLE_5, SPEED_MIDDLE },	//2 Pre Walk
		
/* Neck */
	{ ANGLE_2, ANGLE_5, ANGLE_8, ANGLE_5, ANGLE_8, ANGLE_5, ANGLE_2, ANGLE_5, 100 },	//3 Neck default
	{ ANGLE_2, ANGLE_5, ANGLE_8, ANGLE_5, ANGLE_8, ANGLE_5, ANGLE_2, ANGLE_5, 100 },	//4
	{ ANGLE_2, ANGLE_5, ANGLE_8, ANGLE_5, ANGLE_8, ANGLE_5, ANGLE_2, ANGLE_5, 100 },	//5
	{ ANGLE_2, ANGLE_5, ANGLE_8, ANGLE_5, ANGLE_8, ANGLE_5, ANGLE_2, ANGLE_5, 100 },	//6
	{ ANGLE_2, ANGLE_5, ANGLE_8, ANGLE_5, ANGLE_8, ANGLE_5, ANGLE_2, ANGLE_5, 100 },	//7
	{ ANGLE_1, ANGLE_4, ANGLE_9, ANGLE_6, ANGLE_9, ANGLE_6, ANGLE_1, ANGLE_4, 100 },	//8
	{ ANGLE_2, ANGLE_5, ANGLE_8, ANGLE_5, ANGLE_8, ANGLE_5, ANGLE_2, ANGLE_5, 100 },	//9

/* Walk2 */
    { 222, 357, 930, 706, 910, 706, 47, 352, SPEED_NORMAL },	//10 Walk2
    { 92, 317, 960, 736, 860, 666, 112, 317, SPEED_NORMAL },	//11 Walk2
    { 92, 317, 960, 736, 975, 671, 112, 317, SPEED_NORMAL },	//12 Walk2
    { 92, 317, 800, 656, 975, 671, 112, 317, SPEED_NORMAL },	//13 Walk2
    { 82, 297, 930, 706, 910, 686, 162, 327, SPEED_NORMAL },	//14 Walk2
    { 82, 297, 930, 706, 910, 706, 47, 352, SPEED_NORMAL },		//15 Walk2
	
    { 112, 317, 910, 706, 910, 706, 112, 317,  100 },	//16
    { 112, 317, 910, 706, 910, 706, 112, 317,  100 },	//17
    { 112, 317, 910, 706, 910, 706, 112, 317,  100 },	//18
    { 112, 317, 910, 706, 910, 706, 112, 317,  100 },	//19

	{ ANGLE_1, ANGLE_3, ANGLE_9, ANGLE_7, ANGLE_9, ANGLE_7, ANGLE_1, ANGLE_3, SPEED_SLOW },	//20 Walk2
	{ ANGLE_1, ANGLE_3, ANGLE_9, ANGLE_7, ANGLE_9, ANGLE_7, ANGLE_1, ANGLE_3, SPEED_SLOW },	//21 Walk2
	{ ANGLE_1, ANGLE_3, ANGLE_9, ANGLE_7, ANGLE_9, ANGLE_7, ANGLE_1, ANGLE_3, SPEED_SLOW },	//22 Walk2
	{ ANGLE_1, ANGLE_3, ANGLE_9, ANGLE_7, ANGLE_9, ANGLE_7, ANGLE_1, ANGLE_3, SPEED_SLOW },	//23 Walk2
	{ ANGLE_1, ANGLE_3, ANGLE_9, ANGLE_7, ANGLE_9, ANGLE_7, ANGLE_1, ANGLE_3, SPEED_SLOW },	//24 Walk2
	{ ANGLE_1, ANGLE_3, ANGLE_9, ANGLE_7, ANGLE_9, ANGLE_7, ANGLE_1, ANGLE_3, SPEED_SLOW },	//25 Walk2
	{ ANGLE_1, ANGLE_3, ANGLE_9, ANGLE_7, ANGLE_9, ANGLE_7, ANGLE_1, ANGLE_3, SPEED_SLOW },	//26 Walk2
	{ ANGLE_1, ANGLE_3, ANGLE_9, ANGLE_7, ANGLE_9, ANGLE_7, ANGLE_1, ANGLE_3, SPEED_SLOW },	//27 Walk2
	{ ANGLE_1, ANGLE_3, ANGLE_9, ANGLE_7, ANGLE_9, ANGLE_7, ANGLE_1, ANGLE_3, SPEED_SLOW },	//28 Walk2

/* Turn Left */
	{ ANGLE_0, ANGLE_3, ANGLE_9, ANGLE_8, ANGLE_9, ANGLE_8, ANGLE_0, ANGLE_3, SPEED_SLOW },	//29 Turn Left
	{ ANGLE_0, ANGLE_3, ANGLE_9, ANGLE_7, ANGLE_9, ANGLE_7, ANGLE_0, ANGLE_3, SPEED_SLOW },	//30 Turn Left
	{ ANGLE_1, ANGLE_3, ANGLE_9, ANGLE_7, ANGLE_9, ANGLE_7, ANGLE_1, ANGLE_3, SPEED_SLOW },	//31 Turn Left
	{ ANGLE_0, ANGLE_3, ANGLE_9, ANGLE_8, ANGLE_9, ANGLE_8, ANGLE_0, ANGLE_3, SPEED_SLOW},	//32 Turn Left
	{ ANGLE_0, ANGLE_3, ANGLE_9, ANGLE_7, ANGLE_9, ANGLE_7, ANGLE_0, ANGLE_3, SPEED_SLOW },	//33 Turn Left
	{ ANGLE_1, ANGLE_3, ANGLE_9, ANGLE_7, ANGLE_9, ANGLE_7, ANGLE_1, ANGLE_3, SPEED_SLOW },	//34 Turn Left

/* Turn Right */
	{ ANGLE_1, ANGLE_2, ANGLE_10, ANGLE_7, ANGLE_10, ANGLE_7, ANGLE_1, ANGLE_2, 100 },	//35 Turn Right
	{ ANGLE_1, ANGLE_3, ANGLE_10, ANGLE_7, ANGLE_10, ANGLE_7, ANGLE_1, ANGLE_3, 100 },	//36 Turn Right
	{ ANGLE_1, ANGLE_3, ANGLE_9, ANGLE_7, ANGLE_9, ANGLE_7, ANGLE_1, ANGLE_3, 100 },	//37 Turn Right
	{ ANGLE_1, ANGLE_2, ANGLE_10, ANGLE_7, ANGLE_10, ANGLE_7, ANGLE_1, ANGLE_2, 100 },	//38 Turn Right
	{ ANGLE_1, ANGLE_3, ANGLE_10, ANGLE_7, ANGLE_10, ANGLE_7, ANGLE_1, ANGLE_3, 100 },	//39 Turn Right
    { ANGLE_1, ANGLE_3, ANGLE_9, ANGLE_7, ANGLE_9, ANGLE_7, ANGLE_1, ANGLE_3, 100 },	//40 Turn Right
	
	{ ANGLE_4, ANGLE_7, ANGLE_8, ANGLE_5, ANGLE_9, ANGLE_6, ANGLE_0, ANGLE_3, 100 },	//41 Walk1_1
	{ ANGLE_3, ANGLE_5, ANGLE_8, ANGLE_5, ANGLE_9, ANGLE_5, ANGLE_2, ANGLE_5, 100 },	//42 Walk1_1
	{ ANGLE_3, ANGLE_5, ANGLE_9, ANGLE_5, ANGLE_8, ANGLE_5, ANGLE_0, ANGLE_3, 100 },	//43 Walk1_1
	{ ANGLE_3, ANGLE_5, ANGLE_9, ANGLE_5, ANGLE_9, ANGLE_6, ANGLE_1, ANGLE_4, 100 },	//44 Walk1_1
	{ ANGLE_2, ANGLE_5, ANGLE_6, ANGLE_3, ANGLE_10, ANGLE_7, ANGLE_1, ANGLE_4, 100 },	//45 Walk1_1
	{ ANGLE_2, ANGLE_5, ANGLE_7, ANGLE_5, ANGLE_8, ANGLE_5, ANGLE_1, ANGLE_5, 100 },	//46 Walk1_1
	{ ANGLE_1, ANGLE_5, ANGLE_7, ANGLE_5, ANGLE_10, ANGLE_7, ANGLE_2, ANGLE_5, 100 },	//47 Walk1_1
	{ ANGLE_1, ANGLE_5, ANGLE_7, ANGLE_5, ANGLE_9, ANGLE_6, ANGLE_1, ANGLE_4, 100 },	//48 Walk1_1
	
	{ ANGLE_2, ANGLE_5, ANGLE_8, ANGLE_5, ANGLE_8, ANGLE_5, ANGLE_2, ANGLE_5, 100 },	//49
};

int motion0[] =	{1, 0 };  //Default
int motion1[] =	{1, 1 };  //Pre Walk
//int motion2[] =	{6, 16, 17, 18, 19, 20, 21 };  //Walk1
int motion2[] =	{8, 41, 42, 43, 44, 45, 46, 47, 48};  //Walk1
//int motion3[] =	{8, 22, 23, 24, 25, 26, 27, 28, 29 };  //Walk2
int motion3[] =	{6, 10, 11, 12, 13, 14, 15};  //Walk2
int motion4[] =	{6, 29, 30, 31, 32, 33, 34 };      //Turn Left
int motion5[] =	{6, 35, 36, 37, 38, 39, 40 };      //Turn Right
int motion6[] =	{6, 10, 11, 12, 13, 14, 15 };  //Step
int *motionList[] = { &motion0[0], &motion1[0], &motion2[0], &motion3[0], &motion4[0], &motion5[0], &motion6[0] };
enum ActType {
	ACT_DEFAULT         = 0,
	ACT_PRE_WALK        = 1,
	ACT_WALK1           = 2,
	ACT_WALK2           = 3,
	ACT_TURN_LEFT       = 4,
	ACT_TURN_RIGHT      = 5,
	ACT_WALK_LEFT       = 6,
	ACT_WALK_RIGHT      = 7,
	ACT_TYPE_MAX
};

/*
int mode1act[11][2] = {
	 { 3000, 10 },  //Start Wait, Total Num
     {    0,  1 },  //Default
     {    1,  1 },  //Pre Walk
     {    3,  10 },  //Walk2
     {    4,  4 },  //Turn Left
     {    3,  10 },  //Walk2
     {    5,  4 },  //Turn Right
     {    3,  10 },  //Walk2
     {    5,  4 },  //Turn Right
     {    3,  10 },  //Walk2
     {    0,  1 },  //Default
};
*/

int mode1act[11][2] = {
	 { 1000, 10 },  //Start Wait, Total Num
     {    0,  1 },  //Default
     {    3,  10 },  //Walk2
     {    3,  10 },  //Walk2
     {    3,  10 },  //Walk2
     {    3,  10 },  //Walk2
     {    3,  10 },  //Walk2
     {    3,  10 },  //Walk2
     {    3,  10 },  //Walk2
     {    3,  10 },  //Walk2
     {    0,  1 },  //Default
};

/*
int mode1act[11][2] = {
	 { 1000, 10 },  //Start Wait, Total Num
     {    0,  1 },  //Default
     {    1,  1 },  //Pre Walk
     {    4,  10 },  //Turn Left
     {    4,  10 },  //Turn Left
     {    4,  10 },  //Turn Left
     {    4,  10 },  //Turn Left
     {    4,  10 },  //Turn Left
     {    4,  10 },  //Turn Left
     {    4,  10 },  //Turn Left
     {    0,  1 },  //Default
};
*/



/*
int minmaxList[2][SERVO_MAX] = {
	// L1,  L2,  L3,  L4,  L5,  L6,  R1,  R2,  R3,  R4,  R5,  R6
	{ 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512 },	//min
	{ 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512 },	//max
};

int speedList[ACT_TYPE_MAX][SERVO_MAX] = {
	// L1,  L2,  L3,  L4,  L5,  L6,  R1,  R2,  R3,  R4,  R5,  R6
	{ 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100 },	//0
	{ 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100 },	//1
	{ 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100 },	//2
	{ 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100 },	//3
	{ 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100 },	//4
	{ 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100 },	//5
};
*/

void setAngle(void);
void getAngle(void);
void getLoad(void);
void getVoltage(void);
void sendActAngle( int act );
void sendAck( int ack );
void setMode(void);
void split( char * s1 );
void MotorInit(void);
void MotorControl( int id, int power );
void ServoControl( int act );
void PrintErrorCode(void);
void PrintCommStatus(int CommStatus);
void getServoStatus(void);
void setSpeedTest( int act );
void startMotion( int motion, int status );
void forceMotion( int motion, int times );
void stopMotion(void);
void move(void);
void setModeAction();
void setDefaultTest(void);
void sensorInit(void);
void sensorTest(int);
int (*test(void))[];

//Mode
int mMode = MODE_0;
int SwitchSts = 0;
int modeWait = 0;
int modeCounter = 0;

//Motion
int motionNumber = 0;
int motionTimes = 0;
int motionCount = 1;
int motionTime = 0;
int movingTime = 0;
int motionFirst = -1;

int nextMotionNumber = 0;
int nextMotionTimes = 0;

int iStart = 1;
long watchDogCnt = 0;
int iPreWalkFlag = 0;

// Capture.
int caputureCount1 = 0;
int isCaptureMode = 0;

//Sensor
#if 0
int sensorValue[2] = {0};
int sensorValueOld[2] = {0};
int sensorValueArray[2][5];
int walkCounter = 0;
#else
int sensorValue[3] = {0};
int sensorValueOld[3] = {0};
int sensorValueArray[3][5];
int walkCounter = 0;
#endif

//Event
enum EventType {
	EVT_ACTION          = 0,
	EVT_SET_ANGLE       = 1,
	EVT_GET_NOW_ANGLE   = 2,
	EVT_GET_ACT_ANGLE   = 3,
	EVT_GET_LOAD        = 4,
	EVT_GET_VOLTAGE     = 5,
	EVT_TORQUE_DISABLE  = 6,
	EVT_START_MOTION    = 7,
	EVT_STOP_MOTION     = 8,
	EVT_FORCE_MOTION    = 9,
	EVT_WATCH_DOG       = 10,
	EVT_MOTION_EDIT     = 11,
	EVT_MAX
	};

int (*test(void))[]{
	int a[2][2] = {{3, 5}, {2, 1}};
	return a;
}

int preGyroValue = 0;
int gyroValue = 0;
int gyroTemp = 0;
int gyroCount = 0;
int mainCount = 0;

#if 1
int main(void){
	
	//Start Switch
//	DDRA  = 0x00;
//	PORTA = 0x12;
	
//	int (*atest)[] = test();
//	int a = atest[0][0];
	
	//Start PORT A for switch and IR sensors
	DDRA  = 0xFC;
	PORTA = 0xFE;
	
	//LED Initial
	DDRC  = 0x7F;
	PORTC = 0x7E;
	DDRD  = 0x70;
	PORTD = 0x11;

	MotorInit();
	initSerial();
	char * readData = NULL;	
	int isFinish = 0;
	int gyro = 0; //45 - 455
	int direction = 0;
	int speed = 0;

    sensorInit();
	printf( "### start\r\n");
	
	if (isCaptureMode ==1) dxl_write_byte( BROADCAST_ID, P_TORQUE_ENABLE, 0 );
	while(1){
        sensorTest(0);
		sensorTest(1);
		sensorTest(2);
		/*
		if (gyroValue > 0 && preGyroValue > 0) {
			gyro = preGyroValue - gyroValue;
			if(gyro != 0 && gyroValue != 0) {
				if (gyro < 0) {
					direction = 300;
					speed = ((float)-gyro / (float)410) * 1023;
				}else{
					direction = 700;
					speed = ((float)gyro / (float)410) * 1023;
				}
				// printf( "### GYRO = %d, SPEED = %d, GOAL = %d\r\n", gyro, speed, direction);
				dxl_write_word( 17, P_GOAL_SPEED_L, gyro );
				dxl_write_word( 17, P_GOAL_POSITION_L, direction ); 
			}
		}
		*/
		// getAngle();
		setDefaultTest();
		
//        sensorTest(1);
//        sensorTest(2);

//		setMode();
		if (++mainCount > 1000) {
			mainCount = 0;
			if( checkSerialRead() > 0 ){
				readData = getReadBuffer();
				if( readData != NULL ){
	//				printf( "readData=%s\n", &readData[0] );
					split( &readData[0] );
					switch( serCmd[0] ){
					case EVT_ACTION:
						ServoControl( serCmd[1] );
	//                    setSpeedTest( serCmd[1] );
						sendAck(1);
						break;
					case EVT_START_MOTION:
						startMotion( serCmd[1], serCmd[2] );
						PORTC = ~(1 << (LED_MAX - 2));
						sendAck(1);
						break;
					case EVT_STOP_MOTION:
						stopMotion();
						sendAck(1);
						break;
					case EVT_FORCE_MOTION:
						forceMotion( serCmd[1], serCmd[2] );
						break;
					case EVT_GET_NOW_ANGLE:
						getAngle();
						break;
					case EVT_SET_ANGLE:
						setAngle();
					case EVT_GET_ACT_ANGLE:
						if( serCmd[1] >= ACT_MAX ){
							sendAck(0);
						}else{
							sendActAngle(serCmd[1]);
						}
						break;
					case EVT_GET_LOAD:
						getLoad();
	//					printf( "%d\n", movingTime );
						break;
					case EVT_GET_VOLTAGE:
						getVoltage();
						break;
					case EVT_TORQUE_DISABLE:
						dxl_write_byte( BROADCAST_ID, P_TORQUE_ENABLE, 0 );
						break;
					case EVT_WATCH_DOG:
						watchDogCnt = 0;
						break;
					case EVT_MOTION_EDIT:
						break;
					case 999:
	//					printf( "finish\n");
						sendAck(999);
						isFinish = 1;
						break;
					default:
						sendAck(0);
					}
					if( isFinish > 0 ){
						MotorControl( 0, 0 );
						break;
					}
					memset( readData, 0x00, SERIAL_BUFFER_SIZE );
				}
			}
			memset( &serCmd[0], 0x00, sizeof(int) * SERIAL_BUFFER_SIZE );
		
#if 0
			if (~PINA & SW_START ) {
				if( iStart > 0 ){
					iStart = 0;
					PORTC = LED_BAT|LED_TxD|LED_RxD|LED_AUX|LED_MANAGE|LED_PROGRAM|LED_PLAY;
					if (isCaptureMode != 1) ServoControl( 0 );
				}
			}else{
				if( iStart == 0 ){
					PORTC &= ~LED_PLAY;
					iStart = 1;
				}
				if( modeWait <= 0 ){
					setModeAction();
					move();
				}else{
					modeWait -= MAIN_DELAY;
				}
			}
			if (sensorValue[0] == 0 && sensorValueOld[0] != sensorValue[0]) {
				printf( "### main() sensorValue[0] == 0\n");
				PORTC &= ~LED_PROGRAM;
			}else if (sensorValueOld[0] != sensorValue[0]){
				printf( "### main() sensorValue[0] == 1\n");
				PORTC = LED_BAT|LED_TxD|LED_RxD|LED_AUX|LED_MANAGE|LED_PLAY;
			}
		
			if (sensorValue[1] == 0 && sensorValueOld[1] != sensorValue[1]) {
				printf( "### main() sensorValue[1] == 0\n");
				PORTC &= ~LED_MANAGE;
			}else if (sensorValueOld[1] != sensorValue[1]){
				printf( "### main() sensorValue[1] == 1\n");
				PORTC = LED_BAT|LED_TxD|LED_RxD|LED_AUX|LED_PROGRAM|LED_PLAY;
			}

			if (sensorValue[2] == 0 && sensorValueOld[2] != sensorValue[2]) {
				printf( "### main() sensorValue[2] == 0\n");
				PORTC &= ~LED_AUX;
			}else if (sensorValueOld[2] != sensorValue[2]){
				printf( "### main() sensorValue[2] == 1\n");
				PORTC = LED_BAT|LED_TxD|LED_RxD|LED_MANAGE|LED_PROGRAM|LED_PLAY;
    		}
			sensorValueOld[0] = sensorValue[0];
			sensorValueOld[1] = sensorValue[1];
			sensorValueOld[2] = sensorValue[2];
#endif
		}
		
		_delay_ms(MAIN_DELAY);
		watchDogCnt++;
#if 0		
		caputureCount1++;
		if (caputureCount1 == 25){
			getAngle();
			caputureCount1 = 0;
		}
#endif
	}
}

int wait = 0;
void setDefaultTest(void){
	// printf( "### setDefaultTest\n");
	double thretholdX = 0.3L;
	
	if(wait < 2000) {
		wait++;
	} else {
		int gX = getGX();
			// printf( "###  servoId[i]:%d, angleList[ACT_DEFAULT][i]:%d\r\n",servoId[i],angleList[ACT_DEFAULT][i]);
		if (gX > (int)(thretholdX * 100)) {
			printf( "### Lean Left\r\n");
			wait = 0;
			for(int i = 0; i < SERVO_MAX; i++) {
				if (i == 2) {
					dxl_write_word( servoId[i], P_GOAL_SPEED_L, 200 );
					dxl_write_word( servoId[i], P_GOAL_POSITION_L, angleList[ACT_DEFAULT][i] - 100);
					} else if (i == 8) {
					dxl_write_word( servoId[i], P_GOAL_SPEED_L, 200 );
					dxl_write_word( servoId[i], P_GOAL_POSITION_L, angleList[ACT_DEFAULT][i]);
					} else if ( i == 5 || i == 11) {
					dxl_write_word( servoId[i], P_GOAL_SPEED_L, 200 );
					dxl_write_word( servoId[i], P_GOAL_POSITION_L, angleList[ACT_DEFAULT][i] - 20);
					} else {
					dxl_write_word( servoId[i], P_GOAL_SPEED_L, 200 );
					dxl_write_word( servoId[i], P_GOAL_POSITION_L, angleList[ACT_DEFAULT][i]);
				}
			}
		} else if (gX  < (int)(thretholdX * -100)) {
			printf( "### Lean Right\r\n");
			wait = 0;
			for(int i = 0; i < SERVO_MAX; i++) {
				if (i == 2) {
					dxl_write_word( servoId[i], P_GOAL_SPEED_L, 200 );
					dxl_write_word( servoId[i], P_GOAL_POSITION_L, angleList[ACT_DEFAULT][i]);
					} else if (i == 8 ) {
					dxl_write_word( servoId[i], P_GOAL_SPEED_L, 200 );
					dxl_write_word( servoId[i], P_GOAL_POSITION_L, angleList[ACT_DEFAULT][i] + 100);
					} else if ( i == 5 || i == 11) {
					dxl_write_word( servoId[i], P_GOAL_SPEED_L, 200 );
					dxl_write_word( servoId[i], P_GOAL_POSITION_L, angleList[ACT_DEFAULT][i] + 20);
					} else {
					dxl_write_word( servoId[i], P_GOAL_SPEED_L, 200 );
					dxl_write_word( servoId[i], P_GOAL_POSITION_L, angleList[ACT_DEFAULT][i]);
				}
			}
		} else {
			// printf( "### Not inclined to the left or right\r\n");
			for(int i = 0; i < SERVO_MAX; i++) {
			dxl_write_word( servoId[i], P_GOAL_SPEED_L, 100 );
			dxl_write_word( servoId[i], P_GOAL_POSITION_L, angleList[ACT_DEFAULT][i]);
			}
		}
	}
}

void sensorInit(void){
printf( "### sensorInit\n");
	
//	DDRA  = 0xFC;
//	PORTA = 0xFC;
	
	serial_initialize(57600);
	sei();

	ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1); // ADC Enable, Clock 1/64div.
	ADMUX = ADC_PORT_1; // ADC Port X Select
}

void sensorTest(int iNum){
	
	switch(iNum) {
	case 0:
		ADMUX = ADC_PORT_1; // ADC Port X Select
		PORTA &= ~0x80;
		break;
	case 1:
		ADMUX = ADC_PORT_2; // ADC Port X Select
		PORTA &= ~0x40;
		break;
	case 2:
		ADMUX = ADC_PORT_3; // ADC Port X Select
		PORTA &= ~0x20;
		break;
	}
	
//	if (ADMUX == ADC_PORT_1) {
//		PORTA &= ~0x80;
//	} else {
//		PORTA &= ~0x40;	
//	}
	
//	PORTA &= ~0x80;			// ADC Port 1 IR ON
//	PORTA &= ~0x40;			// ADC Port 2 IR ON
//	PORTA &= ~0x20;			// ADC Port 3 IR ON
//	PORTA &= ~0x10;			// ADC Port 4 IR ON
//	PORTA &= ~0x08;			// ADC Port 5 IR ON
//	PORTA &= ~0x04;			// ADC Port 6 IR ON

	_delay_us(12); // Short Delay for rising sensor signal
	ADCSRA |= (1 << ADIF); // AD-Conversion Interrupt Flag Clear
	ADCSRA |= (1 << ADSC); // AD-Conversion Start

	while( !(ADCSRA & (1 << ADIF)) ); // Wait until AD-Conversion complete

	PORTA = 0xFE; // IR-LED Off
	
	// setGyroX(ADC);
	switch(iNum) {
		case 0:
			setGyroX(ADC);
		break;
		case 1:
			setGyroY(ADC);
		break;
		case 2:
			setGyroZ(ADC);
		break;
	}

	gyroTemp += ADC;
	if (++gyroCount > 4) {
		preGyroValue = gyroValue;
		// gyroValue = gyroTemp / 5;
		gyroValue = gyroTemp;
		// printf( "### gyroValue %d\r\n", gyroValue); // Print Value on USART
		gyroCount = 0;
		gyroTemp = 0;
	}
//	printf( "### sensorTest() ADC:%d, i:%d\r\n", ADC, iNum); // Print Value on USART

//	_delay_ms(50);
}

void motionEdit(){
	int i = 0;
	mode1act[0][0] = serCmd[0];
	mode1act[0][1] = serCmd[1];
	for( i=2; i<SERIAL_BUFFER_SIZE ; i++ ){
		mode1act[i/2][0] = serCmd[i];
		mode1act[i/2][1] = serCmd[i+1];
		i++;
	}
}

void sendAck( int ack ){
	printf("%d\n\n", ack );
}

void setMode(void){
	//		PORTC = ~(LED_BAT|LED_TxD|LED_RxD|LED_AUX|LED_MANAGE|LED_PROGRAM|LED_PLAY);
	//		PORTC = LED_BAT|LED_TxD|LED_RxD|LED_AUX|LED_MANAGE|LED_PROGRAM|LED_PLAY;
	if( ~PIND & SW_BUTTON ){
//        printf( "PIND is OFF\n");
		if( SwitchSts == 0 ){
			SwitchSts = 1;
		}
	}else{
//		printf( "PIND is ON\n");	
		if( SwitchSts == 1 ){
			SwitchSts = 0;
			mMode++;
			if( (LED_MAX - mMode) <= 0 ){
				PORTC = LED_BAT|LED_TxD|LED_RxD|LED_AUX|LED_MANAGE|LED_PROGRAM;
				mMode = MODE_0;
			}else{
				PORTC = ~(1 << (LED_MAX - mMode));
			}
			stopMotion();
			ServoControl( 0 );

			modeCounter = 1;
			if( mMode == MODE_1 ){
				modeWait = mode1act[0][0];
			}
		}
	}
}

void setModeAction(){
	if( motionTimes <= 0 ){
		if( mMode == MODE_1 ){
			if( iPreWalkFlag == 0 &&
			    (mode1act[modeCounter][0] == ACT_WALK1 || mode1act[modeCounter][0] == ACT_WALK1) ){
				startMotion( ACT_PRE_WALK, 1 );
				iPreWalkFlag = 1;
			}else{
				iPreWalkFlag = 0;
				startMotion( mode1act[modeCounter][0], mode1act[modeCounter][1] );
				if( ++modeCounter > mode1act[0][1] ){
//					PORTC = LED_BAT|LED_TxD|LED_RxD|LED_AUX|LED_MANAGE|LED_PROGRAM|LED_PLAY;
//					mMode = MODE_0;
					modeCounter = 1;
				}
			}
		}
	}
}

void MotorInit(void){
	dxl_initialize( 0, DEFAULT_BAUDNUM ); // Not using device index
	//Wheel Mode
//	dxl_write_word( 31, P_CW_ANGLE_LIMIT_L, 0 );
//	dxl_write_word( 31, P_CCW_ANGLE_LIMIT_L, 0 );
	//Set EEP Lock
	dxl_write_word( 31, P_EEP_LOCK, 1 );
	// Set goal speed
	dxl_write_word( BROADCAST_ID, P_GOAL_SPEED_L, 0 );
	dxl_write_byte( BROADCAST_ID, P_TORQUE_ENABLE, 0 );
	_delay_ms(1000);
}

void split( char * s1 )
{
//	char s1[] = "this is a pen. Hello-World...";
	char s2[] = " ,";
	char *tok;
	int cnt = 0;

	tok = strtok( s1, s2 );
	while( tok != NULL ){
//		printf( "%s\n", tok );
		serCmd[cnt++] = atoi(tok);
		tok = strtok( NULL, s2 );  /* 2��ڈȍ~ */
	}

}

int isMoving(void){
//	for(int i=0; i<SERVO_MAX; i++ ){
//		if( dxl_read_byte( servoId[i], P_MOVING ) != 0 ){
//			return 1;
//		}
//	}
	motionTime += MAIN_DELAY;
	if( motionTime < movingTime ){
		return 1;
	}else{
//		printf( "I%d:%d;\n", motionTime, movingTime );
		motionTime = 0;
	}

	return 0;	
}

void setAngle(void){
	int act = serCmd[1];
	if( act >= ACT_MAX ) return;
	
	for( int i=0; i<SERVO_MAX+1; i++ ){
		angleList[act][i] = serCmd[2+i];
	}
}

void getAngle(){
	int tmp[SERVO_MAX] = {0};
	for(int i=0; i<SERVO_MAX; i++ ){
		tmp[i] = dxl_read_word( servoId[i], P_PRESENT_POSITION_L );
	}
	printf( "%d:{%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d}\n",
			EVT_GET_NOW_ANGLE, tmp[0],tmp[1],tmp[2],tmp[3],tmp[4],tmp[5],tmp[6],tmp[7],tmp[8],tmp[9],tmp[10],tmp[11] );

}

void sendActAngle( int act ){
	printf( "%d:%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d\n",
	        EVT_GET_ACT_ANGLE, angleList[act][0],angleList[act][1],angleList[act][2],angleList[act][3],
			angleList[act][4],angleList[act][5],angleList[act][6],angleList[act][7],
			angleList[act][8],angleList[act][9],angleList[act][10],angleList[act][11], angleList[act][12] );
}

void getLoad(void){
	int tmp[SERVO_MAX] = {0};
	for(int i=0; i<SERVO_MAX; i++ ){
		tmp[i] = dxl_read_word( servoId[i], P_PRESENT_LOAD_L );
	}
	
	printf( "%d:%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d\n",
	EVT_GET_LOAD, tmp[0],tmp[1],tmp[2],tmp[3],tmp[4],tmp[5],tmp[6],tmp[7],tmp[8],tmp[9],tmp[10],tmp[11] );
}

void getVoltage(void){
	int tmp[SERVO_MAX] = {0};
	for(int i=0; i<SERVO_MAX; i++ ){
		tmp[i] = dxl_get_lowbyte(dxl_read_word( servoId[i], P_PRESENT_VOLTAGE ));
	}
	
	printf( "%d:%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d\n",
	EVT_GET_VOLTAGE, tmp[0],tmp[1],tmp[2],tmp[3],tmp[4],tmp[5],tmp[6],tmp[7],tmp[8],tmp[9],tmp[10],tmp[11] );
}

void MotorControl( int id, int power ){
	int CommStatus;
	printf( "%d %d\n", id, power );
	if (0) {
		dxl_write_word( id, P_GOAL_SPEED_L, power );
//		dxl_write_word( id, P_GOAL_POSITION_L, power );
		CommStatus = dxl_get_result();
		if( CommStatus == COMM_RXSUCCESS )
			PrintErrorCode();
		else
			PrintCommStatus(CommStatus);
	}else{
		dxl_set_txpacket_id(BROADCAST_ID);
		dxl_set_txpacket_instruction(INST_SYNC_WRITE);
		dxl_set_txpacket_parameter(0, P_GOAL_SPEED_L);
		dxl_set_txpacket_parameter(1, 2);
		dxl_set_txpacket_parameter(2, id);
		dxl_set_txpacket_parameter(3, dxl_get_lowbyte(power));
		dxl_set_txpacket_parameter(4, dxl_get_highbyte(power));
		dxl_set_txpacket_length(4+3*1);
		dxl_txrx_packet();
		CommStatus = dxl_get_result();
		if( CommStatus == COMM_RXSUCCESS )
			PrintErrorCode();
		else
			PrintCommStatus(CommStatus);
	}
}

void startMotion( int motion, int times ){
	nextMotionNumber = motion;
	nextMotionTimes = times;
	if( motionTimes == 0 ){
		motionNumber = nextMotionNumber;
		motionTimes = nextMotionTimes;
		nextMotionTimes = 0;
	}
}

void forceMotion( int motion, int times ){
	motionNumber = motion;
	motionTimes = times;
	nextMotionTimes = 0;
	motionCount = 1;
	motionTime = 0;
}

void stopMotion(void){
	motionCount = 1;
	motionTimes = 0;
	motionTime = 0;
	motionFirst = -1;
}

void move(void){
	if( /* motionTimes > 0 && */ isMoving() == 0 ){
		int *motion = motionList[motionNumber];
//        printf("### motionNumber = %d, motion = %d\n", motionNumber, *motion);
		int max = motion[0];
		if( motionCount > max ){
//			if (motionNumber != ACT_WALK2) {
		        printf("### move 1\n");
				motionNumber = ACT_WALK2;
				/*
			}else{
				if (sensorValue[2] == 1) {
    		        printf("### move 2\n");
					motionNumber = ACT_TURN_LEFT;
					walkCounter = 0;
				} else if (sensorValue[0] == 1 && sensorValue[1] == 1) {
					printf("### move 3\n");
					motionNumber = ACT_WALK2;
					walkCounter = 0;
				} else if (sensorValue[0] == 1 && sensorValue[1] == 0) {
					printf("### move 4\n");
//					motionNumber = ACT_TURN_LEFT;
                    walkCounter = 0;
				} else if (sensorValue[0] == 0 && sensorValue[1] == 1) {
					printf("### move 5\n");
					motionNumber = ACT_TURN_RIGHT;
					walkCounter = 0;
				} else if (++walkCounter > 5) {
					printf("### move 6\n");
					motionNumber = ACT_TURN_LEFT;
					walkCounter = 0;
				}
				*/
//			}
			
        printf("### motionCount > max. motionCount:%d, max:%d\n", motionCount, max);
//			printf("#%d,%d,%d,%d,%d,%d;\n", diffmaxTest[0],diffmaxTest[1],diffmaxTest[2],diffmaxTest[3],diffmaxTest[4],diffmaxTest[5] );
			motionCount = 1;
			/*
			if( motionTimes < 99 && --motionTimes <= 0 ){
				if( nextMotionTimes > 0 ){
					if( (nextMotionNumber == ACT_WALK1 || nextMotionNumber == ACT_WALK2) &&
						motionNumber != ACT_PRE_WALK ){
						motionNumber = ACT_PRE_WALK;
						motionTimes = 1;
					}else if( (motionNumber == ACT_TURN_LEFT && nextMotionNumber == ACT_TURN_RIGHT) ||
						(nextMotionNumber == ACT_TURN_LEFT && motionNumber == ACT_TURN_RIGHT) ){
						motionNumber = ACT_DEFAULT;
						motionTimes = 1;
					}else{
						motionNumber = nextMotionNumber;
						motionTimes = nextMotionTimes;
						nextMotionTimes = 0;
					}
					_delay_ms(300);
				}else{
					stopMotion();
				}
				return;
			}
			*/
		}
		printf("### motionCount:%d\n", motionCount);
		ServoControl( motion[motionCount] );
		motionCount++;
	}
}

void setSpeedTest( int act ){
	//GetAngle
	int angle = 0;
	int diffMax = 0;
	int angleDiff[SERVO_MAX] = {0};
	for(int i=0; i<SERVO_MAX; i++ ){
//		angle = dxl_read_word( servoId[i], P_PRESENT_POSITION_L );
		angle = angleList[0][i];
		angleDiff[i] = angleList[act][i] - angle;
		if( angleDiff[i] < 0 ){
			angleDiff[i] = angleDiff[i] * -1;
		}
		if( diffMax < angleDiff[i] ){
			diffMax = angleDiff[i];
		}
	}
	
	int speed[SERVO_MAX] = {100};
	for(int i=0; i<SERVO_MAX; i++ ){
		speed[i] = (int)((float)(angleList[act][SERVO_MAX]) * ((float)angleDiff[i] / diffMax));
		if( speed[i] == 0 ){
			speed[i] = 1;
		}
	}
	
	movingTime = ((float)CYCLE_TIME/VALUE_MAX) * ((float)VALUE_MAX / angleList[act][SERVO_MAX]) * diffMax;
	
	printf("%d;\n", movingTime );
	
//	printf( "%d:%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d: %d:\n",
//	EVT_ACTION, speed[0],speed[1],speed[2],speed[3],speed[4],
//	speed[5],speed[6],speed[7],speed[8],speed[9],speed[10],speed[11], movingTime );
}

void ServoControl( int act ){
	int i;
	int CommStatus = 0;
	if( act >= ACT_MAX ){
//		printf( "act error: %d / %d\n", act, SERVO_MAX );
		return;
	}
	
	//GetAngle
	int angle = 0;
	int diffMax = 0;
	int angleDiff[SERVO_MAX] = {0};
	for(int i=0; i<SERVO_MAX; i++ ){
//		if( motionFirst < 0 ){
			angle = dxl_read_word( servoId[i], P_PRESENT_POSITION_L );
//		}else{
//			angle = angleList[motionFirst][i];
//		}
		angleDiff[i] = angleList[act][i] - angle;
		if( angleDiff[i] < 0 ){
			angleDiff[i] = angleDiff[i] * -1;
		}
		if( diffMax < angleDiff[i] ){
			diffMax = angleDiff[i];
		}
	}
//	motionFirst = act;
	
	int speed[SERVO_MAX] = {100};
	for(int i=0; i<SERVO_MAX; i++ ){
		speed[i] = (int)((float)(angleList[act][SERVO_MAX]) * ((float)angleDiff[i] / diffMax));
		if( speed[i] == 0 ){
			speed[i] = 1;
		}
	}

//    diffmaxTest[motionCount-1] = diffMax;
//	movingTime = ((float)CYCLE_TIME/VALUE_MAX) * ((float)VALUE_MAX / angleList[act][SERVO_MAX]) * diffMax;
    movingTime = diffMax * (float)(((VALUE_MAX*10)/angleList[act][SERVO_MAX])/2);
	if( movingTime < MAIN_DELAY ){
		movingTime = MAIN_DELAY;
	}
	
	//Speed
	dxl_set_txpacket_id(BROADCAST_ID);
	dxl_set_txpacket_instruction(INST_SYNC_WRITE);
	dxl_set_txpacket_parameter(0, P_GOAL_SPEED_L);
	dxl_set_txpacket_parameter(1, 2);
	for( i=0; i<SERVO_MAX; i++ ){
		dxl_set_txpacket_parameter(2+(3*i), servoId[i]);
		dxl_set_txpacket_parameter(3+(3*i), dxl_get_lowbyte(speed[i]));
		dxl_set_txpacket_parameter(4+(3*i), dxl_get_highbyte(speed[i]));
	}
	dxl_set_txpacket_length(4+3*SERVO_MAX);
	dxl_txrx_packet();
	CommStatus = dxl_get_result();
	if( CommStatus == COMM_RXSUCCESS ){
		PrintErrorCode();
		
		//Angle
		dxl_set_txpacket_id(BROADCAST_ID);
		dxl_set_txpacket_instruction(INST_SYNC_WRITE);
		dxl_set_txpacket_parameter(0, P_GOAL_POSITION_L);
		dxl_set_txpacket_parameter(1, 2);
		for( i=0; i<SERVO_MAX; i++ ){
			dxl_set_txpacket_parameter(2+(3*i), servoId[i]);
			dxl_set_txpacket_parameter(3+(3*i), dxl_get_lowbyte(angleList[act][i]));
			dxl_set_txpacket_parameter(4+(3*i), dxl_get_highbyte(angleList[act][i]));
		}
		dxl_set_txpacket_length(4+3*SERVO_MAX);
		dxl_txrx_packet();
		CommStatus = dxl_get_result();
		if( CommStatus == COMM_RXSUCCESS ){
			PrintErrorCode();
		}else{
			PrintCommStatus(CommStatus);
		}
	}else{
		PrintCommStatus(CommStatus);
	}
}

void ServoReadState(){
	
}

void PrintErrorCode()
{
	if(dxl_get_rxpacket_error(ERRBIT_VOLTAGE) == 1)
	printf("Input voltage error!\n");

	if(dxl_get_rxpacket_error(ERRBIT_ANGLE) == 1)
	printf("Angle limit error!\n");

	if(dxl_get_rxpacket_error(ERRBIT_OVERHEAT) == 1)
	printf("Overheat error!\n");

	if(dxl_get_rxpacket_error(ERRBIT_RANGE) == 1)
	printf("Out of range error!\n");

	if(dxl_get_rxpacket_error(ERRBIT_CHECKSUM) == 1)
	printf("Checksum error!\n");

	if(dxl_get_rxpacket_error(ERRBIT_OVERLOAD) == 1)
	printf("Overload error!\n");

	if(dxl_get_rxpacket_error(ERRBIT_INSTRUCTION) == 1)
	printf("Instruction code error!\n");
}

// Print communication result
void PrintCommStatus(int CommStatus)
{
	switch(CommStatus)
	{
		case COMM_TXFAIL:
		printf("COMM_TXFAIL: Failed transmit instruction packet!\n");
		break;

		case COMM_TXERROR:
		printf("COMM_TXERROR: Incorrect instruction packet!\n");
		break;

		case COMM_RXFAIL:
		printf("COMM_RXFAIL: Failed get status packet from device!\n");
		break;

		case COMM_RXWAITING:
		printf("COMM_RXWAITING: Now recieving status packet!\n");
		break;

		case COMM_RXTIMEOUT:
		printf("COMM_RXTIMEOUT: There is no status packet!\n");
		break;

		case COMM_RXCORRUPT:
		printf("COMM_RXCORRUPT: Incorrect status packet!\n");
		break;

		default:
		printf("This is unknown error code!\n");
		break;
	}
}
#endif








#if 0

#define PI	3.141592f
#define SW_START 0x01

/// Control table address
#define P_GOAL_POSITION_L	30
#define P_GOAL_POSITION_H	31
#define P_GOAL_SPEED_L		32
#define P_GOAL_SPEED_H		33
#define P_TORQUE_LIMIT_L	34
#define P_TORQUE_LIMIT_H	35
#define P_PRESENT_POSITION_L 36
#define P_PRESENT_POSITION_H 37
#define P_PRESENT_LOAD_L    40
#define P_PRESENT_LOAD_H    41
#define P_EEP_LOCK  		47

// Defulat setting
//#define MODE_SYNC
#define MODE_WRITE
#define DEFAULT_BAUDNUM		1 // 1Mbps
#define NUM_ACTUATOR		3 // Number of actuator
#define STEP_THETA			(PI / 100.0f) // Large value is more fast
#define CONTROL_PERIOD		(1000) // msec (Large value is more slow)

#define LED_BAT 0x01
#define LED_TxD 0x02
#define LED_RxD 0x04
#define LED_AUX 0x08
#define LED_MANAGE 0x10
#define LED_PROGRAM 0x20
#define LED_PLAY 0x40

#define SW_START 0x01

volatile int isFinish = 0;
volatile int Value = 0;

void PrintCommStatus(int CommStatus);
void PrintErrorCode(void);
void getServoStatus(void);

typedef struct{
	int id;
	int angleLimitCW;
	int angleLimitCCW;
	int defPosition;
	int PresentPosition;
	int PresentLoad;
}stServo;

stServo *mServoList[NUM_ACTUATOR] = {NULL};

int main(void)
{
#if 0
	int id[NUM_ACTUATOR];
	float phase[NUM_ACTUATOR];
	float theta = 0;
	int AmpPos = 512;
	//int AmpPos = 2048; // for EX series
	int GoalPos;
	int i;
	int CommStatus;
	int isPress = 0;
	int isOn = 0;
	unsigned char ReceivedData;
	int Value;
	mServoList[0] = (stServo *)malloc(sizeof(stServo));
	memset((void *)mServoList[0], 0x00, sizeof(stServo) );
	mServoList[0]->id = 4;

	serial_initialize(57600);
	dxl_initialize( 0, DEFAULT_BAUDNUM ); // Not using device index
	sei();	// Interrupt Enable
	
	printf( "\n\nSyncWrite example for CM-700\n\n" );
	
#ifdef MODE_SYNC
	for( i=0; i<NUM_ACTUATOR; i++ )
	{
		id[i] = i+2;
		phase[i] = 2*PI * (float)i / (float)NUM_ACTUATOR;
	}
#else
	int wPresentPos = 512;
#endif	
	
	//Set EEP Lock
	dxl_write_word( BROADCAST_ID, P_EEP_LOCK, 1 );
	// Set goal speed
	dxl_write_word( BROADCAST_ID, P_GOAL_SPEED_L, 0 );
	// Set goal position
	dxl_write_word( BROADCAST_ID, P_GOAL_POSITION_L, AmpPos );
	dxl_write_word( 4, P_TORQUE_LIMIT_L, 0);
	_delay_ms(1000);
	
	while(1)
	{
		if(~PIND & SW_START){
			isPress = 1;
		}else{
 		    if( isPress == 1 ){
				if( isOn == 0 ){
					isOn = 1;
				}else{
					isOn = 0;
				}
			}
			isPress = 0;
		}
		
//		while( ReceivedData = getchar() != NULL ){
			if(ReceivedData == 'u')
			Value++;
			else if(ReceivedData == 'd')
			Value--;
			printf("%d, %d\r\n", Value, ReceivedData);
//		}
		
		if( isOn ){
#ifdef MODE_SYNC
		// Make syncwrite packet
		dxl_set_txpacket_id(BROADCAST_ID);
		dxl_set_txpacket_instruction(INST_SYNC_WRITE);
		dxl_set_txpacket_parameter(0, P_GOAL_POSITION_L);
		dxl_set_txpacket_parameter(1, 2);
		for( i=0; i<NUM_ACTUATOR; i++ )
		{
			dxl_set_txpacket_parameter(2+3*i, id[i]);
			GoalPos = (int)((sin(theta+phase[i]) + 1.0) * (float)AmpPos);
			printf( "%d  ", GoalPos );
			dxl_set_txpacket_parameter(2+3*i+1, dxl_get_lowbyte(GoalPos));
			dxl_set_txpacket_parameter(2+3*i+2, dxl_get_highbyte(GoalPos));
		}
		dxl_set_txpacket_length((2+1)*NUM_ACTUATOR+4);
		
		printf( "\n" );
		
		dxl_txrx_packet();
		CommStatus = dxl_get_result();
		if( CommStatus == COMM_RXSUCCESS )
		PrintErrorCode();
		else
		PrintCommStatus(CommStatus);
		
		theta += STEP_THETA;

		if( theta > 2*PI )
		theta -= 2*PI;
#else
	    wPresentPos = dxl_read_word( 4, P_PRESENT_POSITION_L );
        printf( "%d\n", wPresentPos );

		dxl_write_word( 2, P_GOAL_POSITION_L, wPresentPos );
		dxl_write_word( 3, P_GOAL_POSITION_L, wPresentPos );
		PrintErrorCode();
#endif
		}
		getServoStatus();
		_delay_ms(CONTROL_PERIOD);
	}
	return 0;
#endif

#if 0
	DDRC  = 0x7F;
	PORTC = 0x7E;
	
	DDRD  = 0x70;
	PORTD = 0x11;

	while (1)
	{
		if(~PIND & SW_START)
		PORTC = ~(LED_BAT|LED_TxD|LED_RxD|LED_AUX|LED_MANAGE|LED_PROGRAM|LED_PLAY);
		else PORTC = LED_BAT|LED_TxD|LED_RxD|LED_AUX|LED_MANAGE|LED_PROGRAM|LED_PLAY;
	}
	return 1;
#endif


	while(isFinish == 0){
		_delay_ms(500);
		getSerialData();
//		ReceivedData = getchar();

		//if(ReceivedData == 'u'){
			//printf("%d\r\n", Value);
			//Value++;
		//}else if(ReceivedData == 'd'){
			//printf("%d\r\n", Value);
			//Value--;
		//}else if(ReceivedData == 10 || ReceivedData == 13 ){
			//printf("%s\r\n", "end");
			//break;
		//}
		printf("%s\r\n", "Loop");
	}

	printf("%s\r\n", "finish");

	return 0;
}

void getSerialData(void){
	if( serial_get_qstate() > 0 ){
		printf("serialReceiveCommand\r\n" );
		unsigned char ReceivedData = getchar();
		if(ReceivedData == 'u'){
			printf("%d\r\n", Value);
			Value++;
		}else if(ReceivedData == 'd'){
			printf("%d\r\n", Value);
			Value--;
		}else if(ReceivedData == 10 || ReceivedData == 13 ){
			printf("%s\r\n", "end");abcd
			isFinish = 1;
		}
	}
}

void getServoStatus(void){
	for( int i=0; i<NUM_ACTUATOR; i++ ){
		if( mServoList[i] != NULL && mServoList[i]->id > 0 ){
		    mServoList[i]->PresentPosition = dxl_read_word( mServoList[i]->id, P_PRESENT_POSITION_L );
		    mServoList[i]->PresentLoad = dxl_read_word( mServoList[i]->id, P_PRESENT_LOAD_L );
			printf( "%d, %d %d\n", mServoList[i]->id, mServoList[i]->PresentPosition, mServoList[i]->PresentLoad );
		}
	}
}


// Print communication result
void PrintCommStatus(int CommStatus)
{
	switch(CommStatus)
	{
		case COMM_TXFAIL:
		printf("COMM_TXFAIL: Failed transmit instruction packet!\n");
		break;

		case COMM_TXERROR:
		printf("COMM_TXERROR: Incorrect instruction packet!\n");
		break;

		case COMM_RXFAIL:
		printf("COMM_RXFAIL: Failed get status packet from device!\n");
		break;

		case COMM_RXWAITING:
		printf("COMM_RXWAITING: Now recieving status packet!\n");
		break;

		case COMM_RXTIMEOUT:
		printf("COMM_RXTIMEOUT: There is no status packet!\n");
		break;

		case COMM_RXCORRUPT:
		printf("COMM_RXCORRUPT: Incorrect status packet!\n");
		break;

		default:
		printf("This is unknown error code!\n");
		break;
	}
}

// Print error bit of status packet
void PrintErrorCode()
{
	if(dxl_get_rxpacket_error(ERRBIT_VOLTAGE) == 1)
	printf("Input voltage error!\n");

	if(dxl_get_rxpacket_error(ERRBIT_ANGLE) == 1)
	printf("Angle limit error!\n");

	if(dxl_get_rxpacket_error(ERRBIT_OVERHEAT) == 1)
	printf("Overheat error!\n");

	if(dxl_get_rxpacket_error(ERRBIT_RANGE) == 1)
	printf("Out of range error!\n");

	if(dxl_get_rxpacket_error(ERRBIT_CHECKSUM) == 1)
	printf("Checksum error!\n");

	if(dxl_get_rxpacket_error(ERRBIT_OVERLOAD) == 1)
	printf("Overload error!\n");

	if(dxl_get_rxpacket_error(ERRBIT_INSTRUCTION) == 1)
	printf("Instruction code error!\n");
}
#endif