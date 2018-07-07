#ifndef LRUCACHE_H
#define LRUCACHE_H

/*创建LRU缓存*/
int LRUCacheCreate(int capacity, void **lruCache);
/*销毁LRU缓存*/
int LRUCacheDestory(void *lruCache);
/*将数据放入LRU缓存中*/
int LRUCacheSet(void *lruCache, char *key, char *data);
/*从缓存中获取数据*/
char* LRUCacheGet(void *lruCache, char *key);
/*打印缓存中的数据， 按访问时间从新到旧的顺序输出*/
void LRUCachePrint(void *lruCache);


#endif 
