//
//  LRUCacheImpl.c
//  LRU
//
//  Created by 宋珂琦 on 2017/4/13.
//  Copyright © 2017年 宋珂琦. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "LRUCache.h"
#include "LRUCacheImpl.h"

static void freeList(LRUCacheS *cache);

/*****************************
 * LRU缓存及缓存单元相关接口及实现
******************************/
/* 创建一个缓存单元 */
static cacheEntryS* newCacheEntry(char key, char data)
{
    cacheEntryS* entry = NULL;
    if (NULL == (entry=malloc(sizeof(*entry)))) {
        perror("malloc");
        return NULL;
    }
    memset(entry, 0, sizeof(*entry));
    entry->key = key;
    entry->data = data;
    return entry;
}

/* 释放一个缓存单元 */
static void freeCacheEntry(cacheEntryS* entry)
{
    if (NULL == entry) return;
    free(entry);
}

/* 创建一个LRU缓存 */
int LRUCacheCreate(int capacity, void **lruCache)
{
    LRUCacheS* cache = NULL;
    if (NULL == (cache=malloc(sizeof(*cache)))) {
        perror("molloc");
        return -1;
    }
    memset(cache, 0, sizeof(*cache));
    cache->cacheCapacity = capacity;
    cache->hashMap = malloc(sizeof(cacheEntryS)*capacity);
    if (NULL == cache->hashMap) {
        free(cache);
        perror("molloc");
        return -1;
    }
    memset(cache->hashMap, 0, sizeof(cacheEntryS)*capacity);
    *lruCache = cache;
    return 0;
}

/* 释放一个LRU缓存 */
int LRUCacheDestory(void *lruCache)
{
    LRUCacheS* cache = (LRUCacheS*)lruCache;
    if (NULL == cache) return 0;
    //free hashMap
    if (cache->hashMap) {
        free(cache->hashMap);
    }
    //free linklist
    freeList(cache);
    free(cache);
    return 0;
}

/**********************************
* 双向链表相关接口及实现
***********************************/
