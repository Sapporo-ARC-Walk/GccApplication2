/*
 * GyroManager.c
 * 3軸加速度センサ KXR94-2050
 * Created: 2016/07/11
 *  Author: Ikeno
 */ 

#include <stdio.h>
#include <math.h>

#define AVEREGE_TIMES 100
#define PI 3.141592

long mTempX = 0;
int mGyroX = 0;
int mGyroCount = 0;

float GYRO_X_MIN = 249.0; // -90deg
float GYRO_X_MAX = 727.0; // +90deg

int mAngleX = 0;
void setGyroX(int);
float getAngleX();

void setGyroX(int gyroX) {
	// Get AVEREGE
	if (mGyroCount >= 0 && mGyroCount < AVEREGE_TIMES) {
		mTempX = mTempX + gyroX;
		mGyroCount++;
	} else if (mGyroCount >= AVEREGE_TIMES)  {
		mGyroX = mTempX / AVEREGE_TIMES;
		printf( "### mGyroX %d\r\n", mGyroX);
		mGyroCount = 0;
		mTempX = 0;
		
		float angleX = getAngleX();
		printf( "### getAngleX %d.%d%d\r\n", (int)(angleX), abs(((int)(angleX * 10) % 10)), abs(((int)(angleX * 100) % 10)));
		
	} else {
		printf( "### Oops!\r\n");
		mGyroCount = 0;
		mTempX = 0;
	}
}

float getAngleX() {
	// 取得した値を0Gの時0となるようシフトさせ、Gを求める
	float gyroXMid = (GYRO_X_MAX + GYRO_X_MIN)/2;
	float gravityX = (mGyroX - gyroXMid)/(gyroXMid/2);
	char *sign;
	if (signbit(gravityX)) {
		sign = "-";
	} else {
		sign = "+";
	}
	printf( "### gravityX %s%d.%d%dG\r\n", sign, (int)(gravityX), abs(((int)(gravityX * 10) % 10)), abs(((int)(gravityX * 100) % 10)));
	
	// ラジアンから度への変換
	float angleX = asin(gravityX) * 180 / PI;
	
	return angleX;
}