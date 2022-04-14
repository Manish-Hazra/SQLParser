#include<iostream>
#include<cstring>
#include<string>
using namespace std;


int dfa=0;
string key[]={"select","delete","insert","value","into","from","where","order","group","by"};

void start(char c){
    int check=((int)c);
    if((check>=65&&check<=90)||(check>=97&&check<=122)){
        dfa=1;
    }
    else{
        dfa=-1;
    }
}

void state1(char c){
    int check=((int)c);
    if((check>=65&&check<=90)||(check>=97&&check<=122)){
        dfa=1;
    }
    else if(check>=48&&check<=57){
        dfa=1;
    }
    else if(c==','){
        dfa=1;
    }
    else if(c=='*'){
        dfa=2;
    }
    else if(isspace(c)){
        dfa=3;
    }
    else if(c=='>'||c=='<'||c=='='){
        dfa=5;
    }
    else if(c==';'){
        dfa=6;
    }
    else{
        dfa=-1;
    }
}

void state2(char c){
    if(isspace(c)){
        dfa=3;
    }
    else{
        dfa=-1;
    }
}

void state3(char c){
    int check=((int)c);
    if((check>=65&&check<=90)||(check>=97&&check<=122)){
        dfa=1;
    }
    else if(check>=48&&check<=57){
        dfa=4;
    }
    else{
        dfa=-1;
    }
}

void state4(char c){
    int check=((int)c);
    if(check>=48&&check<=57){
        dfa=4;
    }
    else if(isspace(c)){
        dfa=3;
    }
    else if(c=';'){
        dfa=6;
    }
    else{
        dfa=-1;
    }
}

void state5(char c){
    int check=((int)c);
    if(c=='='){
        dfa=5;
    }
    else if((check>=65&&check<=90)||(check>=97&&check<=122)){
        dfa=1;
    }
    else if(check>=48&&check<=57){
        dfa=4;
    }
    else{
        dfa=-1;
    }

}
int check_lex(string input){
    int len=input.length();
    for(int i=0;i<len;i++){
        if (dfa == 0)
            start(input[i]);
        else if (dfa == 1)
            state1(input[i]);
        else if (dfa == 2)
            state2(input[i]);
        else if (dfa == 3)
            state3(input[i]);
        else if (dfa == 4)
            state4(input[i]);
        else if (dfa == 5)
            state5(input[i]);
        else
            break;
    }
    if(dfa==6){
        return 1;
    }
    else{
        return 0;
    }
}

string upperstr(string str){
    int len=str.length();
    string result;
    for(int i=0;i<len;i++){
        result+=(char)(str[i]-32);
    }
    return result;
}

int is_num(string str){
    int len=str.length();
    int flag=0;
    for(int i=0;i<len;i++){
        if(isdigit(str[i])){
            flag=0;
        }
        else{
            flag=1;
            break;
        }
    }
    if(flag==0) return 1;
    else return 0;
}

string generate_lex(string input){
    int len=input.length(), start, end;
    string result="", sub;
    char c;
    start=0;
    for(int i=0;i<len;i++){
        c=input[i];
        if(isspace(c)||c==','||c=='>'||c=='<'||c==';'){
            end=i-start;
            sub=input.substr(start,end);
            start=i+1;
            int flag=0;
            for(int j=0;j<10;j++){
                string keyu=upperstr(key[j]);
                if(key[j].compare(sub)==0||keyu.compare(sub)==0){
                    flag=1;
                    break;
                }
            }
            if(flag==1){
                result+=sub;
                result+=c;
            }
            else{
                if(is_num(sub)){
                        result+="num";
                        result+=c;
                    }
                    else{
                        result+="id";
                        result+=c;
                    }
            }
        }
        else if(c=='='){
            if(input[i-1]=='>'||input[i-1]=='<'){
                result+=c;
                start=i+1;
            }
            else{
                end=i-start;
                sub=input.substr(start,end);
                int flag=0;
                for(int j=0;j<10;j++){
                    string keyu=upperstr(key[j]);
                    if(key[j].compare(sub)==0||keyu.compare(sub)==0){
                        flag=1;
                        break;
                    }
                }
                if(flag==1){
                    result+=sub;
                }
                else{
                    if(is_num(sub)){
                        result+="num";
                        result+=c;
                    }
                    else{
                        result+="id";
                        result+=c;
                    }
                }
                start=i+1;
            }
        }
    }
    return result;
}

int main()
{
    string input, result;
    cout<<">>";
    getline(cin, input);
    cout<<"\nGiven Input Query : "<<input;
    if(check_lex(input)){
        result=generate_lex(input);
        cout<<"\nLexical Output : "<<result;
    }
    else{
        cout<<"\nNot Accepted";
    }
    return 0;
}