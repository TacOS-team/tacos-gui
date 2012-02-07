#include "tsock.h"


int main () {
    char buffer [4] = "aaa";
    int tsockServ = tsockListen ("./lol");
    int tsockCanal = tsockAccept(tsockServ);
    tsockWrite(tsockCanal,buffer, (size_t) sizeof(buffer));

    tsockClose(tsockCanal);
    tsockClose(tsockServ);

    return 0;
}

