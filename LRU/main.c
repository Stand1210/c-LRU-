#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lru_cache.h"
#include "lru_cache_impl.h"


int main()
{
    void *LruCache;
    LRUCacheCreate(100, &LruCache); 
    LRUCacheSet(LruCache, "k1", "v1");
    LRUCacheSet(LruCache, "k1", "v2");
    LRUCacheSet(LruCache, "k1", "v2");
    LRUCacheSet(LruCache, "k1", "v3");
    LRUCacheSet(LruCache, "k2", "v8\r\nfdfdfdfdfddfdfd\r\n");

    LRUCachePrint(LruCache);
    char result[100];
    printf("%s",LRUCacheGet(LruCache, "k2"));
    // printf("%s\n",result);
    printf("%d",strlen(LRUCacheGet(LruCache,"k1")));
    puts("\n\n\n");
    printf("%d",NULL==LRUCacheGet(LruCache,"kkkkkkkkk"));
    return 0;
}
