#include<bits/stdc++.h>
using namespace std;

int casei = 0;//因为需要分别列出switch下case的个数，故case在后续结构体中定义为数组。这里casei是数组的编号case[casei]

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

void Matching (string temp, countdata &result){//用途：进行关键词库的比对
    if (temp.size() == 1)
    return;
    
    for (int i = 0; i < 32; i++){
        if (temp == KeyWords[i]){
            cout<<"now testing "<<temp<<endl;
            if (temp == "switch"){
                result.ctsw++;
                casei++;
                //因此，casei是从1开始的。第n组switch对应的是case[n]
                result.ctcase[i] = 0;
            }
            else if (temp == "case")
                result.ctcase[casei]++;
            else if (temp == "double")
                result.cttotal--;//验证double成功时，其实在之前已经当成"do"验证过一次。
            result.cttotal++;
            break;
        }
    }
    return;
}

void analysize(string data, countdata &result){//用途：开始分析单词，将单词送入Matching进行比对
    int l = data.length();
    string temp = "\0";

    for (int i = 0; i < l; i++){
        if (isalpha(data[i])){
            temp += data[i];
            Matching(temp, result);
            }
        else
            temp = "\0";
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

    countdata result;
    analysize(data, result);
    //下一行测试
    cout<<result.cttotal<<endl<<result.ctsw<<endl<<result.ctcase<<endl;

    system("pause");
    return 0;
}   
