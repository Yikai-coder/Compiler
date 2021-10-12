#include "SyntaxAnalyzer.h"
#include "lexical_analyzer.h"
using namespace std;
int main(int argc, char* argv[])
{
    string inputFile;
    string tokenOutput;
    string symbolTableOutput;
    inputFile = "..\\input_code.txt";  // E:\\Github_repositories\\Compiler\\lab1\\input_code.txt
    string LRfileName = "..\\LR1_new.csv";

    lexical_analyzer l_a(inputFile);
    SyntaxAnalyzer Syn(l_a.getTokenList(), LRfileName, l_a.getTokenTable());
    return 0;
}