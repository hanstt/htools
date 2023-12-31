/*
 * Copyright (c) 2014-2019, 2023-2024,
 * Hans Toshihide Törnqvist <hans.tornqvist@gmail.com>
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

#include <htest/htest.h>
#include <assert.h>
#include <fcntl.h>
#include <signal.h>
#include <stdarg.h>
#include <hutils/getopt.h>
#include <hutils/string.h>
#include <src/htest.h>

#if defined(HCONF_mHTEST_bPOSIX)
#	define SUPPORT_FORK
#	define BLUE "\033[1;34m"
#	define GREEN "\033[1;32m"
#	define RED "\033[1;31m"
#	define RESET "\033[0m"
char const c_nul_path[] = "/dev/null";
#elif defined(_MSC_VER)
#	include <io.h>
#	define SUPPORT_JMP
#	define close _close
#	define dup _dup
#	define dup2 _dup2
#	define fdopen _fdopen
#	define open _open
#	define STDERR_FILENO 1
#	define STDOUT_FILENO 2
#	define BLUE (FOREGROUND_BLUE | FOREGROUND_GREEN)
#	define GREEN FOREGROUND_GREEN
#	define RED (FOREGROUND_RED | FOREGROUND_INTENSITY)
char const c_nul_path[] = "NUL";
static jmp_buf g_suite_jmp_buf;
jmp_buf g_htest_try_jmp_buf_;
static HANDLE g_htest_console_;
static WORD RESET;
#endif

static void suite_sighandler(int);
static void try_sighandler(int);
static void usage(int, char const *) FUNC_NONNULL(()) FUNC_NORETURN;

static int g_nul, g_old_stderr, g_old_stdout;
static int g_do_colors, g_do_recover, g_do_verbose;
static HTEST_COLOR_ const c_color_suite = GREEN;
static HTEST_COLOR_ const c_color_test = BLUE;
static HTEST_COLOR_ const c_color_fail = RED;

char const *g_htry_void_file_;
unsigned g_htry_void_line_;

extern struct HTestSuite g_htest_suite_list_[];

void
htest_cov_flush_(void)
{
#ifdef HTEST_DO_COV_FLUSH
	HTEST_COV_FLUSH_;
#endif
}

void
suite_sighandler(int a_signum)
{
	htest_output_restore_();
	htest_set_color_(c_color_fail);
	printf("  Fail:");
	htest_set_color_(RESET);
	if (NULL != g_htry_void_file_) {
		printf("%s:%u:", g_htry_void_file_, g_htry_void_line_);
		g_htry_void_file_ = NULL;
	}
	printf(" Caught signal \"%s\".\n", strsignal(a_signum));
#if defined(SUPPORT_JMP)
	htest_output_suppress_();
	longjmp(g_suite_jmp_buf, 1);
#else
	htest_cov_flush_();
	_exit(EXIT_FAILURE);
#endif
}

void
try_sighandler(int a_signum)
{
	(void)a_signum;
#if defined(SUPPORT_JMP)
	longjmp(g_htest_try_jmp_buf_, 1);
#else
	htest_cov_flush_();
	_exit(EXIT_FAILURE);
#endif
}

int
htest_do_recover(void)
{
	return g_do_recover;
}

void
htest_output_restore_(void)
{
	if (g_do_verbose) {
		return;
	}
	dup2(g_old_stdout, STDOUT_FILENO);
	dup2(g_old_stderr, STDERR_FILENO);
}

void
htest_output_suppress_(void)
{
	if (g_do_verbose) {
		return;
	}
	fflush(stdout);
	dup2(g_nul, STDOUT_FILENO);
	fflush(stderr);
	dup2(g_nul, STDERR_FILENO);
}

void
htest_set_color_(HTEST_COLOR_ a_color)
{
	if (!g_do_colors) {
		return;
	}
#if defined(_MSC_VER)
	SetConsoleTextAttribute(g_htest_console_, a_color);
#else
	printf("%s", a_color);
#endif
}

void
htest_suite_install_sighandler_(void)
{
	signal(SIGABRT, suite_sighandler);
#if defined(SIGBUS)
	signal(SIGBUS, suite_sighandler);
#endif
	signal(SIGABRT, suite_sighandler);
	signal(SIGFPE, suite_sighandler);
	signal(SIGILL, suite_sighandler);
	signal(SIGSEGV, suite_sighandler);
}

void
htest_try_install_sighandler_(void)
{
	signal(SIGABRT, try_sighandler);
#if defined(SIGBUS)
	signal(SIGBUS, try_sighandler);
#endif
	signal(SIGFPE, try_sighandler);
	signal(SIGILL, try_sighandler);
	signal(SIGSEGV, try_sighandler);
}

void
usage(int a_exit_code, char const *a_argv0)
{
	FILE *str;

	str = EXIT_SUCCESS == a_exit_code ? stdout : stderr;
	fprintf(str, "Usage: %s [-c] [-h] [-r] [-s] [-v]\n", a_argv0);
	fprintf(str, " -c  Enable colored output.\n");
	fprintf(str, " -h  Show this.\n");
	fprintf(str, " -r  Disable failure recovery (fork/longjmp).\n");
	fprintf(str, " -v  Show what tested code prints to stdout/stderr.\n");
	exit(a_exit_code);
}

int
main(int const argc, char **const argv)
{
	struct HTestSuite *suite;
	unsigned test_num, test_pass_num;
	int opt;

	g_do_colors = 0;
	g_do_verbose = 0;
	g_do_recover = 1;
	while (-1 != (opt = getopt(argc, argv, "chrv"))) {
		switch (opt) {
		case 'c':
			g_do_colors = 1;
			break;
		case 'h':
			usage(EXIT_SUCCESS, argv[0]);
		case 'r':
			g_do_recover = 0;
			break;
		case 'v':
			g_do_verbose = 1;
			break;
		default:
			usage(EXIT_FAILURE, argv[0]);
		}
	}

	g_nul = open(c_nul_path, O_WRONLY);
	g_old_stdout = dup(STDOUT_FILENO);
	g_old_stderr = dup(STDERR_FILENO);
#if defined(_MSC_VER)
	_set_abort_behavior(0, _WRITE_ABORT_MSG);
	{
		CONSOLE_SCREEN_BUFFER_INFO info;

		g_htest_console_ = GetStdHandle(STD_OUTPUT_HANDLE);
		GetConsoleScreenBufferInfo(g_htest_console_, &info);
		RESET = info.wAttributes;
	}
#endif
	htest_suite_install_sighandler_();
	test_num = 0;
	test_pass_num = 0;
	for (suite = g_htest_suite_list_; NULL != suite->header; ++suite) {
		unsigned test_enumerator;
		unsigned test_index;

		suite->header(c_color_suite, RESET);

		test_enumerator = 0;
		suite->suite(RESET, RESET, RESET, 0, &test_enumerator, NULL);
		if (0 == test_enumerator) {
			continue;
		}
		test_num += test_enumerator;

		for (test_index = 1; test_enumerator >= test_index;
		    ++test_index) {
			if (g_do_recover) {
#if defined(SUPPORT_FORK)
				pid_t pid;
				int status;

				fflush(stdout);
				fflush(stderr);
				pid = fork();
				if (0 > pid) {
					hutils_err(EXIT_FAILURE, "fork");
				} else if (0 == pid) {
					int passed;

					test_enumerator = 0;
					passed = 1;
					suite->suite(c_color_test,
					    c_color_fail, RESET, test_index,
					    &test_enumerator, &passed);
					htest_cov_flush_();
					_exit(passed ? EXIT_SUCCESS :
					    EXIT_FAILURE);
				}
				waitpid(pid, &status, 0);
				if (EXIT_SUCCESS != WEXITSTATUS(status)) {
					htest_set_color_(c_color_fail);
					printf("  Fail:");
					htest_set_color_(RESET);
					printf("Exited.\n");
				} else {
					++test_pass_num;
				}
#elif defined(SUPPORT_JMP)
				/*
				 * This is crap, but some OS:s can't do
				 * better...
				 */
				int passed;

				if (0 == setjmp(g_suite_jmp_buf)) {
					test_enumerator = 0;
					passed = 1;
					suite->suite(c_color_test,
					    c_color_fail, RESET, test_index,
					    &test_enumerator, &passed);
				} else {
					htest_set_color_(c_color_fail);
					printf("  Fail:");
					htest_set_color_(RESET);
					printf("Jumped.\n");
					passed = 0;
				}
				if (passed) {
					++test_pass_num;
				}
#endif
			} else {
				int passed;

				test_enumerator = 0;
				passed = 1;
				suite->suite(c_color_test, c_color_fail,
				    RESET, test_index, &test_enumerator,
				    &passed);
				if (passed) {
					++test_pass_num;
				}
			}
		}
	}
	close(g_nul);
	close(g_old_stdout);
	close(g_old_stderr);

	printf("Passed %u/%u (%.1f%%) tests.\n", test_pass_num, test_num,
	    100.0f * test_pass_num / test_num);
	exit(test_pass_num == test_num ? EXIT_SUCCESS : EXIT_FAILURE);
}
