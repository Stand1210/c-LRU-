#include <stdio.h>
#include <string.h>

#include "./LRU/lru_cache.h"
#include "./LRU/lru_cache_impl.h"

int main(int argc, char **argv){
    void *LruCache;

    //创建缓存器
    if (0 == LRUCacheCreate(3, &LruCache))
        printf("缓存器创建成功，容量为3\n");

    //向缓存器中添加数据
    if (0 != LRUCacheSet(LruCache, "key1", "value1"))
        printf("put (key1, value1) failed!\n");

    if (0 != LRUCacheSet(LruCache, "key2", "value2"))
        printf("put (key2, value2) failed!\n");

    if (0 != LRUCacheSet(LruCache, "key3", "value3"))
        printf("put (key3, value3) failed!\n");

    if (0 != LRUCacheSet(LruCache, "key4", "value4"))
        printf("put (key4, value4) failed!\n");

    if (0 != LRUCacheSet(LruCache, "key5", "value5"))
        printf("put (key5, value5) failed!\n");

    //按照顺序输出缓存器当前的内容
    LRUCachePrint(LruCache);

    //获取缓存器内容
    if (NULL == LRUCacheGet(LruCache, "key1")){
        printf("key1 所对应的数据未被缓存\n");
    }
    else{
        char data[VALUE_SIZE];
        strncpy(data, LRUCacheGet(LruCache, "key1"), VALUE_SIZE);
        printf("key1所对应的数据为:%s", data);
    }

    char value4[VALUE_SIZE];
    strncpy(value4, LRUCacheGet(LruCache, "key4"), VALUE_SIZE);
    printf("key4所对应的数据:%s", value4);

    LRUCachePrint(LruCache);

    //销毁缓存器
    if (0 != LRUCacheDestory(LruCache))
        printf("destory error");
}