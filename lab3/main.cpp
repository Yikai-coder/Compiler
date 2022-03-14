#include "SyntaxAnalyzer.h"
#include "LexicalAnalyzer.h"
using namespace std;
int main(int argc, char* argv[])
{
    string inputFile;
    string outputFile;
    inputFile = ".\\input_code.txt";  // E:\\Github_repositories\\Compiler\\lab1\\input_code.txt
    outputFile = "..\\output.txt";
    string LRfileName = ".\\SimpleSyntax_LR1_new.tsv";   // ʹ��tsv�ļ���ΪLR��Ĵ洢��ʽ�����ⶺ�����⣬ͬʱ��˿ɶ���

    LexicalAnalyzer l_a(inputFile);
    SyntaxAnalyzer Syn(l_a.getTokenList(), LRfileName, l_a.getTokenTable(), outputFile, l_a.getSymbolTable());
    vector<symbol_table_element> symbol_table = l_a.getSymbolTable();
    ofstream ofs(".\\TokenTable.txt");
    cout << "=======================���ű�=======================" << endl;
    ofs<< "=======================���ű�=======================" << endl;
    cout << "NO\t\t" << "name\t\t" << "address\t\t" << "type\t\t" << endl;
    ofs << "NO\t\t" << "name\t\t" << "address\t\t" << "type\t\t" << endl;
    for (int i = 0; i < symbol_table.size(); i++)
    {
        cout << i << "\t\t" << symbol_table[i].name << "\t\t" << symbol_table[i].address << "\t\t" << symbol_table[i].type << endl;
        ofs << i << "\t\t" << symbol_table[i].name << "\t\t" << symbol_table[i].address << "\t\t" << symbol_table[i].type << endl;
    }
    ofs.close();
    return 0;
}