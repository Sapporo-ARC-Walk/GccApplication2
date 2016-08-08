/*
 * GyroManager.h
 *
 * Created: 2016/07/11
 *  Author: Ikeno
 */ 


#ifndef INCFILE1_H_
#define INCFILE1_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>

extern void setGyroX(int);
extern void setGyroY(int);
extern void setGyroZ(int);
extern float getGX();
extern float getGY();
extern float getGZ();

#endif /* INCFILE1_H_ */