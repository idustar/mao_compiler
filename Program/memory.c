/************************************************* 
���ߣ��żѺ� (1552652)
��λ��ͬ�ô�ѧ������� 2015�� 1�� 
���ڣ�2015/12/13
�������Զ�ȡ��ִ��Mao����Դ���룬�������Ļ������� 

memory.c - ������������������Ľ�������ֵ��ȡֵ�� 
**************************************************/  

#include "memory.h"

extern struct Var *g_head, *g_tail;  //����ָ��ͷ��β 

double get_value(int hash, int *type_of_var) { 
    //�ɱ�������hashֵ�������������ر���ֵ������ 
    struct Var *p; //��ʱָ����������ڱ������� 
    for (p = g_head; p != NULL; p = p->next) {  
        if (p->hash == hash) {
		    (*type_of_var) = p->type_of_var;
            return p->value; 
        }
    }
    return_error(8); 
}

int insert_var(int hash, double value, int type_of_var) { 
    //������β�������µı����������������hashֵ������ֵ������
    struct Var *p; //��ʱָ������������������β��
    for (p = g_head; p != NULL; p = p->next) 
        if (p->hash == hash) return_error(7);
    p = (struct Var*)malloc(sizeof(struct Var)); //�����µ��ڴ浥Ԫ 
    p->hash = hash; 
    p->value = value;
    p->type_of_var = type_of_var;
    p->next = NULL;
    if (g_head == NULL)  //���ڵ�һ�������ı��� 
        g_head = g_tail = p;
    else  //������������ 
        g_tail = g_tail->next = p;
}

int change_value(int hash, double value, int *type_of_var) //�ı������ֵ
{
    struct Var *p; //��ʱָ����������ڱ������� 
    for (p = g_head; p != NULL; p = p->next) {
        if (p->hash == hash) {
        	if (p->type_of_var == TYPE_INT) value = (int)value;
        	    //���ñ�������Ϊint����ȡ���������� 
            p->value = value;
            *type_of_var = p->type_of_var;
            return 0; 
        }
    }
    return_error(8); 
}   
