#ifndef AD_METHOD_H
#define AD_METHOD_H

#define _MIN_HASH_VALUE 3
#define _MAX_HASH_VALUE 12
#define _MIN_METHOD_LENGTH 3
#define _MAX_METHOD_LENGTH 7

unsigned int _ad_method_char_lookup(register const char *str, register unsigned int len);

int ad_method_is_valid(register const char *input);

#endif

