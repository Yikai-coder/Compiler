#include "SyntaxAnalyzer.h"
#include "my_include.h"
#include "symbol_table_element.h"

using namespace std;

/// <summary>
/// 读取filename中的action_table，goto_table与产生式，同时将非终结符号也加入token_table中？
/// </summary>
/// <param name="filename">LR表的文件名</param>
/// <param name="token_table">编码表，在这里进一步更新</param>
void SyntaxAnalyzer::ReadActionTable(string& filename, map<string, int> & token_table)
{
	ifstream readfile(filename);
	if (!readfile)
	{
		cout << "Fail to read action table" << endl;
		exit(-1);
	}
	string line;
	int idx_i = 0;
	vector<int> header;
	int goto_table_start;    // 标记goto_table的起始列
	int token_table_end = token_table.size() - 1;
	// 读取一行
	while (getline(readfile, line))
	{
		// 掠过表标题
		if (idx_i == 0)
		{
			int idx_j = 0;
			string field;
			istringstream sin(line);
			while (getline(sin, field, '\t'))
			{
				if (field == "GOTO")
					break;
				idx_j++;
			}
			goto_table_start = idx_j;
			idx_i++;
			continue;
		}
		// 读取表头
		else if (idx_i == 1)
		{
			int idx_j = 0;
			string field;
			istringstream sin(line);
			while (getline(sin, field, '\t'))
			{
				if (idx_j > 0)
				{
					// 读取action_table
					if (idx_j < goto_table_start)
					{
						map<string, int>::const_iterator cit = token_table.find(field);
						if (cit == token_table.end())
						{
							cout << "Unknown token " << field << " in action table!" << endl;
							exit(-1);
						}
						header.push_back(cit->second);
					}
					// 读取goto_table
					else
					{
						int id = token_table_end + (idx_j - goto_table_start) + 1;
						token_table.insert({ field, id });
						header.push_back(id);
					}
				}
				idx_j++;
			}
		}
		// 读取最后一行产生式序列
		else if (line[0] == '[')  // 目前使用[Var]表示非终结符号
		{
			string field;
			istringstream sin(line);
			while (getline(sin, field, '\t'))
			{
				pair<string, vector<string>> production;
				int split_pos = field.find(':');    // 寻找左右部分隔符号逗号的位置
				production.first = field.substr(0, split_pos);   // 左部的起始位置为3，持续到逗号前的单引号
				istringstream right(field.substr(split_pos + 1, field.length() - split_pos - 1));
				string component;
				while (getline(right, component, ' '))
					production.second.push_back(component);
				this->production_list.push_back(production);
			}
		}
		// 读取表内容
		else
		{
			string field;
			istringstream sin(line);
			int idx_j = 0;
			while (getline(sin, field, '\t'))
			{
				if (field != "" && idx_j != 0)
				{
					action act;
					pair<action, int> second;
					if (field[0] == 'r')
					{
						act = r;
						second = { act, stoi(field.substr(2, field.length() - 2)) };
					}
					else if (field[0] == 's')
					{
						act = s;
						second = { act, stoi(field.substr(2, field.length() - 2)) };
					}
					else if (field[0] == 'a')
					{
						act = a;
						second = { act, 0 };
					}
					else if (field[0] == 'j')
					{
						act = j;
						second = { act, stoi(field.substr(2, field.length() - 2)) };
					}
					else
					{
						cout << "Can not read field content of " << field << endl;
						exit(-1);
					}
					pair<int, int> first = { idx_i - 2, header[idx_j-1] };
					this->action_table.insert({ first, second });
				}
				idx_j++;
			}
		}
		idx_i++;
	}
	readfile.close();
}

/// <summary>
/// 构造函数
/// </summary>
/// <param name="token_list">单词序列</param>
/// <param name="LRTableFileName">输入文件名，指LR分析表</param>
/// <param name="token_table">编码表</param>
/// <param name="outputFileName">输出文件名</param>
SyntaxAnalyzer::SyntaxAnalyzer(const vector<pair<int, string>> & token_list, string & LRTableFileName, map<string, int> & token_table, vector<symbol_table_element>& symbol_table)
{
	this->tmp_num = 0;
	ReadActionTable(LRTableFileName, token_table);
	ParseTokenList(token_list, token_table, symbol_table);

}

