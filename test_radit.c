#include <check.h>

#include "radit.h"

START_TEST(test_insert_then_search_an_element)
{
    struct radit_tree t;
    t.root = NULL;
    radit_insert(&t, "key", "value");

    ck_assert_str_eq("value", radit_search(&t, "key"));
    ck_assert_ptr_eq(NULL, radit_search(&t, "fake_key"));
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

START_TEST(test_sixteen_inserts_then_search_sixteen_elements)
{
    struct radit_tree t;
    t.root = NULL;
    radit_insert(&t, "a_key", "a_value");
    radit_insert(&t, "b_key", "b_value");
    radit_insert(&t, "c_key", "c_value");
    radit_insert(&t, "d_key", "d_value");
    radit_insert(&t, "e_key", "e_value");
    radit_insert(&t, "f_key", "f_value");
    radit_insert(&t, "g_key", "g_value");
    radit_insert(&t, "h_key", "h_value");
    radit_insert(&t, "i_key", "i_value");
    radit_insert(&t, "j_key", "j_value");
    radit_insert(&t, "k_key", "k_value");
    radit_insert(&t, "l_key", "l_value");
    radit_insert(&t, "m_key", "m_value");
    radit_insert(&t, "n_key", "n_value");
    radit_insert(&t, "o_key", "o_value");
    radit_insert(&t, "p_key", "p_value");

    ck_assert_str_eq("a_value", radit_search(&t, "a_key"));
    ck_assert_str_eq("b_value", radit_search(&t, "b_key"));
    ck_assert_str_eq("c_value", radit_search(&t, "c_key"));
    ck_assert_str_eq("d_value", radit_search(&t, "d_key"));
    ck_assert_str_eq("e_value", radit_search(&t, "e_key"));
    ck_assert_str_eq("f_value", radit_search(&t, "f_key"));
    ck_assert_str_eq("g_value", radit_search(&t, "g_key"));
    ck_assert_str_eq("h_value", radit_search(&t, "h_key"));
    ck_assert_str_eq("i_value", radit_search(&t, "i_key"));
    ck_assert_str_eq("j_value", radit_search(&t, "j_key"));
    ck_assert_str_eq("k_value", radit_search(&t, "k_key"));
    ck_assert_str_eq("l_value", radit_search(&t, "l_key"));
    ck_assert_str_eq("m_value", radit_search(&t, "m_key"));
    ck_assert_str_eq("n_value", radit_search(&t, "n_key"));
    ck_assert_str_eq("o_value", radit_search(&t, "o_key"));
    ck_assert_str_eq("p_value", radit_search(&t, "p_key"));
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
    //tcase_add_test(testcase, test_two_inserts_then_search_two_elements);
    //tcase_add_test(testcase, test_four_inserts_then_search_four_elements);
    //tcase_add_test(testcase, test_sixteen_inserts_then_search_sixteen_elements);

    srunner_run_all(runner, CK_ENV);
    return 0;
}
