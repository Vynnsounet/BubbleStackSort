#include "game.h"

unsigned int STACKSIZE = 4;
unsigned int COLRANGE = 6;

char *colors = "rgbopcyv";

char pop(Stack *s)
{
    char res = s->elts[s->topindex];
    s->elts[s->topindex] = 0;
    s->topindex--;
    return res;
}

char peek(Stack *s)
{
    return s->elts[s->topindex];
}

void push(Stack *s, char c)
{
    s->topindex++;
    s->elts[s->topindex] = c;
}

int is_empty(Stack *s)
{
    return s->topindex == -1;
}

void swap(char *a, char *b)
{
    char tmp = *a;
    *a = *b;
    *b = tmp;
}

void shuffle(char *arr, int range)
{
    int picked;
    for (int i = range - 1; i >= 0; i--)
    {
        picked = rand() % (i + 1);
        swap(&arr[i], &arr[picked]);
    }
}

int is_uniform(Stack s)
{
    for (unsigned int i = 1; i < STACKSIZE; i++)
    {
        if (s.elts[i - 1] != s.elts[i])
            return 0;
    }
    return 1;
}

Game init_elts(unsigned int colrange, unsigned int stacksize)
{
    Game game;

    char *elts = calloc(sizeof(char), (colrange + 2) * stacksize);
    Stack *StackArray = malloc(sizeof(Stack) * (colrange + 2));
    int begin = 0;
    int topindex = stacksize - 1;

    int ei = 0, ci = 0;
    for (unsigned int i = 0; i < colrange; i++)
    {
        StackArray[i].elts = elts + begin;
        StackArray[i].topindex = topindex;
        for (unsigned int j = 0; j < stacksize; j++)
        {
            elts[ei] = colors[ci];
            ei++;
        }
        ci++;
        begin += stacksize;
    }
    StackArray[colrange].elts = elts + begin;
    StackArray[colrange].topindex = -1;
    begin += stacksize;
    StackArray[colrange + 1].elts = elts + begin;
    StackArray[colrange + 1].topindex = -1;

    shuffle(elts, colrange * stacksize);
    game.StackArray = StackArray;
    game.elts = elts;
    return game;
}

char *matchstring(char c)
{
    switch (c)
    {
    case 'r':
        return "\033[38;2;255;0;0mO\033[0m";

    case 'g':
        return "\033[38;2;0;255;0mO\033[0m";

    case 'b':
        return "\033[38;2;0;0;255mO\033[0m";

    case 'p':
        return "\033[38;2;255;82;197mO\033[0m";

    case 'o':
        return "\033[38;2;255;165;0mO\033[0m";

    case 'c':
        return "\033[38;2;0;255;255mO\033[0m";

    case 'y':
        return "\033[38;2;255;255;0mO\033[0m";

    case 'v':
        return "\033[38;2;160;0;160mO\033[0m";

    default:
        return " ";
    }
}

void print_game(Game game, unsigned int colrange, unsigned int stacksize)
{
    for (int i = stacksize - 1; i >= 0; i--)
    {
        for (unsigned int j = i; j < (colrange + 2) * stacksize; j += stacksize)
        {
            printf("| %s |  ", matchstring(game.elts[j]));
        }
        printf("\n");
    }

    printf("  1");
    for (unsigned int i = 2; i <= colrange + 2; i++)
    {
        printf("      %d", i);
    }
    printf("\n");
}

void end_game(Game game)
{
    free(game.elts);
    free(game.StackArray);
}

int check_legal(Game game, int from, int to)
{
    Stack *fromStack = &game.StackArray[from];
    Stack *toStack = &game.StackArray[to];
    return !is_empty(fromStack) && toStack->topindex < (int)STACKSIZE - 1 &&
           (is_empty(toStack) || peek(fromStack) == peek(toStack));
}

void make_move(Game game, int from, int to)
{
    from--;
    to--;
    while (check_legal(game, from, to))
    {
        char c = pop(&game.StackArray[from]);
        push(&game.StackArray[to], c);
    }
}

int is_won(Game game)
{
    for (unsigned int i = 0; i < COLRANGE + 2; i++)
    {
        if (!is_uniform(game.StackArray[i]))
            return 0;
    }
    return 1;
}

int main(int argc, char *argv[])
{
    srand(time(0));
    shuffle(colors, strlen(colors));
    unsigned int *ptrcol = &COLRANGE;
    unsigned int *ptrstack = &STACKSIZE;
    if (argc == 2)
        *ptrstack = atoi(argv[1]);

    if (argc == 3)
    {
        *ptrstack = atoi(argv[1]);
        *ptrcol = atoi(argv[2]);
    }
    Game test = init_elts(COLRANGE, STACKSIZE);
    int init, dst, check, c;
    do
    {
        CLEAR();
        print_game(test, COLRANGE, STACKSIZE);
        printf("Your move: ");
        check = scanf("%d %d", &init, &dst);
        if (check == 2)
        {
            if (init > 0 && init <= (int)COLRANGE + 2 && dst > 0 && dst <= (int)COLRANGE + 2)
                make_move(test, init, dst);
        }
        else
        {
            while ((c = getchar()) != '\n' && c != EOF)
            {
            };
        }

    } while (!is_won(test) || check != 2);
    CLEAR();
    print_game(test, COLRANGE, STACKSIZE);
    end_game(test);
    printf("\033[38;2;0;255;0mYou won!\n\033[0m");

    return 0;
}
