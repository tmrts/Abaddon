#include "ad_server.h"

/* Example Abaddon usage */
int main(int argc, char *argv[]) {

    /* Abaddon will listen on port 8118 */
    return ad_server_listen(8118);
}
