#include "evalinfo.h"
#include<iostream>
unsigned long precomp[126];
unsigned long indexes[126][4];
void init_eval(){
    int n=0;
    for(int i=0;i<7;i++){
        for(int j=0;j<9;j++){
            if(i<4){
                indexes[n][0]=index(i,j);
                indexes[n][1]=index(i+1,j);
                indexes[n][2]=index(i+2,j);
                indexes[n][3]=index(i+3,j);
                precomp[n++]=index(i,j)|index(i+1,j)|index(i+2,j)|index(i+3,j);
            }
            if(j<6){
                indexes[n][0]=index(i,j);
                indexes[n][1]=index(i,j+1);
                indexes[n][2]=index(i,j+2);
                indexes[n][3]=index(i,j+3);
                precomp[n++]=index(i,j)|index(i,j+1)|index(i,j+2)|index(i,j+3);
            }
            if(i<4&&j<6){
                indexes[n][0]=index(i,j);
                indexes[n][1]=index(i+1,j+1);
                indexes[n][2]=index(i+2,j+2);
                indexes[n][3]=index(i+3,j+3);
                precomp[n++]=index(i,j)|index(i+1,j+1)|index(i+2,j+2)|index(i+3,j+3);
            }
            if(i>2&&j<6){
                indexes[n][0]=index(i,j);
                indexes[n][1]=index(i-1,j+1);
                indexes[n][2]=index(i-2,j+2);
                indexes[n][3]=index(i-3,j+3);
                precomp[n++]=index(i,j)|index(i-1,j+1)|index(i-2,j+2)|index(i-3,j+3);
            }
        }
    }
    std::cout<<" precomp initialized\n";
}
bool evalInfo::load(const boardState&b){
    //int wc=0,bc=0;
    bt=wt=0;
    for(int i=0;i<126;i++){
        if((b.b&precomp[i])==0&&((b.w&precomp[i])!=0)){
            if((b.w&precomp[i])==precomp[i]){
                msl=msh=2000;
                d=1000;
                return true;
            }
            else{
                switch(((b.w&(indexes[i][0]))?1:0)^((b.w&(indexes[i][1]))?2:0)^((b.w&(indexes[i][2]))?4:0)^((b.w&(indexes[i][3]))?8:0)){
                case 7:wt|=indexes[i][3];break;
                case 11:wt|=indexes[i][2];break;
                case 13:wt|=indexes[i][1];break;
                case 14:wt|=indexes[i][0];break;
                }
            }
        }
        else if(((b.b&precomp[i])!=0)&&((b.w&precomp[i])==0)){
            if((b.b&precomp[i])==precomp[i]){
                msl=msh=-2000;
                d=1000;
                return true;
            }
            else{
                switch(((b.b&(indexes[i][0]))?1:0)^((b.b&(indexes[i][1]))?2:0)^((b.b&(indexes[i][2]))?4:0)^((b.b&(indexes[i][3]))?8:0)){
                case 7:bt|=indexes[i][3];break;
                case 11:bt|=indexes[i][2];break;
                case 13:bt|=indexes[i][1];break;
                case 14:bt|=indexes[i][0];break;
                }
            }
        }
    }
    return false;
}
int evalSingleCol(uint64_t wc,uint64_t bc);
#define WOB 7
#define LT 10
#define DT 20
#define BONLDT 10
#define T 5
#define BONP 3
#define BONL 3
#define DOWNW 20
#define DOWNB -20
int evalInfo::evaluate(){
    int ms=0;
    bool lstw,lstb;//lowest
    int nc=-1;
    for(int j=0;j<9;j++)if(getCol(wt|bt,j)!=0){
        if(nc==-1)
            nc=j;
        else if(nc>-1)
            nc=-2;
        uint64_t l=1L<<(j*7);
        lstw=lstb=true;
        for(int i=0;i<7;i++){
            if((wt&l)!=0){
                if(lstw&&lstb){
                    ms+=LT;
                    if(i%2)
                        ms+=BONP;
                }
                else if(i>1&&(bt&(l>>2))!=0){ //threat just 2 blocks above threat of opposite color
                    ms+=WOB;
                    break;
                }
                else if(i>0&&(wt&(l>>1))!=0){ //threat just above a threat of same color. this is after above check for useless dt
                    ms+=DT;
                    if(lstb)
                        ms+=BONLDT;
                    break;
                }
                else if(!((i>1&&(wt&(l>>2)))!=0||(i>0&&(bt&(l>>1))!=0))){ //not useless
                    ms+=T;
                    if(lstb)
                        ms+=BONL;
                }
            }
            if((bt&l)!=0){
                if(lstw&&lstb){
                    ms-=LT;
                    if(i%2)
                        ms-=BONP;
                }
                else if(i>1&&(wt&(l>>2))!=0){
                    ms-=WOB;
                    break;
                }
                else if(i>0&&(bt&(l>>1))!=0){
                    ms-=DT;
                    if(lstw)
                        ms-=BONLDT;
                    break;
                }
                else if(!(i>1&&(bt&(l>>2))!=0||i>0&&(wt&(l>>1))!=0)){ //not useless
                    ms-=T;
                    if(lstw)
                        ms-=BONL;
                }
            }
            if((wt&bt&l)!=0)
                break;
            if((wt&l)!=0)
                lstw=false;
            if((bt&l)!=0)
                lstb=false;
            l<<=1;
        }
    }
    if(nc>-1)
        ms+=evalSingleCol(shiftedCol(wt,nc),shiftedCol(bt,nc));
    return msh=msl=ms;
}
int evalSingleCol(uint64_t wc,uint64_t bc){
    uint64_t l=1;
    for(int i=0;i<7;i++){
        if(i%2==0&&((wc&l)!=0))
            return DOWNW;
        else if(i%2==1&&((bc&l)!=0))
            return DOWNB;
        l<<=1;
    }
    return 0;
}
/*
#define LPT 7
#define LT 10
#define DT 30
#define T 4
#define BONL 2
#define BONP 1;int evalInfo::evaluate(){
    int ms=0;
    bool wed,wod,bed,bod;
    for(int j=0;j<9;j++)if(getCol(wt|bt,j)!=0){
        wed=wod=bed=bod=true;
        uint64_t l=1L<<(j*7);
        for(int i=0;i<7;i++){
            if((wt&bt&l)!=0){
                //no lt as both will get it
                if((i%2==0&&bed)||(i%2==1&&bod))
                    ms+=T; //break
                if((i%2==0&&wod)||(i%2==1&&wed))
                    ms+=LPT; //break
                if((i%2==0&&wed)||(i%2==1&&wod))
                    ms-=T; //break
                if((i%2==0&&bod)||(i%2==1&&bed))
                    ms-=LPT; //break
                break;
            }else if((wt&l)!=0){
                if(wed&&wod&&bed&&bod){
                    ms+=LT;
                    if(i%2==0)
                        ms+=BONL;
                }
                if((i%2==0&&bed)||(i%2==1&&bod))
                    ms+=T; //break
                if((i%2==0&&wod)||(i%2==1&&wed)){
                    ms+=LPT; //break
                    if(i%2==0)\
                        ms+=BONP;
                }
                if(i==0||(wt&(l>>1))!=0)
                    ms+=DT;
                (i%2?wod:wed)=false;
                if((!wed&&!wod)||(!wed&&!bed)||(!wod&&!bod)||(!bed&&!bod))
                    break;
            }else if((bt&l)!=0){
                if(wed&&wod&&bed&&bod){
                    ms-=LT;
                    if(i%2)
                        ms-=BONL;
                }
                if((i%2==0&&wed)||(i%2==1&&wod))
                    ms-=T; //break
                if((i%2==0&&bod)||(i%2==1&&bed)){
                    ms-=LPT; //break
                    if(i%2)
                        ms-=BONP;
                }
                if(i==0||(bt&(l>>1))!=0)
                    ms-=DT;
                (i%2?bod:bed)=false;
                if((!wed&&!wod)||(!wed&&!bed)||(!wod&&!bod)||(!bed&&!bod))
                    break;
            }
            l<<=1;
#ifdef DEBUG
            std::cout<<"("<<i<<","<<j<<")"<<wed<<wod<<bed<<bod<<std::endl;
#endif
        }
    }
    return msh=msl=ms;
}
int evalInfo::evaluate(){
    if(wt==0&&bt==0)
        return 0;
    int nc=-1;
    for(int i=0;i<9;i++)
        if(getCol(wt|bt,i)!=0){
            if(nc==-1)
                nc=i;
            else
                return msh=msl=evalMultiple();
        }
    return msh=msl=evalMultiple()+evalSingleCol(getCol(wt,nc),getCol(bt,nc));
}*/
