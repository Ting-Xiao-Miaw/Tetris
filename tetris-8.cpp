#include "wincursor.h"
#define nSize	4		// How many squares are there in a tetromino
#define nType   7               // How many tetromino types
#define nLeftBorder	9
#define nRightBorder	23
#define nBottomBorder   21
#define nTopBorder      1

enum Movement { UP, ROTATE = 0, DOWN, LEFT, RIGHT };

Screen myScreen;

#include "tetris.h"
#include <ctime>

void init_game();
void game_over();

int main()
{
    srand(time(NULL));
    int i = rand() % nType;
    int x0, c, next_i;
    init_game();
    CTetromino inpt(0, 0, 0);
    inpt.Erase();
    inpt.Point(0);
    LOOP: while (true)
    {
        next_i = rand() % nType;
        x0 = 2 * (rand() % (nRightBorder - nLeftBorder - max_y[i] / 2) + nLeftBorder + 1);
        CTetromino a(i, x0);
        CTetromino b(next_i, nRightBorder * 2 + 3, nTopBorder + 3);
        i = next_i;
        if (a.detect_confliction())     // a newly generated tetromino
            break;                      // conflicts, then game over
        do {
            myScreen.redraw();
            c=myScreen.key();
            switch (c)
            {
                case 'q':
                    game_over();
                    return 0;
                    break;
                case KEY_LEFT:
                case 'h':
                    a.Move(LEFT);
                    break;
                case KEY_RIGHT:
                case 'l':
                    a.Move(RIGHT);
                    break;
                case KEY_UP:
                case 'k':
                    a.Move(ROTATE);
                    break;
                case ' ':
                    while (a.Move(DOWN)) {
                        myScreen.redraw();
                    }
                    b.Erase();
                    goto LOOP;
                    break;
            }
        } while (a.Move(DOWN));
        b.Erase();
    }

    myScreen.redraw();
    game_over();
    return 0;
}

void init_game()
{
    int y0, x0;
    myScreen.setTimeOut(300);
    String star("**");

    for (y0=nTopBorder + 1; y0< nBottomBorder; y0++)
    {
        star.show(y0, nLeftBorder * 2);
        star.show(y0, nRightBorder * 2);
    }

    for (x0=(nLeftBorder + 1) * 2; x0<nRightBorder * 2; x0+=2)
    {
        star.show(nBottomBorder, x0);
    }
    myScreen.redraw();
}

void game_over()
{
    String msg("Game Over!", nTopBorder + 1, nRightBorder*2 + 2);
    msg.show();
    myScreen.redraw();
    myScreen.pause(2);
}
