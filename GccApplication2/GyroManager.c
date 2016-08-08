/*
 * GyroManager.c
 * 3軸加速度センサ KXR94-2050
 * Created: 2016/07/11
 *  Author: Ikeno
 */ 

#include <stdio.h>
#include <math.h>

#define AVEREGE_TIMES 100
#define PI 3.14

long mTempX = 0;
long mTempY = 0;
long mTempZ = 0;
int mGyroX = 0;
int mGyroY = 0;
int mGyroZ = 0;
int mGyroCountX = 0;
int mGyroCountY = 0;
int mGyroCountZ = 0;

// -90deg
float GYRO_X_MIN = 249.0;
float GYRO_Y_MIN = 249.0;
float GYRO_Z_MIN = 307.0;

// +90deg
float GYRO_X_MAX = 727.0; 
float GYRO_Y_MAX = 727.0;
float GYRO_Z_MAX = 705.0;

void setGyroX(int);
void setGyroY(int);
void setGyroZ(int);
float getGX();
float getGY();
float getGZ();

void setGyroX(int gyroX) {
	// Get AVEREGE
	if (mGyroCountX >= 0 && mGyroCountX < AVEREGE_TIMES) {
		mTempX = mTempX + gyroX;
		mGyroCountX++;
	} else if (mGyroCountX >= AVEREGE_TIMES)  {
		mGyroX = mTempX / AVEREGE_TIMES;
		printf( "### mGyroX %d\r\n", mGyroX);
		mGyroCountX = 0;
		mTempX = 0;
		
		float gravityX = getGX();
		
	} else {
		printf( "### Oops!\r\n");
		mGyroCountX = 0;
		mTempX = 0;
	}
}

void setGyroY(int gyroY) {
	// Get AVEREGE
	if (mGyroCountY >= 0 && mGyroCountY < AVEREGE_TIMES) {
		mTempY = mTempY + gyroY;
		mGyroCountY++;
		} else if (mGyroCountY >= AVEREGE_TIMES)  {
		mGyroY = mTempY / AVEREGE_TIMES;
		printf( "### mGyroY %d\r\n", mGyroY);
		mGyroCountY = 0;
		mTempY = 0;
		
		float gravityY = getGY();
		
		} else {
		printf( "### Oops!\r\n");
		mGyroCountY = 0;
		mTempY = 0;
	}
}

void setGyroZ(int gyroZ) {
	// Get AVEREGE
	if (mGyroCountZ >= 0 && mGyroCountZ < AVEREGE_TIMES) {
		mTempZ = mTempZ + gyroZ;
		mGyroCountZ++;
	} else if (mGyroCountZ >= AVEREGE_TIMES)  {
		mGyroZ = mTempZ / AVEREGE_TIMES;
		printf( "### mGyroZ %d\r\n", mGyroZ);
		mGyroCountZ = 0;
		mTempZ = 0;
		
		float gravityZ = getGZ();
		
	} else {
		printf( "### Oops!\r\n");
		mGyroCountZ = 0;
		mTempZ = 0;
	}
}

float getGX() {
	// 取得した値を0Gの時0となるようシフトさせ、Gを求める
	float gyroXMid = (GYRO_X_MAX + GYRO_X_MIN)/2;
	float gravityX = (mGyroX - gyroXMid)/(GYRO_X_MAX - gyroXMid);
	char *sign;
	if (signbit(gravityX)) {
		sign = "-";
	} else {
		sign = "+";
	}
	printf( "### gravityX %s%d.%d%dG\r\n", sign, abs((int)(gravityX)), abs(((int)(gravityX * 10) % 10)), abs(((int)(gravityX * 100) % 10)));

	return gravityX;
}

float getGY() {
	// 取得した値を0Gの時0となるようシフトさせ、Gを求める
	float gyroYMid = (GYRO_Y_MAX + GYRO_Y_MIN)/2;
	float gravityY = (mGyroY - gyroYMid)/(GYRO_Y_MAX - gyroYMid);
	char *sign;
	if (signbit(gravityY)) {
		sign = "-";
	} else {
		sign = "+";
	}
	printf( "### gravityY %s%d.%d%dG\r\n", sign, abs((int)(gravityY)), abs(((int)(gravityY * 10) % 10)), abs(((int)(gravityY * 100) % 10)));
	
	return gravityY;
}

float getGZ() {
	// 取得した値を0Gの時0となるようシフトさせ、Gを求める
	float gyroZMid = (GYRO_Z_MAX + GYRO_Z_MIN)/2;
	float gravityZ = (mGyroZ - gyroZMid)/(GYRO_Z_MAX - gyroZMid);
	char *sign;
	if (signbit(gravityZ)) {
		sign = "-";
	} else {
		sign = "+";
	}
	printf( "### gravityZ %s%d.%d%dG\r\n", sign, abs((int)(gravityZ)), abs(((int)(gravityZ * 10) % 10)), abs(((int)(gravityZ * 100) % 10)));
	
	return gravityZ;
}