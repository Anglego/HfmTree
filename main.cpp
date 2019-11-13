#include <iostream>
#include <string.h>
using namespace std;
typedef struct fnode
{//define the characters's frequency node
    char  key;
    int   freq;
}fnode,*ftable;
typedef struct HNode
{//define  the tree node
    int weight;
    int parent,ld,rd;
}HNode ,*HfmTree;

//define the codenode
typedef char **HfmCode;
int StrCount(string str,ftable &W);
void Select(HfmTree HT,int n,int &s1,int &s2);
void CreateHfmTree(ftable W,HfmTree &HT,int n);
void PrintTree(HfmTree HT,int n);
void CreateHfmCode(HfmTree HT,HfmCode &HC,int n);
void show(ftable W,HfmCode HC,int n);
void EnCode(ftable W,HfmCode HC,string str,string &str2,int n);
void DeCode(ftable W,HfmTree HT,string str2,int n);

//input a string &count frequency
int StrCount(string str,ftable &W)
{
    int i,j,num=0;
   ftable L;
    L=new fnode[26];
    //1initial L
    for(i=0;i<26;i++)
    {
        L[i].key=97+i;
        L[i].freq=0;
        cout<<L[i].key<<L[i].freq<<"  ";
    }//1for
cout<<"初始化完毕"<<endl;

    //2copy character to W &count
    for(i=0;i<str.length();i++)
    {
        for(j=0;j<26;j++)
            {if(str[i]==L[j].key)
            L[j].freq++;}
    }//2for
    //3print result& character numbers
    for(i=0;i<26;i++)
    {
        if(L[i].freq>0)
        {cout<<L[i].key<<L[i].freq<<'\t';
          num++;}
    }
    cout<<endl;
    //4pick out 0 node
    W=new fnode[num+1];
    j=1;
    for(i=0;i<26;i++)
    {
        if(L[i].freq>0)
        {
                W[j].key=L[i].key;W[j].freq=L[i].freq;
                //cout<<W[j].key<<W[j].freq;
                j++;
        }//if
    }//4for
    delete []L;

    return num;
}//StrCount
//select the two minux node
void Select(HfmTree HT,int n,int &s1,int &s2)
{
    int min1,min2,i,temp;
    min1=min2=0x3f3f3f3f;
   //1选出min1
    for(i=1;i<=n;i++)
    {
        if(HT[i].weight<min1&&HT[i].parent==0)
        {
            min1=HT[i].weight;
            s1=i;
        }
    }//for1
    temp=HT[s1].weight;
    HT[s1].weight=0x3f3f3f3f;//mark the used node
    //2for select the min2
    for(i=1;i<=n;i++)
    {
        if(HT[i].weight<min2&&HT[i].parent==0)
        {
            min2=HT[i].weight;
            s2=i;
        }
    }//for2
        HT[s1].weight=temp;//放回
}//select
void CreateHfmTree(ftable W,HfmTree &HT,int n)
{
    int i,j,m,s1,s2 ;
    m=2*n;//total nodes number is  2n-1,use 1~2n-1, 0 left null
    HT=new HNode[m];
    //1for  initial HT
    for(i=0;i<m;i++)
    {
        HT[i].weight=0;HT[i].parent=0;HT[i].ld=0;HT[i].rd=0;
    }//for1
    //copy W[i].weight to HT[i].weight
    for(j=1;j<=n;j++)
    {
        HT[j].weight=W[j].freq;
    }//for2
    //3for  write the new nodes in greedy
    for(i=n+1;i<m;i++)
    {
        Select(HT,i-1,s1,s2);
        HT[s1].parent=i;
        HT[s2].parent=i;
        HT[i].weight=HT[s1].weight+HT[s2].weight;
        HT[i].ld=s1;
        HT[i].rd=s2;
    }//for3

}//CreateHfmTree
void PrintTree(HfmTree HT,int n)
{
    int i;
    cout<<endl;
    cout<<"NO.    "<<"weight"<<'\t'<<"parent"<<'\t'<<"ld"<<'\t'<<"rd"<<endl;
    for(i=0;i<2*n;i++)
    {
        cout<<i<<'\t'<<HT[i].weight<<'\t'<<HT[i].parent<<'\t'<<HT[i].ld<<'\t'<<HT[i].rd<<endl;
    }
}//PrintTree

void CreateHfmCode(HfmTree HT,HfmCode &HC,int n)
{
    //create codes table by the static tree table, from leaves (1~n) to  root
    int i, start,c,f;//start is a counter from back to front，c is the activity node，f is c's parent
    HC=new char*[n+1];
    char *cd=new char[n];
    cd[n-1]='\0';
    for(i=1;i<=n;i++)
    {
        start=n-1;
        c=i;
        f=HT[c].parent;
        while(f!=0)
        {//left 0,right 1
            --start;
            if(HT[f].ld==c) cd[start]='0';
            else cd[start]='1';
            c=f;//to its father
            f=HT[c].parent;
        }//while
        HC[i]=new char[n-start];
        strcpy(HC[i],&cd[start]);
    }//for
    delete cd;
    cout<<"finish coding!"<<endl;

}//CreateHfmCode
void show(ftable W,HfmCode HC,int n)
{
    int i;
    cout<<"NO.    "<<"key     "<<"weight      "<<"code"<<endl;
    for(i=1;i<=n;i++)
    {
        cout<<i<<'\t'<<W[i].key<<'\t'<<W[i].freq<<'\t'<<HC[i]<<endl;
    }
}//show

//copy the coding results into str2,& calcute the compress rate
void EnCode(ftable W,HfmCode HC,string str,string &str2,int n)
{
    int i,j;
         for(i=0;i<=str.size();i++)
    {
           for(j=1;j<=n;j++)
                    if(W[j].key==str[i])// search key in W
                          str2.append(HC[j]);//replace with codes
        }//for
      cout<<"The length of original string is :(a character includes 8bits）:"<<8*str.size()<<endl;
      cout<<endl;
     cout<<endl<<"Coding Result:"<<str2<<"length:"<<str2.size()<<endl;
      cout<<endl;
     double  v= double(str2.size())/double(8*str.size());
     cout<<"Compress Rate="<<v<<endl;
      cout<<endl;
}//Encode
void DeCode(ftable W,HfmTree HT,string str2,int n)
{//DeCoding from root to leaf ,if 0 goto its  left child，if 1goto its right child，until ld &rd is null
    int j=0;int i=2*n-1;
     cout<<"Deconding finish!"<<endl;
      cout<<endl;
   // cout<<str2<<endl;
    while(j<=str2.size())
    {// match str2[j]& HT[i] in HT table
        if (str2[j]=='0') i=HT[i].ld;
        else if(str2[j]=='1') i=HT[i].rd;
       j++;
      if(HT[i].ld==0)
        {cout<<W[i].key; i=2*n-1;}
    }

}
int main()
{
  string str;ftable W;int leafnum;HfmTree HT;HfmCode HC;string str2,str3;
  cout<<endl<<"Input a string，str's max size is "<<str.max_size()<<endl;
  cin>>str;
  cout<<"The length of string is:"<<str.length()<<endl;
  leafnum=StrCount(str,W);
  cout<<"Finish counting! The number of leaf is :"<<leafnum;
 CreateHfmTree(W,HT,leafnum);
  PrintTree(HT,leafnum);
  CreateHfmCode(HT,HC,leafnum);
  EnCode(W,HC,str,str2,leafnum);
  cout<<"Original String:"<<str<<endl;
  cout<<endl;
 DeCode(W, HT,str2,leafnum);

    return 0;
}


