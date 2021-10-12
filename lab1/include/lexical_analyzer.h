#ifndef LEXICAL_ANALYZER_H
#define LEXICAL_ANALYZER_H
#include "my_include.h"
using namespace std;

class lexical_analyzer{
    private:
        map<string, int> token_table;
        vector<string> symbol_table;
        string program;
        vector<pair<int, string>> token_list;

        int checkSymbolTable(string & token);
        string readProgram(string & fileName);
        void preHandle(string & program);
        void initTable();
        vector<pair<int, string>> scanner(string & program);
    public:
        lexical_analyzer(string & inputFile);

        void outputSymbolAndToken(string& tokenOutput, string& symbolTableOutput);

        map<string, int> & getTokenTable(void);

        const vector<pair<int, string>> & getTokenList(void);

        vector<string> & getSymbolTable(void);
};
#endif
