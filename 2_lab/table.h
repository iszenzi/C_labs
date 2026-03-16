#ifndef TABLE_H
#define TABLE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_STR_LEN 256

typedef struct
{
    double key;
    char *value;
} MyStruct;

void free_table(MyStruct *table, int size);
char *read_line(FILE *file);
int read_table(MyStruct *table, int limit, const char *filename);
void print_table(const MyStruct *table, int size);
void shell_sort(MyStruct *table, int size);
int binary_search(const MyStruct *table, int size, double key);
void swap(MyStruct *a, MyStruct *b);

#endif