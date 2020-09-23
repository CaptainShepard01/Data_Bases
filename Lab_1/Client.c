#include <stdio.h>
#include "Client.h"

void print_s(Client client) {
	if (client.isDeleted != true)
		printf("%-15d%-31s%-15d%-15d%-15d%-15d", client.ID, client.fullName, client.isDeleted, client.cloudID, client.prev, client.next);
	else
		printf("REMOVED");
}

void ut_s() {
	FILE* slave;
	fopen_s(&slave, FILE_S, "rb");
	Client client = DEFAULT_CLIENT;

	printf("Clients:\n");

	char ID[] = "ID";
	char name[] = "Full Name";
	char isDeleted[] = "IsDeleted";
	char cloudID[] = "Cloud ID";
	char prev[] = "Previous";
	char next[] = "Next";


	printf("%-15s%-31s%-15s%-15s%-15s%-15s\n", ID, name, isDeleted, cloudID, prev, next);

	while (slave && fread_s(&client, sizeof(Client), sizeof(Client), 1, slave)) {
		print_s(client);
		printf("\n");
	}
	printf("\n");

	fclose(slave);
}

int count_s() {
	FILE* slave;
	Client client;
	int cnt = 0;
	fopen_s(&slave, FILE_S, "rb");
	while (fread_s(&client, sizeof(Client), sizeof(Client), 1, slave)) {
			cnt++;
	}
	fclose(slave);
	return cnt;
}

int count_s_NDeleted() {
	FILE* slave;
	Client client;
	int cnt = 0;

	fopen_s(&slave, FILE_S, "rb");
	while (fread_s(&client, sizeof(Client), sizeof(Client), 1, slave)) {
		if (!client.isDeleted)
			cnt++;
	}

	fclose(slave);
	return cnt;
}

Client get_s(const int mKey, const int sKey) {
	FILE* master;
	fopen_s(&master, FILE_M, "rb");

	Cloud cloud = DEFAULT_CLOUD;

	if (master) {
		fseek(master, mKey * sizeof(Cloud), SEEK_SET);
		fread_s(&cloud, sizeof(Cloud), sizeof(Cloud), 1, master);
		fclose(master);
	}

	Client client = DEFAULT_CLIENT;

	FILE* slave;

	if (cloud.ID == -1 || fopen_s(&slave, FILE_S, "rb") != 0) {
		return client;
	}

	int curPos = cloud.firstClient;

	do {
		fseek(slave, curPos * sizeof(Client), SEEK_SET);
		fread_s(&client, sizeof(Client), sizeof(Client), 1, slave);

		if (client.ID == sKey) {
			fclose(slave);
			return client;
		}
		else
			curPos = client.next;
	} while (client.next != -1);

	fclose(slave);

	Client nullStudent = DEFAULT_CLIENT;

	return nullStudent;
}

void del_s(const int sKey) {
	FILE* slave;
	FILE* slaveGarbage;

	fopen_s(&slaveGarbage, FILE_S_G, "ab");
	fwrite(&sKey, sizeof(int), 1, slaveGarbage);
	fclose(slaveGarbage);

	fopen_s(&slave, FILE_S, "rb");

	Client client = DEFAULT_CLIENT, prev = DEFAULT_CLIENT, next = DEFAULT_CLIENT;

	fseek(slave, sKey * sizeof(Client), SEEK_SET);
	fread_s(&client, sizeof(Client), sizeof(Client), 1, slave);

	client.isDeleted = true;

	int pPrev = client.prev, pNext = client.next, mKey = client.cloudID;

	freopen_s(&slave, FILE_S, "rb+", slave);

	fseek(slave, sKey * sizeof(Client), SEEK_SET);
	fwrite(&client, sizeof(Client), 1, slave);

	freopen_s(&slave, FILE_S, "rb+", slave);

	if (pPrev != -1) {
		fseek(slave, pPrev * sizeof(Client), SEEK_SET);
		fread_s(&prev, sizeof(Client), sizeof(Client), 1, slave);
		prev.next = pNext;
	}

	if (pNext != -1) {
		fseek(slave, pNext * sizeof(Client), SEEK_SET);
		fread_s(&next, sizeof(Client), sizeof(Client), 1, slave);
		next.prev = pPrev;
	}

	freopen_s(&slave, FILE_S, "rb+", slave);

	if (pPrev != -1) {
		fseek(slave, pPrev * sizeof(Client), SEEK_SET);
		fwrite(&prev, sizeof(Client), 1, slave);
		FILE* master;
		Cloud cloud = DEFAULT_CLOUD;
		fopen_s(&master, FILE_M, "rb");
		fseek(master, mKey * sizeof(Cloud), SEEK_SET);
		fread_s(&cloud, sizeof(Cloud), sizeof(Cloud), 1, master);

		cloud.clientNumb--;
		freopen_s(&master, FILE_M, "rb+", master);
		fseek(master, mKey * sizeof(Cloud), SEEK_SET);
		fwrite(&cloud, sizeof(Cloud), 1, master);

		fclose(master);
	}
	else {
		FILE* master;
		Cloud cloud = DEFAULT_CLOUD;
		fopen_s(&master, FILE_M, "rb");
		fseek(master, mKey * sizeof(Cloud), SEEK_SET);
		fread_s(&cloud, sizeof(Cloud), sizeof(Cloud), 1, master);

		cloud.clientNumb--;
		cloud.firstClient = pNext;

		freopen_s(&master, FILE_M, "rb+", master);
		fseek(master, mKey * sizeof(Cloud), SEEK_SET);
		fwrite(&cloud, sizeof(Cloud), 1, master);

		fclose(master);
	}

	if (pNext != -1) {
		fseek(slave, pNext * sizeof(Client), SEEK_SET);
		fwrite(&next, sizeof(Client), 1, slave);
	}

	fclose(slave);
}

