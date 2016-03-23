#ifndef SEARCH_H
#define SEARCH_H
#include"boardstate.h"
int alphaBeta(boardState&b, int alpha, int beta, int d, bool w, bool useTrans);
uint64_t getMove(boardState&b,bool w,int tl);
void displaySearchData(boardState&b,bool w,int d,int k);
#endif // SEARCH_H
