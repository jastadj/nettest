#include <cstdlib>
#include "engine.h"

int main(int argc, char *argv[])
{
    bool clientmode = false;
    if(argc > 1)
    {
        if(argv[1][0] == '1') clientmode = true;
    }

    Engine engine(clientmode);

    engine.start();


    return 0;

}
