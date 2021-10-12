#include "../include/lexical_analyzer.h"
using namespace std;
int main(int argc, char* argv[])
{
    string inputFile;
    string tokenOutput;
    string symbolTableOutput;
    // �û�ʹ��Ĭ������������ļ�·��
    if (argc == 1)
    {
        inputFile = ".\\input_code.txt";  // E:\\Github_repositories\\Compiler\\lab1\\input_code.txt
        tokenOutput = ".\\token.txt";
        symbolTableOutput = ".\\symbolTable.txt";
    }
    // �û��Լ����������ļ�·��
    else if(argc == 2)
    {
        inputFile = argv[1];
        tokenOutput = ".\\token.txt";
        symbolTableOutput = ".\\symbolTable.txt";
    }
    /*
    // �û���������������ļ�·��
    else if (argc == 3)
    {
        inputFile = argv[1];
        outputFile = argv[2];
    }
    else
    {
        printf("Illegal option\n");
        return 0;
    }
    */
    lexical_analyzer l_a(inputFile);
    l_a.outputSymbolAndToken(tokenOutput, symbolTableOutput);
    return 0;
}