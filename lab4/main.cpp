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
    string LRfileName = ".\\SimpleSyntax_LR1_new.tsv";   // ʹ��tsv�ļ���ΪLR��Ĵ洢��ʽ�����ⶺ�����⣬ͬʱ��˿ɶ���

    while (1)
    {
        outputInterface();
        char choice = getchar();
        if (choice != '1' && choice != '2' && choice != '3' && choice != '4')
        {
            cout << "��Ч����" << endl;
            //Sleep(1);
            system("cls");
            continue;
        }
        do {
            cout << "����������Դ�����ļ���" << endl;
            cin >> inputFile;
        } while (checkFile(inputFile));

        int create_if_not_exist = 1;
        do {
            cout << "����������ļ���" << endl;
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
/// ��ӡ����
/// </summary>
void outputInterface()
{
    cout << "===============================================" << endl;
    cout << "               1. �ʷ�����" << endl;
    cout << "               2. �﷨����" << endl;
    cout << "               3. �м��������" << endl;
    cout << "               4. Ŀ���������" << endl;
    cout << "===============================================" << endl;
    cout << "������1-4��ȷ�Ϲ���" << endl;
}

// ����ļ��ܷ�������,create=1��ʾ���ļ�������ʱ�Զ��������ļ�
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