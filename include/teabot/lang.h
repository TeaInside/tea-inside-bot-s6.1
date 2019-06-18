
#ifndef teabot__lang_h
#define teabot__lang_h

#include <string.h>

#define FALLBACK_LANGUAGE(KEY) KEY##_en
#define LANG_CHOICE(KEY, LANG) \
	( \
		(!strcmp(LANG, "en")) ? KEY##_en : ( \
			(!strcmp(LANG, "id")) ? KEY##_id : \
			FALLBACK_LANGUAGE(KEY) \
		) \
	)

#endif
