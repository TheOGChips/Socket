//NOTE: This wrapper class was adapted from the example on the Geeks for Geeks website
//src: https://www.geeksforgeeks.org/socket-programming-cc/

#ifndef SOCKET_HPP
#define SOCKET_HPP

#include <sys/socket.h> //socket(), sockaddr, send(), accept(), recv(), ppoll()
#include <netinet/in.h> //sockaddr_in
#include <poll.h>       //ppoll()
#include <string>       //std::string

enum class socket_mode {SERVER, CLIENT};

class Socket {
    private:
        int32_t socketfd,   //used by server
                remotefd;   //user by server and client
        sockaddr_in addr;
        socklen_t addrlen;
        static const uint16_t BUFFER_SIZE = 1024;
        char buffer[BUFFER_SIZE] = {};
        static bool kbdint; //used for checking if there has been a keyboard interrupt (Ctrl+C or ^C)

        //for polling purposes
        pollfd pfd;
        const nfds_t NFDS = 1;
        timespec timeout;
        sigset_t sigmask;

    public:
        Socket() {}
        Socket(const socket_mode, const uint16_t);
        ~Socket();
        bool ppoll();
        void accept();
        void recv();
        void connect();
        void send(std::string msg);
        static void SIGINT_handler(int32_t);
        bool kbd_int();
};

#endif