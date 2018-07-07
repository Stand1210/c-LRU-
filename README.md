## [LRU_Cache](https://github.com/Andrewpqc/LRU-Cache)
基于双向链表和哈希表实现的LRU进程内缓存包，C语言实现。改编自[https://github.com/Stand1210/c-LRU-](https://github.com/Stand1210/c-LRU-)

### Features
+ 测试全面
+ LRU淘汰算法
+ 双向链表和哈希表的存储模型
+ 基于键值对，键和值的数据类型为字符串
+ 对全局存储结构的写操作上锁，并发安全

### Usage
``` c
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
```
输出:
```
缓存器创建成功，容量为3

>>>>>>>>>>>>>>>
cache (key data):
(key5, value5)(key4, value4)(key3, value3)
<<<<<<<<<<<<<<<
key1 所对应的数据未被缓存
key4所对应的数据:value4
>>>>>>>>>>>>>>>
cache (key data):
(key4, value4)(key5, value5)(key3, value3)
<<<<<<<<<<<<<<<
```
### Last but not Least
本程序中的键和值在存储时所占用的字节数是写死的，不具有动态性，这是本程序的一个缺陷(怪就该C语言的字符串处理太麻烦了)。在应对键值长度与设定的长度不一致的场景下会出现浪费内存或者数据截断的问题。所以根据具体的场景合理的设定键和值所占用的字节数至关重要。设置方法:修改包中的`lru_cache_impl.h`文件中的`KEY_SIZE`、`VALUE_SIZE`两个宏即可，这两个宏初始对应的值分别是50,100.