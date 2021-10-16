#include "SyntaxAnalyzer.h"
#include "LexicalAnalyzer.h"
using namespace std;
int main(int argc, char* argv[])
{
    string inputFile;
    string outputFile;
    inputFile = "..\\2input_code.txt";  // E:\\Github_repositories\\Compiler\\lab1\\input_code.txt
    outputFile = "..\\output.txt";
    string LRfileName = "..\\MySyntax_new.tsv";   // ʹ��tsv�ļ���ΪLR��Ĵ洢��ʽ�����ⶺ�����⣬ͬʱ��˿ɶ���

    LexicalAnalyzer l_a(inputFile);
    SyntaxAnalyzer Syn(l_a.getTokenList(), LRfileName, l_a.getTokenTable(), outputFile);
    return 0;
}