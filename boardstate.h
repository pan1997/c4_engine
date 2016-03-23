#ifndef BOARDSTATE_H
#define BOARDSTATE_H
#include<cstdint>
#include<unordered_map>
#define bottom 72624976668147841
inline uint64_t getCol(uint64_t x,int i){
    return x&(127L<<(i*7));
}
inline uint64_t shiftedCol(uint64_t x,int i){
    return (x>>(i*7))&127L;
}
inline uint64_t index(int r,int c){
    return ((uint64_t)1)<<(r+c*7);
}
inline int getbit(uint64_t l,int i){
    return int(l>>i)&1;
}
inline int getColumnNumber(int64_t x){
    for(int i=0;i<9;i++)
        if(getCol(x,i)!=0)
            return i;
    return -1;
}
inline int _height(uint64_t l,int i){
    switch((l>>(i*7))&127L){
    case 0:return 0;
    case 1:return 1;
    case 3:return 2;
    case 7:return 3;
    case 15:return 4;
    case 31:return 5;
    case 63:return 6;
    case 127:return 7;
    }
    return -1;
}
inline int count(uint64_t i){
    /*int c=0;
    for(int i=0;x>0;i++){
        if(x&1)
            c++;
        x>>=1;
    }
    return c;*/
    i = i - ((i >> 1) & 0x5555555555555555UL);
    i = (i & 0x3333333333333333UL) + ((i >> 2) & 0x3333333333333333UL);
    return (int)((((i + (i >> 4)) & 0xF0F0F0F0F0F0F0FUL) * 0x101010101010101UL) >> 56);
    //*/
}
class boardState
{
    uint64_t w,b;
public:
    boardState(){w=b=0;}
    boardState(uint64_t p,uint64_t q){w=p;b=q;}
    boardState(const boardState&bs){w=bs.w;b=bs.b;}
    bool operator<(const boardState&bs){
        return w<bs.w||(w==bs.w&&b<bs.b);
    }
    uint64_t filled(){return w|b;}
    uint64_t moveList(){
        uint64_t l=filled();
        return ((l<<1)|bottom)&~l;
    }
    int height(int i){
        return _height(b|w,i);
    }
    void move(uint64_t x,bool wh){ //does both do and undo
        (wh?w:b)^=x;
    }
    bool operator==(const boardState&bS)const{
        return w==bS.w&&b==bS.b;
    }
    bool legal(uint64_t m){
        return (((m&bottom)|(m>>1&filled()))!=0)&&count(m)==1;
    }
    friend void print(boardState &b);
    friend class evalInfo;
    friend class std::hash<boardState>;
};
void print(boardState&b);
void print(uint64_t x);
#endif // BOARDSTATE_H
