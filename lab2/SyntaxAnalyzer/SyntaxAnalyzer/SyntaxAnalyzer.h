#pragma once
#include "my_include.h"
// ���빤��̨���ɵ�״̬���ʼ״̬Ϊ0������������#����idΪ0
#define ORIGIN_STATE 0
#define END_SYMBOL_ID 33
// ���÷��ս���ŵı�ţ�������token_table���棬����д��token_table
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
	//private:
		// action_table:<state, token>-><action, num>

public: 
	map<pair<int, int>, pair<action, int>> action_table;  // ����action_table��goto_table
	vector<pair<string, vector<string>>> generator_list;    // ����ʽ����
	// map<string, int> & local_token_table;
 	stack<pair<int, int>> lr_stack;           // ջ��Ԫ��Ϊ<״̬������>�����б������Ϊ�������룬Ҳ����Ϊ���ս���ű���

	void readActionTable(string& filename, map<string, int> & token_table);

	SyntaxAnalyzer(const vector<pair<int, string>> & token_list, string & filename, map<string, int> & token_table);

	void parseTokenList(const vector<pair<int, string>> token_list, const map<string, int>& token_table);
	//public:
	
};

