#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int getNumber(char *p, int index){
    int tmp = (int)strlen(p) - index - 1;
    if(tmp < 0){
        return 0;
    }
    else{
        return (int)p[tmp] - 48;
    }
}

int getCarry(int num, int *tenBit, int *oneBit){
    *tenBit = (int)(num / 10);
    *oneBit = num - *tenBit * 10;
    if (num >= 10){
        return 1;
    }
    return 0;
}

char *accurateAdd(char *a, char *b, size_t len_a, size_t len_b){
    size_t max_size = len_a > len_b ? len_a : len_b;
    char *result = calloc(max_size + 1, sizeof(char));
    char *carry = calloc(max_size + 2, sizeof(char));
    result[max_size] = '\0';
    carry[max_size + 1] = '\0';
    carry[max_size] = '0';

    int count = 0;
    for (size_t i = 0;i < max_size;i ++){
        int tmp, ten, one;
        tmp = getNumber(a, i) + getNumber(b, i);
        count += getCarry(tmp, &ten, &one);
        carry[max_size - i - 1] = (char)(ten + 48);
        result[max_size - i - 1] = (char)(one + 48);

    }
    if (count > 0){
        char *tmp;
        if (carry[0] == '0'){  // 确保carry第一位不为0，否则返回的结果会高位会被补0
            char *new_carry = calloc(strlen(carry), sizeof(char));
            for (int i = 0; i < strlen(carry); i ++){
                new_carry[i] = carry[i + 1];
            }
            free(carry);
            carry = new_carry;
        }

        tmp = accurateAdd(result, carry, strlen(result), strlen(carry));
        free(result);
        result = tmp;
    }
    free(carry);
    return result;
}

char *accurateMulCore(char *a, int b, size_t max_size, int b_size){
    char *result = NULL, *tmp_add_base = NULL;

    for (size_t i = 0;i < max_size;i ++){
        int tmp, ten, one, have;
        tmp = getNumber(a, i) * b;
        if (getCarry(tmp, &ten, &one)){
            have = 2;
        }
        else{
            have = 1;
        }
        int len = (int)(have + b_size + i);
        tmp_add_base = calloc(len + 1, sizeof(char));
        tmp_add_base[len] = '\0';
        if (have == 1){
            tmp_add_base[0] = (char)(one + 48);
        }
        else{
            tmp_add_base[0] = (char)(ten + 48);
            tmp_add_base[1] = (char)(one + 48);
        }
        for (int i_2 = have;i_2 < len; i_2++){
            tmp_add_base[i_2] = '0';
        }

        if (i == 0){
            result = tmp_add_base;
        }
        else{
            char *add_tmp;
            add_tmp = accurateAdd(result, tmp_add_base, strlen(result), strlen(tmp_add_base));
            free(result);
            free(tmp_add_base);
            result = add_tmp;
        }
    }
    return result;
}

char *accurateMul(char *a, char *b, size_t len_a, size_t len_b){
    char *result = NULL;

    for (size_t i = 0;i < len_a;i ++){
        char *tmp;
        tmp = accurateMulCore(b, getNumber(a, i), len_b, i);
        if (i == 0){
            result = tmp;
        }
        else{
            char *tmp_result;
            tmp_result = accurateAdd(result, tmp, strlen(result), strlen(tmp));
            free(result);
            free(tmp);
            result = tmp_result;
        }
    }
    return result;
}

int main() {
    char *a = "4556";
    char *b = "922";
//    char *tmp = accurateAdd(a, b, strlen(a), strlen(b));
//    char *tmp = accurateMulCore(a, 1, strlen(a), 0);
    char *tmp = accurateMul(a, b, strlen(a), strlen(b));
    printf("tmp = %s\n", tmp);
    free(tmp);
    return 0;
}
