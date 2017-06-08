#include <jni.h>
#include <unistd.h>
#include <string>
#include <android/log.h>

using namespace std;

/**
 * c++层 native函数
 */
JNIEXPORT jstring JNICALL sayHello(JNIEnv *env, jobject clazz) {

    return env->NewStringUTF("Hello, I'm native c++ function : sayHello()!");
}

JNIEXPORT jstring JNICALL sayWhat(JNIEnv *env, jobject clazz, jstring java_string) {
	string ret = "Hello, I'm native c++ function : sayWhat()!\nI get java string : ";
    string stringResult;
    if (java_string) {
        const char *nativeData = env->GetStringUTFChars(java_string, nullptr);
        stringResult = string(nativeData);
        env->ReleaseStringUTFChars(java_string, nativeData);
    }
    ret.append(stringResult);
    return env->NewStringUTF(ret.c_str());
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
    { "helloFromJNI", "()Ljava/lang/String;",(void*)sayHello },
    { "whatFromJNI", "(Ljava/lang/String;)Ljava/lang/String;",(void*)sayWhat },
};

/**
 * 注册native方法到java中
 */
static int registerNativeMethods(JNIEnv* env,
								 const char* className,
        						 JNINativeMethod* gMethods,
        						 int numMethods) {
    jclass clazz = env->FindClass(className);
    if (clazz == NULL) {
        return JNI_FALSE;
    }
    if (env->RegisterNatives(clazz, gMethods, numMethods) < 0){
        return JNI_FALSE;
    }

    return JNI_TRUE;
}

static const int JNI_METHODS_NUM = sizeof(method_table) / sizeof(method_table[0]);
/**
 * System.loadLibrary过程中会自动调用JNI_OnLoad，在此进行动态注册
 */
JNIEXPORT jint JNI_OnLoad(JavaVM* vm, void* reserved)
{
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
    if (registerNativeMethods(env, classname, method_table, JNI_METHODS_NUM) == JNI_FALSE) {
		__android_log_print(ANDROID_LOG_ERROR, "JNITag", "JNI: failed to register methods for %s", classname);
		return -1;
	}

    // 返回jni的版本
    return JNI_VERSION_1_6;
}
