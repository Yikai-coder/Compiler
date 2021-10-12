#pragma once
#include "my_include.h"
// 编译工作台生成的状态表初始状态为0，将结束符号#定义id为0
#define ORIGIN_STATE 0
#define END_SYMBOL_ID 33
// 设置非终结符号的编号，紧跟在token_table后面，但不写入token_table
#define NONTERMINAL_CHAR_START 70

using namespace std;
enum action {
	s,    // 移进
	r,    // 规约
	a,    // 完成接受
	j     // 跳转
};
class SyntaxAnalyzer
{
	//private:
		// action_table:<state, token>-><action, num>

public: 
	map<pair<int, int>, pair<action, int>> action_table;  // 包含action_table与goto_table
	vector<pair<string, vector<string>>> generator_list;    // 产生式序列
	// map<string, int> & local_token_table;
 	stack<pair<int, int>> lr_stack;           // 栈的元素为<状态，编码>，其中编码可能为编码表编码，也可以为非终结符号编码

	void readActionTable(string& filename, map<string, int> & token_table);

	SyntaxAnalyzer(const vector<pair<int, string>> & token_list, string & filename, map<string, int> & token_table);

	void parseTokenList(const vector<pair<int, string>> token_list, const map<string, int>& token_table);
	//public:
	
};

