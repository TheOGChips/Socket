#include "Socket.hpp"
#include <string>
#include <iostream>

int main(int argc, char ** argv)
{
    uint16_t port;
    std::string msg;

    switch (argc) {
        case 3: port = atoi(argv[2]);
                msg = argv[1];
                break;

        case 2: port = 8080;
                msg = argv[1];
                std::cout << "Warning: No port number specified..." << std::endl
                          << "         Using default port number " << port << "..." << std::endl;
                break;

        default: port = 8080;
                 msg = "Default message";
                 std::cout << "Warning: No message or port number specified..." << std::endl
                           << "         Using default port number " << port << " to send default message..." << std::endl;
                 
    }
    
    //TODO: Check if putting this cycle in its own function might work
    Socket client (socket_mode::CLIENT, port);
    client.connect();
    if (!client.kbd_int()) {
        client.send(msg);
    }

    return 0;
}