#include<bits/stdc++.h>
using namespace std;

int casei = 0;//需要分别统计每一个switch下的case个数，故在结构体中用数组记录case的个数
bool elsebefore = false;//上一个是else，或上一个是else if
//bool SpaceBefore = false;//为判断else-if而铺垫，避免出现else{if...}的情况
bool ieie = false;//判断if-else，需要剔除含有else-if的结构，所以出现"else if"的时候需要额外注意
    
const string KeyWords[32] = {
    "auto", "break", "case","char",
    "const", "continue", "default", "do",
    "double", "else", "enum", "extern",
    "float", "for", "goto", "if",
    "int", "long", "register", "return",
    "short", "signed", "sizeof", "static",
    "struct", "switch", "typedef", "union",
    "unsigned", "void", "volatile", "while"
};

struct countdata{
    int cttotal = 0;    //总和
    int ctsw = 0;       //switch
    int ctcase[1000] = {0};     //cast
    int ctie = 0;       //if-else
    int ctieie = 0;     //if-else if-else
};
countdata result;

void AddressAdjustment(string add){
    int s = add.length();
    stack <char> fn;

    while (s--)
      fn.push(add[s]);//现在，堆栈为完整的地址
    
    add = "";

    while (!fn.empty()){
        if (fn.top() == '\\')//由于语言本身问题，双斜杠本质上是表示单斜杠
        add += '\\';

        else
        add += fn.top();

        fn.pop();
    }
}
string ReadFile(string add){
    fstream file;
    stringstream buf;

    file.open(add,ios::in);
    buf << file.rdbuf();
    file.close();
    return buf.str();
}

void IfelseJudging(string temp){
    /*核心思路：无论是if-else结构，还是if-else if-else结构，都是if开头，else结尾。
    当中间出现else-if时，取消判断if-else，并开始判断if-else if-else
    */
    if (temp == "if" && !elsebefore){
        ieie = false;//如果上一个不是else-if，则这是一个新的if嵌套
    }
    if (temp == "if" && elsebefore){//一定是else if
        result.ctie--;
        if (ieie)
        result.ctieie--;
        ieie = true;//开始判断是否为if-else if-else
        }
    if (temp == "else" && !ieie){
        result.ctie++;//这里暂时当作if-else处理
    }
    if (temp == "else" && ieie){//可能的情况:一个else作为if-else if-else的结尾，或是一个else if的开始
        result.ctieie++;//暂时当作if-else if-else的末尾，若不是则后续回退
    }
}

void Matching (string temp){//用途：进行关键词库的比对
    for (int i = 0; i < 32; i++){
        if (temp == KeyWords[i]){
            result.cttotal++;

            if (temp == "switch"){
                result.ctsw++;
                casei++;//因此，casei其实是从1开始的。即：第n个switch对应的是case[n]
                result.ctcase[i] = 0;
            }
            else if (temp == "case")
                result.ctcase[casei]++;
            else if (temp == "double")
                result.cttotal--;//验证double成功时，其实在之前已经当成"do"验证过一次。
            else if (temp == "if"){
                IfelseJudging(temp);//将出现if的情况送入if-else judging
            }
            else if (temp == "else"){
                IfelseJudging(temp);//将出现else的情况送入if-else judging
            }
        
            if (temp == "else")
                elsebefore = true;
            else
                elsebefore = false;//判断最后一个词是不是else

            break;      
        }
    }
    return;
}

void analysize(string data){
    //用途：开始分析单词，将单词送入Matching进行比对
    int l = data.length();
    string temp = "\0";

    for (int i = 0; i < l; i++){
        if (data[i] == '{' || data[i] == '\0')
            elsebefore == false;//如果出现换行或{符号，一定不会有else-if结构
            
        if (data[i] == '/' && data[i+1] == '/'){//考虑注释的情况
            while (i++ && i<l){//限制i<l，避免注释在末尾，导致死循环
                if (data[i] == '\n')
                break;
            }
        }
        else if (data[i] == '/' && data[i+1] == '*'){//考虑/*注释
            while (i++){
                if (data[i-1] == '*' && data[i] == '/')
                break;
            }
        }
        else if (data[i] == '"'){//考虑引用内容
            while(i++)
                if (data[i] == '"')
                  break;
        }
        else{
            if (isalpha(data[i])){//即，开始判断单词。将完整的单词送入Matching开始比对关键词
                temp += data[i];
                Matching(temp);
            }
            else
                {
                temp = "\0";
                }
        }
    }
}

int main(){

    string add;   //地址
    int  level;    //完成等级

    cout << "Please inpress the address (including suffix) and the dealing level" << endl;
    cin >> add >> level;

    AddressAdjustment(add);//解决输入地址为单斜杠的问题
    
    string data;//处理的读入数据
    data = ReadFile(add);

    analysize(data);//开始进行分析

    cout<<"total num: "<<result.cttotal<<endl;
    if (level >= 2){
        cout<<"switch num: "<<result.ctsw<<endl;
        cout<<"case num:";
        for (int i = 1; i <= casei ; i++)
        cout<<" "<<result.ctcase[i];
        cout<<endl;
    }
    if (level >= 3)
        cout<<"if-else num: "<<result.ctie<<endl;
    if (level >= 4)
        cout<<"if-elseif-else num: "<<result.ctieie<<endl;

    //system("pause");  //本地测试用
    return 0;
}   