#include "engine.hpp"
#include <iostream>
#include <string>
#include <stdio.h>
#include <string.h>

Engine::Engine()
{
    //init curses
    initCurses();

    //get client or host mode
    switch(getMode())
    {
    case 0:
        initHost();
        hostListen();
        break;
    case 1:
        initClient();
        clientLoop();
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


/////////////////////////////////////////////////////////////////////////////
// HOST
bool Engine::initHost()
{
    curs_set(1);
    echo();
    hostip = sf::IpAddress::getLocalAddress();

    clear();
    printw("Local IP: %s\n\n", hostip.toString().c_str());
    printw("Enter server port: ");
    scanw("%hu", &hostport);

}

void Engine::hostListen()
{

    bool quit = false;
    //create server socket
    sf::UdpSocket socket;
    socket.setBlocking(false);

    sf::IpAddress cipaddress;
    unsigned short cport;
    char cdata[256];
    size_t csize;

    if(socket.bind(hostport) != sf::Socket::Done)
    {
        printw("Error opening server on port %hu\n", hostport);
        return;
    }
    else printw("Opened server on port %hu.  Listening for messages...\n", hostport);

    refresh();

    //start listen loop
    while(!quit)
    {
        if(socket.receive(cdata, sizeof(cdata), csize, cipaddress, cport) == sf::Socket::Done )
        {
            printw("Received %d bytes of data from %s on port %hu\n", csize, cipaddress.toString().c_str(), cport);
            cdata[csize] = '\0';
            printw(">%s\n", cdata);
            refresh();
        }
    }

}

///////////////////////////////////////////////////////////////////////////////
// CLIENT

bool Engine::initClient()
{
    char connecttohost[256];
    curs_set(1);
    echo();

    clear();
    printw("Local port to use: ");
    scanw("%hu", &localport);
    printw("Host IP to connect to: ");
    scanw("%s", connecttohost);
    hostip = sf::IpAddress(connecttohost);
    printw("Enter server port: ");
    scanw("%hu", &hostport);
}

void Engine::clientLoop()
{
    bool quit = false;

    //printw("Connecting to host %s on port %hu...\n", hostip.toString().c_str(), hostport);

    sf::UdpSocket socket;
    socket.setBlocking(false);
    if(socket.bind(localport) == sf::Socket::Done)
    {
        printw("Opened socket on port %hu\n", localport);
    }
    else
    {
        printw("Error opening socket on port %hu!", localport);
        return;
    }

    refresh();

    while(!quit)
    {
        char buf[256];
        std::string bufstr;

        printw(">");
        getstr(buf);

        bufstr = std::string(buf);

        if( !strcmp(buf, "quit")) quit = true;
        else
        {
            //send message to server
            if(socket.send(bufstr.c_str(), bufstr.size(), hostip, hostport) != sf::Socket::Done)
            {
                printw("Error sending packet to server!\n");
            }

        }

        refresh();
    }
}
