#pragma once
#include "my_include.h"
#include "symbol_table_element.h"
// 编译工作台生成的状态表初始状态为0，将结束符号#定义id为0
#define ORIGIN_STATE 0
#define END_SYMBOL_ID 33
// 非终结符号的编号
#define NONTERMINAL_CHAR_START 70

using namespace std;

enum action {
	s,    // 移进
	r,    // 规约
	a,    // 完成接受
	j     // 跳转
};

// 记录在符号栈中的元素
typedef struct symbolElement{
	int addr;
	char* type;
	int width;
}symbolElement;

class SyntaxAnalyzer
{
private:
	map<pair<int, int>, pair<action, int>> action_table;  // 包含action_table与goto_table
	vector <string> var_table;                            // 存放非终结符号
	vector<pair<string, vector<string>>> production_list; // LR分析表中的产生式序列
	vector<pair<string, vector<string>>> production_gen;   // 分析过程中产生的产生式
 	stack<pair<int, int>> lr_stack;                       // 栈的元素为<状态，编码>，其中编码可能为编码表编码，也可以为非终结符号编码
	stack<symbolElement> symbol_stack;                  // TODO: 考虑和上面的栈合并为一个
	vector <tripleCode> tripleCodes;                      // 三地址码序列
	int tmp_num;                                          // 记录临时变量的序号

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

