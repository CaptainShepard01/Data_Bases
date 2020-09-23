#include <stdio.h>
#include <windows.h> 
#include "Cloud.h"
#include "Client.h"

int menu();

int main(void) {
	if (_chdir(".\\Files") == -1) {
		if (_mkdir(".\\Files") == 0)
			_chdir(".\\Files");
	}
	HANDLE  hConsole;
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, 15);

	setupFiles();

	int answer = 0;
	do {		
		answer = menu();

		system("cls");
		char c = 0;
		int ans = 0, mKey = 0, sKey = 0;

		switch (answer)
		{
		case 0:
			ut_m();
			break;
		case 1:
			ut_s();
			break;
		case 2:
			c = fgetc(stdin);
			insert_m();
			break;
		case 3:
			if (count_m_NDeleted() == 0) {
				printf("Create at leat one Cloud first!\n");
				break;
			}
			c = fgetc(stdin);
			insert_s();
			break;
		case 4:
			if (count_m_NDeleted() == 0) {
				printf("There are no Clouds!\n");
				break;
			}
			do {
				system("cls");
				ut_m();
				printf("\nChoose Cloud to delete: ");
				scanf_s("%d", &ans);
			} while (ans < 0 || ans >= count_m() || get_m(ans).isDeleted);

			del_m(ans);
			break;
		case 5:
			if (count_s_NDeleted() == 0) {
				printf("There are no Clients!\n");
				break;
			}

			do {
				system("cls");
				ut_m();
				printf("\nFrom which Cloud to delete Client: ");
				scanf_s("%d", &ans);
			} while (ans < 0 || ans >= count_m()||get_m(ans).isDeleted|| get_m(ans).clientNumb == 0);

			mKey = ans;

			do {
				system("cls");

				ut_m_s(mKey);

				printf("\nWhich Client: ");
				scanf_s("%d", &ans);
			} while (ans < 0 || ans >= count_s() || get_s(mKey, ans).cloudID != mKey ||
				get_s(mKey, ans).isDeleted);

			del_s(ans);
			break;
		case 6:
			if (count_m_NDeleted() == 0) {
				printf("There are no Clouds!\n");
				break;
			}

			do {
				system("cls");
				ut_m();
				printf("\nWhich Cloud to update: ");
				scanf_s("%d", &ans);
			} while (ans < 0 || ans >= count_m());

			system("cls");
			c = getc(stdin);
			update_m(ans);
			break;
		case 7:
			if (count_s_NDeleted() == 0) {
				printf("There are no Clients!\n");
				break;
			}

			do {
				system("cls");
				ut_m();
				printf("\nFrom which Cloud to update Client: ");
				scanf_s("%d", &ans);
			} while (ans < 0 || ans >= count_m()||get_m(ans).clientNumb==0);

			mKey = ans;

			do {
				system("cls");

				ut_m_s(mKey);
				printf("\nWhich Client to update: ");
				scanf_s("%d", &ans);
			} while (ans < 0 || ans >= count_s() || get_s(mKey, ans).cloudID != mKey ||
				get_s(mKey, ans).isDeleted);

			sKey = ans;
			c = getc(stdin);
			update_s(mKey, sKey);
			break;
		default:
			//flag = true;
			break;
		}
		system("pause");
	}while (answer!=8);

	return 0;
}

int menu() {
	HANDLE  hConsole;
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	int key = 0;
	int code;
	do {
		system("cls");
		printf("DataBase Lab_1 Balykov.\n");
		printf("What to do?\n\n");
		key = (key + 9) % 9;
		if (key == 0) {
			SetConsoleTextAttribute(hConsole, 2);
			printf(" 1 --> Show all Clouds.\n");
			SetConsoleTextAttribute(hConsole, 15);
		}
		else {
			SetConsoleTextAttribute(hConsole, 15);
			printf(" 1 --- Show all Clouds.\n");
		}
		if (key == 1) {
			SetConsoleTextAttribute(hConsole, 2);
			printf(" 2 --> Show all Clients.\n");
			SetConsoleTextAttribute(hConsole, 15);
		}
		else {
			SetConsoleTextAttribute(hConsole, 15);
			printf(" 2 --- Show all Clients.\n");
		}
		if (key == 2) {
			SetConsoleTextAttribute(hConsole, 2);
			printf(" 3 --> Create new Cloud.\n");
			SetConsoleTextAttribute(hConsole, 15);
		}
		else {
			SetConsoleTextAttribute(hConsole, 15);
			printf(" 3 --- Create new Cloud.\n");
		}
		if (key == 3) {
			SetConsoleTextAttribute(hConsole, 2);
			printf(" 4 --> Create new Client.\n");
			SetConsoleTextAttribute(hConsole, 15);
		}
		else {
			SetConsoleTextAttribute(hConsole, 15);
			printf(" 4 --- Create new Client.\n");
		}
		if (key == 4) {
			SetConsoleTextAttribute(hConsole, 2);
			printf(" 5 --> Delete Cloud.\n");
			SetConsoleTextAttribute(hConsole, 15);
		}
		else {
			SetConsoleTextAttribute(hConsole, 15);
			printf(" 5 --- Delete Cloud.\n");
		}
		if (key == 5) {
			SetConsoleTextAttribute(hConsole, 2);
			printf(" 6 --> Delete Client.\n");
			SetConsoleTextAttribute(hConsole, 15);
		}
		else {
			SetConsoleTextAttribute(hConsole, 15);
			printf(" 6 --- Delete Client.\n");
		}
		if (key == 6) {
			SetConsoleTextAttribute(hConsole, 2);
			printf(" 7 --> Update Cloud.\n");
			SetConsoleTextAttribute(hConsole, 15);
		}
		else {
			SetConsoleTextAttribute(hConsole, 15);
			printf(" 7 --- Update Cloud.\n");
		}
		if (key == 7) {
			SetConsoleTextAttribute(hConsole, 2);
			printf(" 8 --> Update Client.\n");
			SetConsoleTextAttribute(hConsole, 15);
		}
		else {
			SetConsoleTextAttribute(hConsole, 15);
			printf(" 8 --- Update Client.\n");
		}
		if (key == 8) {
			SetConsoleTextAttribute(hConsole, 2);
			printf(" 9 --> Exit.\n");
			SetConsoleTextAttribute(hConsole, 15);
		}
		else {
			SetConsoleTextAttribute(hConsole, 15);
			printf(" 9 --- Exit.\n");
		}

		code = _getch();
		if (code == 224)
		{
			code = _getch();
			if (code == 80) key++;
			if (code == 72) key--;
		}
	} while (code != 13);
	system("cls");
	return key;
}
