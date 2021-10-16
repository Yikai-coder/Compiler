#include "SyntaxAnalyzer.h"
#include "LexicalAnalyzer.h"
using namespace std;
int main(int argc, char* argv[])
{
    string inputFile;
    string outputFile;
    inputFile = "..\\2input_code.txt";  // E:\\Github_repositories\\Compiler\\lab1\\input_code.txt
    outputFile = "..\\output.txt";
    string LRfileName = "..\\MySyntax_new.tsv";   // 使用tsv文件作为LR表的存储格式，避免逗号问题，同时兼顾可读性

    LexicalAnalyzer l_a(inputFile);
    SyntaxAnalyzer Syn(l_a.getTokenList(), LRfileName, l_a.getTokenTable(), outputFile);
    return 0;
}