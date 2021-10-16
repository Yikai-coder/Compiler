#include "SyntaxAnalyzer.h"
#include "my_include.h"

using namespace std;

/// <summary>
/// ��ȡfilename�е�action_table��goto_table�����ʽ��ͬʱ�����ս����Ҳ����token_table�У�
/// </summary>
/// <param name="filename">LR����ļ���</param>
/// <param name="token_table">������������һ������</param>
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
	int goto_table_start;    // ���goto_table����ʼ��
	int token_table_end = token_table.size() - 1;
	// ��ȡһ��
	while (getline(readfile, line))
	{
		// �ӹ������
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
		// ��ȡ��ͷ
		else if (idx_i == 1)
		{
			int idx_j = 0;
			string field;
			istringstream sin(line);
			while (getline(sin, field, '\t'))
			{
				if (idx_j > 0)
				{
					// ��ȡaction_table
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
					// ��ȡgoto_table
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
		// ��ȡ���һ�в���ʽ����
		else if (line[0] == '[')  // Ŀǰʹ��[Var]��ʾ���ս����
		{
			string field;
			istringstream sin(line);
			while (getline(sin, field, '\t'))
			{
				pair<string, vector<string>> production;
				int split_pos = field.find(':');    // Ѱ�����Ҳ��ָ����Ŷ��ŵ�λ��
				production.first = field.substr(0, split_pos);   // �󲿵���ʼλ��Ϊ3������������ǰ�ĵ�����
				istringstream right(field.substr(split_pos + 1, field.length() - split_pos - 1));
				string component;
				while (getline(right, component, ' '))
					production.second.push_back(component);
				this->production_list.push_back(production);
			}
		}
		// ��ȡ������
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
/// ���캯��
/// </summary>
/// <param name="token_list">��������</param>
/// <param name="LRTableFileName">�����ļ�����ָLR������</param>
/// <param name="token_table">�����</param>
/// <param name="outputFileName">����ļ���</param>
SyntaxAnalyzer::SyntaxAnalyzer(const vector<pair<int, string>> & token_list, string & LRTableFileName, map<string, int> & token_table, string& outputFileName)
{
	
	ReadActionTable(LRTableFileName, token_table);
	ParseTokenList(token_list, token_table, outputFileName);

}

/// <summary>
/// ���������У������﷨����
/// </summary>
/// <param name="token_list">��������</param>
/// <param name="token_table">�����</param>
void SyntaxAnalyzer::ParseTokenList(const vector<pair<int, string>> token_list, const map<string, int> & token_table, string & outputFileName)
{
	this->lr_stack.push({ ORIGIN_STATE, END_SYMBOL_ID });   // ѹ��ջ��ʼ״̬
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
			// Ѱ�Ҳ���ʽ����ӡ
			pair<string, vector<string>> production = this->production_list[field.second];
			OutputProduction(production, outputFile);
			// ���ݲ���ʽ�Ҳ����ȵ���ջ
			int size = production.second.size();
			for (int i = 0; i < size; i++)
				this->lr_stack.pop();
			// ����ջ��״̬�Ͳ���ʽ�󲿲��ұ�ȷ����ת״̬
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
/// ��ӡ����ʽ��������ļ�����
/// </summary>
/// <param name="production">����ʽ���ݽṹ����һ��pair��firstΪ�󲿣�Ϊ�ַ�����secondΪ�Ҳ����Ҳ�Ϊ�ַ�������</param>
/// <param name="outputFile">������ļ����ļ���</param>
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