test: test.c ./LRU/lru_cache.h ./LRU/lru_cache_impl.h ./LRU/lru_cache_impl.c 
	gcc -o test test.c ./LRU/lru_cache.h ./LRU/lru_cache_impl.h ./LRU/lru_cache_impl.c -lpthread 
	
example: example.c ./LRU/lru_cache.h ./LRU/lru_cache_impl.h ./LRU/lru_cache_impl.c 
	gcc -o example example.c ./LRU/lru_cache.h ./LRU/lru_cache_impl.h ./LRU/lru_cache_impl.c -lpthread 

clean:
	rm test example