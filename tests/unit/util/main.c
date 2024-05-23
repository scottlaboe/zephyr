/*
 * Copyright (c) 2019 Oticon A/S
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/ztest.h>
#include <zephyr/sys/util.h>
#include <string.h>

#include "test.inc"

#if __cplusplus
extern "C" {
#endif

ZTEST(util_cxx, test_u8_to_dec) {
	run_u8_to_dec();
}

ZTEST(util_cxx, test_COND_CODE_1) {
	run_COND_CODE_1();
}

ZTEST(util_cxx, test_COND_CODE_0) {
	run_COND_CODE_0();
}

ZTEST(util_cxx, test_UTIL_OR) {
	run_UTIL_OR();
}

ZTEST(util_cxx, test_UTIL_AND) {
	run_UTIL_AND();
}

ZTEST(util_cxx, test_IF_ENABLED) {
	run_IF_ENABLED();
}

ZTEST(util_cxx, test_LISTIFY) {
	run_LISTIFY();
}

ZTEST(util_cxx, test_MACRO_MAP_CAT) {
	run_MACRO_MAP_CAT();
}

/* Test checks if @ref Z_MAX, @ref Z_MIN and @ref Z_CLAMP return correct result
 * and perform single evaluation of input arguments.
 */
ZTEST(util, test_z_max_z_min_z_clamp) {
	zassert_equal(Z_MAX(inc_func(true), 0), 1, "Unexpected macro result");
	/* Z_MAX should have call inc_func only once */
	zassert_equal(inc_func(false), 2, "Unexpected return value");

	zassert_equal(Z_MIN(inc_func(false), 2), 2, "Unexpected macro result");
	/* Z_MIN should have call inc_func only once */
	zassert_equal(inc_func(false), 4, "Unexpected return value");

	zassert_equal(Z_CLAMP(inc_func(false), 1, 3), 3, "Unexpected macro result");
	/* Z_CLAMP should have call inc_func only once */
	zassert_equal(inc_func(false), 6, "Unexpected return value");

	zassert_equal(Z_CLAMP(inc_func(false), 10, 15), 10,
		      "Unexpected macro result");
	/* Z_CLAMP should have call inc_func only once */
	zassert_equal(inc_func(false), 8, "Unexpected return value");
}

ZTEST(util, test_CLAMP) {
	zassert_equal(CLAMP(5, 3, 7), 5, "Unexpected clamp result");
	zassert_equal(CLAMP(3, 3, 7), 3, "Unexpected clamp result");
	zassert_equal(CLAMP(7, 3, 7), 7, "Unexpected clamp result");
	zassert_equal(CLAMP(1, 3, 7), 3, "Unexpected clamp result");
	zassert_equal(CLAMP(8, 3, 7), 7, "Unexpected clamp result");

	zassert_equal(CLAMP(-5, -7, -3), -5, "Unexpected clamp result");
	zassert_equal(CLAMP(-9, -7, -3), -7, "Unexpected clamp result");
	zassert_equal(CLAMP(1, -7, -3), -3, "Unexpected clamp result");

	zassert_equal(CLAMP(0xffffffffaULL, 0xffffffff0ULL, 0xfffffffffULL),
		      0xffffffffaULL, "Unexpected clamp result");
}

ZTEST(util, test_IN_RANGE) {
	zassert_true(IN_RANGE(0, 0, 0), "Unexpected IN_RANGE result");
	zassert_true(IN_RANGE(1, 0, 1), "Unexpected IN_RANGE result");
	zassert_true(IN_RANGE(1, 0, 2), "Unexpected IN_RANGE result");
	zassert_true(IN_RANGE(-1, -2, 2), "Unexpected IN_RANGE result");
	zassert_true(IN_RANGE(-3, -5, -1), "Unexpected IN_RANGE result");
	zassert_true(IN_RANGE(0, 0, UINT64_MAX), "Unexpected IN_RANGE result");
	zassert_true(IN_RANGE(UINT64_MAX, 0, UINT64_MAX), "Unexpected IN_RANGE result");
	zassert_true(IN_RANGE(0, INT64_MIN, INT64_MAX), "Unexpected IN_RANGE result");
	zassert_true(IN_RANGE(INT64_MIN, INT64_MIN, INT64_MAX), "Unexpected IN_RANGE result");
	zassert_true(IN_RANGE(INT64_MAX, INT64_MIN, INT64_MAX), "Unexpected IN_RANGE result");

	zassert_false(IN_RANGE(5, 0, 2), "Unexpected IN_RANGE result");
	zassert_false(IN_RANGE(5, 10, 0), "Unexpected IN_RANGE result");
	zassert_false(IN_RANGE(-1, 0, 1), "Unexpected IN_RANGE result");
}

