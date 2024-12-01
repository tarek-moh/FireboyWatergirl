#include "game.h"


int main()
{
    Game game;
    while (game.running())
    {
        //update..
        game.update();
        //render
        game.render();

    }

    return 0;
}