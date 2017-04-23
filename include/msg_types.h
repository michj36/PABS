#ifndef MSG_TYPES_H_INCLUDED
#define MSG_TYPES_H_INCLUDED

#include <string>
#include <cstdio>
namespace pabs
{
enum msg_type {
	HELLO,
	OLLEH,
	HW_RU,
	OK,
	ERR,
	START_SCN,
	DATA,
	ENOUGH,
	SERVER_END,
	AGENT_END
};

typedef struct {
	char ver;
	char type;
	unsigned time;
	unsigned pkts;
	unsigned bytes;
} data_msg;
typedef struct {
	char ver;
	char type;
	char data[14];
} generic_msg;

typedef union {
	generic_msg gen;
	data_msg dat;
} msg;


inline void printType(msg_type t)
{
	switch(t) {
	case HELLO:
		printf("HELLO");
		break;
	case OLLEH:
		printf("OLLEH");
		break;
	case HW_RU:
		printf("HW_RU");
		break;
	case OK:
		printf("OK");
		break;
	case ERR:
		printf("ERR");
		break;
	case START_SCN:
		printf("START_SCN");
		break;
	case DATA:
		printf("DATA");
		break;
	case ENOUGH:
		printf("ENOUGH");
		break;
	case SERVER_END:
		printf("SERVER_END");
		break;
	case AGENT_END:
		printf("AGENT_END");
		break;
	}
}
inline std::string typeToString(msg_type t)
{
	switch(t) {
	case HELLO:
		return "HELLO";
		break;
	case OLLEH:
		return ("OLLEH");
		break;
	case HW_RU:
		return("HW_RU");
		break;
	case OK:
		return("OK");
		break;
	case ERR:
		return("ERR");
		break;
	case START_SCN:
		return("START_SCN");
		break;
	case DATA:
		return("DATA");
		break;
	case ENOUGH:
		return("ENOUGH");
		break;
	case SERVER_END:
		return("SERVER_END");
		break;
	case AGENT_END:
		return("AGENT_END");
		break;
	}
}
inline void printMsg(data_msg *m)
{
	printf("Ver:%d Type:", m->ver);
	printType((msg_type)m->type);
	printf("Time:%d Pkts:%d Bytes:%d\n", m->time, m->pkts, m->bytes);
}
}

#endif // MSG_TYPES_H_INCLUDED
