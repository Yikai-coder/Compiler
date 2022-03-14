#pragma once
#include "my_include.h"
#include "symbol_table_element.h"
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

// ��¼�ڷ���ջ�е�Ԫ��
typedef struct symbolElement{
	int addr;
	char* type;
	int width;
}symbolElement;

class SyntaxAnalyzer
{
private:
	map<pair<int, int>, pair<action, int>> action_table;  // ����action_table��goto_table
	vector <string> var_table;                            // ��ŷ��ս����
	vector<pair<string, vector<string>>> production_list; // LR�������еĲ���ʽ����
	vector<pair<string, vector<string>>> production_gen;   // ���������в����Ĳ���ʽ
 	stack<pair<int, int>> lr_stack;                       // ջ��Ԫ��Ϊ<״̬������>�����б������Ϊ�������룬Ҳ����Ϊ���ս���ű���
	stack<symbolElement> symbol_stack;                  // TODO: ���Ǻ������ջ�ϲ�Ϊһ��
	vector <tripleCode> tripleCodes;                      // ����ַ������
	int tmp_num;                                          // ��¼��ʱ���������

	void ReadActionTable(string& filename, map<string, int> & token_table);

	void ParseTokenList(const vector<pair<int, string>> token_list, 
						const map<string, int>& token_table,
						vector<symbol_table_element>& symbol_table);



	void ExecuteProductionAction(int productionNum, stack<symbolElement>& symbol_stack, vector<symbol_table_element>& symbol_table);

	int newTmp();

	int CheckSymbolTable(string& token, const vector<symbol_table_element>& symbol_table);

public:
	SyntaxAnalyzer(const vector<pair<int, string>>& token_list, 
					string& LRTableFileName, 
					map<string, int>& token_table, 
					vector<symbol_table_element> & symbol_table);

	const vector <tripleCode> & getTripleCodes();

	void OutPutProductions(string& outputFile);

	void OutputTripleCode(string& outputFile);

};

