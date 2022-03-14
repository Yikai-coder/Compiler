#include "SyntaxAnalyzer.h"
#include "LexicalAnalyzer.h"
#include "CodeGenerator.h"
//#include "my_include.h"
//# include <Windows.h>
using namespace std;

void outputInterface();
int checkFile(const string & file, int create = 0);

int main(int argc, char* argv[])
{
    string inputFile;
    string outputFile;
    string LRfileName = ".\\SimpleSyntax_LR1_new.tsv";   // 使用tsv文件作为LR表的存储格式，避免逗号问题，同时兼顾可读性

    while (1)
    {
        outputInterface();
        char choice = getchar();
        if (choice != '1' && choice != '2' && choice != '3' && choice != '4')
        {
            cout << "无效输入" << endl;
            //Sleep(1);
            system("cls");
            continue;
        }
        do {
            cout << "请输入输入源程序文件名" << endl;
            cin >> inputFile;
        } while (checkFile(inputFile));

        int create_if_not_exist = 1;
        do {
            cout << "请输入输出文件名" << endl;
            cin >> outputFile;
        } while (checkFile(outputFile, create_if_not_exist));

        LexicalAnalyzer scanner(inputFile);
        SyntaxAnalyzer parser(scanner.getTokenList(), LRfileName, scanner.getTokenTable(), scanner.getSymbolTable());
        CodeGenerator code_generator(parser.getTripleCodes());

        switch (choice)
        {
            case '1':
                scanner.OutputSymbolAndToken(outputFile);
                break;
            case '2':
                parser.OutPutProductions(outputFile);
                break;
            case '3':
                parser.OutputTripleCode(outputFile);
                break;
            case '4':
                code_generator.OutputAsmCode(outputFile);
                break;
            default:
                cout << "Invalid option!" << endl;
        }
        break;
    }

    return 0;
}
/// <summary>
/// 打印界面
/// </summary>
void outputInterface()
{
    cout << "===============================================" << endl;
    cout << "               1. 词法分析" << endl;
    cout << "               2. 语法分析" << endl;
    cout << "               3. 中间代码生成" << endl;
    cout << "               4. 目标代码生成" << endl;
    cout << "===============================================" << endl;
    cout << "请输入1-4以确认功能" << endl;
}

// 检查文件能否正常打开,create=1表示在文件不存在时自动创建该文件
int checkFile(const string & file, int create)
{
    ofstream ofs(file, ios::in);
    if (!ofs)
    {
        if (create)
        {
            ofs.close();
            ofs.open(file);
            ofs.close();
            return 0;
        }
        else
            return -1;
    }
    ofs.close();
    return 0;
}