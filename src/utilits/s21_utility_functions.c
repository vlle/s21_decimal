#include "s21_utility.h"
#include "../s21_decimal.h"
// достает нужный бит из инта
// требуется для определения знака стандартного децимала
// и вывода стандартного и альтернативного децимала в двоичной форме
bool s21_get_bit_int(unsigned int num, int pos) {
    return (num >> pos) & 1;
}

// сдвиг вправо для альтернативного децимала
bool s21_right_shift(s21_decimal_alt *alt) {
    bool return_code = 0;
    if (alt -> bits[0] == 1)
        return_code = 1;
        // число было нечетным
    for (int i = 0; i < 95; i++)
        alt -> bits[i] = alt -> bits[i + 1];
    alt -> bits[95] = 0;
    return return_code;
}

// сдвиг влево для альтернативного децимала
// нужно при умножении
bool s21_left_shift(s21_decimal_alt *alt) {
    bool return_code = 0;
    if (alt -> bits[95] == 1) {
        return_code = 1;
    } else {
        for (int i = 95; i > 0; i--)
            alt -> bits[i] = alt -> bits[i - 1];
        alt -> bits[0] = 0;
    }
    return return_code;
}

// достает знак из стандартного децимала
// нужна в преобразовании в альтернативную форму
bool s21_get_sign_std(s21_decimal dec) {
    return s21_get_bit_int(dec.bits[3], 31);
}

// достает положение точки из стандартного децимала
// нужна в преобразовании в альтернативную форму
int s21_get_exp_std(s21_decimal dec) {
    return (dec.bits[3] % 2147483648) >> 16;
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

// простейший рескейл. нужно будет разбить на две функции,
// подключить округление при переполнении
// и выдачу ошибки при совсем переполнении
int s21_rescale(s21_decimal *value_1, s21_decimal *value_2) {
    s21_decimal_alt alt_value_1 = s21_convert_std_to_alt(*value_1);
    s21_decimal_alt alt_value_2 = s21_convert_std_to_alt(*value_2);
    s21_rescale_alt(&alt_value_1, &alt_value_2);
    *value_1 = s21_convert_alt_to_std(alt_value_1);
    *value_2 = s21_convert_alt_to_std(alt_value_2);
}

void init_decimal(s21_decimal *decimal) {
    for (int i = 0; i < 4; i++) {
        decimal->bits[i] = 0;
    }
}

// деление числа на 10
// будет возвращать остаток от деления или сразу округлять
int div_by_ten(s21_decimal_alt *alt) {
    s21_decimal_alt result;
    s21_null_decimal_alt(&result);
    // возможно, эта конструкция не нужна и есть решение получше
    int exp = alt -> exp;
    int sign = alt -> sign;
    result.sign = 0;
    result.exp = 0;

    s21_decimal_alt five;
    s21_null_decimal_alt(&five);
    five.bits[1] = 1;
    five.bits[3] = 1;
    while (compare_bits(*alt, five))
        s21_left_shift(&five);
    if (five.bits[1] == 0)
        s21_right_shift(&five);
    for (int i = 0; i < 96; i++) {
        if (compare_bits(*alt, five)) {
            s21_sub_alt(*alt, five, alt);
            result.bits[0] = 1;
        }
        if (five.bits[1] == 1)
            break;
        else
            s21_right_shift(&five);
        s21_left_shift(&result);
    }
    int res = s21_convert_alt_to_std(*alt).bits[0];
    *alt = result;
    // возможно, эта конструкция не нужна и есть решение получше
    alt -> exp = exp - 1;
    alt -> sign = sign;
    return res;
}

// сравнение битов первого и второго альтернативного децимала
// не учитывает экспоненту
// возвращает 1, если первое число больше или равно второму
// 0 если второе число больше первого
bool compare_bits(s21_decimal_alt alt1, s21_decimal_alt alt2) {
    int i = 95;
    while (i >= 0 && alt1.bits[i] == alt2.bits[i])
        i--;
    return i == -1 ? 1 : alt1.bits[i];
}

// равен ли альтернативный децимал 0
bool is_null(s21_decimal_alt alt) {
    bool result = 0;
    for (int i = 0; i < 96; i++)
        result += alt.bits[i];
    return !result;
}
