#include "MyLog.h"

void MyLog::helloprint(int level, const char *tag, const char *str) {
	switch(level) {
	case 1:
		__android_log_print(ANDROID_LOG_ERROR, tag, "");
		break;
	case 2:
		__android_log_print(ANDROID_LOG_INFO, tag, "");
		break;
	default:
		__android_log_print(ANDROID_LOG_DEBUG, tag, "");
		break;
	}
}
