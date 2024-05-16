/*
 *csim.c-使用C编写一个Cache模拟器，它可以处理来自Valgrind的跟踪和输出统计
 *息，如命中、未命中和逐出的次数。更换政策是LRU。
 * 设计和假设:
 *  1. 每个加载/存储最多可导致一个缓存未命中。（最大请求是8个字节。）
 *  2. 忽略指令负载（I），因为我们有兴趣评估trace.c内容中数据存储性能。
 *  3. 数据修改（M）被视为加载，然后存储到同一地址。因此，M操作可能导致两次缓存命中，或者一次未命中和一次命中，外加一次可能的逐出。
 * 使用函数printSummary() 打印输出，输出hits, misses and evictions 的数，这对结果评估很重要
*/

#include "cachelab.h"
//                    请在此处添加代码  
//****************************Begin*********************
#include <getopt.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>
#include <limits.h>
#include <string.h>
#include <errno.h>

#define MAX_PATH_LENGTH (256)
#define MAX_BUFFER_SIZE (1024)

int show_details = 0;
int index_bits = -1 ,offset_bits = -1;
int group_n, group_line = -1, block_size;
char path[MAX_PATH_LENGTH]; 

void show_help(){
    printf("command should be : csim -s <bits of index> -E <lines of group> -b <bits of offset> -t <tracefile> [-hv]\n");
}

typedef unsigned long long uint64;

typedef struct cache_line
{
    uint64 tag;
    int lru;
    int v;
} cache_line_t;

int timestamp = 0;
int hits, misses, evicts;


cache_line_t* cache;

void init_cache(){
    cache = malloc(group_n * group_line * sizeof(cache_line_t));
}

uint64 get_cache_group(uint64 addr){
    return (addr >> offset_bits) & (group_n - 1);  
}

uint64 get_tag(uint64 addr){
    return (addr >> offset_bits) >> (index_bits);  
}

uint64 find_cache_line(uint64 tag, uint64 group){
    int res = 0, min_timestamp = timestamp + 1;
    for(uint64 index = group * group_line; 
        index < (group + 1) * group_line;
        index++){
        if(cache[index].tag == tag){
            res = index;
            min_timestamp = -1;
        }else if(cache[index].lru < min_timestamp){
            res = index;
            min_timestamp = cache[index].lru;
        }
    }
    if(cache[res].v == 0){
        cache[res].v = 1;
        misses += 1;
    } else if(cache[res].tag == tag){
        hits += 1;
    } else {
        misses += 1;
        evicts += 1;
    }
    cache[res].tag = tag;
    return res;
}

int trace_file(const char* path){
    char buf[MAX_BUFFER_SIZE]; 
    FILE* trace_file = fopen(path, "r");
    while( fgets(buf, MAX_BUFFER_SIZE, trace_file) ){
        char ins;
        uint64 addr; 
        int size;
        sscanf(buf, "%c %llx,%d", &ins, &addr, &size);
        if(ins == 'I') continue;
        find_cache_line(get_tag(addr), get_cache_group(addr));
        if(ins == 'M') find_cache_line(get_tag(addr), get_cache_group(addr));
    }

    return 0;
}

int main(int argc, char** argv)
{  
    int opt = 0, retn = 0;
    while((opt = getopt(argc, argv, "s:E:b:t:hv") )!= -1){
        switch (opt)
        {
        case 's':
            index_bits = atoi(optarg);
            break;
        case 'E':
            group_line = atoi(optarg);
            break;
        case 'b':
            offset_bits = atoi(optarg);
            break;
        case 't':
            strcpy(path, optarg);
            break;
        case 'v':
            show_details = 1;
            break;
        case 'h':
            show_help();
            goto ret;
        default:
            show_help();
            retn = 1;
            goto ret;
        }
    }
    if (index_bits == -1 || group_line == -1 || offset_bits == -1) {
        show_help();
        retn = 1;
        goto ret; 
    }else {
        group_n = 1 << index_bits;
        block_size = 1 << offset_bits;
    }
    init_cache();

    if((retn = trace_file(path)) != 0 ) goto ret;
     
    
    printSummary(hits, misses, evicts); //输出hit、miss和evictions数量统计 




ret:
    return retn;

}
//****************************End**********************#