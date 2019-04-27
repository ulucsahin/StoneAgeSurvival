// Fill out your copyright notice in the Description page of Project Settings.


#include "MyUtility.h"
#include <random>


MyUtility::MyUtility(){
}

MyUtility::~MyUtility(){
}

float MyUtility::RandomFloat(float min, float max) {
	return ((float)rand() / RAND_MAX) * (max - min) + min;
}