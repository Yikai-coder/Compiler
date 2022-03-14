#include "symbol_table_element.h"

symbol_table_element::symbol_table_element(string name) :name(name) {};
symbol_table_element::symbol_table_element(string name, int address, symbol_type type):name(name), address(address), type(type) {};
