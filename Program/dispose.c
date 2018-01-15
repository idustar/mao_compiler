/************************************************* 
作者：杜佳豪 (1552652)
单位：同济大学软件工程 2015级 1班 
日期：2015/12/13
描述：以读取并执行Mao语言源代码，结果在屏幕上输出

dispose.c - 表达式的输入、处理及结果的输出。 
**************************************************/  

#include "dispose.h"

char g_str[1100];  //存放正在处理的语句，以分号为结尾

int input_the_sentences(char *argv) {
    //打开输入文件，并读入语句
	FILE *file_in; //定义输入文件 
	if ((file_in = fopen(argv, "r")) == NULL) return_error(3); 
	    //打开输入文件，判断文件是否能打开。若否，则结束程序 
	char *p_str;
	p_str = g_str + 1; // p_str为字符指针，指向正在输入的字符
	*p_str = fgetc(file_in); 
	while (*p_str != EOF) {   
	    if (*p_str == ';') {
	            //读到分号，则调用analy_the_sentences函数
	    	analy_the_sentences(g_str + 1, p_str - 1);
	    	p_str = g_str; 
	            //清空字符串，以读入下一句  
		}
		if (*p_str < 32 && p_str != g_str) p_str--; //忽略特殊字符（如换行符） 
		*(++p_str) = fgetc(file_in); //读取下一个字符 
	}
	fclose(file_in); //关闭输入文件 
}

int analy_the_sentences(char *p_left, char *p_right) {
	    // 分析语句，分离出含input, int, double的特殊语句
	char *p, *p_word_start, *p_left_bra, *p_right_bra; 
	    // 指针p指向当前字符
	    // 指针p_word_start指向变量名（单词）起始字符
	    // 指针p_left_bra,p_right_bra分别指向左括号、右括号的位置 
	int IsWordPrinting = UNINPUTED, hash, type_of_var, mark_negetive = 1; 
	    // IsWordPrinting表示输出变量名（单词）的状态。
	    // 值: UNINPUTED(1) / INPUTING(2) / FINISH_INPUTING(3) 
	    // mark_negetive表示下一个'-'是否可能为负号。 值: 1=是 0=不是 	
    for (p = p_left; p <= p_right; p++) {   // 处理正负号 
        if (*p == '(' || *p == '/' || *p == '*' || *p == '=' ||
		    *p == '+' || *p == '-') { 
		    if (mark_negetive == 1 && *p == '-') *p = '>';
		    if (mark_negetive == 1 && *p == '+') *p = '<';
		    mark_negetive = 1;
		} 
	 	else 
			if (*p != ' ') mark_negetive = 0;
    }
    /**************************************************************
    *减号的优先度低于乘除号，负号的优先度高于乘除号。 
    *如何判断减号和负号？ 
    *若'-'前一个非空格字符为'('、'/'、'*'、'=','+'，则一定为负号。
	*若'-'前无非空格字符，则一定为负号。
	*若确认'-'为负号，则将其替换为'>'，以便处理。（+号方法同上） 
    **************************************************************/ 	 
	for(p = p_left; p <= p_right; p++) {
		if (IsWordPrinting == UNINPUTED && *p >= 'A' && *p <= '}') { 
		    //判断是否为变量名（单词） 
			p_word_start = p; //记录变量的起始地址 
			IsWordPrinting = INPUTING;
		} 
	    /*变量名(单词)一般以空格或'('结束，在读到时处理变量名(单词)*/
		if (IsWordPrinting == INPUTING && *p == ' ') {  //读到空格 
			hash = Hash(p_word_start, p - 1); //获取该变量名(单词)的hash值 
			if (hash == HASH_INT) {  // 若读入单词int，则新建int变量 
			    define_var(p + 1, p_right, TYPE_INT); 
			    return 0; 
			}  
			if (hash == HASH_DOUBLE) { //若读入单词double，则新建double变量 
			    define_var(p + 1, p_right, TYPE_DOUBLE); 
				return 0; 
			} 
			if (hash == HASH_PRINT) { //若读入单词print，则输出变量值 
				p_left_bra = p;
				while (*p_left_bra != '(') p_left_bra++; //获取左括号地址 
				p_right_bra = p_right;
				while (*p_right_bra != ')') p_right_bra--; //获取右括号地址 
				print_var(p_left_bra + 1, p_right_bra - 1); //获取右括号地址
				return 0;
			}
			IsWordPrinting = FINISH_INPUTING; //语句中第一个单词读取结束
		}
		if (IsWordPrinting == INPUTING && *p == '(') { //读到'(' 
			hash = Hash(p_word_start, p - 1);  //获取该变量名(单词)的hash值
			if (hash == HASH_PRINT) {  //若读入单词print，则输出变量值 
				p_left_bra = p; 
				p_right_bra = p_right; 
				while (*p_right_bra != ')') p_right_bra--;  //获取右括号地址 
				print_var(p_left_bra + 1, p_right_bra - 1);
				return 0;
			}
			IsWordPrinting = FINISH_INPUTING;  //语句中第一个单词读取结束
		}
	} 
	calculate(p_left, p_right, &type_of_var);
	    //排除上述特殊语句，则该语句应为表达式，处理该表达式
}

