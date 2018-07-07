#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lru_cache.h"
#include "lru_cache_impl.h"



static void freeList(LRUCacheS *cache);

/*****************************
 * LRU缓存及缓存单元相关接口及实现
******************************/
/* 创建一个缓存单元 */
static cacheEntryS* newCacheEntry(char *key, char *data)
{
    
    cacheEntryS* entry = NULL;
    if (NULL == (entry=malloc(sizeof(*entry)))) {
        perror("malloc");
        return NULL;
    }
    memset(entry, 0, sizeof(*entry));
    strncpy(entry->key,key,KEY_SIZE);
    strncpy(entry->data,data,VALUE_SIZE);
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
    cache->hashMap = malloc(sizeof(cacheEntryS*)*capacity);
    if (NULL == cache->hashMap) {
        free(cache);
        perror("molloc");
        return -1;
    }
    memset(cache->hashMap, 0, sizeof(cacheEntryS*)*capacity);
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
/* 从双向链表中删除指定节点 */
static void removeFromList(LRUCacheS *cache, cacheEntryS *entry)
{
    /*链表为空*/
    if (cache->lruListSize==0) {
        return;
    }
    
    if (entry==cache->lruListHead && entry==cache->lruListTail) {
        /* 链表中仅剩当前一个节点 */
        cache->lruListHead = cache->lruListTail = NULL;
    } else if (entry == cache->lruListHead) {
        /*欲删除节点位于表头*/
        cache->lruListHead = entry->lruListNext;
        cache->lruListHead->lruListPrev = NULL;
    } else if (entry == cache->lruListTail) {
        /*欲删除节点位于表尾*/
        cache->lruListTail = entry->lruListPrev;
        cache->lruListTail->lruListNext = NULL;
    } else {
        /*其他非表头表尾情况，直接摘抄节点*/
        entry->lruListPrev->lruListNext = entry->lruListNext;
        entry->lruListNext->lruListPrev = entry->lruListPrev;
    }
    /*删除成功， 链表节点数减1*/
    cache->lruListSize--;
    
}

/*将节点插入到链表表头*/
static cacheEntryS* insertToListHead(LRUCacheS *cache, cacheEntryS *entry)
{
    cacheEntryS *removedEntry = NULL;
    
    if (++cache->lruListSize > cache->cacheCapacity) {
        /*如果缓存满了， 即链表当前节点数已等于缓存容量， 
        那么需要先删除链表表尾节点， 即淘汰最久没有被访问到的缓存数据单元*/
        removedEntry = cache->lruListTail;
        removeFromList(cache, cache->lruListTail);
    }
    
    if (cache->lruListHead == NULL && cache->lruListTail == NULL) {
        /*如果当前链表为空链表*/
        cache->lruListHead = cache->lruListTail = entry;
    } else {
        /*当前链表非空， 插入表头*/
        entry->lruListNext = cache->lruListHead;
        entry->lruListPrev = NULL;
        cache->lruListHead->lruListPrev = entry;
        cache->lruListHead = entry;
    }
    
    return removedEntry;
}

/*释放整个链表*/
static void freeList(LRUCacheS* cache)
{
    /*链表为空*/
    if (0 == cache->lruListSize) {
        return;
    }
    
    cacheEntryS *entry = cache->lruListHead;
    /*遍历删除链表上所有节点*/
    while(entry) {
        cacheEntryS *temp = entry->lruListNext;
        freeCacheEntry(entry);
        entry = temp;
    }
    cache->lruListSize = 0;
}

/*辅助醒接口， 用于保证最近访问的节点总是位于链表表头*/
static void updateLRUList(LRUCacheS *cache, cacheEntryS *entry)
{
    /*将节点从链表中摘抄*/
    removeFromList(cache, entry);
    /*将节点插入链表表头*/
    insertToListHead(cache, entry);
}

/*********************************************************************************
* 哈希表相关接口实现
**********************************************************************************/

/*哈希函数*/
static unsigned int hashKey(LRUCacheS *cache,char* key)  
{  
    unsigned int len=strlen(key);
    unsigned int b    = 378551;  
    unsigned int a    = 63689;  
    unsigned int hash = 0;  
    unsigned int i    = 0;  
    for(i = 0; i < len; key++, i++)  
    {  
        hash = hash * a + (unsigned int)(*key);  
        a    = a * b;  
    }  
    return hash%(cache->cacheCapacity);  
}  

/*从哈希表获取缓存单元*/
static cacheEntryS *getValueFromHashMap(LRUCacheS *cache, char *key)
{
    /*1.使用哈希函数定位数据存放于哪个槽*/
    cacheEntryS *entry = cache->hashMap[hashKey(cache, key)];
    
    /*2.遍历查询槽内链表， 找到准确的数据项*/
    while (entry) {
        if (!strncmp(entry->key,key,KEY_SIZE)) {
            break;
        }
        entry = entry->hashListNext;
    }
    
    return entry;
}

/*向哈希表插入缓存单元*/
static void insertentryToHashMap(LRUCacheS *cache, cacheEntryS *entry)
{
    /*1.使用哈希函数定位数据存放在哪个槽*/
    cacheEntryS *n = cache->hashMap[hashKey(cache, entry->key)];
    if (n != NULL) {
        /*2.如果槽内已有其他数据项， 将槽内的数据项与当前欲加入数据项链成链表， 
        当前欲加入数据项为表头*/
        entry->hashListNext = n;
        n->hashListPrev = entry;
    }
    /*3.将数据项加入数据槽内*/
    cache->hashMap[hashKey(cache, entry->key)] = entry;
}

/*从哈希表删除缓存单元*/
static void removeEntryFromHashMap(LRUCacheS *cache, cacheEntryS *entry)
{
    /*无需做任何删除操作的情况*/
    if (NULL==entry || NULL==cache || NULL==cache->hashMap) {
        return ;
    }
    
    /*1.使用哈希函数定位数据位于哪个槽*/
    cacheEntryS *n = cache->hashMap[hashKey(cache, entry->key)];
    /*2.遍历槽内链表， 找到欲删除的节点， 将节点从哈希表删除*/
    while (n) {
        if (n->key == entry->key) { /*找到欲删除节点， 将节点从哈希表摘抄*/
            if (n->hashListPrev) {
                n->hashListPrev->hashListNext = n->hashListNext;
            } else {
                cache->hashMap[hashKey(cache, entry->key)] = n->hashListNext;
            }
            if (n->hashListNext) {
                n->hashListNext->hashListPrev = n->hashListPrev;
            }
            return ;
        }
        n = n->hashListNext;
    }
    
}

/*******************************************************************************
* 缓存存取接口
********************************************************************************/
/* 将数据放入LRU缓存中*/
int LRUCacheSet(void *lruCache, char *key, char *data)
{

    LRUCacheS *cache = (LRUCacheS *)lruCache;
    /*从哈希表中查找数据是否已经存在缓存中*/
    cacheEntryS *entry = getValueFromHashMap(cache, key);
    if (entry != NULL) {    /*数据已经在缓存中*/
        /*更新数据， 将该数据更新到链表表头*/
        strncpy(entry->data, data,VALUE_SIZE);
        updateLRUList(cache, entry);
    } else {
        /*数据没在缓存中*/
        /*新建缓存单元*/
        entry = newCacheEntry(key, data);
        
        /*将新建缓存单元插入链表表头*/
        cacheEntryS *removedEntry = insertToListHead(cache, entry);
        if (NULL != removedEntry) {
            /*将新建缓存单元插入链表表头过程中， 发生缓存满了的情况， 
            需要淘汰最久没有被访问到的缓存数据单元*/
            removeEntryFromHashMap(cache, removedEntry);
            freeCacheEntry(removedEntry);
        }
        /*将新建缓存单元插入哈希表*/
        insertentryToHashMap(cache, entry);
    }
    return 0;
}

/*从缓存中获取数据*/
char *LRUCacheGet(void *lruCache, char *key)
{
    LRUCacheS *cache = (LRUCacheS *)lruCache;
    /*从哈希表查找数据是否已经在缓存中*/
    cacheEntryS* entry = getValueFromHashMap(cache, key);
    if (NULL != entry) {
        /*缓存中存在该数据， 更新至链表表头*/
        updateLRUList(cache, entry);
        /*返回数据*/
        return entry->data;
    } else {
        /*缓存中不存在相关数据*/
        return NULL;
    }
}

/**************************************************
* 调试接口
***************************************************/
/*遍历缓存列表， 打印缓存中的数据， 按访问时间从新到旧的顺序输出*/
void LRUCachePrint(void *lruCache)
{
    LRUCacheS *cache = (LRUCacheS *)lruCache;
    if (NULL == cache || 0 == cache->lruListSize) {
        return ;
    }
    fprintf(stdout, "\n>>>>>>>>>>>>>>>\n");
    fprintf(stdout, "cache (key data):\n");
    cacheEntryS* entry = cache->lruListHead;
    while (entry) {
        fprintf(stdout, "(%s, %s)", entry->key, entry->data);
        entry = entry->lruListNext;
    }
    fprintf(stdout, "\n<<<<<<<<<<<<<<<\n");
}
