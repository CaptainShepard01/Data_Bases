#pragma once
#include "File_functions.h"

bool isEmptyFile(const char* filename) {
	FILE* file;

	fopen_s(&file, filename, "rb");

	int x;

	if (fread_s(&x, sizeof(int), sizeof(int), 1, file) == 0) {
		if (file)
			fclose(file);
		return true;
	}

	if (file)
		fclose(file);
	return false;
}

void deleteNFromTheTop(const char* filename, int N) {
	FILE* file;
	fopen_s(&file, filename, "rb");

	if (file)
		fseek(file, N * sizeof(int), SEEK_SET);
	else
		return;

	FILE* temp;
	if (fopen_s(&temp, "temp", "wb") != 0)
		return;

	int tempNum = 0;

	while (file && temp && fread_s(&tempNum, sizeof(int), sizeof(int), 1, file))
		fwrite(&tempNum, sizeof(int), 1, temp);


	fclose(temp);
	fclose(file);


	rename(filename, "garbage");
	rename("temp", filename);

	remove("garbage");
}

int getFirstInt(const char* filename) {
	FILE* file;
	fopen_s(&file, filename, "rb");
	int num = 0;
	if (file) {
		fread_s(&num, sizeof(int), sizeof(int), 1, file);
		fclose(file);
	}
	else
		return INT_MAX;
	deleteNFromTheTop(filename, 1);

	return num;
}

void setupFiles() {
	FILE* master, * slave, * gMaster, * gSlave;

	fopen_s(&master, FILE_M, "ab");
	fopen_s(&slave, FILE_S, "ab");
	fopen_s(&gMaster, FILE_M_G, "ab");	
	fopen_s(&gSlave, FILE_S_G, "ab");


	fclose(master);
	fclose(slave);
	fclose(gMaster);
	fclose(gSlave);

}