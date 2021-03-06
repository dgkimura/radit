#include <check.h>

#include "radit.h"

START_TEST(test_search_an_empty_tree)
{
    struct radit_tree t;
    radit_init(&t);

    ck_assert_ptr_eq(NULL, radit_search(&t, "fake_key"));
}
END_TEST

START_TEST(test_insert_then_search_an_element)
{
    struct radit_tree t;
    radit_init(&t);

    radit_insert(&t, "key", "value");

    ck_assert_str_eq("value", radit_search(&t, "key"));
}
END_TEST

START_TEST(test_two_inserts_then_search_two_elements)
{
    struct radit_tree t;
    radit_init(&t);

    radit_insert(&t, "key", "value");
    radit_insert(&t, "another_key", "another_value");

    ck_assert_str_eq("value", radit_search(&t, "key"));
    ck_assert_str_eq("another_value", radit_search(&t, "another_key"));
}
END_TEST

START_TEST(test_insert_element_and_update_value_pointer_then_search_element)
{
    char *k = "key";
    char v[] = "initial_value";

    struct radit_tree t;
    radit_init(&t);

    radit_insert(&t, k, v);

    memcpy(v, "next_value", 11);

    ck_assert_str_eq("next_value", radit_search(&t, "key"));
}
END_TEST

START_TEST(test_two_inserts_with_common_prefix_then_search_two_elements)
{
    struct radit_tree t;
    radit_init(&t);

    radit_insert(&t, "key_first", "value_first");
    radit_insert(&t, "key_second", "value_second");

    ck_assert_str_eq("value_first", radit_search(&t, "key_first"));
    ck_assert_str_eq("value_second", radit_search(&t, "key_second"));
}
END_TEST

START_TEST(test_string_and_substring_insert_then_search_two_elements)
{
    struct radit_tree t;
    radit_init(&t);

    radit_insert(&t, "the_beginning_of_a_long_string", "value_1");
    radit_insert(&t, "the_beginning", "value_0");

    ck_assert_str_eq("value_1", radit_search(&t, "the_beginning_of_a_long_string"));
    ck_assert_str_eq("value_0", radit_search(&t, "the_beginning"));
}
END_TEST

START_TEST(test_substring_and_string_insert_then_search_elements)
{
    struct radit_tree t;
    radit_init(&t);

    radit_insert(&t, "the_beginning", "value_0");
    radit_insert(&t, "the_beginning_of_a_long_string", "value_1");

    ck_assert_str_eq("value_0", radit_search(&t, "the_beginning"));
    ck_assert_str_eq("value_1", radit_search(&t, "the_beginning_of_a_long_string"));
}
END_TEST

START_TEST(test_insert_three_elemnts_then_search_elements)
{
    struct radit_tree t;
    radit_init(&t);

    radit_insert(&t, "a_key", "a_value");
    radit_insert(&t, "b_key", "b_value");
    radit_insert(&t, "c_key", "c_value");

    ck_assert_str_eq("a_value", radit_search(&t, "a_key"));
    ck_assert_str_eq("b_value", radit_search(&t, "b_key"));
    ck_assert_str_eq("c_value", radit_search(&t, "c_key"));
}
END_TEST

START_TEST(test_insert_nomatching_prefix_followed_by_matching_prefix_and_then_search_elements)
{
    struct radit_tree t;
    radit_init(&t);

    /* First insert 2 elements with no-common prefix 'a' and 't' */
    radit_insert(&t, "a_key_with_no_common_prefix", "value_0");
    radit_insert(&t, "the_start", "value_1");

    /* Then insert sub-elemnt that extends previously inserted element */
    radit_insert(&t, "the_start_of_a_long_string", "value_2");

    /* Then insert sub-elemnt that is substring of previously inserted element */
    radit_insert(&t, "the", "value_3");

    ck_assert_str_eq("value_0", radit_search(&t, "a_key_with_no_common_prefix"));
    ck_assert_str_eq("value_1", radit_search(&t, "the_start"));
    ck_assert_str_eq("value_2", radit_search(&t, "the_start_of_a_long_string"));
    ck_assert_str_eq("value_3", radit_search(&t, "the"));
}
END_TEST

