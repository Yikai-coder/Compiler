#include "CodeGenerator.h"

using namespace std;

/// <summary>
/// ��������ַ��
/// </summary>
/// <param name="tripleCodes">����ַ������</param>
void CodeGenerator::parseTripleCodes(const vector<tripleCode> & tripleCodes)
{
	int res_reg_num;
	int arg1_reg_num;
	int arg2_reg_num;
	for (vector<tripleCode>::const_iterator it = tripleCodes.begin(); it != tripleCodes.end(); it++)
	{
		// ����Ϊ�������Ĵ������Ȳ鿴��������Ƿ����ڼĴ�����
		res_reg_num = checkVarInReg(it->result);
		if (res_reg_num == -1)
		{
			res_reg_num = getReg(it->result);
		}
		// Ȼ��Ϊarg1����Ĵ������Ȳ鿴��������Ƿ����ڼĴ�����
		arg1_reg_num = checkVarInReg(it->arg1);
		if (arg1_reg_num == -1)
		{
			arg1_reg_num = getReg(it->arg1);
		}
		// ���ݲ�ͬ��operation���в���
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
/// ��ʼ���Ĵ���
/// </summary>
void CodeGenerator::init()
{
	for (int i = 0; i < REGNUM; i++)
		reg[i] = "\0";
}
/// <summary>
/// ���ĳ�������Ƿ���ڼĴ�����
/// </summary>
/// <param name="var">������</param>
/// <returns>�Ĵ�����</returns>
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
/// Ϊĳ����������һ���Ĵ���
/// </summary>
/// <param name="var">������</param>
/// <returns>�Ĵ�����</returns>
int CodeGenerator::getReg(string var)
{
	// ����Ѱ�ҿ��еļĴ���
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
	// ���мĴ�������ռ���ˣ���Ҫ����һ����Ĭ��ȫ������reg0
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
/// ����Ŀ�����
/// </summary>
/// <param name="tripleCodes"></param>
/// <returns></returns>
vector<ASMCode>& CodeGenerator::GetAsmCode()
{
	return this->ASMCodes;
}
/// <summary>
/// �����ɵ�Ŀ�����������ļ�
/// </summary>
/// <param name="outputFile">����ļ���</param>
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
