#include "engine.hpp"
#include <iostream>
#include <string>

Engine::Engine()
{
    //init curses
    initCurses();

    //get client or host mode
    switch(getMode())
    {
    case 0:
        initHost();
        break;
    case 1:
        break;
    }

}

Engine::~Engine()
{


}

bool Engine::initCurses()
{
    //init screen
    initscr();
    //allow extra keys
    keypad(stdscr, true);
    //no echo
    noecho();

    return true;
}

int Engine::getMode()
{
    int ch = 0;
    int selection = 0;
    std::vector<std::string> modes;
    bool quit = false;

    //disable cursors
    curs_set(0);

    modes.push_back("Host");
    modes.push_back("Client");

    while(!quit)
    {
        clear();
        printw("Enter mode:\n");
        for(int i = 0; i < int(modes.size()); i++)
        {

            printw("   %d. ", i+1);
            if(selection == i) attron(A_REVERSE);
            printw("%s\n", modes[i].c_str());
            attroff(A_REVERSE);
        }
        //printw("%d\n", ch);

        ch = getch();

        //user pressed escape
        if(ch == 27) return -1;
        //user pressed up arrow
        else if(ch == 259)
        {
            selection--;
            if(selection < 0) selection = 0;
        }
        //user pressed down arrow
        else if(ch == 258)
        {
            selection++;
            if(selection >= int(modes.size())) selection = modes.size()-1;
        }
        //user pressed return
        else if(ch == 10)
        {
            return selection;
        }

    }


}

bool Engine::initHost()
{
    curs_set(1);
    echo();
    hostip = sf::IpAddress::getLocalAddress();

    clear();
    printw("Local IP: %s\n\n", hostip.toString().c_str());
    printw("Enter server port:");
    scanw("%hh", hostport);

}
