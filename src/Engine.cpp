#include "Engine.h"

    sf::Packet& operator <<(sf::Packet& packet, const netmsg& m)
    {
    return packet << m.type << m.msg;
    }
    sf::Packet& operator >>(sf::Packet& packet, netmsg& m)
    {
    return packet >> m.type >> m.msg;
    }

Engine::Engine(int nmode)
{
    //ctor
    runMode = nmode;


}

Engine::~Engine()
{
    //dtor
}

void Engine::start()
{
    mainLoop();

}

void Engine::ServerListen()
{
       bool quit = false;

        //bind socket
        socket.bind(PORT);
        socket.setBlocking(false);
        std::cout << "Listening on " << PORT << std::endl;

        while(!quit)
        {
            sf::Packet ReceivedPkt;
            sf::IpAddress SenderIP;
            unsigned short SenderPort;
            netmsg msg;

            if(!socket.receive(ReceivedPkt, SenderIP, SenderPort))
            {
                ReceivedPkt >> msg;
                if(msg.type == 1) std::cout << "Client requesting acknowledge...\n";
            }
        }
}
void Engine::mainLoop()
{
    bool quit = false;

    if(runMode == MODE_SERVER)
    {

        //start server listen thread
        serverListenThread = new sf::Thread(&Engine::ServerListen, this);
        serverListenThread->launch();

        //start console
        while(!quit)
        {
            std::string buf;
            std::cout << "SERVER>";
            std::getline(std::cin, buf);

        }
    }
    else if(runMode == MODE_CLIENT)
    {
        std::string username;
        std::cout << "Enter name:";
        std::getline(std::cin, username);

        //send request to server
        sf::Packet req;
        netmsg reqmsg;
        reqmsg.type = 1;
        req << reqmsg;
        socket.send(req, "127.0.0.1", PORT);
        std::cout << "Request sent to server...\n";

        //wait for a response
        socket.bind(PORT);
        sf::Packet pktReqAck;
        sf::IpAddress SenderIP;
        unsigned short SenderPort;
        netmsg reqackmsg;

        if(!socket.receive(pktReqAck, SenderIP, SenderPort))
        {
            ReceivedPkt >> reqackmsg;
            std::cout << "Request acknowledged\n";
        }


        while(!quit)
        {
            std::string inputbuf;
            std::cout << "CLIENT>";
            std::getline(std::cin, inputbuf);

            if(inputbuf == "quit") quit = true;

            sf::Packet ToSend;
            ToSend << inputbuf;
            socket.send(ToSend, "127.0.0.1", PORT);
        }
    }
}
