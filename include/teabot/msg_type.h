
#ifndef teabot__msg_type_h
#define teabot__msg_type_h

typedef enum _msg_type {
	msg_text = (1 << 0),
	msg_photo = (1 << 1),
	msg_video = (1 << 2),
	msg_voice = (1 << 3),
} msg_type;

#endif
