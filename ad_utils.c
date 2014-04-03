#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#include "ad_utils.h"

int ad_utils_is_directory(const char *path)
{
    return (strrchr(path, '/') - path) == (strlen(path) - 1);
}

int ad_utils_strcmp_ic(register const char *str1, register const char *str2)
{
    register unsigned int i;
    unsigned char c1, c2;

    while ( str1 && str2 )
    {
        c1 = tolower(*str1++);
        c2 = tolower(*str2++);

        if (c1 != 0 && c1 == c2) { continue; }
        else                     { break; }
    }
    return c1 - c2;
}

size_t ad_utils_substr_exists(const char *str, const char *substr)
{
    size_t substr_count = 0;
    const char *end, *p, *s = str;

    while(s < str && (p = strstr(s, substr)))
    {
        s = p + strlen(substr);
        substr_count++;
    }

    return substr_count;
}

char **ad_utils_split_str(const char *str, const char *delimiters)
{
    char *token;
    char *src = malloc(sizeof(char) * (strlen(str) + 1));
    char **tokens = malloc(sizeof(char *));
    size_t i;

    strcpy(src, str);

    token = strtok(src, delimiters);
    tokens[0] = token;

    for(i = 1; token = strtok(NULL, delimiters); i++)
    {
        tokens = realloc(tokens, sizeof(char *) * (i + 2));

        tokens[i] = token;
    }

    tokens[i + 1] = NULL; 

    return tokens;
}
