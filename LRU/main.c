//
//  main.c
//  LRU 缓存测试用例
//
//  Created by 宋珂琦 on 2017/4/13.
//  Copyright © 2017年 宋珂琦. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include "LRUCache.h"

/*错误处理宏*/
#define HANDLE_ERROR(msg) \
        do{ fprintf(stderr, "%s fail.\n", msg);exit(-1);}while(0);

/* 封装缓存数据存储接口， 此处我们让data同时充当key的角色 */
#define LRUCACHE_PUTDATA(cache, data) \
do {\
    if (0 != LRUCacheSet(cache, data, data)) {\
        fprintf(stderr, "put (%c %c) to cache fail. \n", data, data); \
    }else {\
        fprintf(stdout, "put (%c %c) to cache success.\n", data, data); \
    } \
} while (0);

/* 封装缓存数据存储接口 */
#define LRUCACHE_GETDATA(cache, key) \
do {\
    char data = LRUCacheGet(cache, key);\
    if ('\0' == data) \
        fprintf(stderr, "get data (Key: %c) from cache fail.\n", key);\
    else if (key == data) \
        fprintf(stdout, "get (%c, %c) from cache.\n", key, data);\
} while (0);


void testCase1(void)
{
    fprintf(stdout, "=================================\n");
    fprintf(stdout, "In testCase 1:\n");
    void *LruCache;
    if (0 != LRUCacheCreate(5, &LruCache)) {
        HANDLE_ERROR("LRUCacheCreate");
    }
    
    LRUCACHE_PUTDATA(LruCache, 'A');
    LRUCACHE_GETDATA(LruCache, 'A');
    
    LRUCACHE_PUTDATA(LruCache, 'B');
    LRUCACHE_GETDATA(LruCache, 'B');
    
    LRUCACHE_PUTDATA(LruCache, 'C');
    LRUCACHE_GETDATA(LruCache, 'C');
    LRUCachePrint(LruCache);
    
    LRUCACHE_PUTDATA(LruCache, 'D');
    LRUCACHE_GETDATA(LruCache, 'D');
    
    LRUCACHE_PUTDATA(LruCache, 'E');
    LRUCACHE_GETDATA(LruCache, 'E');
    
    LRUCACHE_PUTDATA(LruCache, 'A');
    LRUCACHE_GETDATA(LruCache, 'A');
    
    LRUCACHE_PUTDATA(LruCache, 'F');
    LRUCACHE_GETDATA(LruCache, 'F');
    
    LRUCachePrint(LruCache);
    
    LRUCACHE_PUTDATA(LruCache, 'F');
    LRUCACHE_GETDATA(LruCache, 'F');
    
    LRUCachePrint(LruCache);
    
    if (0 != LRUCacheDestory(LruCache))
        HANDLE_ERROR("LRUCacheDestory");
    
    fprintf(stdout, "In testCase 1 finished\n");
    fprintf(stdout, "=================================\n");
}

void testCase2()
{
    fprintf(stdout, "=================================\n");
    fprintf(stdout, "In testCase 2:\n");
    void *LruCache;
    if (0 != LRUCacheCreate(3, &LruCache)) {
        HANDLE_ERROR("LRUCacheCreate");
    }
    LRUCACHE_PUTDATA(LruCache, 'W');
    LRUCACHE_PUTDATA(LruCache, 'X');
    LRUCACHE_PUTDATA(LruCache, 'W');
    LRUCACHE_PUTDATA(LruCache, 'Y');
    LRUCACHE_PUTDATA(LruCache, 'Z');
    LRUCachePrint(LruCache);
    
    LRUCACHE_GETDATA(LruCache, 'Z');
    LRUCACHE_GETDATA(LruCache, 'Y');
    LRUCACHE_GETDATA(LruCache, 'W');
    LRUCACHE_GETDATA(LruCache, 'X');
    LRUCACHE_GETDATA(LruCache, 'W');
    LRUCachePrint(LruCache);
    
    /*YZWYX!*/
    LRUCACHE_PUTDATA(LruCache, 'Y');
    LRUCACHE_PUTDATA(LruCache, 'Z');
    LRUCACHE_PUTDATA(LruCache, 'W');
    LRUCACHE_PUTDATA(LruCache, 'Y');
    LRUCACHE_PUTDATA(LruCache, 'X');
    LRUCachePrint(LruCache);
    
    
    LRUCACHE_GETDATA(LruCache, 'X');
    LRUCACHE_GETDATA(LruCache, 'Y');
    LRUCACHE_GETDATA(LruCache, 'W');
    LRUCACHE_GETDATA(LruCache, 'Z');
    LRUCACHE_GETDATA(LruCache, 'Y');
    LRUCachePrint(LruCache); /*WYX*/
    
    /*XYXY!*/
    LRUCACHE_PUTDATA(LruCache, 'X');
    LRUCACHE_PUTDATA(LruCache, 'Y');
    LRUCACHE_PUTDATA(LruCache, 'X');
    LRUCACHE_PUTDATA(LruCache, 'Y');
    LRUCachePrint(LruCache);/*YX*/
    
    LRUCACHE_GETDATA(LruCache, 'Y');
    LRUCACHE_GETDATA(LruCache, 'X');
    LRUCACHE_GETDATA(LruCache, 'Y');
    LRUCACHE_GETDATA(LruCache, 'X');
    LRUCachePrint(LruCache); /*XY*/
    
    fprintf(stdout, "In testCase 2 finished\n");
    fprintf(stdout, "=================================\n");
}

void testCase3()
{
    fprintf(stdout, "=================================\n");
    fprintf(stdout, "In testCase 3:\n");
    void *LruCache;
    if (0 != LRUCacheCreate(5, &LruCache)) {
        HANDLE_ERROR("LRUCacheCreate");
    }
    LRUCACHE_PUTDATA(LruCache, 'E');
    LRUCACHE_PUTDATA(LruCache, 'I');
    LRUCACHE_PUTDATA(LruCache, 'E');
    LRUCACHE_PUTDATA(LruCache, 'I');
    LRUCACHE_PUTDATA(LruCache, 'O');
    LRUCachePrint(LruCache);
    
    LRUCACHE_GETDATA(LruCache, 'A');
    LRUCACHE_GETDATA(LruCache, 'I');
    LRUCACHE_GETDATA(LruCache, 'B');
    LRUCACHE_GETDATA(LruCache, 'O');
    LRUCACHE_GETDATA(LruCache, 'C');
    LRUCACHE_PUTDATA(LruCache, 'E');
    LRUCachePrint(LruCache);
    
    fprintf(stdout, "In testCase 3 finished\n");
    fprintf(stdout, "=================================\n");
}
int main()
{
    
    testCase1();
    testCase2();
    testCase3();
    return 0;
}
