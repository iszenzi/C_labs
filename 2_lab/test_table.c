#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <assert.h>
#include <stdbool.h>
#include "table.h"

static char *random_string(int min_len, int max_len)
{
    int len = rand() % (max_len - min_len + 1) + min_len;
    char *s = malloc(len + 1);
    if (!s)
        return NULL;
    for (int i = 0; i < len; ++i)
        s[i] = (char)('a' + rand() % 26);
    s[len] = '\0';
    return s;
}

static void assert_sorted(const MyStruct *t, int n)
{
    for (int i = 1; i < n; ++i)
        assert(t[i - 1].key <= t[i].key);
}

void test1_shell_sort_random()
{
    int n = 28;
    MyStruct *t = calloc(n, sizeof(MyStruct));
    assert(t != NULL);
    for (int i = 0; i < n; ++i)
    {
        t[i].key = (double)(rand() % 900) / 10.0;
        t[i].value = random_string(5, 12);
        assert(t[i].value != NULL);
    }
    shell_sort(t, n);
    assert_sorted(t, n);
    free_table(t, n);
}

void test2_shell_sort_reverse()
{
    int n = 24;
    MyStruct *t = calloc(n, sizeof(MyStruct));
    assert(t != NULL);
    for (int i = 0; i < n; ++i)
    {
        t[i].key = (double)(n - i);
        t[i].value = random_string(3, 8);
    }
    shell_sort(t, n);
    assert_sorted(t, n);
    assert(t[0].key == 1.0);
    assert(t[n - 1].key == (double)n);
    free_table(t, n);
}

void test3_shell_sort_sorted()
{
    int n = 16;
    MyStruct *t = calloc(n, sizeof(MyStruct));
    assert(t != NULL);
    for (int i = 0; i < n; ++i)
    {
        t[i].key = (double)i;
        t[i].value = random_string(3, 8);
    }
    shell_sort(t, n);
    assert_sorted(t, n);
    assert(t[0].key == 0.0);
    assert(t[n - 1].key == (double)(n - 1));
    free_table(t, n);
}

void test4_binary_search()
{
    int n = 32;
    MyStruct *t = calloc(n, sizeof(MyStruct));
    assert(t != NULL);
    for (int i = 0; i < n; ++i)
    {
        t[i].key = (double)i;
        t[i].value = random_string(3, 8);
    }
    int idx = binary_search(t, n, 10.0);
    assert(idx == 10);
    idx = binary_search(t, n, -1.0);
    assert(idx == -1);
    free_table(t, n);
}

void test5_file_io_simple()
{
    MyStruct *table = calloc(15, sizeof(MyStruct));
    assert(table != NULL);
    int n = read_table(table, 15, "random.txt");

    assert(n >= 15);
    assert(table[0].value != NULL);

    shell_sort(table, n);
    assert_sorted(table, n);

    int idx = binary_search(table, n, table[n / 2].key);
    assert(idx >= 0);

    free_table(table, n);
}

bool all_tests()
{
    test1_shell_sort_random();
    printf("Test 1 complete\n");
    test2_shell_sort_reverse();
    printf("Test 2 complete\n");
    test3_shell_sort_sorted();
    printf("Test 3 complete\n");
    test4_binary_search();
    printf("Test 4 complete\n");
    test5_file_io_simple();
    printf("Test 5 complete\n");
    return true;
}

int main()
{
    all_tests();
    return 0;
}
