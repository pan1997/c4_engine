#include "search.h"
#include<unordered_map>
#include "boardstate.h"
#include "evalinfo.h"
#include<iostream>
#include<chrono>
#include<cmath>
#define hsize 70000000 // less than 20 seconds optimised
#define ply 8
inline uint64_t hx(uint64_t x){
    return x^(1610612741*(x>>32));
}
namespace std{
template<>struct hash<boardState>{
	std::size_t operator()(const boardState&b)const{
        return hx(b.w)+(786433*hx(b.b));
	}
};
}
int mdr[]={4,3,5,2,6,1,7,0,8}; //static move ordering
std::unordered_map<boardState,evalInfo> transpositionTable(hsize);
//transposition_table transpositionTable;
int nodes,fuprune,prt;
#define R 3
int alphaBeta(boardState&b,int alpha,int beta,int d,bool w,bool useTrans){ // do not store msl and msh rest can be stored
    nodes++;
    auto it=transpositionTable.find(b);
    evalInfo*e;
    {
        if(it!=transpositionTable.end()){
            e=&(it->second);
            if(e->d>=d){
                if(e->msl>=beta) return e->msl;
                if(e->msh<=alpha) return e->msh;
                if(e->msh==e->msl) return e->msh;
                alpha=std::max(alpha,e->msl);
                beta=std::min(beta,e->msh);
            }
            else{
                if(e->msh<=-2000||e->msl>=2000)
                    return e->msh;
                e->msh=2000;
                e->msl=-2000;
                e->d=d;
            }
        }
        else{
            e=&(transpositionTable.emplace(b,b).first->second);
            if(e->d>=d) //ie game end
                return e->msh;
        }
    }
    int g,ms;
    uint64_t ml=b.moveList();
    if(ml==0) //game ends in draw as no completion above
        return 0;
    uint64_t m=e->forced(ml,w);
    //if(((m!=0&&b.legal(m))||b.legal(m=ml))&&(e->forced(ml,!w)==0||e->forced(ml,!w)==m)){
    if((m!=0&&b.legal(m)&&(e->forced(ml,!w)==0||e->forced(ml,!w)==m))||b.legal(m=ml)){
        e->bm=m;
        b.move(m,w);
        g=alphaBeta(b,alpha,beta,d,!w,useTrans); //extend
        b.move(m,w);
        if(useTrans){
            if(g>alpha&&g<beta)e->msh=e->msl=g;
            else if(g<=alpha)e->msh=g;
            else e->msl=g;
            //e->d=d+ply; causes drop in play strength
        }
        return g;
    }
    else if(d<=0){
        g=e->evaluate();
        if(useTrans)
            e->msh=e->msl=g;
        return g;
    }
    else{
        if(w){
            g=-2000;
            int a=alpha;
            for(int i=-1;i<9&&g<beta;i++){
                if(i==-1)
                    m=e->bm;
                else
                    m=getCol(ml,mdr[i]);
                if(m!=0&&m!=UINT64_MAX){ //non zero move
                    b.move(m,true);
                    ms=alphaBeta(b,a,beta,d-ply,false,useTrans);
                    b.move(m,true);
                    if(ms>g){
                    //nab++;
                        g=ms;
                        e->bm=m;
                        if(ms>a)
                            a=ms;
                    }
                }
            }
        }else{
            g=2000;
            int bt=beta;
            for(int i=-1;i<9&&g>alpha;i++){
                if(i==-1)
                    m=e->bm;
                else
                    m=getCol(ml,mdr[i]);
                if(m!=0&&m!=UINT64_MAX){ //non zero move
                    b.move(m,false);
                    ms=alphaBeta(b,alpha,bt,d-ply,true,useTrans);
                    b.move(m,false);
                    if(ms<g){
                    //nab++;
                        g=ms;
                        e->bm=m;
                        if(ms<bt)
                            bt=ms;
                    }
                }
            }
        }
    }
    if(useTrans){
        if(g>alpha&&g<beta)e->msh=e->msl=g;
        else if(g<=alpha)e->msh=g;
        else e->msl=g;
    }
    return g;
}
int getpv(boardState&b,bool w,int*pv,bool pend){
    if(transpositionTable.count(b)){
        evalInfo*e=&(transpositionTable[b]);
        uint64_t bm=e->getBestMove();
        if(count(bm)!=1){
            if(pend)
                print(b);
            return 0;
        }
        b.move(bm,w);
        int l=getpv(b,!w,pv+1,pend);
        b.move(bm,w);
        *pv=getColumnNumber(bm);
        return 1+l;
    }
    else if(pend)
        print(b);
    return 0;
}
void displaySearchData(boardState&b,bool w,int d,int k){
    auto it=transpositionTable.find(b);
    if(it!=transpositionTable.end()&&d>0){
        evalInfo*e=&(it->second);
        uint64_t ml=b.moveList();
        uint64_t m;
        for(int i=0;i<9;i++)
            if((m=getCol(ml,i))!=0){
                b.move(m,w);
                for(int j=0;j<k;j++)
                    std::cout<<"  ";
                std::cout<<getColumnNumber(m);
                if(m==e->bm)
                    std::cout<<"__";
                else std::cout<<"  ";
                for(int j=0;j<d;j++)
                    std::cout<<" ";
                it=transpositionTable.find(b);
                if(it!=transpositionTable.end())
                    std::cout<<"("<<it->second.msl<<","<<it->second.msh<<")"<<"\n";
                else std::cout<<"--??\n";
                displaySearchData(b,!w,d-1,k+1);
                b.move(m,w);
            }
    }
}
int mtdf(boardState&b,bool w,int d,int fg,int*ar){
    int g=fg;
    int l=-2000;
    int u=2000;
    while(l<u){
        if(g==l) fg=g+1;else fg=g;
        g=alphaBeta(b,fg-1,fg,d*ply,w,true);
        if(g<fg)u=g;
        else l=g;
        int l=getpv(b,w,ar,false);
        if(fg!=g){
            std::cout<<"ms "<<g<<" pv->";
            for(int i=0;i<l;i++)
                std::cout<<ar[i]<<" ";
            std::cout<<"\n";
        }
    }
    return l;
}
uint64_t getMove(boardState&b,bool w,int tl){
    uint64_t ml=b.moveList();
    evalInfo el(b);
    if(el.forced(ml,!w)!=0){
        std::cout<<"BM "<<getColumnNumber(el.forced(ml,!w))<<" END"<<std::endl;
        return ml;
    }
    if(el.forced(ml,w)!=0){
        if(b.legal(el.forced(ml,w))){
            std::cout<<"BM "<<getColumnNumber(el.forced(ml,w))<<" FORCED"<<std::endl;
            return el.forced(ml,w);
        }
        else{
            std::cout<<"BM "<<getColumnNumber(el.forced(ml,w))<<" RESIGN"<<std::endl;
            return getCol(el.forced(ml,w),getColumnNumber(el.forced(ml,w)));
        }
    }
    if(b.legal(ml)){
        std::cout<<"BM "<<getColumnNumber(ml)<<" OL ms 0"<<std::endl;
        return ml;
    }
    nodes=0;
    fuprune=0;
    prt=0;
    transpositionTable.clear();
    auto start=std::chrono::system_clock::now();
    int tm=0;
    int i;
    int ms=0;
    std::cout<<"MAX D "<<81-count(b.filled())<<"\n";
    int ar[100];
    for(i=0;i<80-count(b.filled());i++){
        //ms=alphaBeta(b,-2001,2001,i,w);
        ms=mtdf(b,w,i,ms,ar);
        //std::cout<<"ms "<<transpositionTable[b].getmsl()<<" pv->";
        //int l=getpv(b,w,ar,false);
        //for(int i=0;i<l;i++)
        //    std::cout<<ar[i]<<" ";
        //std::cout<<"----------------";
        //std::cout<<nodes<<" "<<transpositionTable.size()<<std::endl;
        std::cout.flush();
        auto now=std::chrono::system_clock::now();
        if((tm=std::chrono::duration_cast<std::chrono::milliseconds>(now-start).count())>tl)
            break;
        if(ms>=2000||ms<=-2000)
            break;
    }
    uint64_t bm=transpositionTable[b].getBestMove();
    //getpv(b,w,ar,true);
    std::cout<<"BM "<<getColumnNumber(bm)<<" in "<<tm<<" MIllis ms "<<transpositionTable[b].getmsl()<<" "<<prt<<"/"<<fuprune<<" null "<<nodes<<" nodes@"<<nodes/(tm+1)<<"kNodes/sec "<<i<<" plys BF"<<std::pow(nodes,1.0/i)<<"\n";
    return bm;
}
