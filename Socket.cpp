#include "Socket.hpp"
#include <iostream> //std::cout, std::cerr, std::endl
#include <unistd.h> //close()
#include <csignal>  //signal(), sigemptyset(), sigaddset(), raise()
#include <cstdlib>
#include <arpa/inet.h>

bool Socket::kbdint;

Socket::Socket(const socket_mode MODE, const uint16_t PORT)
{
    kbdint = false;
    signal(SIGINT, SIGINT_handler);
    addrlen = sizeof(addr);

    //for polling purposes
    timeout.tv_sec = 0;
    timeout.tv_nsec = 10000000; //10 msec
    sigemptyset(&sigmask);
    sigaddset(&sigmask, SIGINT);

    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);

    //TODO: Make it where IP address can be sent in as command line/constructor argument as well

    if (MODE == socket_mode::SERVER) {
        // Creating socket file descriptor
        if ((socketfd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
            std::cerr << "Socket creation failed..." << std::endl;
            raise(SIGINT);
        }

        // Forcefully attaching socket to the port 8080
        /*if (setsockopt(socketfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
            perror("setsockopt");
            exit(EXIT_FAILURE);
        }*/

        //addr.sin_family = AF_INET;
        addr.sin_addr.s_addr = INADDR_ANY;
        //addr.sin_port = htons(PORT);

        if (bind(socketfd, reinterpret_cast<sockaddr*>(&addr), addrlen) < 0) {
            std::cerr << "Binding socket failed..." << std::endl;
            raise(SIGINT);
        }
        
        if (listen(socketfd, 3) < 0) {
            std::cerr << "Listening failed..." << std::endl;
            raise(SIGINT);
        }
    }
    else {
        if ((remotefd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
            std::cerr << "Socket creation failed..." << std::endl;
            raise(SIGINT);
        }

        //addr.sin_family = AF_INET;
        //addr.sin_port = htons(PORT);
    
        // Convert IPv4 and IPv6 addresses from text to binary form
        if (inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr) <= 0) {
            std::cerr << "Invalid address or address not supported" << std::endl;
            raise(SIGINT);
        }
    }
}

Socket::~Socket()
{
    close(remotefd);    // Close the connected socket
    close(socketfd);    // Closing the listening socket
    //shutdown(socketfd, SHUT_RDWR);
    std::cout << "Socket closed" << std::endl;
}

bool Socket::ppoll()
{
    pfd.fd = socketfd;      //NOTE: These 2 lines actually need to be done every time despite
    pfd.events = POLLIN;    //      there being no mention of it in the documentation.
    ::ppoll(&pfd, NFDS, &timeout, &sigmask);
    return (pfd.revents & POLLIN);
}

void Socket::accept()
{
    if ((remotefd = ::accept(socketfd, reinterpret_cast<sockaddr*>(&addr), &addrlen)) < 0) {
        std::cerr << "Accepting connection failed..." << std::endl;
        raise(SIGINT);
    }
}

void Socket::recv()
{
    static const uint8_t FLAGS = 0;
    static uint16_t size_prev = 0,
                    size_read = 0;  //NOTE: This must be declared here for this to work properly

    //size_read = ::read(remotefd, buffer, BUFFER_SIZE);
    size_read = ::recv(remotefd, buffer, BUFFER_SIZE, FLAGS);
    if (size_read < 0) {
        std::cerr << "Error reading incoming message" << std::endl;
        return;
    }
    
    if (size_prev >= size_read) {    //NOTE: This is needed because the previous contents of the buffer
        buffer[size_read] = '\0';   //      are preserved, and the null terminator isn't set by ::recv
    }
    size_prev = size_read;
    
    std::cout << buffer << std::endl;
}

void Socket::connect()
{
    if ((::connect(remotefd, reinterpret_cast<sockaddr*>(&addr), addrlen)) < 0) {
        std::cerr << "Connection failed..." << std::endl;
        raise(SIGINT);
    }
}

void Socket::send(std::string msg)
{
    static const uint8_t FLAGS = 0;
    ::send(remotefd, msg.c_str(), msg.length(), FLAGS);
    std::cout << "Message sent" << std::endl;
}

void Socket::SIGINT_handler(int32_t sig)
{
    std::cout << "Inside SIGINT handler..." << std::endl;
    kbdint = true;
}

bool Socket::kbd_int()
{
    return kbdint;
}