const vector<tripleCode>& SyntaxAnalyzer::getTripleCodes()
{
	// TODO: 在此处插入 return 语句
	return this->tripleCodes;
}


/// <summary>
/// 处理单词序列，进行语法分析
/// </summary>
/// <param name="token_list">单词序列</param>
/// <param name="token_table">编码表</param>
void SyntaxAnalyzer::ParseTokenList(const vector<pair<int, string>> token_list, const map<string, int> & token_table, vector<symbol_table_element>& symbol_table)
{
	this->lr_stack.push({ ORIGIN_STATE, END_SYMBOL_ID });   // 压入栈初始状态
	symbolElement tmp_ele;
	this->symbol_stack.push(tmp_ele);
	vector<pair<int, string>>::const_iterator it = token_list.begin();
	//ofstream outputFile(outputFileName);
	while (!lr_stack.empty())
	{
		int state = this->lr_stack.top().first;
		map<pair<int, int>, pair<action, int>>::iterator cit = (this->action_table).find({ state, it->first });
		if (cit == action_table.end())
		{
			cout << "Can not find state (" << state <<"," << it->first << ") in action table" << endl;
			exit(-1);
		}
		pair<action, int> field = cit->second;
		if (field.first == s)
		{
			this->lr_stack.push({ field.second, it->first });
			symbolElement ele;
			if (it->first == 1 || it->first == 2)
				ele.addr = stoi(it->second);
			else
				ele.addr = 1e7;
			this->symbol_stack.push(ele);
			it++;
		}
		else if (field.first == r)
		{
			// 寻找产生式并打印
			pair<string, vector<string>> production = this->production_list[field.second];
			// 执行规约产生式对应的语义动作
			ExecuteProductionAction(field.second, this->symbol_stack, symbol_table);
			//OutputProduction(production, outputFile);
			production_gen.push_back(production);
			// 根据产生式右部长度弹出栈
			int size = production.second.size();
			for (int i = 0; i < size; i++)
				this->lr_stack.pop();
			// 根据栈顶状态和产生式左部查找表确定跳转状态
			map<string, int>::const_iterator nit = token_table.find(production.first);
			if (nit == token_table.end())
			{
				cout << "Unknown left component " << production.first << " of production" << endl;
				exit(-1);
			}
			map<pair<int, int>, pair<action, int>>::iterator cit1 = (this->action_table).find({ this->lr_stack.top().first, nit->second });
			if (cit1 == this->action_table.end())
			{
				cout << "Can not find state " << this->lr_stack.top().first <<" " << nit->second << " in goto table" << endl;
				exit(-1);
			}
			this->lr_stack.push({ cit1->second.second, nit->second });
		}
		else if (field.first == a)
		{
			cout << "Accept a program" << endl;
			break;
		}
		else
		{
			cout << "Unknown action of " << field.first << endl;
			exit(-1);
		}
	}
	//outputFile.close();
}

/// <summary>
/// 打印产生式并输出到文件当中
/// </summary>
/// <param name="production">产生式数据结构，是一个pair，first为左部，为字符串；second为右部，右部为字符串数组</param>
/// <param name="outputFile">输出的文件的文件流</param>
void SyntaxAnalyzer::OutPutProductions(string& outputFile)
{
	ofstream ofs(outputFile);
	for(vector<pair<string, vector<string>>>::iterator production = this->production_gen.begin(); production!= this->production_gen.end(); production++)
	{
		vector<string>::iterator sit = production->second.begin();
		cout << production->first << "->";
		ofs << production->first << "->";
		while (sit != production->second.end())
		{
			cout << *sit << " ";
			ofs << *sit << " ";
			sit++;
		}
		cout << endl;
		ofs << endl;
	}
	ofs.close();
}

void SyntaxAnalyzer::OutputTripleCode(string& outputFile)
{
	ofstream ofs(outputFile);
	for (vector <tripleCode>::iterator it = tripleCodes.begin(); it != tripleCodes.end(); it++)
	{
		ofs << it->operation <<'\t' << it->arg1 <<'\t' << it->arg2 <<'\t' << it->result << endl;
	}
	ofs.close();
}


