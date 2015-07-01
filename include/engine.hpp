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
    short int hostport;
    sf::IpAddress hostip;


public:
    Engine();
    ~Engine();

};

#endif // CLASS_ENGINE
