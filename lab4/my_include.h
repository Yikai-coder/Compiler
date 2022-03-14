#pragma once
#include <map>
#include <string>
#include <vector>
#include <stack>
#include <fstream>
#include <iostream>
#include <sstream>
//#include <Windows.h>
using namespace std;
// 三地址码结构体 
typedef struct tripleCode {
	string operation;
	string arg1;
	string arg2;
	string result;
}tripleCode;