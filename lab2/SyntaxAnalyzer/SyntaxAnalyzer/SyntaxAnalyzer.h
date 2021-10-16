#pragma once
#include "my_include.h"
// ���빤��̨���ɵ�״̬���ʼ״̬Ϊ0������������#����idΪ0
#define ORIGIN_STATE 0
#define END_SYMBOL_ID 33
// ���ս���ŵı��
#define NONTERMINAL_CHAR_START 70

using namespace std;

enum action {
	s,    // �ƽ�
	r,    // ��Լ
	a,    // ��ɽ���
	j     // ��ת
};
class SyntaxAnalyzer
{
private:
	map<pair<int, int>, pair<action, int>> action_table;  // ����action_table��goto_table
	vector<pair<string, vector<string>>> production_list; // ����ʽ����
 	stack<pair<int, int>> lr_stack;                       // ջ��Ԫ��Ϊ<״̬������>�����б������Ϊ�������룬Ҳ����Ϊ���ս���ű���

	void ReadActionTable(string& filename, map<string, int> & token_table);

	void ParseTokenList(const vector<pair<int, string>> token_list, 
						const map<string, int>& token_table,
						string & outputFileName);

	void OutputProduction(std::pair<std::string, 
							std::vector<std::string>>& production, 
							std::ofstream & outputFile);

public:
	SyntaxAnalyzer(const vector<pair<int, string>>& token_list, 
									string& LRTableFileName, 
									map<string, int>& token_table, 
									string& outputFileName);
};

