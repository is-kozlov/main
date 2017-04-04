#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/*

<plus_minus> = <mult_div> {+|- <mult_div>}
<mult_div> = <unary> {*|/ <unary>}
<unary> = <number> | (-1)*unary | (<plus_minus>)

*/

typedef enum {num, plus, minus, mult, divis, l_bracket, r_bracket, end, error, space} Lexem; 

Lexem currlex;
double currnum;  //if currlex = num
size_t ERROR = 0;

double unary(void);
double mult_div(void);
double plus_minus(void);


size_t is_digit(char c) {
    return c >= '0' && c <= '9';    
}

//for debug:
void print_currlex(void) {
    switch (currlex) {
        case num:
            printf("num\n");
            break;
        case plus:
            printf("plus\n");
            break;
        case minus:
            printf("minus\n");
            break;
        case mult:
            printf("mult\n");
            break;
        case divis:
            printf("divis\n");
            break;
        case l_bracket:
            printf("l_bracket\n");
            break;
        case r_bracket:
            printf("r_bracket\n");
            break;
        case end:
            printf("end\n");
            break;
        case error:
            printf("error\n");
            break;
        case space:
            printf("space\n");
            break;
    }
}

//-----------------------------------------------------------------------------

void get_lexem(void) {
    char c = getchar();
    if(c == '+') currlex = plus;
    else if(c == '-') currlex = minus;
    else if(c == '*') currlex = mult;
    else if(c == '/') currlex = divis;
    else if(c == '(') currlex = l_bracket;
    else if(c == ')') currlex = r_bracket;
    else if(c == '\n' || c == EOF) currlex = end;
    else if(c == ' ') currlex = space;
    else if(is_digit(c)) {
        double a = c - '0';
        while(is_digit(c = getchar()) || c == ' ') {
            if(c != ' ') a = a*10 + c - '0';                
        }
        if(c == '.') {
            int pwr = -1;
            while(is_digit(c = getchar()) || c == ' ') {
                if(c != ' ') a = a + (c-'0') * pow(10, pwr--);                
            }
        }
        currnum = a;
        currlex = num;
        ungetc (c, stdin);
    }
    else currlex = error;
    return;
}

//-----------------------------------------------------------------------------

double plus_minus(void) {
    double a;
    Lexem l;
    a = mult_div();
    while(currlex == plus || currlex == minus || currlex == space) {
        l = currlex;
        get_lexem();
        if(l == plus) a += mult_div();
        else if(l == minus) a -= mult_div();       
    }
    return a;
}

//-----------------------------------------------------------------------------

double mult_div(void) {  
    double a;    
    Lexem l;
    a = unary();
    while(currlex == mult || currlex == divis || currlex == space) {
        l = currlex;
        get_lexem();
        if(l == mult) a *= mult_div();
        else if(l == divis) a /= mult_div();       
    }
    return a;
}

//-----------------------------------------------------------------------------

double unary(void) {
    double a;
    switch (currlex) {
        case num:
            get_lexem();
            return currnum;
        case l_bracket:
            get_lexem();
            a = plus_minus();
            if(currlex == r_bracket) {
                get_lexem();
                return a;
            }
            else {
                ERROR = 1;
                return 0;
            }
        case minus:
            get_lexem();
            return (-1)*unary();
        default:
            ERROR = 1;
            return 0;
    }
}

//-----------------------------------------------------------------------------

int main() {
    double a;
    get_lexem();
    a = plus_minus();
    if(!ERROR && currlex == end) printf("%.2lf\n", a);
    else printf("[error]\n");
    return 0;
}
