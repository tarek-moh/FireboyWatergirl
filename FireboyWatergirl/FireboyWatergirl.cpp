#include "game.h"
#include "startMenu.h"

int main()
{
    StartMenu startMenu;
    while (startMenu.running())
    {
        //update..
        startMenu.update();
        //render
        startMenu.render();

    }

    return 0;
}