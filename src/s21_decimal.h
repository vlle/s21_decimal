#ifndef SRC_S21_DECIMAL_H_
#define SRC_S21_DECIMAL_H_
#define MINUS_SIGN 2147483648
#include <stdbool.h>

typedef struct {
    unsigned int bits[4];
} s21_decimal;

typedef struct {
    bool bits[192];
    bool sign;
    int exp;
} s21_decimal_alt;

typedef enum {
    OPERATION_OK = 0,
    NUMBER_TOO_BIG,
    NUMBER_TOO_SMALL,
    DIVISION_BY_ZERO
} operation_result;

typedef enum {
    CONVERTATION_OK = 0,
    CONVERTATION_ERROR = 1
} convertation_result;

int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_mod(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);

int s21_is_equal(s21_decimal a, s21_decimal b);
int s21_is_not_equal(s21_decimal a, s21_decimal b);
int s21_is_greater(s21_decimal a, s21_decimal b);
int s21_is_greater_or_equal(s21_decimal a, s21_decimal b);
int s21_is_less(s21_decimal a, s21_decimal b);
int s21_is_less_or_equal(s21_decimal a, s21_decimal b);

int s21_from_int_to_decimal(int src, s21_decimal *dst);
int s21_from_float_to_decimal(float src, s21_decimal *dst);
int s21_from_decimal_to_int(s21_decimal src, int *dst);
int s21_from_decimal_to_float(s21_decimal src, float *dst);


int s21_round(s21_decimal value, s21_decimal *res);
int s21_floor(s21_decimal value, s21_decimal *res);
int s21_negate(s21_decimal value, s21_decimal *res);
int s21_truncate(s21_decimal value, s21_decimal *res);

#endif  // SRC_S21_DECIMAL_H_