ZTEST(util, test_FOR_EACH) {
	#define FOR_EACH_MACRO_TEST(arg) *buf++ = arg
	#define FOR_EACH_MACRO_TEST2(arg) arg

	uint8_t array[3] = {0};
	uint8_t *buf = array;

	FOR_EACH(FOR_EACH_MACRO_TEST, (;), 1, 2, 3);

	zassert_equal(array[0], 1, "Unexpected value %d", array[0]);
	zassert_equal(array[1], 2, "Unexpected value %d", array[1]);
	zassert_equal(array[2], 3, "Unexpected value %d", array[2]);

	uint8_t test0[] = { 0, FOR_EACH(FOR_EACH_MACRO_TEST2, (,))};

	BUILD_ASSERT(sizeof(test0) == 1, "Unexpected length due to FOR_EACH fail");

	uint8_t test1[] = { 0, FOR_EACH(FOR_EACH_MACRO_TEST2, (,), 1)};

	BUILD_ASSERT(sizeof(test1) == 2, "Unexpected length due to FOR_EACH fail");
}

ZTEST(util, test_FOR_EACH_NONEMPTY_TERM) {
	#define SQUARE(arg) (arg * arg)
	#define SWALLOW_VA_ARGS_1(...) EMPTY
	#define SWALLOW_VA_ARGS_2(...)
	#define REPEAT_VA_ARGS(...) __VA_ARGS__

	uint8_t array[] = {
		FOR_EACH_NONEMPTY_TERM(SQUARE, (,))
		FOR_EACH_NONEMPTY_TERM(SQUARE, (,),)
		FOR_EACH_NONEMPTY_TERM(SQUARE, (,), ,)
		FOR_EACH_NONEMPTY_TERM(SQUARE, (,), EMPTY, EMPTY)
		FOR_EACH_NONEMPTY_TERM(SQUARE, (,), SWALLOW_VA_ARGS_1(a, b))
		FOR_EACH_NONEMPTY_TERM(SQUARE, (,), SWALLOW_VA_ARGS_2(c, d))
		FOR_EACH_NONEMPTY_TERM(SQUARE, (,), 1)
		FOR_EACH_NONEMPTY_TERM(SQUARE, (,), 2, 3)
		FOR_EACH_NONEMPTY_TERM(SQUARE, (,), REPEAT_VA_ARGS(4))
		FOR_EACH_NONEMPTY_TERM(SQUARE, (,), REPEAT_VA_ARGS(5, 6))
		255
	};

	size_t size = ARRAY_SIZE(array);

	zassert_equal(size, 7, "Unexpected size %d", size);
	zassert_equal(array[0], 1, "Unexpected value %d", array[0]);
	zassert_equal(array[1], 4, "Unexpected value %d", array[1]);
	zassert_equal(array[2], 9, "Unexpected value %d", array[2]);
	zassert_equal(array[3], 16, "Unexpected value %d", array[3]);
	zassert_equal(array[4], 25, "Unexpected value %d", array[4]);
	zassert_equal(array[5], 36, "Unexpected value %d", array[5]);
	zassert_equal(array[6], 255, "Unexpected value %d", array[6]);
}

static void fsum(uint32_t incr, uint32_t *sum)
{
	*sum = *sum + incr;
}

ZTEST(util_cxx, test_CLAMP) {
	run_CLAMP();
}

ZTEST(util_cxx, test_IN_RANGE) {
	run_IN_RANGE();
}

ZTEST(util_cxx, test_FOR_EACH) {
	run_FOR_EACH();
}

ZTEST(util_cxx, test_FOR_EACH_NONEMPTY_TERM) {
	run_FOR_EACH_NONEMPTY_TERM();
}

ZTEST(util_cxx, test_FOR_EACH_FIXED_ARG) {
	run_FOR_EACH_FIXED_ARG();
}

ZTEST(util_cxx, test_FOR_EACH_IDX) {
	run_FOR_EACH_IDX();
}

ZTEST(util_cxx, test_FOR_EACH_IDX_FIXED_ARG) {
	run_FOR_EACH_IDX_FIXED_ARG();
}

ZTEST(util_cxx, test_IS_EMPTY) {
	run_IS_EMPTY();
}

ZTEST(util_cxx, test_IS_EQ) {
	run_IS_EQ();
}

ZTEST(util_cxx, test_LIST_DROP_EMPTY) {
	run_LIST_DROP_EMPTY();
}

ZTEST(util_cxx, test_nested_FOR_EACH) {
	run_nested_FOR_EACH();
}

ZTEST(util_cxx, test_GET_ARG_N) {
	run_GET_ARG_N();
}

ZTEST(util_cxx, test_GET_ARGS_LESS_N) {
	run_GET_ARGS_LESS_N();
}

ZTEST(util_cxx, test_mixing_GET_ARG_and_FOR_EACH) {
	run_mixing_GET_ARG_and_FOR_EACH();
}

ZTEST(util_cxx, test_IS_ARRAY_ELEMENT)
{
	run_IS_ARRAY_ELEMENT();
}

ZTEST(util_cxx, test_ARRAY_INDEX)
{
	run_ARRAY_INDEX();
}

ZTEST(util_cxx, test_PART_OF_ARRAY)
{
	run_PART_OF_ARRAY();
}

