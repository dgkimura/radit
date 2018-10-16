#include <check.h>

#include "radit.h"

START_TEST(test_x)
{
}
END_TEST

int
main(void)
{
    Suite *suite = suite_create("radit");
    TCase *testcase = tcase_create("radit");
    SRunner *runner = srunner_create(suite);

    suite_add_tcase(suite, testcase);
    tcase_add_test(testcase, test_x);

    srunner_run_all(runner, CK_ENV);
    return 0;
}
