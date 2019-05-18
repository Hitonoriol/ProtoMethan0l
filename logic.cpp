#include "headers/Methan0l.h"

#define false 0
#define true 1

int Runner::op_preced(const char c) {
    switch(c)    {
        case '|':
            return 6;
        case '&':
            return 5;
        case '!':
            return 4;
        case '*':  case '/': case '%':
            return 3;
        case '+': case '-':
            return 2;
        case '=':
            return 1;
    }
    return 0;
}

bool Runner::op_left_assoc(const char c) {
    switch(c)    {
        case '*': case '/': case '%': case '+': case '-': case '|': case '&':
            return true;
        case '=': case '!':
            return false;
    }
    return false;
}

unsigned int Runner::op_arg_count(const char c) {
    switch(c) {
        case '*': case '/': case '%': case '+': case '-': case '=': case '&': case '|':
            return 2;
        case '!':
            return 1;
        default:
            return c - 'A';
    }
    return 0;
}

#define is_operator(c)  (c == '+' || c == '-' || c == '/' || c == '*' || c == '!' || c == '%' || c == '=' || c == '&' || c == '|')
#define is_function(c)  (c >= 'A' && c <= 'Z')
#define is_ident(c)     ((c >= '0' && c <= '9') || (c >= 'a' && c <= 'z'))

bool Runner::shunting_yard(const char *input, char *output) {
    const char *strpos = input, *strend = input + strlen(input);
    char c, *outpos = output;

    char stack[32];
    unsigned int sl = 0;
    char     sc;

    while(strpos < strend) {
        c = *strpos;
        if(c != ' ') {
            if(is_ident(c))  {
                *outpos = c; ++outpos;
            }
            else if(is_function(c)) {
                stack[sl] = c;
                ++sl;
            }
            else if(c == ',') {
                bool pe = false;
                while(sl > 0) {
                    sc = stack[sl - 1];
                    if(sc == '(')  {
                        pe = true;
                        break;
                    }
                    else {
                        *outpos = sc;
                        ++outpos;
                        sl--;
                    }
                }
                if(!pe)   {
                    panic("Error: separator or parentheses mismatched");
                    return false;
                }
            }
            else if(is_operator(c))  {
                while(sl > 0)    {
                    sc = stack[sl - 1];
                    if(is_operator(sc) &&
                        ((op_left_assoc(c) && (op_preced(c) >= op_preced(sc))) ||
                           (op_preced(c) > op_preced(sc))))   {
                        *outpos = sc;
                        ++outpos;
                        sl--;
                    }
                    else   {
                        break;
                    }
                }
                stack[sl] = c;
                ++sl;
            }
            else if(c == '(')   {
                stack[sl] = c;
                ++sl;
            }
            else if(c == ')')    {
                bool pe = false;
                while(sl > 0)     {
                    sc = stack[sl - 1];
                    if(sc == '(')    {
                        pe = true;
                        break;
                    }
                    else  {
                        *outpos = sc;
                        ++outpos;
                        sl--;
                    }
                }
                if(!pe)  {
                    panic("Error: parentheses mismatched");
                    return false;
                }
                sl--;
                if(sl > 0)   {
                    sc = stack[sl - 1];
                    if(is_function(sc))   {
                        *outpos = sc;
                        ++outpos;
                        sl--;
                    }
                }
            }
            else
                return false;
        }
        ++strpos;
    }
    while(sl > 0)  {
        sc = stack[sl - 1];
        if(sc == '(' || sc == ')')   {
            panic("Error: parentheses mismatched\n");
            return false;
        }
        *outpos = sc;
        ++outpos;
        --sl;
    }
    *outpos = 0;
    return true;
}

bool Runner::evalBoolExpr(const char * expr)  {
    char output[500] = {0};
    char * op;
    bool tmp = false;
    char part1[250], part2[250];

    if(!shunting_yard(expr, output))
      return false;

    while (strlen(output) > 1) {
        op = &output[0];
        while (!is_operator(*op) && *op != '\0')
          op++;
        if (*op == '\0') {
          return false;
        }
        else if (*op == '!') {
            tmp = !(*(op-1) - 48);
            *(op-1) = '\0';
        }
        else if(*op == '&') {
            tmp = (*(op-1) - 48) && (*(op-2) - 48);
            *(op-2) = '\0';
        }
        else if (*op == '|') {
            tmp = (*(op-1) - 48) || (*(op-2) - 48);
            *(op-2) = '\0';
        }
        memset(part1, 0, sizeof(part1));
        memset(part2, 0, sizeof(part2));
        strcpy(part1, output);
        strcpy(part2, op+1);
        memset(output, 0, sizeof(output));
        strcat(output, part1);
        strcat(output, ((tmp==false) ? "0" : "1"));
        strcat(output, part2);
    }
    return *output - 48;
}
