#pragma once
#include <stdio.h>
#include <stdbool.h>
#include <climits>
#include "Cloud.h"
#include "Client.h"

bool isEmptyFile(const char* filename);
void deleteNFromTheTop(const char* filename, int N);
int getFirstInt(const char* filename);
void setupFiles();