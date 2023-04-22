#include <check.h>
#include <unistd.h>

#include "../src/dispatcher/socket_server.h"

START_TEST(test_server)
{
	int err_rtn = 0;
	struct socket_server *server = create_server(&err_rtn);

	ck_assert(server);

	int err = destroy_server(server);
	ck_assert_int_eq(err, 0);
}
END_TEST

Suite *server_test_suite(void)
{
	Suite *s1 = suite_create("Server");
	TCase *tc1 = tcase_create("TC 1");

	tcase_add_test(tc1, test_server);

	suite_add_tcase(s1, tc1);

	return s1;
}