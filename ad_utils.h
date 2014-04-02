#ifndef AD_UTILS_H
#define AD_UTILS_H

int ad_utils_strcmp_ic(register const char *str1, register const char *str2);

int ad_utils_is_directory(const char *path);

size_t ad_utils_substr_exists(const char *str, const char *substr);

char **ad_utils_split_str(const char *str, const char *delimiters);

#endif
