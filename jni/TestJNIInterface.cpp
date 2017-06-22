#include <jni.h>
#include <unistd.h>
#include <string>
#include <sys/stat.h>
#include <android/log.h>
#include "util/MyLog.h"
#include "client/linux/handler/exception_handler.h"
#include <curl/curl.h>

using namespace std;

static google_breakpad::ExceptionHandler *handler = NULL;

// dump file save path
bool DumpCallback(const google_breakpad::MinidumpDescriptor& descriptor,
		void* context, bool succeeded) {
	printf("Dump path: %s\n", descriptor.path());
	MyLog::helloprint(1, "JNITag", "sxcvxdvsvsd");
	__android_log_print(ANDROID_LOG_ERROR, "JNITag", "Dump path: %s\n",
			descriptor.path());
	return succeeded;
}

void crash() {
	volatile int* a = (int*) (NULL);
	*a = 1;
}

/**
 * c++层 native函数
 */
JNIEXPORT jstring JNICALL sayHello(JNIEnv *env, jobject clazz) {
	crash();
	return env->NewStringUTF("Hello, I'm native c++ function : sayHello()!");
}

JNIEXPORT jstring JNICALL sayWhat(JNIEnv *env, jobject clazz,
		jstring java_string) {
	string ret =
			"Hello, I'm native c++ function : sayWhat()!\nI get java string : ";
	string stringResult;
	if (java_string) {
		const char *nativeData = env->GetStringUTFChars(java_string, nullptr);
		stringResult = string(nativeData);
		env->ReleaseStringUTFChars(java_string, nativeData);
	}
	ret.append(stringResult);
	return env->NewStringUTF(ret.c_str());
}

static size_t OnWriteData(void *buffer, size_t size, size_t nmemb, void *lpVoid) {
    std::string *str = dynamic_cast<std::string *>((std::string *)lpVoid);
    if (NULL == str || NULL == buffer) {
        return -1;
    }

    char *pData = (char *)buffer;
    str->append(pData, size * nmemb);
    return nmemb;
}

static int OnDebug(CURL *, curl_infotype itype, char *pData, size_t size, void *) {
    if (itype == CURLINFO_TEXT) {
        __android_log_print(ANDROID_LOG_ERROR, "JNITag", "[TEXT]%s", pData);
    } else if (itype == CURLINFO_HEADER_IN) {
        __android_log_print(ANDROID_LOG_ERROR, "JNITag", "[HEADER_IN]%s", pData);
    } else if (itype == CURLINFO_HEADER_OUT) {
        __android_log_print(ANDROID_LOG_ERROR, "JNITag", "[HEADER_OUT]%s", pData);
    } else if (itype == CURLINFO_DATA_IN) {
        __android_log_print(ANDROID_LOG_ERROR, "JNITag", "[DATA_IN]%s", pData);
    } else if (itype == CURLINFO_DATA_OUT) {
        __android_log_print(ANDROID_LOG_ERROR, "JNITag", "[DATA_OUT]%s", pData);
    }
    return 0;
}

JNIEXPORT void JNICALL setUpSSL(JNIEnv *env, jobject clazz) {
	__android_log_print(ANDROID_LOG_ERROR, "JNITag", "Hello, I'm native c++ function : setUpSSL()!\n");
	CURL *curl;
	CURLcode code;
	CURLcode curlCode;
	string strResponse;

	curl_global_init(CURL_GLOBAL_DEFAULT);

	curl = curl_easy_init();
	if(curl) {
	    // curl_easy_setopt(curl, CURLOPT_URL, "https://example.com/");
	    curl_easy_setopt(curl, CURLOPT_URL, "https://192.168.93.129:8888");
	    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, OnWriteData);
	    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&strResponse);

	    curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);
	    curl_easy_setopt(curl, CURLOPT_DEBUGFUNCTION, OnDebug);

		/*
		 * If you want to connect to a site who isn't using a certificate that is
		 * signed by one of the certs in the CA bundle you have, you can skip the
		 * verification of the server's certificate. This makes the connection
		 * A LOT LESS SECURE.
		 *
		 * If you have a CA cert for the server stored someplace else than in the
		 * default bundle, then the CURLOPT_CAPATH option might come handy for
		 * you.
		 */
	    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 1L);


	    /*
	     * If the site you're connecting to uses a different host name that what
	     * they have mentioned in their server certificate's commonName (or
	     * subjectAltName) fields, libcurl will refuse to connect. You can skip
	     * this check, but this will make the connection less secure.
	     */
	    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 2L);

//	    curlCode = curl_easy_setopt(curl, CURLOPT_CAPATH, "/data/data/com.example.testjni/");
//	    printf("curlCode: %d\n", (int)curlCode);
//	    __android_log_print(ANDROID_LOG_ERROR, "JNITag", "CURLOPT_CAPATH -> curlCode: %d\n", (int)curlCode);

