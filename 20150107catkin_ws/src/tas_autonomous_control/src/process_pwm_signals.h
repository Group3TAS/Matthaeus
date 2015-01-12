#ifndef PROCESS_PWM_DATA_H_GUARD
#define PROCESS_PWM_DATA_H_GUARD

#include "process_pwm_signals.h"
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <ios>
#include <cstdio>
#include <sstream>

using namespace std;

void pwm_data_write(int t,int speed,int angle);

#endif