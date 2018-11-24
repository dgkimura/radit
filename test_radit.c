#include <check.h>

#include "radit.h"

START_TEST(test_insert_then_search_an_element)
{
    struct radit_tree t;
    t.root = NULL;
    radit_insert(&t, "key", "value");

    ck_assert_str_eq("value", radit_search(&t, "key"));
}
END_TEST

START_TEST(test_two_inserts_then_search_two_elements)
{
    struct radit_tree t;
    t.root = NULL;
    radit_insert(&t, "key", "value");
    radit_insert(&t, "another_key", "another_value");

    ck_assert_str_eq("value", radit_search(&t, "key"));
    ck_assert_str_eq("another_value", radit_search(&t, "another_key"));
}
END_TEST

START_TEST(test_four_inserts_then_search_four_elements)
{
    struct radit_tree t;
    t.root = NULL;
    radit_insert(&t, "a_key", "a_value");
    radit_insert(&t, "b_key", "b_value");
    radit_insert(&t, "c_key", "c_value");
    radit_insert(&t, "d_key", "d_value");

    ck_assert_str_eq("a_value", radit_search(&t, "a_key"));
    ck_assert_str_eq("b_value", radit_search(&t, "b_key"));
    ck_assert_str_eq("c_value", radit_search(&t, "c_key"));
    ck_assert_str_eq("d_value", radit_search(&t, "d_key"));
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
    tcase_add_test(testcase, test_two_inserts_then_search_two_elements);
    tcase_add_test(testcase, test_four_inserts_then_search_four_elements);

    srunner_run_all(runner, CK_ENV);
    return 0;
}