//	    curlCode = curl_easy_setopt(curl, CURLOPT_CERTINFO, 1L);
//	    printf("curlCode: %d\n", (int)curlCode);
//	    __android_log_print(ANDROID_LOG_ERROR, "JNITag", "CURLOPT_CERTINFO -> curlCode: %d\n", (int)curlCode);

	    long lSize;
	    char * buffer;
	    size_t result;

	    unsigned char ca_certs_pem[10000]={0};
	    unsigned char client_certs_pem[10000]={0};
	    unsigned char client_key_pem[10000]={0};

		FILE* fp = fopen("/data/data/com.example.testjni/ca.crt", "r");
		if (fp == NULL) {
			printf("ca.crt -> fopen failed\n");
			__android_log_print(ANDROID_LOG_ERROR, "JNITag", "ca.crt -> fopen failed\n");
		} else {
			printf("ca.crt -> fopen succeed\n");
			__android_log_print(ANDROID_LOG_ERROR, "JNITag", "ca.crt -> fopen succeed\n");
			// obtain file size:
			fseek (fp , 0 , SEEK_END);
			lSize = ftell (fp);
			rewind (fp);
			buffer = (char*) malloc (sizeof(char)*lSize);
			if (buffer == NULL) {
				printf("read cert alloc Memory error\n");
			}
			// copy the file into the buffer:
			result = fread (buffer,1,lSize,fp);
			if (result != lSize) {
				printf("Reading cert error\n");
			}
			fclose (fp);
			memcpy(ca_certs_pem,buffer,lSize);
			free (buffer);
		}

		FILE* fp1 = fopen("/data/data/com.example.testjni/client.crt", "r");
		if (fp1 == NULL) {
			printf("client.crt -> fopen failed\n");
			__android_log_print(ANDROID_LOG_ERROR, "JNITag", "client.crt -> fopen failed\n");
		} else {
			printf("client.crt -> fopen succeed\n");
			__android_log_print(ANDROID_LOG_ERROR, "JNITag", "client.crt -> fopen succeed\n");
			// obtain file size:
			fseek (fp1 , 0 , SEEK_END);
			lSize = ftell (fp1);
			rewind (fp1);
			buffer = (char*) malloc (sizeof(char)*lSize);
			if (buffer == NULL) {
				printf("read cert alloc Memory error\n");
			}
			// copy the file into the buffer:
			result = fread (buffer,1,lSize,fp1);
			if (result != lSize) {
				printf("Reading cert error\n");
			}
			fclose (fp1);
			memcpy(client_certs_pem,buffer,lSize);
			free (buffer);
		}

		FILE* fp2 = fopen("/data/data/com.example.testjni/client.key", "r");
		if (fp2 == NULL) {
			printf("client.key -> fopen failed\n");
			__android_log_print(ANDROID_LOG_ERROR, "JNITag", "client.key -> fopen failed\n");
		} else {
			printf("client.key -> fopen succeed\n");
			__android_log_print(ANDROID_LOG_ERROR, "JNITag", "client.key -> fopen succeed\n");
			// obtain file size:
			fseek (fp2 , 0 , SEEK_END);
			lSize = ftell (fp2);
			rewind (fp2);
			buffer = (char*) malloc (sizeof(char)*lSize);
			if (buffer == NULL) {
				printf("read cert alloc Memory error\n");
			}
			// copy the file into the buffer:
			result = fread (buffer,1,lSize,fp2);
			if (result != lSize) {
				printf("Reading cert error\n");
			}
			fclose (fp2);
			memcpy(client_key_pem,buffer,lSize);
			free (buffer);
		}
	    curlCode = curl_easy_setopt(curl, CURLOPT_CAINFO, ca_certs_pem);
	    printf("curlCode: %d\n", (int)curlCode);
	    __android_log_print(ANDROID_LOG_ERROR, "JNITag", "CURLOPT_CAINFO -> curlCode: %d\n", (int)curlCode);

	    curlCode = curl_easy_setopt(curl, CURLOPT_SSLCERT, client_certs_pem);
	    printf("curlCode: %d\n", (int)curlCode);
	    __android_log_print(ANDROID_LOG_ERROR, "JNITag", "CURLOPT_SSLCERT -> curlCode: %d\n", (int)curlCode);

	    curlCode = curl_easy_setopt(curl, CURLOPT_SSLKEY, client_key_pem);
	    printf("curlCode: %d\n", (int)curlCode);
	    __android_log_print(ANDROID_LOG_ERROR, "JNITag", "CURLOPT_SSLKEY -> curlCode: %d\n", (int)curlCode);


