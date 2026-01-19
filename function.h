#pragma once
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define MAXFILM 100
#define MAXTITL 30
#define MAXAUTL 30
#define SIZE sizeof(struct film)

struct film {
    char name[MAXTITL];
    char author[MAXAUTL];
    float score;
};

struct film;
void openning();
int load_count(FILE* fp);
char* s_gets(char* st, int n);
void index(int* pcount, FILE* pcinema);
void rank(int* pcount, FILE* pcinema);
void add(int* pcount, struct film cinema[MAXFILM], FILE* pcinema);
void del(int* pcount, struct film cinema[MAXFILM], FILE* pcinema);
void reset(int* pcount, FILE** pcinema);
void instructions();
void save_count(FILE* fp, int count);
void backup(FILE* pcinema);