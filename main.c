#include "ad_server.h"

/* Example Abaddon usage */
int main(int argc, char *argv[]) {

    /* Abaddon will listen on port 8098 */
    return ad_server_listen(6948);
}
