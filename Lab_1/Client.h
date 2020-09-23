#pragma once
#include <stdbool.h>
#include "Cloud.h"
#include "File_functions.h"

#define FILE_M "clouds"
#define FILE_S "clients"

#define FILE_S_G "clients_garbage"

#define DEFAULT_CLIENT {.ID = -1, .fullName = NULL, /*.dataSize = -1,*/  \
.isDeleted = true, .cloudID = -1, .prev = -1, .next = -1}

typedef struct {
	int ID;
	char fullName[30];
	//int dataSize;
	bool isDeleted;
	int cloudID;

	int prev;
	int next;
}Client;

Client get_s(const int mKey, const int sKey);
void del_s(const int sKey);
void print_s(Client client);
void ut_s();
int count_s();
int count_s_NDeleted();
Client initialize_client();
void insert_s();
void update_s(int mKey, int sKey);