ZTEST(util_cxx, test_ARRAY_INDEX_FLOOR)
{
	run_ARRAY_INDEX_FLOOR();
}

ZTEST(util_cxx, test_BIT_MASK)
{
	run_BIT_MASK();
}

ZTEST(util_cxx, test_BIT_MASK64)
{
	run_BIT_MASK64();
}

ZTEST(util_cxx, test_IS_BIT_MASK)
{
	run_IS_BIT_MASK();
}

ZTEST(util_cxx, test_IS_SHIFTED_BIT_MASK)
{
	run_IS_SHIFTED_BIT_MASK();
}

ZTEST(util_cxx, test_DIV_ROUND_UP)
{
	run_DIV_ROUND_UP();
}

ZTEST(util_cxx, test_DIV_ROUND_CLOSEST)
{
	run_DIV_ROUND_CLOSEST();
}

ZTEST_SUITE(util_cxx, NULL, NULL, NULL, NULL, NULL);

#if __cplusplus
}
#endif

ZTEST(util_cc, test_u8_to_dec) {
	run_u8_to_dec();
}

ZTEST(util_cc, test_COND_CODE_1) {
	run_COND_CODE_1();
}

ZTEST(util_cc, test_COND_CODE_0) {
	run_COND_CODE_0();
}

ZTEST(util_cc, test_UTIL_OR) {
	run_UTIL_OR();
}

ZTEST(util_cc, test_UTIL_AND) {
	run_UTIL_AND();
}

ZTEST(util_cc, test_IF_ENABLED) {
	run_IF_ENABLED();
}

ZTEST(util_cc, test_LISTIFY) {
	run_LISTIFY();
}

ZTEST(util_cc, test_MACRO_MAP_CAT) {
	run_MACRO_MAP_CAT();
}

ZTEST(util_cc, test_z_max_z_min_z_clamp) {
	run_z_max_z_min_z_clamp();
}

ZTEST(util_cc, test_CLAMP) {
	run_CLAMP();
}

ZTEST(util_cc, test_IN_RANGE) {
	run_IN_RANGE();
}

ZTEST(util_cc, test_FOR_EACH) {
	run_FOR_EACH();
}

ZTEST(util_cc, test_FOR_EACH_NONEMPTY_TERM) {
	run_FOR_EACH_NONEMPTY_TERM();
}

ZTEST(util_cc, test_FOR_EACH_FIXED_ARG) {
	run_FOR_EACH_FIXED_ARG();
}

ZTEST(util_cc, test_FOR_EACH_IDX) {
	run_FOR_EACH_IDX();
}

ZTEST(util_cc, test_FOR_EACH_IDX_FIXED_ARG) {
	run_FOR_EACH_IDX_FIXED_ARG();
}

ZTEST(util_cc, test_IS_EMPTY) {
	run_IS_EMPTY();
}

ZTEST(util_cc, test_IS_EQ) {
	run_IS_EQ();
}

ZTEST(util_cc, test_LIST_DROP_EMPTY) {
	run_LIST_DROP_EMPTY();
}

ZTEST(util_cc, test_nested_FOR_EACH) {
	run_nested_FOR_EACH();
}

ZTEST(util_cc, test_GET_ARG_N) {
	run_GET_ARG_N();
}

ZTEST(util_cc, test_GET_ARGS_LESS_N) {
	run_GET_ARGS_LESS_N();
}

ZTEST(util_cc, test_mixing_GET_ARG_and_FOR_EACH) {
	run_mixing_GET_ARG_and_FOR_EACH();
}

ZTEST(util_cc, test_IS_ARRAY_ELEMENT)
{
	run_IS_ARRAY_ELEMENT();
}

ZTEST(util_cc, test_ARRAY_INDEX)
{
	run_ARRAY_INDEX();
}

ZTEST(util_cc, test_PART_OF_ARRAY)
{
	run_PART_OF_ARRAY();
}

ZTEST(util_cc, test_ARRAY_INDEX_FLOOR)
{
	run_ARRAY_INDEX_FLOOR();
}

ZTEST(util_cc, test_BIT_MASK)
{
	run_BIT_MASK();
}

ZTEST(util_cc, test_BIT_MASK64)
{
	run_BIT_MASK64();
}

ZTEST(util_cc, test_IS_BIT_MASK)
{
	run_IS_BIT_MASK();
}

ZTEST(util_cc, test_IS_SHIFTED_BIT_MASK)
{
	run_IS_SHIFTED_BIT_MASK();
}

ZTEST(util_cc, test_DIV_ROUND_UP)
{
	run_DIV_ROUND_UP();
}

ZTEST(util_cc, test_DIV_ROUND_CLOSEST)
{
	run_DIV_ROUND_CLOSEST();
}

ZTEST(util_cc, test_GET_EVEN_ARGS)
{
	run_GET_EVEN_ARGS();
}

ZTEST(util_cc, test_GET_ODD_ARGS)
{
	run_GET_ODD_ARGS();
}

ZTEST_SUITE(util_cc, NULL, NULL, NULL, NULL, NULL);
