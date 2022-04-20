#include<iostream>
#include<cstring>
#include<string>
#include<fstream>
#include"stack.h"
using namespace std;

//Global variable declarations//
Stack<string> input;
Stack<string> pr_stack;

int prods_used[100];
int tot_pr=0;

struct prod{
    string token[20];
    int tot;
} prodtokens[19];

struct parse_table{
    string term[19];
    int prods[19];
} non_term[9];
string prod_first[19];
string nont[19];
int nontnum=0;

struct first_set{
    string term;
    int tot;
    string set[20];
} first[9];

struct follow_set{
    string term;
    int tot;
    string set[20];
} follow[9];

string prods[19];
string non_terminals[9];
string terminals[19];
int nt=0, t=0;

//Function to identify non-terminals in the grammar//
int identify_nt(string str){
    int len=str.size();
    int start=0, end;
    int i;
    for(i=0;i<len;i++){
        char c=str[i];
        if(isspace(c)){
            end=i;
            string sub=str.substr(start, end);
            nont[nontnum]=sub;
            nontnum++;
            int flag=0;
            for(int j=0; j<nt; j++){
                string exnt=non_terminals[j];
                if(exnt.compare(sub)==0){
                    flag=1;
                    break;
                }
            }
            if(flag==0){
                non_terminals[nt]=sub;
                nt++;
            }
            break;
        }
    }

    return (i+4);
}

//Function to identify terminals in the grammar//
void identify_t(string str, int st, int pr_no){
    int len=str.size();
    int start=st, end;
    for(int i=st;i<len;i++){
        char c=str[i];
        if(isspace(c)){
            end=i-start;
            string sub=str.substr(start, end);
            prodtokens[pr_no].token[prodtokens[pr_no].tot]=sub;
            prodtokens[pr_no].tot+=1;
            if(sub=="''"){
                break;
            }
            start=i+1;
            int flag=0;
            for(int j=0;j<9;j++){
                if(sub.compare(non_terminals[j])==0){
                    flag=1;
                    break;
                }
            }
            if(flag==0){
                int fl=0;
                for(int k=0;k<t;k++){
                    if(sub.compare(terminals[k])==0){
                        fl=1;
                        break;
                    }
                }
                if(fl==0){
                    terminals[t]=sub;
                    t++;
                }
            }
        }
    }
}


//Function to calculate first set of non-terminals//
void calc_first(string str, int st, string non, int prod_num){
    int len=str.size();
    int start=st, end;
    for(int i=st;i<len;i++){
        char c=str[i];
        if(isspace(c)){
            end=i-start;
            string sub=str.substr(start, end);
            prod_first[prod_num]=sub;
            start=i+1;
            int flag=0;
            int j;
            for(j=0;j<9;j++){
                if(sub.compare(non_terminals[j])==0){
                    flag=1;
                    break;
                }
            }
            if(flag==1){
                int total=first[j].tot;
                for(int l=0;l<9;l++){
                    if(first[l].term.compare(non)==0){
                        for(int m=0;m<total;m++){
                            int totf=first[l].tot;
                            first[l].set[totf]=first[j].set[m];
                            first[l].tot+=1;
                        }
                        break;
                    }
                }
            }
            else{
               for(int l=0;l<9;l++){
                    if(first[l].term.compare(non)==0){
                        int totf=first[l].tot;
                        first[l].set[totf]=sub;
                        first[l].tot+=1;
                        break;
                    }
                } 
            }
            break;
        }
    }
}

//Function to calculate follow set of non-terminals//
void calc_follow(string str, int st, string non){
    int len=str.size();
    int start=st, end;
    for(int i=st;i<len;i++){
        char c=str[i];
        if(isspace(c)){
            end=i-start;
            string sub=str.substr(start, end);
            start=i+1;
            int flag=0;
            int j;
            for(j=0;j<9;j++){
                if(sub.compare(non_terminals[j])==0){
                    flag=1;
                    break;
                }
            }
            string pres=non_terminals[j];
            if(flag==1){
                for(int k=start;k<len;k++){
                    char ch=str[k];
                    if(isspace(ch)){
                        string sub2=str.substr(start, k-start);
                        int fl=0;
                        int l;
                        for(l=0;l<9;l++){
                            if(sub2.compare(non_terminals[l])==0){
                                fl=1;
                                break;
                            }
                        }
                        string next=non_terminals[l];
                        if(fl==0){
                            for(int n=0;n<9;n++){
                                if(pres.compare(follow[n].term)==0){
                                    follow[n].set[(follow[n].tot)]=sub2;
                                    follow[n].tot+=1;
                                    break;
                                }
                            }
                        }
                        else{
                            for(int n=0;n<9;n++){
                                if(pres.compare(follow[n].term)==0){
                                    for(int q=0; q<9; q++){
                                        if(next.compare(first[q].term)==0){
                                            int curtot=first[q].tot;
                                            for(int p=0;p<curtot;p++){
                                                string subfir=first[q].set[p];
                                                if(subfir.compare("''")==0){
                                                    int curtotf=follow[q].tot;
                                                    for(int z=0;z<curtotf;z++){
                                                        int flg=0;
                                                        for(int y=0;y<(follow[n].tot);y++){
                                                            if((follow[n].set[(follow[n].tot)]).compare((follow[q].set[z]))==0){
                                                                flg=1;
                                                                break;
                                                            }
                                                        }
                                                        if(flg==0)
                                                        {follow[n].set[(follow[n].tot)]=follow[q].set[z];
                                                        follow[n].tot+=1;}
                                                    }
                                                }
                                                else{
                                                    follow[n].set[(follow[n].tot)]=subfir;
                                                    follow[n].tot+=1;
                                                }
                                            }
                                            break;
                                        }
                                    }
                                    break;
                                }
                            }
                        }
                        break;
                    }
                }
            }
        }
    }
}

