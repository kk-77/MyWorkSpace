#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*实现Hash简单功能*/

#define BUCKETCOUNT 16 /*桶数量*/

/*Hash条目*/
struct hashEntry
{
    const char *key;
    char *value;
    struct hashEntry *next;
};

typedef struct hashEntry entry;

struct hashTable
{
    entry bucket[BUCKETCOUNT]; //default define 16 bucket
};

typedef struct hashTable table;
/*初始化哈希表*/
void initHashTable(table *t)
{

    if (t == NULL)
    {
        return;
    }

    for (int i = 0; i < BUCKETCOUNT; i++)
    {
        t->bucket[i].key = NULL;
        t->bucket[i].next = NULL;
        t->bucket[i].value = NULL;
    }
}
/*释放哈希表*/
void freeHashTable(table *t)
{

    entry *e, *ep;
    if (t == NULL)
    {
        return;
    }
    for (int i = 0; i < BUCKETCOUNT; i++)
    {
        e = &(t->bucket[i]);
        while (e->next != NULL)
        {
            ep = e->next;
            e->next = ep->next;
            //free(ep->key);
            free(ep->value);
            free(ep);
        }
    }
}
/*哈希散列算法*/
int keyToIndex(const char *key)
{
    int index, len, i;
    if (key == NULL)
        return -1;

    len = strlen(key);
    index = (int)key[0];
    for (i = 1; i < len; ++i)
    {
        index *= 1103515245 + (int)key[i];
    }
    index >>= 27;
    index &= (BUCKETCOUNT - 1); // 最后索引的范围为0-15
    return index;
}
/*向哈希表中插入数据*/
int insertEntry(table *t, const char *key, const char *value)
{
    entry *e, *ep;

    if (t == NULL || key == NULL || value == NULL)
    {
        return -1;
    }

    int index = keyToIndex(key);
    if (t->bucket[index].key == NULL)
    { /*bucket no key and add new key*/
        t->bucket[index].key = strdup(key);
        t->bucket[index].value = strdup(value);
    }
    else
    {
        e = &(t->bucket[index]); //save the entry that will add to the bucket
        while (e != NULL)
        {
            if (strcmp(e->key, key) == 0)
            {                                //if the key is same as the original key
                int len1 = strlen(value);    //will add key value length
                int len2 = strlen(e->value); //original value length
                if (len1 > len2)
                {
                    free(e->value);
                    e->value = (char *)malloc(len1 + 1);
                }
                memcpy(e->value, value, len1 + 1);
                return index;
            }
            ep = e; //if key != original key find next entry
            e = e->next;
        }
        //e == NULL need to malloc
        e = (entry *)malloc(sizeof(entry));
        e->key = strdup(key);
        e->value = strdup(value);
        e->next = NULL;
        ep->next = e; //let the entry point to the new entry
    }
    return index;
}
//找到了返回value的地址，没找到返回NULL
const char *findValueByKey(const table *t, const char *key)
{
    int index = keyToIndex(key);
    entry *e;

    if (t == NULL || key == NULL)
    {
        return NULL;
    }

    e = &(t->bucket[index]);
    if (e == NULL)
        return NULL;
    while (e != NULL)
    {
        if (strcmp(e->key, key) == 0)
        {
            return e->value;
        }
        e = e->next;
    }
    return NULL;
}
//在哈希表中查找key对应的entry
//找到了返回entry，并将其从哈希表中移除
//没找到返回NULL
entry *removeEntry(table *t, char *key)
{
    int index = keyToIndex(key);
    if (t == NULL || key == NULL)
        return NULL;
    entry *e, *ep;

    e = &(t->bucket[index]);
    while (e != NULL)
    {
        if (strcmp(e->key, key) == 0)
        {
            if (e == &(t->bucket[index]))
            {
                ep = e->next;
                if (ep != NULL)
                {
                    entry tmp = *e; //做浅拷贝交换
                    *e = *ep;       //相当于链表的头节点已经移除
                    *ep = tmp;      //这就是移除下来的链表头节点
                    ep->next = NULL;
                }
                else
                {
                    ep = (entry *)malloc(sizeof(entry));
                    *ep = *e;
                    e->key = e->value = NULL;
                    e->next = NULL; //ep save the entry that you want remove
                }
            }
            else
            { /*exec here you must exec the e= e->next;*/
                ep = &(t->bucket[index]);
                while (ep->next != e)
                {
                    ep = ep->next;
                }
                ep->next = e->next;
                e->next = NULL;
                ep = e;
            }
            return ep;
        }
        e = e->next;
    }

    return NULL;
}

void printTable(table *t)
{
    int i;
    entry *e;
    if (t == NULL)
        return;
    for (i = 0; i < BUCKETCOUNT; ++i)
    {
        printf("\nbucket[%d]:\n", i);
        e = &(t->bucket[i]);
        while (e->key != NULL)
        {
            printf("\t%s\t=\t%s\n", e->key, e->value);
            if (e->next == NULL)
                break;
            e = e->next;
        }
    }
}

int main(int argc, char const *argv[])
{
    table t;
    initHashTable(&t);

    insertEntry(&t, "电脑型号", "华硕 X550JK 笔记本电脑");
    insertEntry(&t, "操作系统", "Windows 8.1 64位 (DirectX 11)");
    insertEntry(&t, "处理器", "英特尔 Core i7 - 4710HQ @ 2.50GHz 四核");
    insertEntry(&t, "主板", "华硕 X550JK(英特尔 Haswell)");
    insertEntry(&t, "内存", "4 GB(Hynix / Hyundai)");
    insertEntry(&t, "主硬盘", "日立 HGST HTS541010A9E680(1 TB / 5400 转 / 分)");
    insertEntry(&t, "显卡", "NVIDIA GeForce GTX 850M       (2 GB / 华硕)");
    insertEntry(&t, "显示器", "奇美 CMN15C4(15.3 英寸)");
    insertEntry(&t, "光驱", "松下 DVD - RAM UJ8E2 S DVD刻录机");
    insertEntry(&t, "声卡", "Conexant SmartAudio HD @ 英特尔 Lynx Point 高保真音频");
    insertEntry(&t, "网卡", "瑞昱 RTL8168 / 8111 / 8112 Gigabit Ethernet Controller / 华硕");
    insertEntry(&t, "主板型号", "华硕 X550JK");
    insertEntry(&t, "芯片组", "英特尔 Haswell");
    insertEntry(&t, "BIOS", "X550JK.301");
    insertEntry(&t, "制造日期", "06 / 26 / 2014");
    insertEntry(&t, "主人", "就是我");
    insertEntry(&t, "价格", "六十张红色毛主席");
    insertEntry(&t, "主硬盘", "换了个120G的固态");

    entry *e = removeEntry(&t, "BIOS");
    if (e != NULL)
    {
        puts("找到后要释放");
        //free(e->key);
        free(e->value);
        free(e);
        e = NULL;
    }

    printTable(&t);
    const char *keys[] = {"显示器", "主人", "没有", "处理器"};
    for (int i = 0; i < 4; ++i)
    {
        const char *value = findValueByKey(&t, keys[i]);
        if (value != NULL)
        {
            printf("find %s\t=\t%s\n", keys[i], value);
        }
        else
        {
            printf("not found %s\n", keys[i]);
        }
    } 

    freeHashTable(&t);
    getchar();
    return 0;
}
