#include "Engine.h"

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
void Engine::mainLoop()
{
    if(runMode == MODE_SERVER)
    {
        bool quit = false;

        //bind socket
        socket.bind(PORT);
        std::cout << "Listening on " << PORT << std::endl;

        while(!quit)
        {
            sf::Packet ReceivedPkt;
            sf::IpAddress SenderIP;
            unsigned short SenderPort;
            std::string msg;

            socket.receive(ReceivedPkt, SenderIP, SenderPort);

            ReceivedPkt >> msg;

            std::cout << msg << std::endl;

        }


    }
    else if(runMode == MODE_CLIENT)
    {
        bool quit = false;

        while(!quit)
        {
            std::string inputbuf;
            std::cout << ">";
            std::getline(std::cin, inputbuf);

            if(inputbuf == "quit") quit = true;

            sf::Packet ToSend;
            ToSend << inputbuf;
            socket.send(ToSend, "127.0.0.1", PORT);
            std::cout << "UDP message sent.\n";
        }
    }
}
