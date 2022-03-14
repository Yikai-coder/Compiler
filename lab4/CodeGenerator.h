#pragma once
#include "my_include.h"
#define REGNUM 10
using namespace std;
// Ŀ����� ins arg1 arg2
typedef struct ASMCode {
	string ins;
	string arg1;
	string arg2;
	string arg3;
}ASMCode;

class CodeGenerator
{
private:
	string reg[REGNUM];              // ��¼ÿ���Ĵ����д�ŵı����ڷ��ű��е����
	vector<ASMCode> ASMCodes;
	void parseTripleCodes(const vector<tripleCode>& tripleCodes);
	void init();
	int checkVarInReg(string var);
	int getReg(string var);
public:
	CodeGenerator(const vector<tripleCode>& tripleCodes);
	vector<ASMCode> & GetAsmCode();
	void OutputAsmCode(string& outputFile);
};

