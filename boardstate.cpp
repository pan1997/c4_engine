#include "boardstate.h"
#include<iostream>
void print(boardState &b){
    for(int i=0;i<7;i++){
        for(int j=0;j<9;j++)
            std::cout<<(getbit(b.w,6-i+j*7)?'W':(getbit(b.b,6-i+j*7)?'B':' '))<<"|";
        std::cout<<"\n";
    }
    std::cout<<"0|1|2|3|4|5|6|7|8|\n";
}
void print(uint64_t x){
    for(int i=0;i<7;i++){
        for(int j=0;j<9;j++)
            std::cout<<(getbit(x,6-i+j*7)?"1":" ")<<"|";
        std::cout<<"\n";
    }
    std::cout<<"0|1|2|3|4|5|6|7|8\n";
}
