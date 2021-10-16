#include "SyntaxAnalyzer.h"
#include "my_include.h"

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
SyntaxAnalyzer::SyntaxAnalyzer(const vector<pair<int, string>> & token_list, string & LRTableFileName, map<string, int> & token_table, string& outputFileName)
{
	
	ReadActionTable(LRTableFileName, token_table);
	ParseTokenList(token_list, token_table, outputFileName);

}

/// <summary>
/// 处理单词序列，进行语法分析
/// </summary>
/// <param name="token_list">单词序列</param>
/// <param name="token_table">编码表</param>
void SyntaxAnalyzer::ParseTokenList(const vector<pair<int, string>> token_list, const map<string, int> & token_table, string & outputFileName)
{
	this->lr_stack.push({ ORIGIN_STATE, END_SYMBOL_ID });   // 压入栈初始状态
	vector<pair<int, string>>::const_iterator it = token_list.begin();
	ofstream outputFile(outputFileName);
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
			it++;
		}
		else if (field.first == r)
		{
			// 寻找产生式并打印
			pair<string, vector<string>> production = this->production_list[field.second];
			OutputProduction(production, outputFile);
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
	outputFile.close();
}

/// <summary>
/// 打印产生式并输出到文件当中
/// </summary>
/// <param name="production">产生式数据结构，是一个pair，first为左部，为字符串；second为右部，右部为字符串数组</param>
/// <param name="outputFile">输出的文件的文件流</param>
void SyntaxAnalyzer::OutputProduction(std::pair<std::string, std::vector<std::string>>& production, std::ofstream& outputFile)
{
	vector<string>::iterator sit = production.second.begin();
	cout << production.first << "->";
	outputFile << production.first << "->";
	while (sit != production.second.end())
	{
		cout << *sit << " ";
		outputFile << *sit << " ";
		sit++;
	}
	cout << endl;
	outputFile << endl;
}