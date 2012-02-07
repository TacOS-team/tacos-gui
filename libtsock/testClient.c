#include <stdio.h>
#include "tsock.h"

int main () {
    char buffer [4];
    int tsockClient = tsockConnect("./lol");
    tsockRead(tsockClient, buffer, (size_t) sizeof(buffer));
    printf("%s\n",buffer);
    tsockClose (tsockClient);
    return 0;
}
    
