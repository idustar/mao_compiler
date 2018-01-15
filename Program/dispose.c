/************************************************* 
���ߣ��żѺ� (1552652)
��λ��ͬ�ô�ѧ������� 2015�� 1�� 
���ڣ�2015/12/13
�������Զ�ȡ��ִ��Mao����Դ���룬�������Ļ�����

dispose.c - ���ʽ�����롢�������������� 
**************************************************/  

#include "dispose.h"

char g_str[1100];  //������ڴ������䣬�Էֺ�Ϊ��β

int input_the_sentences(char *argv) {
    //�������ļ������������
	FILE *file_in; //���������ļ� 
	if ((file_in = fopen(argv, "r")) == NULL) return_error(3); 
	    //�������ļ����ж��ļ��Ƿ��ܴ򿪡�������������� 
	char *p_str;
	p_str = g_str + 1; // p_strΪ�ַ�ָ�룬ָ������������ַ�
	*p_str = fgetc(file_in); 
	while (*p_str != EOF) {   
	    if (*p_str == ';') {
	            //�����ֺţ������analy_the_sentences����
	    	analy_the_sentences(g_str + 1, p_str - 1);
	    	p_str = g_str; 
	            //����ַ������Զ�����һ��  
		}
		if (*p_str < 32 && p_str != g_str) p_str--; //���������ַ����绻�з��� 
		*(++p_str) = fgetc(file_in); //��ȡ��һ���ַ� 
	}
	fclose(file_in); //�ر������ļ� 
}