int define_var(char *p_left, char *p_right, int type_of_var) {
	    //定义变量，从可能含有一个或多个变量名的字符串中提取出变量名 
	char *p;
	if (p_left > p_right) return 0;
	for (p = p_right; p >= p_left; p--) {
	    //从右向左搜索，若搜索到逗号，则逗号右侧的子串为变量名。 
		if (*p == ',')  {
			insert_var(var_name_validity(p + 1, p_right), 0, type_of_var);
			    //检查变量正确性并建立变量
			define_var(p_left, p - 1, type_of_var); //处理逗号左侧的字符串 
			return 0;				
		}
	}
	insert_var(var_name_validity(p_left, p_right), 0, type_of_var);
	   //检查变量正确性并建立变量 
}

int print_var(char *p_left, char *p_right) {
	    //获取变量名，打印变量的值
	int type_of_var;  //储存变量的类型，由函数calculate返回 
	double ans = calculate(p_left, p_right, &type_of_var); 
	    //将单一变量名当作表达式，利用calculate函数返回其值和类型
	if (type_of_var == TYPE_INT)
	    printf("%d\n", (int)ans); //用于int变量，直接输出 
	else
	    printf("%.6f\n", ans);	//用于double变量，保留六位小数输出 
}

double calculate(char *p_left, char *p_right, int *type_of_var) { 
    //表达式处理，返回表达式值和类型
    //本函数按以下优先级处理运算符：赋值号->加减号->乘除号->负号->括号
    if (p_left > p_right) return_error(5); //表达式错误报错 
	if (*p_left == ' ') return calculate(p_left + 1, p_right, type_of_var);
    if (*p_right == ' ') return calculate(p_left, p_right - 1, type_of_var); 
	    //忽略表达式两段多余的空格
    int count_bra = 0, a_type, b_type;  
	    //count_bra储存入栈括号数目，a_type,b_type储存运算符两边表达式的类型 
    double ans;
    char *p;
    for (p = p_left; p <= p_right; p++) {
	    switch (*p) {
	    	case '(': count_bra--; break;  //遇到左括号，出栈
            case ')': count_bra++; break;  //遇到右括号，入栈 
            case '=':
			    if (count_bra == 0) {  //当栈内有括号时，忽略该运算符
				    ans = calculate(p + 1, p_right, &b_type);
					change_value(Hash(p_left, p - 1), ans, &a_type);
					*type_of_var = a_type;
					return ans;
				} break;
		}
    }
    /************************************
    * 处理赋值号
	* 将赋值号左侧当成变量名处理
	* 将赋值号右侧当成表达式处理
	* 将表达式的值赋给该变量
	* 将右侧表达式的值作为整个表达式的值 
	************************************/  
    for (p = p_right; p >= p_left; p--) {
        switch (*p) {
			case '(': count_bra--; break; 
            case ')': count_bra++; break; 
            case '+':  //加号的处理 
			    if (count_bra == 0) {
			        ans = calculate(p_left, p - 1, &a_type) +
					    calculate(p + 1, p_right, &b_type);
					*type_of_var = a_type | b_type;
					// 此处运用位运算或
					// 若两者其一为double类型，则表达式返回double 
					return ans;
				} break;
            case '-':  //减号的处理 
			    if (count_bra == 0) {
				    ans = calculate(p_left, p - 1, &a_type) -
					    calculate(p + 1, p_right, &b_type); 
					*type_of_var = a_type | b_type;
					return ans;
				} break;
        }
    } 
    for (p = p_right; p >= p_left; p--) {
        switch (*p) {
			case '(': count_bra--; break;
            case ')': count_bra++; break;
            case '*':  //乘号的处理
			    if (count_bra == 0) {
				    ans = calculate(p_left, p - 1, &a_type) *
					    calculate(p + 1, p_right, &b_type); 
					*type_of_var = a_type | b_type;
					return ans;
				} break;
            case '/':  //除号的处理 
			    if (count_bra == 0) {
				    double divisor;
				    if ((divisor = calculate(p + 1, p_right, &b_type)) == 0)
				        return_error(4); //先计算除数的值。若为零，则报错
				    ans = calculate(p_left, p - 1, &a_type) / divisor;
				    if ((*type_of_var = (a_type | b_type)) == TYPE_INT)
				        return (int)ans; //若为两整型相除，则相除后取整
				    else
					    return ans; //若非两整型相除，则相除后值不作处理
			    } break;
        }
    } 
    if (*p_left == '>') //负号的处理，直接将负号后的表达号的值取相反数
	    return -calculate (p_left + 1, p_right, type_of_var);
	if (*p_left == '<') //正号的处理
	    return calculate (p_left + 1, p_right, type_of_var);
    if (*p_left == '(' && *p_right == ')') 
	    return calculate(p_left + 1, p_right - 1, type_of_var); 
		//括号的处理，直接返回括号内表达式的值 
    return str_to_value(p_left, p_right, type_of_var);
	    //若以上情况均不满足且表达式正确,则表达式为变量名或数值
}

