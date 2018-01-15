/************************************************* 
作者：杜佳豪 (1552652)
单位：同济大学软件工程 2015级 1班 
日期：2015/12/13
描述：以读取并执行Mao语言源代码，结果在屏幕上输出。 
**************************************************/  

#include "dispose.h"

struct Var *g_head, *g_tail;

int main(int argc, char *argv[]) {
	g_head = g_tail = NULL; // 初始化链表。 
	input_the_sentences(argv[1]); //调用input_the_sentences函数，输入并处理语句。 
	return 0;
}