//Function to update follow set of non-terminals//
void update_follow(){
    string temp[20];
    for(int i=0;i<9;i++){
        int tot=0;
        for(int j=0;j<(follow[i].tot);j++){
            int flg=0;
            for(int k=0;k<tot;k++){
                if((follow[i].set[j]).compare(temp[k])==0){
                    flg=1;
                    break;
                }
            }
            if(flg==0){
                temp[tot]=(follow[i].set[j]);
                tot++;
            }
        }
        follow[i].tot=tot;
        for(int j=0;j<tot;j++){
            follow[i].set[j]=temp[j];
        }
    }
}

//Function to generate parse table of grammar//
void parse_table(){
    cout<<"\n\nParsing Table\n\nNON-T   ";
    for(int i=0;i<19;i++){
        cout<<terminals[i]<<"\t";
    }
    for(int i=0;i<9;i++){
        for(int j=0;j<19;j++){
            non_term[i].term[j]=terminals[j];
        }
    }
    for(int i=0;i<19;i++){
        int fl=0;
        int j=0;
        if(prod_first[i].compare("''")==0){
            int temp;
            for(temp=0;temp<9;temp++){
                if(nont[i].compare(non_terminals[temp])==0){
                    break;
                }
            }
            for(int l=0;l<follow[temp].tot;l++){
                for(int k=0;k<19;k++){
                    if(follow[temp].set[l].compare(non_term[temp].term[k])==0){
                        non_term[temp].prods[k]=i+1;
                    }
                }
            }
            continue;
        }
        for(j=0;j<9;j++){
            if(non_terminals[j].compare(prod_first[i])==0){
                fl=1;
                break;
            }
        }
        if(fl==0){
            int temp=0;
            for(temp=0;temp<9;temp++){
                if(nont[i].compare(non_terminals[temp])==0){
                    break;
                }
            }
            for(int k=0;k<19;k++){
                if(prod_first[i].compare(non_term[temp].term[k])==0){
                    non_term[temp].prods[k]=i+1;
                }
            }
        }
        else{
            int temp=0;
            for(temp=0;temp<9;temp++){
                if(nont[i].compare(non_terminals[temp])==0){
                    break;
                }
            }
            for(int l=0;l<first[j].tot;l++){
                for(int k=0;k<19;k++){
                    if(first[j].set[l].compare(non_term[temp].term[k])==0){
                        non_term[temp].prods[k]=i+1;
                    }
                }
            }
        }
    }
    for(int j=0;j<9;j++){
        cout<<"\n"<<non_terminals[j]<<"\t";
        for(int i=0;i<19;i++){
            cout<<non_term[j].prods[i]<<"\t";
        }
        cout<<"\n";
    }
}

//Function to read productions of grammar from text file//
void build(string str){
    fstream newfile;
    newfile.open(str,ios::in);
    if (newfile.is_open()){
        string tp;
        int i=0;
        while(getline(newfile, tp)){
            prods[i]=tp;
            i++;
        }
        newfile.close();
    }
}

