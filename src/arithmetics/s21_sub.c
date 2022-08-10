#include "../utilits/s21_utility.h"
#include "../s21_decimal.h"

int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
    int return_code = 0;
    s21_rescale(&value_1, &value_2);
    s21_decimal_alt alt_value_1 = s21_convert_std_to_alt(value_1);
    s21_decimal_alt alt_value_2 = s21_convert_std_to_alt(value_2);
    s21_decimal_alt alt_result;
    s21_null_decimal_alt(&alt_result);
    if (alt_value_1.sign == alt_value_2.sign) {
        if (s21_is_greater_or_equal(value_1, value_2) && \
            alt_value_1.sign == 0 || s21_is_less_or_equal(value_1, value_2) \
            && alt_value_1.sign == 1) {
                // нормальные побитовые вычисления
        return_code = s21_sub_alt(alt_value_1, alt_value_2, &alt_result);
                // }
            } else {
                alt_value_1.sign = alt_value_1.sign ^ 1;
                alt_value_2.sign = alt_value_2.sign ^ 1;
                value_1 = s21_convert_alt_to_std(alt_value_2);
                value_2 = s21_convert_alt_to_std(alt_value_1);
                return_code = s21_sub(value_1, value_2, result);
            }
        // }  // else приведение к большему
    } else {
        alt_value_2.sign = alt_value_2.sign ^ 1;
        value_2 = s21_convert_alt_to_std(alt_value_2);
        return_code = s21_add(value_1, value_2, result);
        alt_result = s21_convert_std_to_alt(*result);
    }
    *result = s21_convert_alt_to_std(alt_result);
    return return_code;
}

// сложение децималов
