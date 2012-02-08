#include "tsock.h"

int main() {
    char buffer[4] = "aaa";
    int tsockServ = tsock_listen("lol.sock");
    int tsockCanal = tsock_accept(tsockServ);
    tsock_write(tsockCanal, buffer, (size_t) sizeof(buffer));

    tsock_close(tsockCanal);
    tsock_close(tsockServ);

    return 0;
}

