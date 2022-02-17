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

int doSomething() {
    int sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
    return 0;
}