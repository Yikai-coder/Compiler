#include "LexicalAnalyzer.h"
// 宏函数，用于判断token表中是否存在token
#define checkTokenTable(cit, token)\
        cit = this->token_table.find(token);\
        if(cit==token_table.end())\
        {\
            cout<<"Unknown token "<<token<<endl;\
            exit(0);\
        }\

using namespace std;
/**
 * @brief  根据输入的文件名字符串读取源程序
 * @note   
 * @param  &fileName: 文件路径字符串
 * @retval 存储程序的字符串
 */
string LexicalAnalyzer::ReadProgram(string &fileName)
{
    ifstream ifs(fileName);
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
void LexicalAnalyzer::PreHandleProgram(string& program)
{
    bool inSingleQuote = false;
    bool inDoubleQuote = false;
    string::iterator it = program.begin();
    while (it!= program.end())
    {
        char chr = *it;
        // 检测到单引号
        if(chr == '\'')
        {
            inSingleQuote = !inSingleQuote;
            it++;
            continue;
        }
        // 检测双引号
        if(chr == '"')
        {
            inDoubleQuote = !inDoubleQuote;
            it++;
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
                    if(*(it-1)=='/')
                    {
                        string::iterator tmp_it = it + 1;
                        while (*tmp_it != '\n')
                            tmp_it++;
                        it = program.erase(it - 1, tmp_it);

                    }
                    else
                    {
                        it++;              // 下标向前挪动一个位置到第一个'/'的地方
                    }
                    break;
                }
                case '*':
                {
                    // 前一个字符是'/'，再接收到一个'*'，为多行注释
                    if(*(it - 1) =='/')
                    {
                        string::iterator tmp_it = it + 1;
                        while (1)
                        {
                            if (*tmp_it == '*')
                                if (*(tmp_it + 1) == '/')
                                    break;
                            tmp_it++;
                        }
                        it = program.erase(it - 1, tmp_it+2);
                    }
                    else
                    {
                        it++;
                    }
                    break;
                }
                case ';':
                {
                    *it = '#';
                    it++;
                    break;
                }
                // case ' ':
                // case '\t':
                // case '\n':
                //     program.erase(idx, 1);
                //     break;
                default:
                {
                    it++;
                }
            }
        }
        else
        {
            it++;
        }
    }
}

/**
 * @brief  初始化编码表
 * @note   
 * @retval None
 */
void LexicalAnalyzer::InitTable()
{
    this->token_table["$"]               = 0;   // 程序结束符
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
    //this->token_table["||"]              = 55;   因为编译工作台不支持||，所以手动将||和&&改成单词，程序不变，由词法分析器进行改动
    this->token_table["or"]              = 55;
        //this->token_table["&&"]              = 56;
    this->token_table["and"]             = 56;
    this->token_table["^"]               = 57;
    this->token_table["["]               = 58;    
    this->token_table["]"]               = 59; 
    this->token_table["{"]               = 60;    
    this->token_table["}"]               = 61;
    this->token_table["'"]               = 62;    
    this->token_table["\""]              = 63;
    this->token_table["octal_constant"]  = 64;    
    this->token_table["hex_constant"]    = 65;
    this->token_table["+="]              = 66;    
    this->token_table["-="]              = 67;
    this->token_table["*="]              = 68;
    this->token_table["/="]              = 69;
    this->token_table["#"]               = 70;    // 句子结束符
}   
/**
 * @brief  扫描经过预处理的程序字符串，生成单词序列与符号表
 * @note   
 * @param  program: 经过预处理的程序字符串
 * @retval 单词序列<编码值，单词/符号表下标>
 */