Client initialize_client() {
	Client client;
	client.isDeleted = false;

	char ch = 0;

	printf("Full name: ");
	//ch = fgetc(stdin);
	fgets(client.fullName, 50, stdin);
	client.fullName[strlen(client.fullName) - 1] = '\0';


	int maxCloud = count_m();

	do {
		system("cls");
		ut_m();
		printf("Choose your Cloud: ");
		scanf_s("%d", &client.cloudID);
	} while (client.cloudID < 0 || client.cloudID >= maxCloud || get_m(client.cloudID).isDeleted);

	return client;
}

void insert_s() {
	FILE* slave, * master;
	int pos = -1;

	Client client = initialize_client();
	client.prev = -1;

	if (!isEmptyFile(FILE_S_G))
		pos = getFirstInt(FILE_S_G);
	else
		pos = count_s();

	client.ID = pos;
	Cloud cloud = get_m(client.cloudID);
	cloud.clientNumb++;
	client.next = cloud.firstClient;
	Client oldFirst = get_s(cloud.ID, cloud.firstClient);
	cloud.firstClient = pos;
	oldFirst.prev = pos;

	fopen_s(&master, FILE_M, "rb+");

	if (master) {
		fseek(master, cloud.ID * sizeof(Cloud), SEEK_SET);
		fwrite(&cloud, sizeof(Cloud), 1, master);
		fclose(master);
	}

	fopen_s(&slave, FILE_S, "rb+");

	if (slave) {
		if (oldFirst.ID != -1) {
			fseek(slave, oldFirst.ID * sizeof(Client), SEEK_SET);
			fwrite(&oldFirst, sizeof(Client), 1, slave);
		}

		fseek(slave, client.ID * sizeof(Client), SEEK_SET);
		fwrite(&client, sizeof(Client), 1, slave);

		fclose(slave);
	}
}

void update_s(int mKey, int sKey) {
	Client client = get_s(mKey, sKey);
	FILE* slave, * master;

	int ans = 0;
	int maxCloud = count_m();
	char ch = 0;


	do {
		system("cls");
		printf("Which field to change?\n1 --> Full Name\n2 --> Cloud\nelse --> Exit\n");
		scanf_s("%d", &ans);
		ans--;
		switch (ans)
		{
		case 0:
			system("cls");
			printf("Full Name: ");

			ch = fgetc(stdin);

			fgets(client.fullName, 30, stdin);
			client.fullName[strlen(client.fullName) - 1] = '\0';

			fopen_s(&slave, FILE_S, "rb+");

			if (slave) {
				fseek(slave, client.ID * sizeof(Client), SEEK_SET);
				fwrite(&client, sizeof(Client), 1, slave);
				fclose(slave);
			}
			break;
		case 1:
			do {
				system("cls");
				ut_m();
				printf("\nEnter new Cloud: ");
				scanf_s("%d", &client.cloudID);
			} while (client.cloudID < 0 || client.cloudID >= maxCloud);

			del_s(sKey);

			int pos = getFirstInt(FILE_S_G);

			client.ID = pos;
			Cloud cloud = get_m(client.cloudID);
			client.next = cloud.firstClient;
			Client oldFirst = get_s(cloud.ID, cloud.firstClient);
			cloud.firstClient = pos;
			cloud.clientNumb++;
			oldFirst.prev = pos;

			fopen_s(&master, FILE_M, "rb+");

			if (master) {
				fseek(master, cloud.ID * sizeof(Cloud), SEEK_SET);
				fwrite(&cloud, sizeof(Cloud), 1, master);

				fclose(master);
			}
			fopen_s(&slave, FILE_S, "rb+");

			if (slave) {
				fseek(slave, client.ID * sizeof(Client), SEEK_SET);
				fwrite(&client, sizeof(Client), 1, slave);

				fclose(slave);
			}
			break;
		default:
			break;
		}
	} while (ans >= 0 && ans <= 1);
}