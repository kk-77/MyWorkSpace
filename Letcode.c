#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
/*Letcode：两数相加*/
struct ListNode
{
    int val;
    struct ListNode *next;
};

int get_List_num(struct ListNode *list)
{
    int temp = 0;
    int m = 1;

    while (list != NULL)
    {
        temp = list->val * m + temp;
        m *= 10;
        list = list->next;
    }

    return temp;
}

struct ListNode *CreatList(int n)
{ //creat n Node
    struct ListNode *head = NULL;
    struct ListNode *p;
    for (int i = 0; i < n; i++)
    {
        p = (struct ListNode *)malloc(sizeof(struct ListNode));
        scanf("%d", &p->val);
        p->next = head;
        head = p;
    }
    return head;
}

struct ListNode *DeleteList(struct ListNode *head)
{
    struct ListNode *p, *q;
    p = head;
    while (p)
    {
        q = p->next;
        free(p);
        p = q;
    }

    return NULL;
}

int main(int argc, char const *argv[])
{
    // int i = 1, l1_num,l2_num,l3_num;
    // struct ListNode *list1 = CreatList(3);
    // struct ListNode *list2 = CreatList(3);
    
    // l1_num = get_List_num(list1);
    // l2_num = get_List_num(list2);
    // printf("l1_num = %d  l2_num = %d\r\n", l1_num, l2_num);
    // l3_num = l1_num + l2_num;


    // list1 = DeleteList(list1);
    // list2 = DeleteList(list2);
    return 0;
}
