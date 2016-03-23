#include<iostream>
#include<sstream>
#include<ios>
#include"boardstate.h"
#include"evalinfo.h"
#include"search.h"
using namespace std;
void game(bool w_comp,bool b_comp,int tl){
    boardState b(0,0);
    evalInfo curr(b);
    unsigned long bm;
    print(b);
    cout<<"Game begins"<<endl;
    for(;;){
        if(w_comp)
            bm=getMove(b,true,tl);
        else{
            int a;
            if(!(cin>>a)){
                cout<<"Input Error";
                return;
            }
            bm=index(b.height(a),a);
        }
        if(count(bm)!=1){
            cout<<"White moved error \n";
            print(bm);
            return;
        }
        b.move(bm,true);
        print(b);
        if(curr.load(b)){
            print(b);
            cout<<"WHITE WINS\n";
            return;
        }
        if(b_comp)
            bm=getMove(b,false,tl);
        else{
            int a;
            if(!(cin>>a)){
                cout<<"Input Error";
                return;
            }
            bm=index(b.height(a),a);
        }
        if(count(bm)!=1){
            cout<<"black moved error\n";
            print(bm);
            return;
        }
        b.move(bm,false);
        if(curr.load(b)){
            print(b);
            cout<<"BLACK WINS\n";
            return;
        }
        print(b);
    }
}
void game(){
    boardState b(0,0);
    string in;
    string cmd;
    bool w=true;
    while(getline(cin,in)){
        istringstream s(in);
        s>>cmd;
        if(cmd=="analyse"){
            int m;
            s>>m;
            cout<<"analysis started tl "<<m<<endl;
            getMove(b,w,m);
        }
        else if(cmd=="board"){
            print(b);
        }
        else if(cmd=="position"){
            b=boardState(0,0);
            int m;
            w=true;
            while(s>>m){
                b.move(index(b.height(m),m),w);
                w=!w;
            }
        }
        else if(cmd=="drop"){
            int m;
            while(s>>m){
                b.move(index(b.height(m),m),w);
                w=!w;
            }
        }
        else if(cmd=="clear"){
            b=boardState(0,0);
            w=true;
        }
        else if(cmd=="display"){
            int d;
            s>>d;
            displaySearchData(b,w,d,0);
        }
        else if(cmd=="forced"){
            evalInfo e(b);
            print(e.forced(b.moveList(),w));
            print(e.forced(b.moveList(),!w));
            cout<<"\n";
        }
        else if(cmd=="extended"){
            evalInfo e(b);
            uint64_t m=e.forced(b.moveList(),w);
            cout<<((m!=0&&b.legal(m))||(b.legal(m=b.moveList())))&&(e.forced(b.moveList(),!w)==0);
            print(m);
        }
        else if(cmd=="threats"){
            evalInfo e(b);
            print(e.wt);
            print(e.bt);
            cout<<e.evaluate()<<endl;
        }
        else if(cmd=="exit"){
            return;
        }
        cout.flush();
    }
}
int main()
{
    ios::sync_with_stdio(false);
    init_eval();
    cout<<"lyanna 3.1"<<endl;
    //game(true,false,2000);
    game();
    return 0;
}
