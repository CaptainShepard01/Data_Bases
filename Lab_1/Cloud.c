#include <stdio.h>
#include "Cloud.h"
#include <climits>

void print_m(Cloud cloud) {
	if (cloud.isDeleted != true)
		printf("%-15d%-51s%-15s%-15d%-15d%-15d", cloud.ID, cloud.company, cloud.size, cloud.clientNumb, cloud.isDeleted, cloud.firstClient);
	else
		printf("REMOVED");
}

void ut_m() {
	FILE* master;
	fopen_s(&master, FILE_M, "rb");
	Cloud cloud = DEFAULT_CLOUD;

	printf("Clouds:\n");

	char ID[] = "ID";
	char company[] = "Company Name";
	char size[] = "Size";
	char clientNmb[] = "Client Number";
	char isDeleted[] = "IsDeleted";
	char firstClient[] = "First Client";

	printf("%-15s%-51s%-15s%-15s%-15s%-15s\n", ID, company, size, clientNmb, isDeleted, firstClient);

	while (master && fread_s(&cloud, sizeof(Cloud), sizeof(Cloud), 1, master)) {
		print_m(cloud);
		printf("\n");
	}
	printf("\n");

	fclose(master);
}

void ut_m_s(const int mKey) {
	Cloud cloud = get_m(mKey);
	int curOrig = cloud.firstClient;	

	char ID[] = "ID";
	char name[] = "Full Name";
	char isDeleted[] = "IsDeleted";
	char cloudID[] = "Cloud ID";
	char prev[] = "Previous";
	char next[] = "Next";


	printf("%-15s%-31s%-15s%-15s%-15s%-15s\n", ID, name, isDeleted, cloudID, prev, next);

	Client client;
	while (curOrig != -1) {
		FILE* slave;
		fopen_s(&slave, FILE_S, "rb");
		fseek(slave, curOrig * sizeof(Client), SEEK_SET);
		fread_s(&client, sizeof(Client), sizeof(Client), 1, slave);
		fclose(slave);
		print_s(client);
		printf("\n");
		curOrig = client.next;
	}


	printf("\n");

}

Cloud get_m(const int mKey) {
	FILE* master;
	fopen_s(&master, FILE_M, "rb");

	Cloud cloud = DEFAULT_CLOUD;

	if (master) {
		fseek(master, mKey * sizeof(Cloud), SEEK_SET);
		fread_s(&cloud, sizeof(Cloud), sizeof(Cloud), 1, master);
		fclose(master);
	}
	fclose(master);

	return cloud;
}

void del_m(const int mKey) {
	FILE* masterGarbage;

	fopen_s(&masterGarbage, FILE_M_G, "ab");
	fwrite(&mKey, sizeof(int), 1, masterGarbage);
	fclose(masterGarbage);


	FILE* master;
	int curOrig = get_m(mKey).firstClient;

	while (curOrig != -1) {
		del_s(curOrig);
		curOrig = get_m(mKey).firstClient;
	}

	Cloud cloud = DEFAULT_CLOUD;

	fopen_s(&master, FILE_M, "rb+");
	fseek(master, mKey * sizeof(Cloud), SEEK_SET);
	fwrite(&cloud, sizeof(Cloud), 1, master);
	fclose(master);
}

int count_m() {
	FILE* master;
	Cloud cloud;
	int cnt = 0;
	if (isEmptyFile(FILE_M))
		return 0;
	fopen_s(&master, FILE_M, "rb");
	while (fread_s(&cloud, sizeof(Cloud), sizeof(Cloud), 1, master)) {
		cnt++;
	}
	fclose(master);
	return cnt;
}

int count_s_m(const int mKey) {
	FILE* master, * slave;
	Cloud cloud;
	Client client;
	int cnt = 0;

	fopen_s(&master, FILE_M, "rb");
	fseek(master, mKey * sizeof(Cloud), 1, SEEK_SET);
	fread_s(&cloud, sizeof(Cloud), sizeof(Cloud), 1, master);
	fclose(master);

	int current = cloud.firstClient;

	while (current != -1) {
		cnt++;
		current = (get_s(mKey, current)).next;
	}

	return cnt;
}

int count_m_NDeleted() {
	FILE* master;
	Cloud cloud;
	int cnt = 0;

	fopen_s(&master, FILE_M, "rb");
	while (fread_s(&cloud, sizeof(Cloud), sizeof(Cloud), 1, master)) {
		if (!cloud.isDeleted)
			cnt++;
	}

	fclose(master);
	return cnt;
}

Cloud initialize_cloud() {
	Cloud cloud;
	cloud.isDeleted = false;
	cloud.firstClient = -1;

	char ch = 0;

	printf("Company name: ");
	//ch = fgetc(stdin); 
	fgets(cloud.company, 50, stdin);
	cloud.company[strlen(cloud.company) - 1] = '\0';

	printf("Size: ");
	//ch = fgetc(stdin);
	fgets(cloud.size, 10, stdin);
	cloud.size[strlen(cloud.size) - 1] = '\0';

	return cloud;
}

void insert_m() {
	FILE* master;
	int pos = -1;
	if (!isEmptyFile(FILE_M_G))
		pos = getFirstInt(FILE_M_G);
	else
		pos = count_m();

	Cloud cloud = initialize_cloud();
	fopen_s(&master, FILE_M, "rb+");

	cloud.ID = pos;
	cloud.firstClient = -1;
	cloud.clientNumb = 0;

	if (master) {
		fseek(master, pos * sizeof(Cloud), SEEK_SET);
		fwrite(&cloud, sizeof(Cloud), 1, master);
		fclose(master);
	}
}

void update_m(int mKey) {
	Cloud cloud = get_m(mKey);
	FILE* master;

	int ans = 0;
	char ch = 0;


	do {
		system("cls");
		printf("Which field to change?\n1 --> Company Name\n2 --> Size\nElse --> Exit\n");
		scanf_s("%d", &ans);
		ans--;
		switch (ans)
		{
		case 0:
			system("cls");
			printf("Company name: ");
			ch = fgetc(stdin);
			fgets(cloud.company, 50, stdin);
			cloud.company[strlen(cloud.company) - 1] = '\0';
			break;
		case 1:
			system("cls");
			printf("Size: ");
			ch = fgetc(stdin);
			fgets(cloud.size, 10, stdin);
			cloud.size[strlen(cloud.size) - 1] = '\0';
			break;
		default:
			break;
		}
	} while (ans >= 0 && ans <= 1);

	fopen_s(&master, FILE_M, "rb+");

	if (master) {
		fseek(master, cloud.ID * sizeof(Cloud), SEEK_SET);
		fwrite(&cloud, sizeof(Cloud), 1, master);
		fclose(master);
	}
}