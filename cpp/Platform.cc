#include <jni.h>
#include <string>

#ifdef __cplusplus
extern "C" {
#endif
JNIEXPORT jstring JNICALL Java_platform_Platform_nativeGetString(JNIEnv *env,
                                                                 jobject thiz) {
  jclass clazz = env->GetObjectClass(thiz);
  if (clazz == nullptr) {
    return env->NewStringUTF("Get Object error!");
  }
  jmethodID method_id = env->GetMethodID(
      clazz, "registerDelegate", "(Lplatform/Platform$PlatformDelegate;)V");
  if (method_id == nullptr) {
    return env->NewStringUTF("Get methodid error!");
  }

  jclass delegate_class = env->FindClass("platform/Platform$PlatformDelegate");
  if (delegate_class == nullptr) {
    return env->NewStringUTF("Get delegate class error!");
  }

  jmethodID delegate_class_constructor_id =
      env->GetMethodID(delegate_class, "<init>", "(Ljava/lang/String;)V");
  if (delegate_class_constructor_id == nullptr) {
    return env->NewStringUTF("Get delegate class constructor error!");
  }

  jstring delegate_name = env->NewStringUTF("Delegate from native!");

  if (delegate_name == nullptr) {
    return env->NewStringUTF("build delegate_name error!");
  }

  jobject delegate_object = env->NewObject(
      delegate_class, delegate_class_constructor_id, delegate_name);
  if (delegate_object == nullptr) {
    return env->NewStringUTF("build delegate error!");
  }

  env->CallVoidMethod(thiz, method_id, delegate_object);

  return env->NewStringUTF("Hello from JNI !");
}

JNIEXPORT void JNICALL Java_platform_Platform_nativeUpdate(JNIEnv *env,
                                                           jobject obj) {
  jclass clazz;
  clazz = env->GetObjectClass(obj);
  if (clazz == nullptr) {
    return;
  }
  jfieldID label_id = env->GetFieldID(clazz, "label", "Ljava/lang/String;");
  if (label_id == nullptr) {
    return;
  }
  jstring label_java_string = (jstring)env->GetObjectField(obj, label_id);
  if (label_java_string == nullptr) {
    return;
  }

  const char *buf = env->GetStringUTFChars(label_java_string, nullptr);
  std::string buf_out = std::string(buf) + " by native!";

  jstring temp = env->NewStringUTF(buf_out.c_str());
  env->SetObjectField(obj, label_id, temp);

  env->ReleaseStringUTFChars(label_java_string, buf);
  env->DeleteLocalRef(label_java_string);
  env->DeleteLocalRef(temp);

  jfieldID static_count_field_id = env->GetStaticFieldID(clazz, "count", "I");
  if (static_count_field_id == nullptr) {
    return;
  }

  jint count_java = env->GetStaticIntField(clazz, static_count_field_id);

  env->SetStaticIntField(clazz, static_count_field_id, count_java + 2);

  jmethodID static_method_id = env->GetStaticMethodID(
      clazz, "addSuffix", "(Ljava/lang/String;)Ljava/lang/String;");
  if (nullptr == static_method_id) {
    return;
  }

  jstring input = env->NewStringUTF("Delegate from native!");

  jstring output =
      (jstring)env->CallStaticObjectMethod(clazz, static_method_id, input);

  static_method_id = env->GetStaticMethodID(clazz, "printNativeMessage",
                                            "(Ljava/lang/String;)V");
  if (nullptr == static_method_id) {
    return;
  }

  env->CallStaticObjectMethod(clazz, static_method_id, output);

  env->DeleteLocalRef(clazz);
}

#ifdef __cplusplus
}
#endif
