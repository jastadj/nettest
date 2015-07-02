#ifndef CLASS_ENGINE
#define CLASS_ENGINE

#include <SFML\Network.hpp>
#include "curses.h"

class Engine
{
private:

    //init
    bool initCurses();
    int getMode();

    //host
    bool initHost();
    void hostListen();
    unsigned short hostport;
    sf::IpAddress hostip;

    //client
    bool initClient();
    void clientLoop();
    unsigned short localport;


public:
    Engine();
    ~Engine();

};

#endif // CLASS_ENGINE
