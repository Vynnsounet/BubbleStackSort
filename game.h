#ifndef GAME_H
#define GAME_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define CLEAR() printf("\e[1;1H\e[2J")
typedef struct stack
{
    int topindex;
    char *elts;

} Stack;

typedef struct game
{
    char *elts;
    Stack *StackArray;

} Game;

#endif // !GAME_H
