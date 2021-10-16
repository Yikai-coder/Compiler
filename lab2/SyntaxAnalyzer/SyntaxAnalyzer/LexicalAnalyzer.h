#ifndef LexicalAnalyzer_H
#define LexicalAnalyzer_H
#include "my_include.h"
using namespace std;

class LexicalAnalyzer{
    private:
        map<string, int> token_table;
        vector<string> symbol_table;
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

        vector<string> & getSymbolTable(void);
};
#endif
