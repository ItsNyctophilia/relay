#include <check.h>
#include <unistd.h>

#include "../src/signal_hdlr.h"

START_TEST(test_signal_handler)
{
	struct sigaction action = { 0 };
	action.sa_sigaction = sig_handler;
	int err = set_signals(&action);
	kill(getpid(), 1);
	ck_assert_int_eq(err, 0);
}
END_TEST

Suite *sig_test_suite(void)
{
	Suite *s1 = suite_create("Signals");
	TCase *tc1 = tcase_create("TC 1");

	tcase_add_test(tc1, test_signal_handler);

	suite_add_tcase(s1, tc1);

	return s1;
}
