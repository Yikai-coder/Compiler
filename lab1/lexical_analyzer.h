#ifndef LEXICAL_ANALYZER_H
#define LEXICAL_ANALYZER_H
#include <string>
#include <map>
#include <vector>
#include <fstream>
#include <iostream>
using namespace std;
class lexical_analyzer{
    private:
        map<string, int> token_table;
        vector<string> symbol_table;
        string program;
        vector<pair<int, string>> token_list;

        int checkSymbolTable(string & token);
        void output(string & outputFile);
    public:
        // string program;
        string readProgram(string & fileName);
        void preHandle(string & program);
        void initTable();
        vector<pair<int, string>> scanner(string & program);
        lexical_analyzer(string & inputFile, string & outputFile);

};
#endif
