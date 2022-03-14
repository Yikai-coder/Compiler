#ifndef LexicalAnalyzer_H
#define LexicalAnalyzer_H
#include "my_include.h"
#include "symbol_table_element.h"
using namespace std;

class LexicalAnalyzer{
    private:
        map<string, int> token_table;
        vector<symbol_table_element> symbol_table;
        string program;
        vector<pair<int, string>> token_list;

        int CheckSymbolTable(string & token);
        string ReadProgram(string & fileName);
        void PreHandleProgram(string & program);
        void InitTable();
        vector<pair<int, string>> ScanProgram(string & program);
    public:
        LexicalAnalyzer(string & inputFile);

        void OutputSymbolAndToken(string& tokenOutput, string& symbolTableOutput);

        map<string, int> & getTokenTable(void);

        const vector<pair<int, string>> & getTokenList(void);

        vector<symbol_table_element>& LexicalAnalyzer::getSymbolTable(void);
};
#endif
