#ifndef __MY_LOG_H__
#define __MY_LOG_H__

#include <android/log.h>

class MyLog {
public:
	static void helloprint(int level, const char *tag, const char *str);
};

#endif  //__MY_LOG_H__
