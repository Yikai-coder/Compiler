#include "SyntaxAnalyzer.h"
#include "my_include.h"
using namespace std;
/// <summary>
/// ��ȡfilename�е�action_table��goto_table�����ʽ��ͬʱ�����ս����Ҳ����token_table�У�
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
		// ��ȡ��ͷ
		else if (idx_i == 1)
		{
			int idx_j = 0;
			string field;
			istringstream sin(line);
			while (getline(sin, field, ';'))
			{
				// ��Ա�ͷ����","�����⴦��
				// csv�ļ���","���ָ��Ԫ���еĶ��Żᱻת����","���Ӷ�����getline������ʱ����������������
				//if (field == "\"")
				//{
				//	getline(sin, field, ',');
				//	field = ",";
				//}
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
			while (getline(sin, field, ';'))
			{
				pair<string, vector<string>> generator;
				int split_pos = field.find(':');    // Ѱ�����Ҳ��ָ����Ŷ��ŵ�λ��
				generator.first = field.substr(0, split_pos);   // �󲿵���ʼλ��Ϊ3������������ǰ�ĵ�����
				istringstream right(field.substr(split_pos + 1, field.length() - split_pos - 1));
				string component;
				while (getline(right, component, ' '))
					generator.second.push_back(component);
				this->generator_list.push_back(generator);
			}
		}
		// ��ȡ������
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
	this->lr_stack.push({ ORIGIN_STATE, END_SYMBOL_ID });   // ѹ��ջ��ʼ״̬
	vector<pair<int, string>>::const_iterator it = token_list.begin();
	// TODO:ѭ��������û���
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
			// Ѱ�Ҳ���ʽ����ӡ
			pair<string, vector<string>> generator = this->generator_list[field.second];
			vector<string>::iterator sit = generator.second.begin();
			cout << generator.first << "->";
			while (sit != generator.second.end())
			{
				cout << *sit << " ";
				sit++;
			}
			cout << endl;
			// ���ݲ���ʽ�Ҳ����ȵ���ջ
			int size = generator.second.size();
			for (int i = 0; i < size; i++)
				this->lr_stack.pop();
			// ����ջ��״̬�Ͳ���ʽ�󲿲��ұ�ȷ����ת״̬
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