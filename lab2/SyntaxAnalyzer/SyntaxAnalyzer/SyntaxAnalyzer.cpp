#include "SyntaxAnalyzer.h"
#include "my_include.h"
using namespace std;
/// <summary>
/// 读取filename中的action_table，goto_table与产生式，同时将非终结符号也加入token_table中？
/// </summary>
/// <param name="filename"></param>
void SyntaxAnalyzer::readActionTable(string& filename, map<string, int> & token_table)
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
			while (getline(sin, field, ';'))
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
			while (getline(sin, field, ';'))
			{
				// 针对表头出现","的特殊处理
				// csv文件用","做分割，单元格中的逗号会被转化成","，从而在用getline解析的时候会解析出两个引号
				//if (field == "\"")
				//{
				//	getline(sin, field, ',');
				//	field = ",";
				//}
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
			while (getline(sin, field, ';'))
			{
				pair<string, vector<string>> generator;
				int split_pos = field.find(':');    // 寻找左右部分隔符号逗号的位置
				generator.first = field.substr(0, split_pos);   // 左部的起始位置为3，持续到逗号前的单引号
				istringstream right(field.substr(split_pos + 1, field.length() - split_pos - 1));
				string component;
				while (getline(right, component, ' '))
					generator.second.push_back(component);
				this->generator_list.push_back(generator);
			}
		}
		// 读取表内容
		else
		{
			string field;
			istringstream sin(line);
			int idx_j = 0;
			while (getline(sin, field, ';'))
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

SyntaxAnalyzer::SyntaxAnalyzer(const vector<pair<int, string>> & token_list, string & filename, map<string, int> & token_table)
{
	readActionTable(filename, token_table);
	parseTokenList(token_list, token_table);

}

void SyntaxAnalyzer::parseTokenList(const vector<pair<int, string>> token_list, const map<string, int> & token_table)
{
	this->lr_stack.push({ ORIGIN_STATE, END_SYMBOL_ID });   // 压入栈初始状态
	vector<pair<int, string>>::const_iterator it = token_list.begin();
	// TODO:循环条件还没想好
	while (1)
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
			it++;
		}
		else if (field.first == r)
		{
			// 寻找产生式并打印
			pair<string, vector<string>> generator = this->generator_list[field.second];
			vector<string>::iterator sit = generator.second.begin();
			cout << generator.first << "->";
			while (sit != generator.second.end())
			{
				cout << *sit << " ";
				sit++;
			}
			cout << endl;
			// 根据产生式右部长度弹出栈
			int size = generator.second.size();
			for (int i = 0; i < size; i++)
				this->lr_stack.pop();
			// 根据栈顶状态和产生式左部查找表确定跳转状态
			map<string, int>::const_iterator nit = token_table.find(generator.first);
			if (nit == token_table.end())
			{
				cout << "Unknown left component " << generator.first << " of generator" << endl;
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
}