void SyntaxAnalyzer::ExecuteProductionAction(int productionNum, stack<symbolElement>& symbol_stack, vector<symbol_table_element>& symbol_table)
{
	switch (productionNum)
	{
		// M-> epsilon
		case 0:
		{
			symbolElement ele;
			symbol_stack.pop();
			symbol_stack.push(ele);
		}
		// D->int
		case 1:
		{
			symbolElement ele;
			ele.type = "int";
			ele.width = 4;
			symbol_stack.pop();
			symbol_stack.push(ele);
			symbol_table_element tbl_ele("D");
			tbl_ele.type = INT;
			symbol_table.push_back(tbl_ele);
			break;
		}
		// M -> S M
		case 2: 
		{
			symbol_table_element tbl_ele("M");
			int new_addr = symbol_table.size() - 1;
			int addr1 = symbol_stack.top().addr;
			symbol_stack.pop();
			int addr2 = symbol_stack.top().addr;
			symbol_stack.pop();
			//printf("%d  %d  %d\n", new_addr, addr1, addr2);
			tbl_ele.address = new_addr;
			symbol_table.push_back(tbl_ele);
			symbolElement ele;
			ele.addr = new_addr;
			symbol_stack.push(ele);
			break;
		}
		// S->D ID #
		case 3:
		{
			// 首先弹出句末符号
			symbol_stack.pop();
			int id_addr = symbol_stack.top().addr;
			symbol_table[id_addr].type = INT;
			symbol_stack.pop();
			symbol_stack.pop();
			symbolElement ele;
			symbol_stack.push(ele);
			break;
		}
		// E->A
		case 4:
		{
			int addr = symbol_stack.top().addr;
			symbol_stack.pop();
			symbolElement ele;
			ele.addr = addr;
			symbol_stack.push(ele);
			symbol_table_element tbl_ele("E");
			tbl_ele.address = addr;
			symbol_table.push_back(tbl_ele);
			break;
		}
		// A->B
		case 5:
		{
			int addr = symbol_stack.top().addr;
			symbol_stack.pop();
			symbolElement ele;
			ele.addr = addr;
			symbol_stack.push(ele);
			symbol_table_element tbl_ele("A");
			tbl_ele.address = addr;
			symbol_table.push_back(tbl_ele);
			break; 
		}
		// B->ID
		case 6:
		// B-> int_constant
		case 7:
		{
			int id_addr = symbol_stack.top().addr;
			symbol_stack.pop();
			symbolElement ele;
			ele.addr = id_addr;
			symbol_stack.push(ele);
			symbol_table_element tbl_ele("B");
			tbl_ele.address = id_addr;
			symbol_table.push_back(tbl_ele);
			break;
		}
		// S->ID = E #
		case 8:
		{
			symbol_stack.pop();            // 弹出句末符号
			int addr = symbol_stack.top().addr;
			symbol_stack.pop();
			symbol_stack.pop();
			int id_addr = symbol_stack.top().addr;
			symbol_stack.pop();
			//ofs << "=\t" << symbol_table[id_addr].name <<"\t \t" << symbol_table[addr].name << endl;
			//printf("=\t%d\t \t%d\n", id_addr, addr);
			//cout << "=\t" << symbol_table[id_addr].name << "\t \t" << symbol_table[addr].name << endl;
			tripleCode code;
			code.operation = "ASSIGN";
			code.arg1 = symbol_table[id_addr].name;
			code.arg2 = -1;
			code.result = symbol_table[addr].name;
			this->tripleCodes.push_back(code);
			symbolElement ele;
			symbol_stack.push(ele);
			//symbol_table_element tbl_ele("B");
			//tbl_ele.address = id_addr;
			//symbol_table.push_back(tbl_ele);
			break;
		}
		// E->E+A
		case 9:
		{
			int tmp_num = newTmp();
			string var_name = "tmp" + to_string(tmp_num);
			symbol_table_element tbl_ele(var_name);
			symbol_table.push_back(tbl_ele);
			int new_addr = symbol_table.size() - 1;
			int addr2 = symbol_stack.top().addr;
			symbol_stack.pop();
			symbol_stack.pop();
			int addr1 = symbol_stack.top().addr;
			symbol_stack.pop();

			//ofs << "+\t" << symbol_table[addr1].name <<"\t"<< symbol_table[addr2].name <<"\t" << symbol_table[new_addr].name << endl;
			//cout << "+\t" << symbol_table[addr1].name << "\t" << symbol_table[addr2].name << "\t" << symbol_table[new_addr].name << endl;

			//printf("+\t%d\t%d\t%d\n", addr1, addr2, new_addr);
			tripleCode code;
			code.operation = "PLUS";
			code.arg1 = symbol_table[addr1].name;
			code.arg2 = symbol_table[addr2].name;
			code.result = symbol_table[new_addr].name;
			this->tripleCodes.push_back(code);
			symbolElement ele;
			ele.addr = new_addr;
			symbol_stack.push(ele);
			break; 
		}
		// E->E-A
		case 10:
		{
			symbol_table_element tbl_ele("tmp"+to_string(newTmp()));
			symbol_table.push_back(tbl_ele);
			int new_addr = symbol_table.size() - 1;
			int addr2 = symbol_stack.top().addr;
			symbol_stack.pop();
			symbol_stack.pop();
			int addr1 = symbol_stack.top().addr;
			symbol_stack.pop();
			//ofs << "-\t" << symbol_table[addr1].name << "\t" << symbol_table[addr2].name << "\t" << symbol_table[new_addr].name << endl;
			//cout << "-\t" << symbol_table[addr1].name << "\t" << symbol_table[addr2].name << "\t" << symbol_table[new_addr].name << endl;
			tripleCode code;
			code.operation = "MINUS";
			code.arg1 = symbol_table[addr1].name;
			code.arg2 = symbol_table[addr2].name;
			code.result = symbol_table[new_addr].name;
			this->tripleCodes.push_back(code);
			symbolElement ele;
			ele.addr = new_addr;
			symbol_stack.push(ele);
			break;
		}
		// A->A*B
		case 11:
		{
			symbol_table_element tbl_ele("tmp" + to_string(newTmp()));
			symbol_table.push_back(tbl_ele);
			int new_addr = symbol_table.size() - 1;
			int addr2 = symbol_stack.top().addr;
			symbol_stack.pop();
			symbol_stack.pop();
			int addr1 = symbol_stack.top().addr;
			symbol_stack.pop();
			//ofs << " * " << addr1 <<"\t" << addr2 <<"\t" << new_addr << endl;
			//printf("*\t%d\t%d\t%d\n", addr1, addr2, new_addr);
			//ofs << "*\t" << symbol_table[addr1].name << "\t" << symbol_table[addr2].name << "\t" << symbol_table[new_addr].name << endl;
			//cout << "*\t" << symbol_table[addr1].name << "\t" << symbol_table[addr2].name << "\t" << symbol_table[new_addr].name << endl;
			tripleCode code;
			code.operation = "MULTIPLE";
			code.arg1 = symbol_table[addr1].name;
			code.arg2 = symbol_table[addr2].name;
			code.result = symbol_table[new_addr].name;
			this->tripleCodes.push_back(code);
			symbolElement ele;
			ele.addr = new_addr;
			symbol_stack.push(ele);
			break;
		}
		// B->(E)
		case 12:
		{
			symbol_stack.pop();
			int addr = symbol_stack.top().addr;
			symbol_stack.pop();
			symbol_stack.pop();
			symbolElement ele;
			ele.addr = addr;
			symbol_stack.push(ele);
			symbol_table_element tbl_ele("B");
			tbl_ele.address = addr;
			symbol_table.push_back(tbl_ele);
			break;
		}
	}
	//ofs.close();
}

int SyntaxAnalyzer::newTmp()
{
	return this->tmp_num++;
}

int SyntaxAnalyzer::CheckSymbolTable(string& token, const vector<symbol_table_element>& symbol_table)
{
	for (int i = 0; i < symbol_table.size(); i++)
	{
		if (symbol_table[i].name == token)
			return i;
	}
	return -1;
}