#pragma once

#include <iostream>
#include <fstream>
#include <algorithm>
#include <map>
#include <string>

bool processInput(std::string &line, double &val);
bool handle_date(std::string &date, std::string &line1);
void print_the_result(std::map<std::string, float> &data ,std::string &date, double &value);