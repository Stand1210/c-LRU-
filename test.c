
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

#include "./LRU/lru_cache.h"
#include "./LRU/lru_cache_impl.h"

/*错误处理宏*/
#define HANDLE_ERROR(msg) \
        do{ fprintf(stderr, "%s fail.\n", msg);exit(-1);}while(0);

/* 封装缓存数据存储接口， 此处我们让data同时充当key的角色 */
#define LRUCACHE_PUTDATA(cache, data) \
do {\
    if (0 != LRUCacheSet(cache, data, data)) {\
        fprintf(stderr, "put (%s %s) to cache fail. \n", data, data); \
    }else {\
        fprintf(stdout, "put (%s %s) to cache success.\n", data, data); \
    } \
} while (0);

/* 封装缓存数据存储接口 */
#define LRUCACHE_GETDATA(cache, key) \
do {\
    if (NULL == LRUCacheGet(cache,key)) \
        fprintf(stderr, "get data (Key: %s) from cache fail.\n", key);\
    else{\
        char data[VALUE_SIZE];\
        strncpy(data,LRUCacheGet(cache, key),VALUE_SIZE);\
        if(!strncmp(data,key,KEY_SIZE))\
            fprintf(stdout, "get (%s, %s) from cache.\n", key, data);\
    } \
} while (0);


void *testCase1(void *varpg)
{
    pthread_detach(pthread_self());
    fprintf(stdout, "=================================\n");
    fprintf(stdout, "In testCase 1:\n");
    void *LruCache;
    if (0 != LRUCacheCreate(5, &LruCache)) {
        HANDLE_ERROR("LRUCacheCreate");
    }
    
    LRUCACHE_PUTDATA(LruCache, "AA");
    LRUCACHE_GETDATA(LruCache, "AA");
    
    LRUCACHE_PUTDATA(LruCache, "BB");
    LRUCACHE_GETDATA(LruCache, "BB");
    
    LRUCACHE_PUTDATA(LruCache, "CC");
    LRUCACHE_GETDATA(LruCache, "CC");
    LRUCachePrint(LruCache);
    
    LRUCACHE_PUTDATA(LruCache, "DD");
    LRUCACHE_GETDATA(LruCache, "DD");
    
    LRUCACHE_PUTDATA(LruCache, "EE");
    LRUCACHE_GETDATA(LruCache, "EE");
    
    LRUCACHE_PUTDATA(LruCache, "AA");
    LRUCACHE_GETDATA(LruCache, "AA");
    
    LRUCACHE_PUTDATA(LruCache, "FF");
    LRUCACHE_GETDATA(LruCache, "FF");
    
    LRUCachePrint(LruCache);
    
    LRUCACHE_PUTDATA(LruCache, "FF");
    LRUCACHE_GETDATA(LruCache, "FF");
    
    LRUCachePrint(LruCache);
    
    if (0 != LRUCacheDestory(LruCache))
        HANDLE_ERROR("LRUCacheDestory");
    
    fprintf(stdout, "In testCase 1 finished\n");
    fprintf(stdout, "=================================\n");
}

void *testCase2(void *varpg)
{
    pthread_detach(pthread_self());
    fprintf(stdout, "=================================\n");
    fprintf(stdout, "In testCase 2:\n");
    void *LruCache;
    if (0 != LRUCacheCreate(3, &LruCache)) {
        HANDLE_ERROR("LRUCacheCreate");
    }
    LRUCACHE_PUTDATA(LruCache, "WW");
    LRUCACHE_PUTDATA(LruCache, "XX");
    LRUCACHE_PUTDATA(LruCache, "WW");
    LRUCACHE_PUTDATA(LruCache, "YY");
    LRUCACHE_PUTDATA(LruCache, "ZZ");
    LRUCachePrint(LruCache);
    
    LRUCACHE_GETDATA(LruCache, "ZZ");
    LRUCACHE_GETDATA(LruCache, "YY");
    LRUCACHE_GETDATA(LruCache, "WW");
    LRUCACHE_GETDATA(LruCache, "XX");
    LRUCACHE_GETDATA(LruCache, "WW");
    LRUCachePrint(LruCache);
    
    /*YZWYX!*/
    LRUCACHE_PUTDATA(LruCache, "YY");
    LRUCACHE_PUTDATA(LruCache, "ZZ");
    LRUCACHE_PUTDATA(LruCache, "WW");
    LRUCACHE_PUTDATA(LruCache, "YY");
    LRUCACHE_PUTDATA(LruCache, "XX");
    LRUCachePrint(LruCache);
    
    
    LRUCACHE_GETDATA(LruCache, "XX");
    LRUCACHE_GETDATA(LruCache, "YY");
    LRUCACHE_GETDATA(LruCache, "WW");
    LRUCACHE_GETDATA(LruCache, "ZZ");
    LRUCACHE_GETDATA(LruCache, "YY");
    LRUCachePrint(LruCache); /*WYX*/
    
    /*XYXY!*/
    LRUCACHE_PUTDATA(LruCache, "XX");
    LRUCACHE_PUTDATA(LruCache, "YY");
    LRUCACHE_PUTDATA(LruCache, "XX");
    LRUCACHE_PUTDATA(LruCache, "YY");
    LRUCachePrint(LruCache);/*YX*/
    
    LRUCACHE_GETDATA(LruCache, "YY");
    LRUCACHE_GETDATA(LruCache, "XX");
    LRUCACHE_GETDATA(LruCache, "YY");
    LRUCACHE_GETDATA(LruCache, "XX");
    LRUCachePrint(LruCache); /*XY*/
    
    fprintf(stdout, "In testCase 2 finished\n");
    fprintf(stdout, "=================================\n");
}

void *testCase3(void *varpg)
{
    pthread_detach(pthread_self());
    fprintf(stdout, "=================================\n");
    fprintf(stdout, "In testCase 3:\n");
    void *LruCache;
    if (0 != LRUCacheCreate(5, &LruCache)) {
        HANDLE_ERROR("LRUCacheCreate");
    }
    LRUCACHE_PUTDATA(LruCache, "EE");
    LRUCACHE_PUTDATA(LruCache, "II");
    LRUCACHE_PUTDATA(LruCache, "EE");
    LRUCACHE_PUTDATA(LruCache, "II");
    LRUCACHE_PUTDATA(LruCache, "OO");
    LRUCachePrint(LruCache);
    
    LRUCACHE_GETDATA(LruCache, "AA");
    LRUCACHE_GETDATA(LruCache, "II");
    LRUCACHE_GETDATA(LruCache, "BB");
    LRUCACHE_GETDATA(LruCache, "OO");
    LRUCACHE_GETDATA(LruCache, "CC");
    LRUCACHE_PUTDATA(LruCache, "EE");
    LRUCachePrint(LruCache);
    
    fprintf(stdout, "In testCase 3 finished\n");
    fprintf(stdout, "=================================\n");
}

int main(int argc, char **argv)
{
    int flag;
    printf("选择测试模式:\n1---->多线程模式\n2--->单线程模式\n你的选择:");
    scanf("%d",&flag);
    if(flag!=1 && flag!=2){
        printf("输入错误，程序退出");
        return 0;
    }
    if(flag==1){
        pthread_t t1,t2,t3;
        pthread_create(&t1,NULL,testCase1,NULL);
        pthread_create(&t2,NULL,testCase2,NULL);
        pthread_create(&t3,NULL,testCase3,NULL);
        pthread_exit(NULL);
    }else if(flag==2){
        testCase1(NULL);
        testCase2(NULL);
        testCase3(NULL);
    }
    return 0;
}