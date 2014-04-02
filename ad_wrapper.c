#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include "ad_wrapper.h"

void system_error(const char *error_msg)
{
    perror(error_msg);
    exit(EXIT_FAILURE);
}

int check_error(int return_value, char *func_name)
{
    if(return_value == -1)
        errno = return_value;
        system_error(func_name);
    return return_value;
}
