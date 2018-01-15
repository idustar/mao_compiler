/************************************************* 
作者：杜佳豪 (1552652)
单位：同济大学软件工程 2015级 1班 
日期：2015/12/13
描述：以读取并执行Mao语言源代码，结果在屏幕上输出。 

memory.c - 链表操作，包括变量的建立、赋值、取值。 
**************************************************/  

#include "memory.h"

extern struct Var *g_head, *g_tail;  //链表指针头、尾 

double get_value(int hash, int *type_of_var) { 
    //由变量名的hash值遍历搜索，返回变量值及类型 
    struct Var *p; //临时指针变量，用于遍历搜索 
    for (p = g_head; p != NULL; p = p->next) {  
        if (p->hash == hash) {
		    (*type_of_var) = p->type_of_var;
            return p->value; 
        }
    }
    return_error(8); 
}

int insert_var(int hash, double value, int type_of_var) { 
    //向链表尾部加入新的变量，储存其变量名hash值、变量值、类型
    struct Var *p; //临时指针变量，将加入链表的尾部
    for (p = g_head; p != NULL; p = p->next) 
        if (p->hash == hash) return_error(7);
    p = (struct Var*)malloc(sizeof(struct Var)); //开辟新的内存单元 
    p->hash = hash; 
    p->value = value;
    p->type_of_var = type_of_var;
    p->next = NULL;
    if (g_head == NULL)  //对于第一个建立的变量 
        g_head = g_tail = p;
    else  //对于其他变量 
        g_tail = g_tail->next = p;
}

int change_value(int hash, double value, int *type_of_var) //改变变量的值
{
    struct Var *p; //临时指针变量，用于遍历搜索 
    for (p = g_head; p != NULL; p = p->next) {
        if (p->hash == hash) {
        	if (p->type_of_var == TYPE_INT) value = (int)value;
        	    //若该变量类型为int，则取整放入链表。 
            p->value = value;
            *type_of_var = p->type_of_var;
            return 0; 
        }
    }
    return_error(8); 
}   
