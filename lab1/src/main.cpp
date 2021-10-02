#include "../include/lexical_analyzer.h"
using namespace std;
int main(void)
{
    string inputFile = "E:\\Github_repositories\\Compiler\\lab1\\test.txt";
    string outputFile = "E:\\Github_repositories\\Compiler\\lab1\\output.txt";
    lexical_analyzer l_a(inputFile);
    l_a.outputSymbolAndToken(outputFile);
    return 0;
}