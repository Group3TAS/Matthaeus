#ifndef WLAN_DATA_H_GUARD
#define WLAN_DATA_H_GUARD

#include "wlan_data.h"
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <ios>
#include <cstdio>
#include <sstream>

/*

Technik Autonomer Systeme

17 January 2015

Group 3

Header file for wlan_data function, which is 
called in the program pose_data

*/

namespace wlan_data 
{
  std::string wlan_address(std::string&);
  std::string wlan_signal(std::string&);
  std::string wlan_essid(std::string&);
  std::string wlan_quality(std::string&);
}

void wlan_data_write(float, float,std::vector<float>, std::vector<float>);

#endif
