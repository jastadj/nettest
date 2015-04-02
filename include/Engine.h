#ifndef ENGINE_H
#define ENGINE_H

#include <SFML\Network.hpp>
#include <iostream>
#include <string>

#define MODE_SERVER 0
#define MODE_CLIENT 1

#define PORT 4567
#define IPADDRESS 127.0.0.1

struct netmsg
{
    int type;
    std::string msg;
};

class Engine
{
private:

    int runMode;

    //mainloop
    void mainLoop();

    //threads
    sf::Thread *serverListenThread;
    void ServerListen();

    //socket
    sf::UdpSocket socket;
    sf::IpAddress ipaddress;
    unsigned short port;

public:
    Engine(int nmode);
    ~Engine();

    void start();
};

#endif // ENGINE_H
