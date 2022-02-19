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

/**
 * One-and-done, start listening on port 8765 and return after first accept
 *
 * @return the server socket int value
 */
int nativeListen() {
    static const int PORT = 8765;

    int server_fd, client_fd;
    struct sockaddr_in address;
    int addrLen = sizeof(address);
    int backlogLen = 5;
    int opt = 1;
    int bytesRead;
    char buffer[1024] = {0};

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        __android_log_print(ANDROID_LOG_WARN, "nativeListen", "socket error");
        return server_fd;
    }

    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
                   &opt, sizeof(opt))) {
        __android_log_print(ANDROID_LOG_WARN, "nativeListen", "setsockopt error");
        return server_fd;
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );

    __android_log_print(ANDROID_LOG_DEBUG, "nativeListen", "binding port: %d", PORT);
    if (bind(server_fd, (struct sockaddr *)&address,
             sizeof(address))<0)
    {
        __android_log_print(ANDROID_LOG_WARN, "nativeListen", "bind error");
        return server_fd;
    }
    if (listen(server_fd, backlogLen) < 0)
    {
        __android_log_print(ANDROID_LOG_WARN, "nativeListen", "listen error");
        return server_fd;
    }

    // this _should_ block
    if ((client_fd = accept(server_fd, (struct sockaddr *)&address,
                            (socklen_t*)&addrLen)) < 0)
    {
        __android_log_print(ANDROID_LOG_WARN, "nativeListen", "accept error");
        return server_fd;
    }
    __android_log_print(ANDROID_LOG_DEBUG, "nativeListen", "accepted: %d", client_fd);
    bytesRead = read(client_fd , buffer, 1024);
    if (bytesRead >= 0) {
        __android_log_print(ANDROID_LOG_DEBUG, "nativeListen", "bytesRead: %d", bytesRead);
        __android_log_print(ANDROID_LOG_DEBUG, "nativeListen", "bytesRead: %s", buffer);

        send(client_fd, buffer, sizeof(buffer), 0);
        __android_log_print(ANDROID_LOG_DEBUG, "nativeListen", "echoed");
    }

    return server_fd;
}