/*
	    curlCode = curl_easy_setopt(curl, CURLOPT_CAINFO, "ca.crt");
	    printf("curlCode: %d\n", (int)curlCode);
	    __android_log_print(ANDROID_LOG_ERROR, "JNITag", "CURLOPT_CAINFO -> curlCode: %d\n", (int)curlCode);

	    curlCode = curl_easy_setopt(curl, CURLOPT_SSLCERT, "client.crt");
	    printf("curlCode: %d\n", (int)curlCode);
	    __android_log_print(ANDROID_LOG_ERROR, "JNITag", "CURLOPT_SSLCERT -> curlCode: %d\n", (int)curlCode);

	    curlCode = curl_easy_setopt(curl, CURLOPT_SSLCERTTYPE, "PEM");
		printf("curlCode: %d\n", (int)curlCode);
	    __android_log_print(ANDROID_LOG_ERROR, "JNITag", "CURLOPT_SSLCERTTYPE -> curlCode: %d\n", (int)curlCode);

	    // curlCode = curl_easy_setopt(curl, CURLOPT_SSLCERTPASSWD, "");
	    // printf("curlCode: %d\n", (int)curlCode);

	     curlCode = curl_easy_setopt(curl, CURLOPT_SSLKEY, "client.key");
	     printf("curlCode: %d\n", (int)curlCode);
	     __android_log_print(ANDROID_LOG_ERROR, "JNITag", "CURLOPT_SSLKEY -> curlCode: %d\n", (int)curlCode);
*/

		/* Perform the request, res will get the return code */
	    code = curl_easy_perform(curl);
	    printf("code: %d\n", (int)code);
	    __android_log_print(ANDROID_LOG_ERROR, "JNITag", "curl_easy_perform -> code: %d\n", (int)code);
	    __android_log_print(ANDROID_LOG_ERROR, "JNITag", "strResponse:%s\n", strResponse.c_str());

	    /* Check for errors */
	    if (code == CURLE_OK) {
	      printf("curl_easy_perform() succeed!\n");
	    } else if (code == CURLE_SSL_CONNECT_ERROR) {
	      printf("curl_easy_perform() failed, code is CURLE_SSL_CONNECT_ERROR\n");
	    } else {
	      printf("curl_easy_perform() failed: %s\n", curl_easy_strerror(code));
	    }

	    /* always cleanup */
	    curl_easy_cleanup(curl);
	  }

	  curl_global_cleanup();
}

/**
 * Java和JNI函数的绑定表
 * JNINativeMethod由三部分组成,可添加多组对应:
 * (1)Java中的函数名;
 * (2)函数签名,格式为(输入参数类型)返回值类型;
 *    ()Ljava/lang/String; ()表示无参，Ljava/lang/String;表示返回String，在对象类名（包括包名，‘/’间隔）前面加L，分号结尾
 * (3)native函数名
 */
static JNINativeMethod method_table[] = {
		{ "helloFromJNI", "()Ljava/lang/String;", (void*) sayHello },
		{ "whatFromJNI", "(Ljava/lang/String;)Ljava/lang/String;", (void*) sayWhat },
		{ "setUpSSLJNI", "()V", (void*) setUpSSL },
};

/**
 * 注册native方法到java中
 */
static int registerNativeMethods(JNIEnv* env, const char* className,
		JNINativeMethod* gMethods, int numMethods) {
	jclass clazz = env->FindClass(className);
	if (clazz == NULL) {
		return JNI_FALSE;
	}
	if (env->RegisterNatives(clazz, gMethods, numMethods) < 0) {
		return JNI_FALSE;
	}

	return JNI_TRUE;
}

static const int JNI_METHODS_NUM = sizeof(method_table)
		/ sizeof(method_table[0]);
/**
 * System.loadLibrary过程中会自动调用JNI_OnLoad，在此进行动态注册
 */
JNIEXPORT jint JNI_OnLoad(JavaVM* vm, void* reserved) {
	JNIEnv* env = NULL;
	jint result = -1;

	if (vm->GetEnv((void**) &env, JNI_VERSION_1_6) != JNI_OK) {
		return result;
	}

	/**
	 * Java class name
	 * consist of packageName and className
	 * For com.example.testjni.MainActivity, classname is com/example/testjni/MainActivity
	 */
	char classname[] = "com/example/testjni/MainActivity"; //Java class name, consist of packageName.className
	if (registerNativeMethods(env, classname, method_table, JNI_METHODS_NUM)
			== JNI_FALSE) {
		__android_log_print(ANDROID_LOG_ERROR, "JNITag",
				"JNI: failed to register methods for %s", classname);
		return -1;
	}

	/* Create a directory with read, write and execute access for the owner and read access for the group */
	string dmppath = "/sdcard/testjni/minidump";
	string dmpparentpath = "/sdcard/testjni";

	if (access(dmpparentpath.c_str(), 0) != 0) {
		mkdir(dmpparentpath.c_str(), S_IRWXU | S_IRGRP);
	}
	if (!access(dmpparentpath.c_str(), 0)) {
		if (access(dmppath.c_str(), 0) != 0) {
			mkdir(dmppath.c_str(), S_IRWXU | S_IRGRP);
		}
	}
	if (!access(dmppath.c_str(), 0)) {
		google_breakpad::MinidumpDescriptor descriptor(dmppath);
		handler = new google_breakpad::ExceptionHandler(descriptor, NULL,
				DumpCallback, NULL, true, -1);
	}

// 返回jni的版本
	return JNI_VERSION_1_6;
}