double str_to_value(char *p_left, char *p_right, int *type_of_var) { 
    //处理字符串中的变量名和数值，并返回其值 
    char *p;
    double ans_str_to_value = strtod(p_left, NULL); 
	    //strtod函数用于将字符串转化为实数，若字符串为非实数则返回0 
    if (ans_str_to_value == 0 && ((*p_left >= 'A' && *p_left <= 'Z') ||
        (*p_left >= 'a' && *p_left <= 'z') || *p_left == '_'))
        return get_value(Hash(p_left, p_right), type_of_var);
        //若返回0，则由首字母判断是否为字符型（不可能为空格）
		//若是，则调用get_value函数返回其值。 
    for (p = p_left; p <= p_right; p++) 
	    //若输出为实数或整数，通过搜索小数点判断数值的类型
        if (*p == '.') {
        	// 若搜素到小数点，则该数值类型为double 
		    *type_of_var = TYPE_DOUBLE;
			return ans_str_to_value;
		}
    *type_of_var = TYPE_INT; //否则该数值类型为int 
	return ans_str_to_value;  
}

static int Hash(char *p_left, char *p_right) { 
    //使用哈希函数,将变量名（单词）转化为整值返回
	if (p_left > p_right) return 0;  
	if (*p_left == ' ') return Hash(p_left + 1, p_right); 
    if (*p_right == ' ') return Hash(p_left, p_right - 1); //忽略多余的空格
    if (*p_left == '(' && *p_right == ')')
	    return Hash(p_left + 1, p_right - 1); //忽略多余的括号
    int hash = 0; 
    char *p;
    for (p = p_left; p <= p_right; p++) 
	    hash = *p + (hash << 6) + (hash << 16) - hash;  
    return hash;
}

int var_name_validity(char *p_left, char *p_right) { 
    //判断变量命名是否合法，返回Hash值 
    if (p_left > p_right) return_error(6);  
	if (*p_left == ' ') return var_name_validity(p_left + 1, p_right); 
    if (*p_right == ' ') return var_name_validity(p_left, p_right - 1); 
	    //忽略多余的空格
    if (*p_left == '(' && *p_right == ')')
	    return var_name_validity(p_left + 1, p_right - 1); //忽略多余的括号
    if (p_right - p_left >= 20) return_error(2); //变量名长度若大于20则报错 
	char *p;
	int hash, IsAlphaInputed = 0;//储存变量名（单词）输入状态。 
     for (p = p_left; p <= p_right; p++) { 
        if (IsAlphaInputed == 0) { //对于还未输入非空字符的情况 
            if ((*p >= 'A' && *p <= 'Z') || (*p >= 'a' && *p <= 'z') || 
			    *p == '_') 
		        IsAlphaInputed = 1; //表示已经输入非空字符 
		    else if (*p > ' ')  //若首字符非大、小写字母或下划线则报错	
			    return_error(1);	    
		} else if (IsAlphaInputed == 1) { // 对于已经输入非空字符的情况 
		    if (!((*p >= 'A' && *p <= 'Z') || (*p >= 'a' && *p <= 'z') ||
			    (*p >= '0' && *p <= '9') || *p == '_' || *p == ' ')) 
			    return_error(1);
			if (*p == ' ') IsAlphaInputed = 2; //表示输入结束	    
		} else if (IsAlphaInputed == 2)
			if (*p > ' ') return_error(1); //若单词(变量名)后接非空字符则报错
	}
	if ((hash = Hash(p_left, p_right)) == HASH_INT || hash == HASH_DOUBLE || 
	    hash == HASH_PRINT)
		return_error(9);
}

int return_error(int error_code) { //返回错误 
    switch(error_code) {
        case 1: printf("Variable Name ERROR: Irregular Naming\n"); break;
        // 错误1 错误的变量名：命名不符合规则 
        case 2: printf("Variable Name ERROR: AN OVERLENGTH NAME\n"); break; 
        // 错误2 错误的变量名：变量名长度大于20 
    	case 3: printf("Open File Failed\n"); break;
    	// 错误3 文件打开错误
		case 4: printf("divided by ZERO\n"); break;
		// 错误4 被零除
		case 5: printf("Expression ERROR\n"); break;
		// 错误5 表达式错误
		case 6: printf("Variable Name ERROR: Cannot Be Blank\n"); break;
		// 错误6 错误的变量名：空白的变量名 
		case 7: printf("Declare Variable Failed: Declared\n"); break;
		// 错误7 声明变量失败：重复声明
		case 8: printf("Undeclared Variable(Procedure) Is Found\n"); break;
		// 错误8 使用未定义变量(过程) 
		case 9: printf("Variable Name ERROR: Reserved Name\n"); break;
		// 错误9 错误的变量名：使用了保留字段int, double, print 
	} 
	exit(0); 
}
