#include <stdio.h>
#include "../utilits/s21_structures.h"

// достает нужный бит из инта
// требуется для опроеделения знака стандартного децимала
// и вывода стандартного и альтернативного децимала в двоичной форме
bool s21_get_bit_int(unsigned int num, int pos) {
    return (num >> pos) & 1;
}

// достает знак из стандартного децимала
// нужна в преобразовании в альтернативную форму
bool s21_get_sign_std(s21_decimal dec) {
    return s21_get_bit_int(dec.bits[3], 31);
}

// достает положение точки из стандартного децимала
// нужна в преобразовании в альтернативную форму
int s21_get_exp_std(s21_decimal dec) {
    return (dec.bits[3] >> 16) % 2147483648;
}

// распечатка бинарной формы стандартного децимала
// распечатываются склеенные второй, первый и нулевой бит
// и через пробел -- третий бит
void print_binary_representation_std(s21_decimal std) {
    for (int i = 2; i >= 0; i--) {
        for (int j = 31; j >= 0; j--)
            printf("%i", s21_get_bit_int(std.bits[i], j));
    }
    printf(" ");
    for (int j = 31; j >= 0; j--)
        printf("%i", s21_get_bit_int(std.bits[3], j));
    printf("\n");
}

// распечатка бинарной формы альтернативного децимала
// распечатываются все биты от старшего к младшему
// и через пробелы - знак (1 - минус, 0 - плюс)
// и положение точки
void print_binary_representation_alt(s21_decimal_alt alt) {
    for (int i = 95; i >= 0; i--)
        printf("%i", alt.bits[i]);
    printf(" %i %i", alt.sign, alt.exp);
}

// зануление стандартного децимала
// просто потому что надо
void s21_null_decimal(s21_decimal *std) {
    for (int i = 0; i < 4; i++)
        std -> bits[i] = 0;
}

// зануление альтернативного децимала
// просто потому что надо
void s21_null_decimal_alt(s21_decimal_alt *alt) {
    alt -> sign = 0;
    alt -> exp = 0;
    for (int i = 0; i < 96; i++)
        alt -> bits[i] = 0;
}

// преобразование стандартного децимала в альтернативный
// протестировано, работает с корректными децималами
// поломается на децимале со слишком большим положением точки
s21_decimal_alt s21_convert_std_to_alt(s21_decimal std) {
    s21_decimal_alt alt;
    alt.sign = s21_get_sign_std(std);
    alt.exp = s21_get_exp_std(std);
    int i = 0;
    for (int j = 0; j < 3; j++) {
        for (int k = 0; k < 32; k++)
            alt.bits[i++] = s21_get_bit_int(std.bits[j], k);
    }
    return alt;
}

// преобразование альтернативного децимала
// протестировано, работает с корректными децималами
// поломается на децимале со слишком большим положением точки
// скорее всего, возниконовение такого децимала нужно предусматривать в вычислительных функциях 
s21_decimal s21_convert_alt_to_std(s21_decimal_alt alt) {
    s21_decimal std;
    s21_null_decimal(&std);
    int k = 95;
    for (int i = 2; i >= 0; i--)
        for (int j = 31; j >= 0; j--)
            std.bits[i] = (std.bits[i] << 1) + alt.bits[k--];
    std.bits[3] = alt.sign;
    std.bits[3] <<= 10;
    k = 16;
    for (int i = 4; i >= 0; i--) {
        std.bits[3] <<= 1;
        if (alt.exp >= k) {
            alt.exp -= k;
            std.bits[3]++;
        }
        k >>= 1;
    }
    std.bits[3] <<= 16;
    return std;
}

int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
    int return_code = 0;
    s21_decimal_alt alt_value_1 = s21_convert_std_to_alt(value_1);
    s21_decimal_alt alt_value_2 = s21_convert_std_to_alt(value_2);
    s21_decimal_alt alt_result;
    s21_null_decimal_alt(&alt_result);
    if (alt_value_1.sign == alt_value_2.sign) {
        // завтра напишу вычитание
    } else {
        alt_value_2.sign = alt_value_2.sign ^ 1;
        value_2 = s21_convert_alt_to_std(alt_value_2);
        return_code = s21_add(value_1, value_2, &result);
    }
    return return_code;
}

int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
    int return_code = 0;
    s21_decimal_alt alt_value_1 = s21_convert_std_to_alt(value_1);
    s21_decimal_alt alt_value_2 = s21_convert_std_to_alt(value_2);
    s21_decimal_alt alt_result;
    s21_null_decimal_alt(&alt_result);
    if (alt_value_1.sign == alt_value_2.sign) {
        if (alt_value_1.exp == alt_value_2.exp) {
            int t_bit = 0;  // бит переноса
            for (int i = 0; i < 96; i++) {
                alt_result.bits[i] = \
                alt_value_1.bits[i] ^ alt_value_2.bits[i] ^ t_bit;
                t_bit = 0;
                // нужно попробовать максимально упростить это выражение
                if ((alt_value_1.bits[i] & alt_value_2.bits[i]) || \
                (alt_value_1.bits[i] & t_bit) || \
                (alt_value_2.bits[i] & t_bit))
                // "если хотя бы два бита из трех равны единице"
                    t_bit = 1;
            }
            if (t_bit == 1)
                return_code = 0;  // произошло переполнение
        }  // else домножение меньшего числа на 10 и уменьшение экспоненты
    } else {  // else вычитание вместо сложения
        if (alt_value_1.sign) {
            alt_value_1.sign = 0;
            value_1 = s21_convert_alt_to_std(alt_value_1);
            return_code = s21_sub(value_2, value_1, &value_3);
        } else {
            alt_value_2.sign = 0;
            value_2 = s21_convert_alt_to_std(alt_value_2);
            return_code = s21_sub(value_1, value_2, &value_3);     
        }
    }
    *result = s21_convert_alt_to_std(alt_result);
    return return_code;
}

int main(void) {
    s21_decimal dec1;
    s21_null_decimal(&dec1);
    dec1.bits[0] = 100;
    dec1.bits[1] = 100;
    dec1.bits[2] = 10;

    s21_decimal dec2;
    s21_null_decimal(&dec2);
    dec2.bits[0] = 100;
    dec2.bits[1] = 100;
    dec2.bits[2] = 10;

    s21_decimal dec3;
    s21_null_decimal(&dec3);
    dec3.bits[0] = 0;
    dec3.bits[1] = 0;
    dec3.bits[2] = 0;

    int i = s21_add(dec1, dec2, &dec3);
    print_binary_representation_std(dec1);
    print_binary_representation_std(dec2);
    print_binary_representation_std(dec3);
    return 0;
}