vector<pair<int, string>> LexicalAnalyzer::ScanProgram(string & program)
{
    vector<pair<int, string>> tokens;
    map<string, int>::iterator cit;
    int idx = 0;
    int len = program.size();
    while(idx < len)
    {
        char chr = program[idx];
        string token;
        // 空格，制表符，换行符直接略过
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
                    checkTokenTable(cit, "hex_constant");
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
                    checkTokenTable(cit, "octal_constant");
                    tokens.push_back({cit->second, token});
                    idx = ptr-1;
                }
                // 0
                else
                {
                    token+=chr;
                    checkTokenTable(cit, "int_constant");
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
                checkTokenTable(cit, "int_constant");
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
                int num = this->CheckSymbolTable(token);
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
                case '%':
                case '^':
                case '?':
                case '(':
                case ')':
                case '[':
                case ']':
                case '{':
                case '}':
                case '$':
                case '#':
                { 
                    token+=chr;
                    checkTokenTable(cit, token);
                    tokens.push_back({cit->second, token});
                    break;
                }
                // 字符/字符串常量
                case '\'':
                case '"':
                {
                    token+=chr;
                    checkTokenTable(cit, token);
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
                    checkTokenTable(cit, "string_constant");
                    tokens.push_back({cit->second, token});
                    // 将末尾引号送入单词序列
                    token = string(program, ptr, 1);
                    checkTokenTable(cit, token);
                    tokens.push_back({cit->second, token});
                    idx = ptr;
                    break;
                }
                case '+':
                {
                    token+=chr;
                    if(idx+1<len)
                    {
                        if(program[idx+1]=='+' || program[idx+1]=='=')
                        {
                            token+=program[idx+1];
                            idx++;
                        }
                    }
                    checkTokenTable(cit, token);
                    tokens.push_back({cit->second, token});
                    break;
                }
                case '-':
                {
                    token+=chr;
                    if(idx+1<len)
                    {
                        if(program[idx+1]=='-' || program[idx+1]=='=')
                        {
                            token+=program[idx+1];
                            idx++;
                        }
                    }
                    checkTokenTable(cit, token);
                    tokens.push_back({cit->second, token});
                    break;
                }
                case '*':
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
                    checkTokenTable(cit, token);
                    tokens.push_back({cit->second, token});
                    break;
                }
                case '/':
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
                    checkTokenTable(cit, token);
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
                            //token+=program[idx+1];
                            token = "and";
                            idx++;
                        }
                    }
                    checkTokenTable(cit, token);
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
                            //token+=program[idx+1];
                            token = "or";
                            idx++;
                        }
                    }
                    checkTokenTable(cit, token);
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
                    checkTokenTable(cit, token);
                    cit = this->token_table.find(token);
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
                    checkTokenTable(cit, token);
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
                    checkTokenTable(cit, token);
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
                    checkTokenTable(cit, token);
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
    tokens.push_back({ 0, "$" });
    return tokens;
}
/**
 * @brief  检查符号表中是否存在token
 * @note   
 * @param  token: 待查找字符串
 * @retval 字符串在符号表中的下标,-1表示没有找到
 */
int LexicalAnalyzer::CheckSymbolTable(string & token)
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
void LexicalAnalyzer::OutputSymbolAndToken(string & tokenOutput, string & symbolTableOutput)
{
    ofstream ofs(tokenOutput);
    //ofs<<"Token list:"<<endl;
    for(int i = 0; i < token_list.size(); i++)
        ofs<<'('<<token_list[i].first<<", "<<token_list[i].second<<')'<<endl;
    ofs.close();
    ofs.open(symbolTableOutput);
    //ofs<<"Symbol table:"<<endl;
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
LexicalAnalyzer::LexicalAnalyzer(string & inputFile)
{
    InitTable();
    program = ReadProgram(inputFile);
    PreHandleProgram(program);
    token_list = ScanProgram(program);
}

/**
 * @brief  对外接口用于访问编码表
 * @note   
 * @retval 
 */
map<string, int> & LexicalAnalyzer::getTokenTable(void)
{
    return this->token_table;
}

/**
 * @brief  对外界接口访问符号表
 * @note   
 * @retval 
 */
vector<string> & LexicalAnalyzer::getSymbolTable(void)
{
    return this->symbol_table;
}
/**
 * @brief  对外接口访问单词序列
 * @note   
 * @retval 
 */
const vector<pair<int, string>> & LexicalAnalyzer::getTokenList(void)
{
    return this->token_list;
}