//Utility function to perform all required tasks//
//do_non_terminal_terminal_first_follow()//
void do_nt_t_f_f(){
    int nt=0;
   int prst[19];
   for(int i=0; i<19; i++){
       cout<<"\n"<<prods[i];
       prst[i]=identify_nt(prods[i]);
   }
   cout<<"\n\nNon-Terminals";
   for(int i=0;i<9;i++) cout<<"\n"<<non_terminals[i];
   for(int i=0;i<19;i++){
       prodtokens[i].tot=0;
       identify_t(prods[i],prst[i], i);
   }
   cout<<"\n\nTerminals";
   for(int i=0;i<19;i++) cout<<"\n"<<terminals[i];
   for(int i=0;i<9;i++){
       first[i].term=non_terminals[i];
       follow[i].term=non_terminals[i];
       first[i].tot=0;
       follow[i].tot=0;
   }
   for(int i=18;i>=0;i--){
       calc_first(prods[i],prst[i],nont[i],i);
   }
   follow[0].set[0]="$";
   follow[0].tot+=1;
   for(int i=18;i>=0;i--){
       calc_follow(prods[i],prst[i],nont[i]);
   }
}

//Function to print first and follow set//
void print_f_f(){
    cout<<"\n\nNon-Terminals\tFirst\t\t\t\tFollow";
   for(int i=0;i<9;i++){
       int total=first[i].tot;
       int totalf=follow[i].tot;
       cout<<"\n"<<first[i].term<<"  \t\t{ ";
       for(int j=0; j<total-1; j++){ 
           if(first[i].set[j].compare("''")==0){
               cout<<"'' , ";
           }
           else{
               cout<<"'"<<first[i].set[j]<<"' , ";
            }
       }
       cout<<"'"<<first[i].set[total-1]<<"' }";
       cout<<"\t\t\t\t\t\t{ ";
       for(int j=0; j<totalf-1; j++){ 
            cout<<"'"<<follow[i].set[j]<<"' , ";
       }
       cout<<"'"<<follow[i].set[totalf-1]<<"' }";
    }
}

//Function to parse an input string//
bool parse(){
    string in_top=input.topElement();
    int fl=-1;
    for(int i=0;i<3;i++){
        cout<<in_top<<"  "<<prod_first[i];
        if(in_top.compare(prod_first[i])==0){
            fl=i;
            prods_used[tot_pr]=i;
            tot_pr++;
            break;
        }
    }
    if(fl!=-1){
        for(int k=(prodtokens[fl].tot-1);k>=0;k--){
            string str=(prodtokens[fl].token[k]);
            pr_stack.push(str);
        }
        do{
            string in_top=input.topElement();
            string st_top=pr_stack.topElement();
            string nt;
            int flg=0;
            int z=0;
            for(z=0;z<19;z++){
                if(nont[z].compare(st_top)==0){
                    if(prod_first[z].compare(in_top)==0){
                        cout<<pr_stack.pop()<<"<<\n";
                        prods_used[tot_pr]=z;
                        tot_pr++;
                        for(int y=(prodtokens[z].tot-1);y>=0;y--){
                            if((prodtokens[z].token[y]).compare(";")!=0){
                                pr_stack.push((prodtokens[z].token[y]));
                            }
                        }
                        flg=1;
                        break;
                    }
                }
            }
            if(flg==0){
                int flag=0;
                for(int x=0;x<19;x++){
                    if(nont[x].compare(st_top)==0){
                        for(int v=0;v<9;v++){
                            if(prod_first[x].compare(non_terminals[v])==0){
                                cout<<pr_stack.pop()<<"<<\n";
                                prods_used[tot_pr]=x;
                                tot_pr++;
                                pr_stack.push(prod_first[x]);
                                flag=1;
                                break;
                            }
                        }
                    }
                }
                if(flag==0){
                    for(int x=0;x<19;x++){
                        if(nont[x].compare(st_top)==0){
                            if(prod_first[x].compare("''")==0){
                                prods_used[tot_pr]=x;
                                tot_pr++;
                                cout<<pr_stack.pop()<<"<<\n";
                                break;
                            }
                        }
                    }
                }
            }
            int fl=0;
            if(in_top.compare(st_top)==0){
                cout<<input.pop()<<"<<in\n";
                cout<<pr_stack.pop()<<"<<\n";
            }  
        }while(!input.isEmpty());
        return true;
    }
}


//Driver code//
int main(){
   build("grammar.txt");
   do_nt_t_f_f();
   update_follow();
   print_f_f();
    parse_table();
    Stack<string> temp;
    fstream newfile;
    newfile.open("temp.txt",ios::in); //open a file to perform read operation using file object
   if (newfile.is_open()){   //checking whether the file is open
      string tp;
      while(getline(newfile, tp)){ //read data from file object and put it into string.
         temp.push(tp);
      }
      newfile.close(); //close the file object.
   }
   do{
       input.push((temp.pop()));
   }while(!temp.isEmpty());
    if(parse()){
    }
    cout<<"\nProductions Used(in the order):";
    for(int i=0;i<tot_pr;i++){
        cout<<"\n"<<prods[(prods_used[i])];
    }
}