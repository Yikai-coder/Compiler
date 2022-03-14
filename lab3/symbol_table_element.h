#pragma once
#include "my_include.h"
using namespace std;
enum symbol_type {
	INT,
	FLOAT,
	DOUBLE
};

class symbol_table_element
{
public:
	string name;
	int address;
	symbol_type type;
	symbol_table_element(string name);
	symbol_table_element(string name, int address, symbol_type type);
};

