#include <stdio.h>
#include "tsock.h"

int main() {
    char buffer[4];
    int tsockClient = tsock_connect("lol.sock");
    tsock_read(tsockClient, buffer, (size_t) sizeof(buffer));
    printf("%s\n",buffer);
    tsock_close(tsockClient);

    return 0;
}
    
