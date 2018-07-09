

#ifndef LRUCACHEIMPL_H
#define LRUCACHEIMPL_H


#include <semaphore.h>

#define KEY_SIZE 50
#define VALUE_SIZE 100

/*定义LRU缓存的缓存单元*/
typedef struct cacheEntryS{
    char key[KEY_SIZE];   /* 数据的key */
    char data[VALUE_SIZE];  /* 数据的data */
    
    sem_t entry_lock;

    struct cacheEntryS *hashListPrev;   /* 缓存哈希表指针， 指向哈希链表的前一个元素 */
    struct cacheEntryS *hashListNext;   /* 缓存哈希表指针， 指向哈希链表的后一个元素 */
    
    struct cacheEntryS *lruListPrev;    /* 缓存双向链表指针， 指向链表的前一个元素 */
    struct cacheEntryS *lruListNext;    /* 缓存双向链表指针， 指向链表后一个元素 */
}cacheEntryS;


/* 定义LRU缓存 */
typedef struct LRUCacheS{
    sem_t cache_lock;
    int cacheCapacity;  /*缓存的容量*/
    int lruListSize;    /*缓存的双向链表节点个数*/
    
    cacheEntryS **hashMap;  /*缓存的哈希表*/

    cacheEntryS *lruListHead;   /*缓存的双向链表表头*/
    cacheEntryS *lruListTail;   /*缓存的双向链表表尾*/
}LRUCacheS;


#endif 
