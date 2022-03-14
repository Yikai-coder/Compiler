#include "CodeGenerator.h"

using namespace std;

/// <summary>
/// 处理三地址码
/// </summary>
/// <param name="tripleCodes">三地址码序列</param>
void CodeGenerator::parseTripleCodes(const vector<tripleCode> & tripleCodes)
{
	int res_reg_num;
	int arg1_reg_num;
	int arg2_reg_num;
	for (vector<tripleCode>::const_iterator it = tripleCodes.begin(); it != tripleCodes.end(); it++)
	{
		// 首先为结果分配寄存器，先查看结果变量是否已在寄存器中
		res_reg_num = checkVarInReg(it->result);
		if (res_reg_num == -1)
		{
			res_reg_num = getReg(it->result);
		}
		// 然后为arg1分配寄存器，先查看结果变量是否已在寄存器中
		arg1_reg_num = checkVarInReg(it->arg1);
		if (arg1_reg_num == -1)
		{
			arg1_reg_num = getReg(it->arg1);
		}
		// 根据不同的operation进行操作
		if (it->operation == "ASSIGN")
		{
			ASMCode code;
			code.ins = "MOV";
			code.arg1 = "R" + to_string(res_reg_num);
			code.arg2 = "R" + to_string(arg1_reg_num);
			code.arg3 = "\0";
			ASMCodes.push_back(code);
		}
		else if (it->operation == "PLUS")
		{
			arg2_reg_num = checkVarInReg(it->arg2);
			if (arg2_reg_num == -1)
			{
				arg2_reg_num = getReg(it->arg2);
			}
			ASMCode code;
			code.ins = "ADD";
			code.arg1 = "R" + to_string(res_reg_num);
			code.arg2 = "R" + to_string(arg1_reg_num);
			code.arg3 = "R" + to_string(arg2_reg_num);
			ASMCodes.push_back(code);
		}
		else if (it->operation == "MINUS")
		{
			arg2_reg_num = checkVarInReg(it->arg2);
			if (arg2_reg_num == -1)
			{
				arg2_reg_num = getReg(it->arg2);
			}
			ASMCode code;
			code.ins = "SUB";
			code.arg1 = "R" + to_string(res_reg_num);
			code.arg2 = "R" + to_string(arg1_reg_num);
			code.arg3 = "R" + to_string(arg2_reg_num);
			ASMCodes.push_back(code);
		}
		else if (it->operation == "MULTIPLE")
		{
			arg2_reg_num = checkVarInReg(it->arg2);
			if (arg2_reg_num == -1)
			{
				arg2_reg_num = getReg(it->arg2);
			}
			ASMCode code;
			code.ins = "MUL";
			code.arg1 = "R" + to_string(res_reg_num);
			code.arg2 = "R" + to_string(arg1_reg_num);
			code.arg3 = "R" + to_string(arg2_reg_num);
			ASMCodes.push_back(code);
		}
		else
		{
			cout << "Unknown operation" << it->operation << endl;
			exit(-1);
		}
	}
}
/// <summary>
/// 初始化寄存器
/// </summary>
void CodeGenerator::init()
{
	for (int i = 0; i < REGNUM; i++)
		reg[i] = "\0";
}
/// <summary>
/// 检查某个变量是否存在寄存器中
/// </summary>
/// <param name="var">变量名</param>
/// <returns>寄存器号</returns>
int CodeGenerator::checkVarInReg(string var)
{
	for (int i = 0; i < REGNUM; i++)
	{
		if (this->reg[i] == var)
			return i;
	}
	return -1;
}
/// <summary>
/// 为某个变量分配一个寄存器
/// </summary>
/// <param name="var">变量名</param>
/// <returns>寄存器号</returns>
int CodeGenerator::getReg(string var)
{
	// 首先寻找空闲的寄存器
	for (int i = 0; i < REGNUM; i++)
	{
		if (reg[i] == "\0")
		{
			reg[i] = var;
			ASMCode code;
			code.ins = "MOV";
			code.arg1 = var;
			code.arg2 = "R" + to_string(i);
			ASMCodes.push_back(code);
			return i;
		}
	}
	// 所有寄存器都被占用了，需要换出一个，默认全部换出reg0
	ASMCode code;
	code.ins = "MOV";
	code.arg1 = "R0";
	code.arg2 = reg[0];
	ASMCodes.push_back(code);
	reg[0] = var;
	code.ins = "MOV";
	code.arg1 = var;
	code.arg2 = "R" + to_string(0);
	ASMCodes.push_back(code);
	return 0;
}

CodeGenerator::CodeGenerator(const vector<tripleCode>& tripleCodes)
{
	init();
	parseTripleCodes(tripleCodes);
}

/// <summary>
/// 返回目标代码
/// </summary>
/// <param name="tripleCodes"></param>
/// <returns></returns>
vector<ASMCode>& CodeGenerator::GetAsmCode()
{
	return this->ASMCodes;
}
/// <summary>
/// 将生成的目标代码输出到文件
/// </summary>
/// <param name="outputFile">输出文件名</param>
void CodeGenerator::OutputAsmCode(string& outputFile)
{
	ofstream ofs(outputFile);
	for (int i = 0; i < this->ASMCodes.size(); i++)
	{
		ofs << this->ASMCodes[i].ins << " " << this->ASMCodes[i].arg1 << " "<< this->ASMCodes[i].arg3 << " "<<this->ASMCodes[i].arg2 << endl;
		cout << this->ASMCodes[i].ins << " " << this->ASMCodes[i].arg1 << " " << this->ASMCodes[i].arg3 << " " << this->ASMCodes[i].arg2 << endl;
	}
	ofs.close();
}