START_TEST(test_sixteen_inserts_then_search_sixteen_elements)
{
    struct radit_tree t;
    radit_init(&t);

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

START_TEST(test_delete_from_empty_tree)
{
    struct radit_tree t;
    radit_init(&t);

    radit_delete(&t, "key");

    ck_assert_ptr_eq(NULL, radit_search(&t, "key"));
}
END_TEST

START_TEST(test_delete_from_one_element_tree)
{
    struct radit_tree t;
    radit_init(&t);

    radit_insert(&t, "key", "value");
    ck_assert_str_eq("value", radit_search(&t, "key"));

    radit_delete(&t, "fake_key");
    ck_assert_str_eq("value", radit_search(&t, "key"));

    radit_delete(&t, "key");
    ck_assert_ptr_eq(NULL, radit_search(&t, "key"));
}
END_TEST

START_TEST(test_delete_from_two_element_tree)
{
    struct radit_tree t;
    radit_init(&t);

    radit_insert(&t, "a_key", "a_value");
    radit_insert(&t, "b_key", "b_value");
    ck_assert_str_eq("a_value", radit_search(&t, "a_key"));
    ck_assert_str_eq("b_value", radit_search(&t, "b_key"));

    radit_delete(&t, "a_key");
    ck_assert_ptr_eq(NULL, radit_search(&t, "a_key"));
    ck_assert_str_eq("b_value", radit_search(&t, "b_key"));
}
END_TEST

START_TEST(test_delete_superstring_from_tree)
{
    struct radit_tree t;
    radit_init(&t);

    radit_insert(&t, "the_beginning", "value_0");
    radit_insert(&t, "the_beginning_of_a_long_string", "value_1");

    ck_assert_str_eq("value_0", radit_search(&t, "the_beginning"));
    ck_assert_str_eq("value_1", radit_search(&t, "the_beginning_of_a_long_string"));

    radit_delete(&t, "the_beginning_of_a_long_string");
    ck_assert_ptr_eq(NULL, radit_search(&t, "the_beginning_of_a_long_string"));
    ck_assert_str_eq("value_0", radit_search(&t, "the_beginning"));
}
END_TEST

START_TEST(test_delete_substring_from_tree)
{
    struct radit_tree t;
    radit_init(&t);

    radit_insert(&t, "the_beginning", "value_0");
    radit_insert(&t, "the_beginning_of_a_long_string", "value_1");

    ck_assert_str_eq("value_0", radit_search(&t, "the_beginning"));
    ck_assert_str_eq("value_1", radit_search(&t, "the_beginning_of_a_long_string"));

    radit_delete(&t, "the_beginning");
    ck_assert_ptr_eq(NULL, radit_search(&t, "the_beginning"));
    ck_assert_str_eq("value_1", radit_search(&t, "the_beginning_of_a_long_string"));
}
END_TEST

START_TEST(test_destroy_deletes_all_elements_from_a_tree)
{
    struct radit_tree t;
    radit_init(&t);

    radit_insert(&t, "key", "value");
    radit_insert(&t, "another_key", "another_value");
    radit_insert(&t, "another_another_key", "another_another_value");

    radit_destroy(&t);

    ck_assert_ptr_eq(NULL, radit_search(&t, "key"));
    ck_assert_ptr_eq(NULL, radit_search(&t, "another_key"));
    ck_assert_ptr_eq(NULL, radit_search(&t, "another_another_key"));
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
    tcase_add_test(testcase, test_search_an_empty_tree);
    tcase_add_test(testcase, test_two_inserts_then_search_two_elements);
    tcase_add_test(testcase, test_insert_element_and_update_value_pointer_then_search_element);
    tcase_add_test(testcase, test_two_inserts_with_common_prefix_then_search_two_elements);
    tcase_add_test(testcase, test_string_and_substring_insert_then_search_two_elements);
    tcase_add_test(testcase, test_substring_and_string_insert_then_search_elements);
    tcase_add_test(testcase, test_insert_three_elemnts_then_search_elements);
    tcase_add_test(testcase, test_insert_nomatching_prefix_followed_by_matching_prefix_and_then_search_elements);
    tcase_add_test(testcase, test_sixteen_inserts_then_search_sixteen_elements);
    tcase_add_test(testcase, test_delete_from_empty_tree);
    tcase_add_test(testcase, test_delete_from_one_element_tree);
    tcase_add_test(testcase, test_delete_from_two_element_tree);
    tcase_add_test(testcase, test_delete_superstring_from_tree);
    tcase_add_test(testcase, test_delete_substring_from_tree);
    tcase_add_test(testcase, test_destroy_deletes_all_elements_from_a_tree);

    srunner_run_all(runner, CK_ENV);
    return 0;
}
