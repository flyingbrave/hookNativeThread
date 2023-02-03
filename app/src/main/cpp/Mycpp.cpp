//
// Created by brave on 2023/2/2.
//
#include <jni.h>
#include <string>
#include <pthread.h>
#include <__threading_support>
#include <ostream>
#include <android/log.h>
#include <iostream>
#include <sys/mman.h>
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>


#define LOGD(TAG, FMT, args...) __android_log_print(ANDROID_LOG_DEBUG, TAG, FMT, ##args)
#define TAG "tag5"



bool isHook = true;

int my_pthread_create(pthread_t* __pthread_ptr, pthread_attr_t const* __attr, void* (*__start_routine)(void*), void* p1)
{
    if(isHook){
        isHook = false;
        LOGD(TAG, "%s","pthread hook power by brave");
        return pthread_create(__pthread_ptr,__attr,__start_routine,p1);
    } else{
        return 0;
    }
}


#define PAGE_START(addr) ((addr) & PAGE_MASK)
#define PAGE_END(addr)   (PAGE_START(addr) + PAGE_SIZE)


void hook()
{
    char       line[512];
    FILE      *fp;
    uintptr_t  base_addr = 0;
    uintptr_t  addr;

    //寻找基地址
    if(NULL == (fp = fopen("/proc/self/maps", "r"))) return;
    while(fgets(line, sizeof(line), fp))
    {
        if(NULL != strstr(line, "libhelloworld.so") &&
           sscanf(line, "%" PRIxPTR"-%*lx %*4s 00000000", &base_addr) == 1)
        break;
    }
    fclose(fp);

    LOGD(TAG, " 基地址 %u", base_addr);

    if(0 == base_addr) return;



    //得到真实的函数地址 可由readif -r 看到
    addr = base_addr + 0x034ca8;
    LOGD(TAG, " 真实地址 %u", addr);

    // 添加读写权限
    mprotect((void *)PAGE_START(addr), PAGE_SIZE, PROT_READ | PROT_WRITE);
    LOGD(TAG, " %s", "添加完权限");
    // 替换为函数地址
    *(void **)addr = (unsigned*)&my_pthread_create;
    LOGD(TAG,  "替换为函数地址  %u",addr);
    // 清除缓存
    __builtin___clear_cache(static_cast<char *>((void *) PAGE_START(addr)),
                            static_cast<char *>((void *) PAGE_END(addr)));
    LOGD(TAG, " %s", "清除缓存");
}




extern "C"
JNIEXPORT void JNICALL
Java_com_yxy_helloworld_MainActivity_hookThread(JNIEnv *env, jclass clazz) {
    LOGD(TAG, "hookThread 6666  ");
    hook();

}
