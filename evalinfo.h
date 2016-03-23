#ifndef EVALINFO_H
#define EVALINFO_H
#include<cstdint>
#include"boardstate.h"
void init_eval();
class evalInfo
{
    uint64_t wt,bt;
    int msl,msh;
    int d;
    uint64_t bm;
public:
    evalInfo():wt(0),bt(0),msl(-2000),msh(+2000),d(-100),bm(UINT64_MAX){}
    evalInfo(boardState&b):evalInfo(){
        load(b);
    }
    bool load(const boardState&b);//correct depth and msl and msh too
    int evaluate();
    //inline int evalSingleCol(uint64_t wc, uint64_t bc); //both cloumns shifted to column 0
    //int evalMultiple();
    inline uint64_t forced(uint64_t l,bool w){
        return (w?bt:wt)&l;
    }
    friend int alphaBeta(boardState&b,int alpha,int beta,int d,bool w,bool x);
    friend void displaySearchData(boardState&b,bool w,int d,int k);
    friend void game();
    inline uint64_t getBestMove(){return bm;}
    inline int getmsl(){return msl;}
    inline int getmsh(){return msh;}
};
#endif // EVALINFO_H
