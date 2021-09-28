#include "lexical_analyzer.h"
using namespace std;
/**
 * @brief  根据输入的文件名字符串读取源程序
 * @note   
 * @param  &fileName: 文件路径字符串
 * @retval 存储程序的字符串
 */
string lexical_analyzer::readProgram(string &fileName)
{
    // ifstream program(fileName);
    ifstream ifs(fileName);
    // ifs.open("E:\\Github_repositories\\Compiler\\lab1\\input_code.txt");
    // ifs.open("input_code.txt");   // 无法直接在VSCODE中运行
    if(!ifs.is_open())
    {
        cout<<"Fail to open file"<<fileName<<endl;
        exit(0);
    }
    string program = string((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
    ifs.close();
    return program;
};

/**
 * @brief  预处理程序，去除程序中的空格与注释
 * @note   
 * @param  program: 程序字符串
 * @retval None
 */
void lexical_analyzer::preHandle(string& program)
{
    bool inSingleQuote = false;
    bool inDoubleQuote = false;
    int idx = 0;
    int len = program.size();
    while(program[idx] != *program.end())
    {
        char chr = program[idx];
        // 检测到单引号
        if(chr == '\'')
        {
            inSingleQuote = !inSingleQuote;
            idx++;
            continue;
        }
        // 检测双引号
        if(chr == '"')
        {
            inDoubleQuote = !inDoubleQuote;
            idx++;
            continue;
        }
        // 既不在单引号也不在双引号中
        if(!inSingleQuote && !inDoubleQuote)
        {
            switch(chr)
            {
                case '/':
                {
                    // 前一个字符是'/'，再接收到一个'/'，为单行注释
                    if(program[idx-1]=='/')
                    {
                        int tmp_idx = idx+1;
                        while(program[tmp_idx]!='\n')
                            tmp_idx++;
                        program.erase(idx-1, tmp_idx-idx+1);
                        idx--;
                    }
                    else 
                        idx++;                                // 下标向前挪动一个位置到第一个'/'的地方
                    break;
                }
                case '*':
                {
                    // 前一个字符是'/'，再接收到一个'*'，为多行注释
                    if(program[idx-1]=='/')
                    {
                        int tmp_idx = idx+1;
                        while(1)
                        {
                            if(program[tmp_idx] == '*')
                                if(program[tmp_idx+1]=='/')
                                    break;
                            tmp_idx++;
                        }
                        program.erase(idx-1, tmp_idx-idx+3);
                        idx--;                                  // 下标向前挪动一个位置到原来'/'的地方
                    }
                    else
                        idx++;
                    break;
                }
                // case ' ':
                // case '\t':
                // case '\n':
                //     program.erase(idx, 1);
                //     break;
                default:
                    idx++;
            }
        }
        else
            idx++;
    } 
}
/**
 * @brief  初始化编码表
 * @note   
 * @retval None
 */
void lexical_analyzer::initTable()
{
    this->token_table["id"]              = 1;
    this->token_table["int_constant"]    = 2;
    this->token_table["bool_constant"]   = 3;  // true or false
    this->token_table["string_constant"] = 4;
    this->token_table["int"]             = 5;
    this->token_table["short"]           = 6;
    this->token_table["long"]            = 7;
    this->token_table["float"]           = 8;
    this->token_table["double"]          = 9;
    this->token_table["char"]            = 10;
    this->token_table["struct"]          = 11;
    this->token_table["union"]           = 12;
    this->token_table["enum"]            = 13;
    this->token_table["typedef"]         = 14;
    this->token_table["const"]           = 15;
    this->token_table["unsigned"]        = 16;
    this->token_table["signed"]          = 17;
    this->token_table["extern"]          = 18;
    this->token_table["void"]            = 19;
    this->token_table["if"]              = 20;
    this->token_table["else"]            = 21;
    this->token_table["switch"]          = 22;
    this->token_table["case"]            = 23;
    this->token_table["for"]             = 24;
    this->token_table["do"]              = 25;
    this->token_table["while"]           = 26;
    this->token_table["continue"]        = 27;
    this->token_table["break"]           = 28;
    this->token_table["default"]         = 29;
    this->token_table["sizeof"]          = 30;
    this->token_table["return"]          = 31;
    this->token_table[","]               = 32;
    this->token_table[";"]               = 33;
    this->token_table[":"]               = 34;
    this->token_table["+"]               = 35;
    this->token_table["-"]               = 36;
    this->token_table["*"]               = 37;
    this->token_table["/"]               = 38;
    this->token_table["%"]               = 39;
    this->token_table["++"]              = 40;
    this->token_table["--"]              = 41;
    this->token_table["!"]               = 42;    
    this->token_table["?"]               = 43;
    this->token_table["("]               = 44;
    this->token_table[")"]               = 45;
    this->token_table["&"]               = 46;
    this->token_table["|"]               = 47;
    this->token_table["="]               = 48;
    this->token_table[">"]               = 49;
    this->token_table[">="]              = 50;
    this->token_table["<"]               = 51;
    this->token_table["<="]              = 52;
    this->token_table["=="]              = 53;
    this->token_table["!="]              = 54;
    this->token_table["||"]              = 55;
    this->token_table["&&"]              = 56;
    this->token_table["^"]               = 57;
    this->token_table["["]               = 58;    
    this->token_table["]"]               = 59; 
    this->token_table["{"]               = 60;    
    this->token_table["}"]               = 61;
    this->token_table["'"]               = 62;    
    this->token_table["\""]              = 63;
    this->token_table["octal_constant"]  = 64;    
    this->token_table["hex_constant"]    = 65;

}
/**
 * @brief  扫描经过预处理的程序字符串，生成单词序列与符号表
 * @note   
 * @param  program: 经过预处理的程序字符串
 * @retval 单词序列<编码值，单词/符号表下标>
 */
vector<pair<int, string>> lexical_analyzer::scanner(string & program)
{
    vector<pair<int, string>> tokens;
    map<string, int>::iterator cit;
    int idx = 0;
    int len = program.size();
    while(idx < len)
    {
        char chr = program[idx];
        string token;
        if(chr==' ' || chr=='\t' || chr=='\n');
        // 数字
        else if(isdigit(chr))
        {
            // 判断是否是8进制与16进制
            if(chr=='0')
            {
                int ptr = idx+1;
                // 16进制
                if(program[ptr]=='x')
                {
                    ptr++;
                    while(ptr<len)
                    {
                        if(!isdigit(program[ptr]) 
                           && !(program[ptr]>='A' && program[ptr]<='F')
                           && !(program[ptr]>='a' && program[ptr]<='f')
                        )
                            break;
                        ptr++;
                    }
                    if(ptr==len)
                    {
                        cout<<"Expected EOF is missing"<<endl;
                        exit(0);
                    }
                    cit = this->token_table.find("hex_constant");
                    if(cit==token_table.end())
                    {
                        cout<<"Unknown token "<<"hex_constant"<<endl;
                        exit(0);
                    }
                    tokens.push_back({cit->second, token});
                    idx = ptr-1;
                }
                // 八进制
                else if(program[ptr]>='0' && program[ptr]<='7')
                {
                    while(ptr<len)
                    {
                        if(program[ptr]<'0' || program[ptr]>'7')
                            break;
                        ptr++;
                    }    
                    if(ptr==len)
                    {
                        cout<<"Expected EOF is missing"<<endl;
                        exit(0);
                    }
                    cit = this->token_table.find("octal_constant");
                    if(cit==token_table.end())
                    {
                        cout<<"Unknown token "<<"octal_constant"<<endl;
                        exit(0);
                    }
                    tokens.push_back({cit->second, token});
                    idx = ptr-1;
                }
                // 0
                else
                {
                    token+=chr;
                    cit = this->token_table.find("int_constant");
                    if(cit==token_table.end())
                    {
                        cout<<"Unknown token "<<"int_constant"<<endl;
                        exit(0);
                    }
                    tokens.push_back({cit->second, token});
                }
            }
            // 十进制数
            else
            {
                int ptr = idx+1;
                while(ptr<len)
                {
                    if(!isdigit(program[ptr]))
                        break;
                    ptr++;
                }
                if(ptr==len)
                {
                    cout<<"Expected EOF is missing"<<endl;
                    exit(0);
                }
                token = string(program, idx, ptr-idx);
                cit = this->token_table.find("int_constant");
                if(cit==token_table.end())
                {
                    cout<<"Unknown token "<<"int_constant"<<endl;
                    exit(0);
                }
                tokens.push_back({cit->second, token});
                idx = ptr-1;
            }
        }
        // 字母或下划线开头
        else if(isalpha(chr) || chr=='_')
        {
            token+=chr;
            int ptr = idx+1;
            while(isalnum(program[ptr]) || chr=='_')
            {
                token+=program[ptr];
                ptr++;
            }
            cit = this->token_table.find(token);
            if(cit==this->token_table.end())
            {
                int num = this->checkSymbolTable(token);
                if(num!=-1)
                    tokens.push_back({1, to_string(num)});
                else
                {
                    this->symbol_table.push_back(token);
                    tokens.push_back({1, to_string(this->symbol_table.size()-1)});
                }
            }
            else
            {
                tokens.push_back({cit->second, token});
            }
            idx = ptr-1;
        }
        // 符号
        else
        {
            switch(chr)
            {
                case ',':
                case ';':
                case ':':
                case '*':
                case '/':
                case '%':
                case '^':
                case '?':
                case '(':
                case ')':
                case '[':
                case ']':
                case '{':
                case '}':
                { 
                    token+=chr;
                    cit = this->token_table.find(token);
                    if(cit==token_table.end())
                    {
                        cout<<"Unknown token "<<token<<endl;
                        exit(0);
                    }
                    tokens.push_back({cit->second, token});
                    break;
                }
                // 字符/字符串常量
                case '\'':
                case '"':
                {
                    token+=chr;
                    cit = this->token_table.find(token);
                    if(cit==token_table.end())
                    {
                        cout<<"Unknown token "<<token<<endl;
                        exit(0);
                    }
                    tokens.push_back({cit->second, token});
                    // 将引号中间的字符串找出来定为字符串常量
                    int ptr = idx+1;
                    while(ptr<len)
                    {
                        if(program[ptr]==chr)
                        {
                            // 避免引号前是转义字符
                            if(program[ptr-1]=='\\')
                                ptr++;
                            else 
                                break;
                        }
                        else 
                            ptr++;
                    }
                    // 没有找到成对的引号
                    if(ptr==len)
                    {
                        cout<<"Expected ' or \" is missing"<<endl;
                        exit(0);
                    }
                    // 将字符串变量送入单词序列
                    token = string(program, idx+1, ptr-idx-1);
                    cit = this->token_table.find("string_constant");
                    if(cit==token_table.end())
                    {
                        cout<<"Unknown token "<<"string_constant"<<endl;
                        exit(0);
                    }
                    tokens.push_back({cit->second, token});
                    // 将末尾引号送入单词序列
                    token = string(program, ptr, 1);
                    cit = this->token_table.find(token);
                    if(cit==token_table.end())
                    {
                        cout<<"Unknown token "<<token<<endl;
                        exit(0);
                    }
                    tokens.push_back({cit->second, token});
                    idx = ptr;
                    break;
                }
                case '+':
                {
                    token+=chr;
                    if(idx+1<len)
                    {
                        if(program[idx+1]=='+')
                        {
                            token+=program[idx+1];
                            idx++;
                        }
                    }
                    cit = this->token_table.find(token);
                    if(cit==token_table.end())
                    {
                        cout<<"Unknown token "<<token<<endl;
                        exit(0);
                    }
                    tokens.push_back({cit->second, token});
                    break;
                }
                case '-':
                {
                    token+=chr;
                    if(idx+1<len)
                    {
                        if(program[idx+1]=='-')
                        {
                            token+=program[idx+1];
                            idx++;
                        }
                    }
                    cit = this->token_table.find(token);
                    if(cit==token_table.end())
                    {
                        cout<<"Unknown token "<<token<<endl;
                        exit(0);
                    }
                    tokens.push_back({cit->second, token});
                    break;
                }
                case '&':
                {
                    token+=chr;
                    if(idx+1<len)
                    {
                        if(program[idx+1]=='&')
                        {
                            token+=program[idx+1];
                            idx++;
                        }
                    }
                    cit = this->token_table.find(token);
                    if(cit==token_table.end())
                    {
                        cout<<"Unknown token "<<token<<endl;
                        exit(0);
                    }
                    tokens.push_back({cit->second, token});
                    break;
                }
                case '|':
                {
                    token+=chr;
                    if(idx+1<len)
                    {
                        if(program[idx+1]=='|')
                        {
                            token+=program[idx+1];
                            idx++;
                        }
                    }
                    cit = this->token_table.find(token);
                    if(cit==token_table.end())
                    {
                        cout<<"Unknown token "<<token<<endl;
                        exit(0);
                    }
                    tokens.push_back({cit->second, token});
                    break;
                }
                case '!':
                {
                    token+=chr;
                    if(idx+1<len)
                    {
                        if(program[idx+1]=='=')
                        {
                            token+=program[idx+1];
                            idx++;
                        }
                    }
                    cit = this->token_table.find(token);
                    if(cit==token_table.end())
                    {
                        cout<<"Unknown token "<<token<<endl;
                        exit(0);
                    }
                    tokens.push_back({cit->second, token});
                    break;
                }
                case '=':
                {
                    token+=chr;
                    if(idx+1<len)
                    {
                        if(program[idx+1]=='=')
                        {
                            token+=program[idx+1];
                            idx++;
                        }
                    }
                    cit = this->token_table.find(token);
                    if(cit==token_table.end())
                    {
                        cout<<"Unknown token "<<token<<endl;
                        exit(0);
                    }
                    tokens.push_back({cit->second, token});
                    break;
                }
                case '<':
                {
                    token+=chr;
                    if(idx+1<len)
                    {
                        if(program[idx+1]=='=')
                        {
                            token+=program[idx+1];
                            idx++;
                        }
                    }
                    cit = this->token_table.find(token);
                    if(cit==token_table.end())
                    {
                        cout<<"Unknown token "<<token<<endl;
                        exit(0);
                    }
                    tokens.push_back({cit->second, token});
                    break;
                }
                case '>':
                {
                    token+=chr;
                    if(idx+1<len)
                    {
                        if(program[idx+1]=='=')
                        {
                            token+=program[idx+1];
                            idx++;
                        }
                    }
                    cit = this->token_table.find(token);
                    if(cit==token_table.end())
                    {
                        cout<<"Unknown token "<<token<<endl;
                        exit(0);
                    }
                    tokens.push_back({cit->second, token});
                    break;
                }
                default:
                {
                    cout<<"Unknown character "<<chr<<endl;
                    exit(0);
                }
            }
        }
        idx++;
    }
    return tokens;
}
/**
 * @brief  检查符号表中是否存在token
 * @note   
 * @param  token: 待查找字符串
 * @retval 字符串在符号表中的下标,-1表示没有找到
 */
int lexical_analyzer::checkSymbolTable(string & token)
{
    for(int i = 0; i < this->symbol_table.size(); i++)
    {
        if(this->symbol_table[i]==token)
            return i;
    }
    return -1;
}

/**
 * @brief 将生成的单词序列与符号表输出到文件
 * @note   
 * @param  outputFile: 输出文件名
 * @retval None
 */
void lexical_analyzer::output(string & outputFile)
{
    ofstream ofs(outputFile);
    ofs<<"Token list:"<<endl;
    for(int i = 0; i < token_list.size(); i++)
        ofs<<'('<<token_list[i].first<<", "<<token_list[i].second<<')'<<endl;
    ofs<<"Symbol table:"<<endl;
    for(int i = 0; i < symbol_table.size(); i++)
        ofs<<'('<<i<<", "<<symbol_table[i]<<')'<<endl;
    ofs.close();
}
/**
 * @brief  构造函数，对外唯一接口，读取输入文件名与输出文件名
 * @note   
 * @param  inputFile: 输入文件名
 * @param  outputFile: 输出文件名
 * @retval 
 */
lexical_analyzer::lexical_analyzer(string & inputFile, string & outputFile)
{
    initTable();
    program = readProgram(inputFile);
    preHandle(program);
    token_list = scanner(program);
    output(outputFile);
}

int main(void)
{
    string inputFile = "E:\\Github_repositories\\Compiler\\lab1\\input_code.txt";
    string outputFile = "E:\\Github_repositories\\Compiler\\lab1\\output.txt";
    lexical_analyzer l_a(inputFile, outputFile);
    return 0;

}