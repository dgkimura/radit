#include <check.h>

#include "radit.h"

START_TEST(test_insert_then_search_an_element)
{
    struct radit_tree t;
    t.root = NULL;
    radit_insert(&t, (const unsigned char *)"key", "value");

    ck_assert_str_eq("value", radit_search(&t, (const unsigned char *)"key"));
}
END_TEST

int
main(void)
{
    Suite *suite = suite_create("radit");
    TCase *testcase = tcase_create("radit");
    SRunner *runner = srunner_create(suite);

    suite_add_tcase(suite, testcase);
    tcase_add_test(testcase, test_insert_then_search_an_element);

    srunner_run_all(runner, CK_ENV);
    return 0;
}
