/************************************************* 
���ߣ��żѺ� (1552652)
��λ��ͬ�ô�ѧ������� 2015�� 1�� 
���ڣ�2015/12/13
�������Զ�ȡ��ִ��Mao����Դ���룬�������Ļ������� 
**************************************************/  

#include "dispose.h"

struct Var *g_head, *g_tail;

int main(int argc, char *argv[]) {
	g_head = g_tail = NULL; // ��ʼ������ 
	input_the_sentences(argv[1]); //����input_the_sentences���������벢������䡣 
	return 0;
}


