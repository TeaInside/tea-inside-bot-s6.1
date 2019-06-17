
#include <teabot/helpers.h>

size_t teabot_curl_write_callback(void *contents, size_t size, size_t nmemb, void *userp) {
	size_t realsize = size * nmemb;
	char *ptr;

	curl_data *mem = (curl_data *)userp;

	ptr = (char *)realloc(mem->val, mem->len + realsize + 1);
	if(ptr == NULL) {
		/* out of memory! */ 
		warning_log("not enough memory (realloc returned NULL)\n");
		return 0;
	}

	mem->val = ptr;
	memcpy(&(mem->val[mem->len]), contents, realsize);
	mem->len += realsize;
	mem->val[mem->len] = 0;

	return realsize;
}

char *__ltrim(char *str, const char *seps)
{
    size_t totrim;
    if (seps == NULL) {
        seps = "\t\n\v\f\r ";
    }
    totrim = strspn(str, seps);
    if (totrim > 0) {
        size_t len = strlen(str);
        if (totrim == len) {
            str[0] = '\0';
        }
        else {
            memmove(str, str + totrim, len + 1 - totrim);
        }
    }
    return str;
}

char *__rtrim(char *str, const char *seps)
{
    size_t i = strlen(str) - 1;

    if (seps == NULL) {
        seps = "\t\n\v\f\r ";
    }

    while (i >= 0 && strchr(seps, str[i]) != NULL) {
        str[i] = '\0';
        i--;
    }
    return str;
}

char *__trim(char *str, const char *seps)
{
    return __ltrim(__rtrim(str, seps), seps);
}

static unsigned char hexchars[] = "0123456789ABCDEF";

char *php_url_encode(char const *s, size_t len)
{
    register unsigned char c;
    unsigned char *to;
    unsigned char const *from, *end;
    zend_string *start;

    from = (unsigned char *)s;
    end = (unsigned char *)s + len;
    start = zend_string_safe_alloc(3, len, 0, 0);
    to = (unsigned char*)ZSTR_VAL(start);

    while (from < end) {
        c = *from++;

        if (c == ' ') {
            *to++ = '+';
#ifndef CHARSET_EBCDIC
        } else if ((c < '0' && c != '-' && c != '.') ||
                   (c < 'A' && c > '9') ||
                   (c > 'Z' && c < 'a' && c != '_') ||
                   (c > 'z')) {
            to[0] = '%';
            to[1] = hexchars[c >> 4];
            to[2] = hexchars[c & 15];
            to += 3;
#else /*CHARSET_EBCDIC*/
        } else if (!isalnum(c) && strchr("_-.", c) == NULL) {
            /* Allow only alphanumeric chars and '_', '-', '.'; escape the rest */
            to[0] = '%';
            to[1] = hexchars[os_toascii[c] >> 4];
            to[2] = hexchars[os_toascii[c] & 15];
            to += 3;
#endif /*CHARSET_EBCDIC*/
        } else {
            *to++ = c;
        }
    }
    *to = '\0';

    start = zend_string_truncate(start, to - (unsigned char*)ZSTR_VAL(start), 0);

    return start;
}