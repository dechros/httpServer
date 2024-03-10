#ifndef SOCKET_H
#define SOCKET_H

#include <stdio.h>
#include <stdbool.h>
#include <winsock2.h>
#include <pthread.h>
#include <string.h>

void socketInit(const int port);
void socketDestroy(void);

#endif