int analy_the_sentences(char *p_left, char *p_right) {
	    // ������䣬�������input, int, double���������
	char *p, *p_word_start, *p_left_bra, *p_right_bra; 
	    // ָ��pָ��ǰ�ַ�
	    // ָ��p_word_startָ������������ʣ���ʼ�ַ�
	    // ָ��p_left_bra,p_right_bra�ֱ�ָ�������š������ŵ�λ�� 
	int IsWordPrinting = UNINPUTED, hash, type_of_var, mark_negetive = 1; 
	    // IsWordPrinting��ʾ��������������ʣ���״̬��
	    // ֵ: UNINPUTED(1) / INPUTING(2) / FINISH_INPUTING(3) 
	    // mark_negetive��ʾ��һ��'-'�Ƿ����Ϊ���š� ֵ: 1=�� 0=���� 	
    for (p = p_left; p <= p_right; p++) {   // ���������� 
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
    *���ŵ����ȶȵ��ڳ˳��ţ����ŵ����ȶȸ��ڳ˳��š� 
    *����жϼ��ź͸��ţ� 
    *��'-'ǰһ���ǿո��ַ�Ϊ'('��'/'��'*'��'=','+'����һ��Ϊ���š�
	*��'-'ǰ�޷ǿո��ַ�����һ��Ϊ���š�
	*��ȷ��'-'Ϊ���ţ������滻Ϊ'>'���Ա㴦����+�ŷ���ͬ�ϣ� 
    **************************************************************/ 	 
	for(p = p_left; p <= p_right; p++) {
		if (IsWordPrinting == UNINPUTED && *p >= 'A' && *p <= '}') { 
		    //�ж��Ƿ�Ϊ�����������ʣ� 
			p_word_start = p; //��¼��������ʼ��ַ 
			IsWordPrinting = INPUTING;
		} 
	    /*������(����)һ���Կո��'('�������ڶ���ʱ���������(����)*/
		if (IsWordPrinting == INPUTING && *p == ' ') {  //�����ո� 
			hash = Hash(p_word_start, p - 1); //��ȡ�ñ�����(����)��hashֵ 
			if (hash == HASH_INT) {  // �����뵥��int�����½�int���� 
			    define_var(p + 1, p_right, TYPE_INT); 
			    return 0; 
			}  
			if (hash == HASH_DOUBLE) { //�����뵥��double�����½�double���� 
			    define_var(p + 1, p_right, TYPE_DOUBLE); 
				return 0; 
			} 
			if (hash == HASH_PRINT) { //�����뵥��print�����������ֵ 
				p_left_bra = p;
				while (*p_left_bra != '(') p_left_bra++; //��ȡ�����ŵ�ַ 
				p_right_bra = p_right;
				while (*p_right_bra != ')') p_right_bra--; //��ȡ�����ŵ�ַ 
				print_var(p_left_bra + 1, p_right_bra - 1); //��ȡ�����ŵ�ַ
				return 0;
			}
			IsWordPrinting = FINISH_INPUTING; //����е�һ�����ʶ�ȡ����
		}
		if (IsWordPrinting == INPUTING && *p == '(') { //����'(' 
			hash = Hash(p_word_start, p - 1);  //��ȡ�ñ�����(����)��hashֵ
			if (hash == HASH_PRINT) {  //�����뵥��print�����������ֵ 
				p_left_bra = p; 
				p_right_bra = p_right; 
				while (*p_right_bra != ')') p_right_bra--;  //��ȡ�����ŵ�ַ 
				print_var(p_left_bra + 1, p_right_bra - 1);
				return 0;
			}
			IsWordPrinting = FINISH_INPUTING;  //����е�һ�����ʶ�ȡ����
		}
	} 
	calculate(p_left, p_right, &type_of_var);
	    //�ų�����������䣬������ӦΪ���ʽ������ñ��ʽ
}

int define_var(char *p_left, char *p_right, int type_of_var) {
	    //����������ӿ��ܺ���һ���������������ַ�������ȡ�������� 
	char *p;
	if (p_left > p_right) return 0;
	for (p = p_right; p >= p_left; p--) {
	    //�������������������������ţ��򶺺��Ҳ���Ӵ�Ϊ�������� 
		if (*p == ',')  {
			insert_var(var_name_validity(p + 1, p_right), 0, type_of_var);
			    //��������ȷ�Բ���������
			define_var(p_left, p - 1, type_of_var); //�����������ַ��� 
			return 0;				
		}
	}
	insert_var(var_name_validity(p_left, p_right), 0, type_of_var);
	   //��������ȷ�Բ��������� 
}

int print_var(char *p_left, char *p_right) {
	    //��ȡ����������ӡ������ֵ
	int type_of_var;  //������������ͣ��ɺ���calculate���� 
	double ans = calculate(p_left, p_right, &type_of_var); 
	    //����һ�������������ʽ������calculate����������ֵ������
	if (type_of_var == TYPE_INT)
	    printf("%d\n", (int)ans); //����int������ֱ����� 
	else
	    printf("%.6f\n", ans);	//����double������������λС����� 
}

double calculate(char *p_left, char *p_right, int *type_of_var) { 
    //���ʽ�������ر��ʽֵ������
    //���������������ȼ��������������ֵ��->�Ӽ���->�˳���->����->����
    if (p_left > p_right) return_error(5); //���ʽ���󱨴� 
	if (*p_left == ' ') return calculate(p_left + 1, p_right, type_of_var);
    if (*p_right == ' ') return calculate(p_left, p_right - 1, type_of_var); 
	    //���Ա��ʽ���ζ���Ŀո�
    int count_bra = 0, a_type, b_type;  
	    //count_bra������ջ������Ŀ��a_type,b_type������������߱��ʽ������ 
    double ans;
    char *p;
    for (p = p_left; p <= p_right; p++) {
	    switch (*p) {
	    	case '(': count_bra--; break;  //���������ţ���ջ
            case ')': count_bra++; break;  //���������ţ���ջ 
            case '=':
			    if (count_bra == 0) {  //��ջ��������ʱ�����Ը������
				    ans = calculate(p + 1, p_right, &b_type);
					change_value(Hash(p_left, p - 1), ans, &a_type);
					*type_of_var = a_type;
					return ans;
				} break;
		}
    }
    /************************************
    * ����ֵ��
	* ����ֵ����൱�ɱ���������
	* ����ֵ���Ҳ൱�ɱ��ʽ����
	* �����ʽ��ֵ�����ñ���
	* ���Ҳ���ʽ��ֵ��Ϊ�������ʽ��ֵ 
	************************************/  
    for (p = p_right; p >= p_left; p--) {
        switch (*p) {
			case '(': count_bra--; break; 
            case ')': count_bra++; break; 
            case '+':  //�ӺŵĴ��� 
			    if (count_bra == 0) {
			        ans = calculate(p_left, p - 1, &a_type) +
					    calculate(p + 1, p_right, &b_type);
					*type_of_var = a_type | b_type;
					// �˴�����λ�����
					// ��������һΪdouble���ͣ�����ʽ����double 
					return ans;
				} break;
            case '-':  //���ŵĴ��� 
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
            case '*':  //�˺ŵĴ���
			    if (count_bra == 0) {
				    ans = calculate(p_left, p - 1, &a_type) *
					    calculate(p + 1, p_right, &b_type); 
					*type_of_var = a_type | b_type;
					return ans;
				} break;
            case '/':  //���ŵĴ��� 
			    if (count_bra == 0) {
				    double divisor;
				    if ((divisor = calculate(p + 1, p_right, &b_type)) == 0)
				        return_error(4); //�ȼ��������ֵ����Ϊ�㣬�򱨴�
				    ans = calculate(p_left, p - 1, &a_type) / divisor;
				    if ((*type_of_var = (a_type | b_type)) == TYPE_INT)
				        return (int)ans; //��Ϊ������������������ȡ��
				    else
					    return ans; //����������������������ֵ��������
			    } break;
        }
    } 
    if (*p_left == '>') //���ŵĴ���ֱ�ӽ����ź�ı��ŵ�ֵȡ�෴��
	    return -calculate (p_left + 1, p_right, type_of_var);
	if (*p_left == '<') //���ŵĴ���
	    return calculate (p_left + 1, p_right, type_of_var);
    if (*p_left == '(' && *p_right == ')') 
	    return calculate(p_left + 1, p_right - 1, type_of_var); 
		//���ŵĴ���ֱ�ӷ��������ڱ��ʽ��ֵ 
    return str_to_value(p_left, p_right, type_of_var);
	    //������������������ұ��ʽ��ȷ,����ʽΪ����������ֵ
}

double str_to_value(char *p_left, char *p_right, int *type_of_var) { 
    //�����ַ����еı���������ֵ����������ֵ 
    char *p;
    double ans_str_to_value = strtod(p_left, NULL); 
	    //strtod�������ڽ��ַ���ת��Ϊʵ�������ַ���Ϊ��ʵ���򷵻�0 
    if (ans_str_to_value == 0 && ((*p_left >= 'A' && *p_left <= 'Z') ||
        (*p_left >= 'a' && *p_left <= 'z') || *p_left == '_'))
        return get_value(Hash(p_left, p_right), type_of_var);
        //������0����������ĸ�ж��Ƿ�Ϊ�ַ��ͣ�������Ϊ�ո�
		//���ǣ������get_value����������ֵ�� 
    for (p = p_left; p <= p_right; p++) 
	    //�����Ϊʵ����������ͨ������С�����ж���ֵ������
        if (*p == '.') {
        	// �����ص�С���㣬�����ֵ����Ϊdouble 
		    *type_of_var = TYPE_DOUBLE;
			return ans_str_to_value;
		}
    *type_of_var = TYPE_INT; //�������ֵ����Ϊint 
	return ans_str_to_value;  
}

static int Hash(char *p_left, char *p_right) { 
    //ʹ�ù�ϣ����,�������������ʣ�ת��Ϊ��ֵ����
	if (p_left > p_right) return 0;  
	if (*p_left == ' ') return Hash(p_left + 1, p_right); 
    if (*p_right == ' ') return Hash(p_left, p_right - 1); //���Զ���Ŀո�
    if (*p_left == '(' && *p_right == ')')
	    return Hash(p_left + 1, p_right - 1); //���Զ��������
    int hash = 0; 
    char *p;
    for (p = p_left; p <= p_right; p++) 
	    hash = *p + (hash << 6) + (hash << 16) - hash;  
    return hash;
}

int var_name_validity(char *p_left, char *p_right) { 
    //�жϱ��������Ƿ�Ϸ�������Hashֵ 
    if (p_left > p_right) return_error(6);  
	if (*p_left == ' ') return var_name_validity(p_left + 1, p_right); 
    if (*p_right == ' ') return var_name_validity(p_left, p_right - 1); 
	    //���Զ���Ŀո�
    if (*p_left == '(' && *p_right == ')')
	    return var_name_validity(p_left + 1, p_right - 1); //���Զ��������
    if (p_right - p_left >= 20) return_error(2); //����������������20�򱨴� 
	char *p;
	int hash, IsAlphaInputed = 0;//��������������ʣ�����״̬�� 
     for (p = p_left; p <= p_right; p++) { 
        if (IsAlphaInputed == 0) { //���ڻ�δ����ǿ��ַ������ 
            if ((*p >= 'A' && *p <= 'Z') || (*p >= 'a' && *p <= 'z') || 
			    *p == '_') 
		        IsAlphaInputed = 1; //��ʾ�Ѿ�����ǿ��ַ� 
		    else if (*p > ' ')  //�����ַ��Ǵ�Сд��ĸ���»����򱨴�	
			    return_error(1);	    
		} else if (IsAlphaInputed == 1) { // �����Ѿ�����ǿ��ַ������ 
		    if (!((*p >= 'A' && *p <= 'Z') || (*p >= 'a' && *p <= 'z') ||
			    (*p >= '0' && *p <= '9') || *p == '_' || *p == ' ')) 
			    return_error(1);
			if (*p == ' ') IsAlphaInputed = 2; //��ʾ�������	    
		} else if (IsAlphaInputed == 2)
			if (*p > ' ') return_error(1); //������(������)��ӷǿ��ַ��򱨴�
	}
	if ((hash = Hash(p_left, p_right)) == HASH_INT || hash == HASH_DOUBLE || 
	    hash == HASH_PRINT)
		return_error(9);
}

int return_error(int error_code) { //���ش��� 
    switch(error_code) {
        case 1: printf("Variable Name ERROR: Irregular Naming\n"); break;
        // ����1 ����ı����������������Ϲ��� 
        case 2: printf("Variable Name ERROR: AN OVERLENGTH NAME\n"); break; 
        // ����2 ����ı����������������ȴ���20 
    	case 3: printf("Open File Failed\n"); break;
    	// ����3 �ļ��򿪴���
		case 4: printf("divided by ZERO\n"); break;
		// ����4 �����
		case 5: printf("Expression ERROR\n"); break;
		// ����5 ���ʽ����
		case 6: printf("Variable Name ERROR: Cannot Be Blank\n"); break;
		// ����6 ����ı��������հ׵ı����� 
		case 7: printf("Declare Variable Failed: Declared\n"); break;
		// ����7 ��������ʧ�ܣ��ظ�����
		case 8: printf("Undeclared Variable(Procedure) Is Found\n"); break;
		// ����8 ʹ��δ�������(����) 
		case 9: printf("Variable Name ERROR: Reserved Name\n"); break;
		// ����9 ����ı�������ʹ���˱����ֶ�int, double, print 
	} 
	exit(0); 
}
