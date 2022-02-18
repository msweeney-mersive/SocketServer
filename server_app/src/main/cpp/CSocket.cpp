#include "CSocket.h"

#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/poll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <android/log.h>

int doSomething() {
    int server_socket, client_socket, valread;
    struct sockaddr_in server_address;
    struct sockaddr_in client_address;
    char buffer[1024] = {0};

    const int port = 8765;

    server_socket = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);

    if (server_socket <= 0)
    {
        __android_log_print(ANDROID_LOG_ERROR, "doSomething", "socket failed");
    } else {
        server_address.sin_family = AF_INET;
        server_address.sin_addr.s_addr = INADDR_ANY;
        server_address.sin_port = htons( port );

        if (bind(server_socket, (struct sockaddr *)&server_address,
                 sizeof(server_address)) < 0)
        {
            __android_log_print(ANDROID_LOG_ERROR, "doSomething", "bind failed");
        } else {
            __android_log_print(ANDROID_LOG_DEBUG, "doSomething", "bind success on port %d", port);
            // one and done
            socklen_t len = sizeof(client_address);
            int rcvMsgSz = recvfrom(server_socket, buffer, 255, 0,
                                    (struct sockaddr *)&client_address,
                                    &len);

            __android_log_print(ANDROID_LOG_DEBUG, "doSomething", "recvfrom: %d", rcvMsgSz);

            sendto(server_socket, buffer, rcvMsgSz, 0, (struct sockaddr *)&client_address,len);
        }
    }

    return server_socket;
}