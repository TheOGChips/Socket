#include "Socket.hpp"
#include <unistd.h> //system()
#include <iostream> //std::cout, std::endl

int main(int argc, char ** argv)
{
    system("stty -echoctl");
    uint16_t port;

    switch (argc) {
        case 2: port = atoi(argv[1]);
                break;
        
        default: port = 8080;
                 std::cout << "Warning: No port number specified..." << std::endl
                           << "         Using default port number " << port << "..." << std::endl;
    }

    Socket server = Socket(socket_mode::SERVER, port); 
    while (!server.kbd_int()) {
        if (server.ppoll()) {
            server.accept();
            server.recv();
        }
    }
    
    system("stty echoctl");
    return 0;
}