
#include <stdio.h>
#include <stdarg.h>
#include <teabot/logger/logger.h>

int __log(const char *format, ...) {

	va_list args;

	va_start(args, format);
	int ret = vprintf(format, args);
	printf("\n");
	va_end(args);

	return ret;
}
