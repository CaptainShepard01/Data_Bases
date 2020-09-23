#pragma once
#include <stdbool.h>
#include <string.h>

#include "Client.h"
#include "File_functions.h"

#define FILE_M "clouds"
#define FILE_S "clients"

#define FILE_M_G "clouds_garbage"

#define DEFAULT_CLOUD {.ID = -1, .company = NULL, .size = NULL, .clientNumb = 0, \
.isDeleted = true, .firstClient = -1}


typedef struct {
	int ID;
	char company[50];
	char size[10];
	int clientNumb;
	bool isDeleted;
	int firstClient;
}Cloud;

Cloud get_m(const int mKey);
void del_m(const int mKey);
void print_m(Cloud cloud);
void ut_m();
int count_m();
int count_s_m(const int mKey);
int count_m_NDeleted();
Cloud initialize_cloud();
void insert_m();
void insert_m();
void update_m(int mKey);
void ut_m_s(const int mKey);