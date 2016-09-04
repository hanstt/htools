/*
 * Copyright (c) 2014-2016 Hans Toshihide Törnqvist <hans.tornqvist@gmail.com>
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#ifndef HTEST_HTEST_H
#define HTEST_HTEST_H

#include <sys/types.h>
#if defined(_MSC_VER)
#	define HTEST_COLOR_ WORD
#else
#	include <sys/wait.h>
#	include <unistd.h>
#	define HTEST_COLOR_ char const *
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <hutils/err.h>
#include <hutils/macros.h>

#if defined(DO_GCOV_FLUSH)
#	define GCOV_FLUSH __gcov_flush()
void __gcov_flush(void);
#else
#	define GCOV_FLUSH
#endif

struct HTestSuite {
	void	(*header)(HTEST_COLOR_, HTEST_COLOR_);
	void	(*suite)(HTEST_COLOR_, HTEST_COLOR_, HTEST_COLOR_, int, int *,
	    int *);
};

/* Test. */

#define HTEST(name)\
static void \
htest_test_header_##name##_(HTEST_COLOR_ a_color_header_, HTEST_COLOR_\
    a_color_reset_)\
{\
	htest_set_color_(a_color_header_);\
	printf(" Test("__FILE__":%d:"#name")\n", __LINE__);\
	htest_set_color_(a_color_reset_);\
}\
static void \
htest_test_##name##_(HTEST_COLOR_ a_color_fail_, HTEST_COLOR_ a_color_reset_,\
    int *a_result_)

/* Suite. */

#define HTEST_SUITE_PROTO(name)\
void htest_suite_header_##name##_(HTEST_COLOR_, HTEST_COLOR_);\
void htest_suite_##name##_(HTEST_COLOR_, HTEST_COLOR_, HTEST_COLOR_, int,\
    int *, int *)
#define HTEST_SUITE(name)\
HTEST_SUITE_PROTO(name);\
void \
htest_suite_header_##name##_(HTEST_COLOR_ a_color_header_, HTEST_COLOR_\
    a_color_reset_)\
{\
	htest_set_color_(a_color_header_);\
	printf("Suite("__FILE__":%d:"#name")\n", __LINE__);\
	htest_set_color_(a_color_reset_);\
}\
void \
htest_suite_##name##_(HTEST_COLOR_ a_color_header_, HTEST_COLOR_ a_color_fail_,\
    HTEST_COLOR_ a_color_reset_, int a_test_index_, int *a_test_enumerator_,\
    int *a_result_)
#define HTEST_ADD(name) do {\
	++*a_test_enumerator_;\
	if (*a_test_enumerator_ == a_test_index_) {\
		htest_test_header_##name##_(a_color_header_, a_color_reset_);\
		htest_output_suppress_();\
		htest_test_##name##_(a_color_fail_, a_color_reset_,\
		    a_result_);\
		htest_output_restore_();\
	}\
} WHILE_0

/* Generated suite list. */

#define HTEST_SUITE_LIST_BEGIN \
struct HTestSuite g_htest_suite_list_[] = {
#define HTEST_SUITE_LIST_ADD(name) \
	{htest_suite_header_##name##_, htest_suite_##name##_},
#define HTEST_SUITE_LIST_END \
	{NULL, NULL}\
};

/* Tests. */

#define HTRY_FAIL_HEADER_ do {\
	htest_output_restore_();\
	htest_set_color_(a_color_fail_);\
	printf("  Fail:");\
	htest_set_color_(a_color_reset_);\
	printf(__FILE__":%d: ", __LINE__);\
} WHILE_0
#define HTRY_FAIL_FOOTER_ do {\
	htest_output_suppress_();\
	*a_result_ = 0;\
} WHILE_0

#define HTRY(Type, fmt, a, op, b) do {\
	Type const aa_ = a;\
	Type const bb_ = b;\
	IF_CONST(!(aa_ op bb_)) {\
		HTRY_FAIL_HEADER_;\
		printf("'"#a"'=%"#fmt" "#op" '"#b\
		    "'=%"#fmt"\n", aa_, bb_);\
		HTRY_FAIL_FOOTER_;\
	}\
} WHILE_0
#define HTRY_DBL(a, op, b) HTRY(double, e, a, op, b)
#define HTRY_FLT(a, op, b) HTRY(float, e, a, op, b)
#define HTRY_I(a, op, b) HTRY(int, d, a, op, b)
#define HTRY_PTR(a, op, b) HTRY(void const *, p, a, op, b)
#define HTRY_U(a, op, b) HTRY(unsigned int, u, a, op, b)

#define HTRY_BOOL(expr) do {\
	IF_CONST(!(expr)) {\
		HTRY_FAIL_HEADER_;\
		printf("'"#expr"'\n");\
		HTRY_FAIL_FOOTER_;\
	}\
} WHILE_0

#define HTRY_STR(a, op, b) do {\
	char const *aa_ = a;\
	char const *bb_ = b;\
	if (NULL == aa_) {\
		HTRY_FAIL_HEADER_;\
		printf("'%s'=\"%s\" "#op" '%s'"\
		    "=\"%s\".\n", #a, aa_, #b, bb_);\
		HTRY_FAIL_FOOTER_;\
	} else if (NULL == bb_) {\
		HTRY_FAIL_HEADER_;\
		printf("'%s'=\"%s\" "#op" '%s'"\
		    "=\"%s\".\n", #a, aa_, #b, bb_);\
		HTRY_FAIL_FOOTER_;\
	} else if (!(strcmp(aa_, bb_) op 0)) {\
		HTRY_FAIL_HEADER_;\
		printf("'%s'=\"%s\" "#op" '%s'"\
			"=\"%s\".\n", #a, aa_, #b, bb_); \
		HTRY_FAIL_FOOTER_; \
	}\
} WHILE_0

#define HTRY_VOID(expr) do {\
	(void)a_color_fail_;\
	(void)a_color_reset_;\
	(void)a_result_;\
	expr;\
} WHILE_0
#if defined(_MSC_VER)
#	include <setjmp.h>
#	define HTRY_SIGNAL_DTOR(expr, dtor) do {\
	if (0 == setjmp(g_htest_try_jmp_buf_)) {\
		g_htest_dtor_ = dtor;\
		htest_try_install_sighandler_();\
		expr;\
		g_htest_dtor_();\
		GCOV_FLUSH;\
		HTRY_FAIL_HEADER_;\
		printf("Expected signal.\n");\
		HTRY_FAIL_FOOTER_;\
	}\
htest_output_suppress_();\
	htest_suite_install_sighandler_();\
} WHILE_0
extern jmp_buf g_htest_try_jmp_buf_;
#else
#	define HTRY_SIGNAL_DTOR(expr, dtor) do {\
	pid_t pid_;\
	int status_;\
	pid_ = fork();\
	if (0 > pid_) {\
		err(EXIT_FAILURE, "fork");\
	} else if (0 == pid_) {\
		g_htest_dtor_ = dtor;\
		htest_try_install_sighandler_();\
		expr;\
		g_htest_dtor_();\
		GCOV_FLUSH;\
		_exit(EXIT_SUCCESS);\
	}\
	waitpid(pid_, &status_, 0);\
	if (0 == status_) {\
		HTRY_FAIL_HEADER_;\
		printf("Expected signal.\n");\
		HTRY_FAIL_FOOTER_;\
	}\
} WHILE_0
#endif
#define HTRY_SIGNAL(expr) HTRY_SIGNAL_DTOR(expr, htest_dtor_noop_)

void	htest_dtor_noop_(void);
void	htest_output_restore_(void);
void	htest_output_suppress_(void);
void	htest_set_color_(HTEST_COLOR_);
void	htest_suite_install_sighandler_(void);
void	htest_try_install_sighandler_(void);

extern void	(*g_htest_dtor_)(void);

#endif
