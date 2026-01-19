#include"function.h"

int main() {
	FILE* pcinema;
	int choice=0;
	struct film cinema[MAXFILM];

	pcinema = fopen("film.dat", "r+b");
	if (pcinema == NULL) 
		pcinema = fopen("film.dat", "w+b");

	int count = load_count(pcinema);
	int* pcount = &count;

	openning();

	while (scanf("%d",&choice) != 0) {
		system("cls");
		switch (choice) {
		case 1:index(pcount, pcinema); break;
		case 2:rank(pcount, pcinema); break;
		case 3:add(pcount, cinema, pcinema); break;
		case 4:del(pcount, cinema, pcinema); break;
		case 5:reset(pcount, pcinema); break;
		case 6:instructions();
		}
			system("cls");
			openning();
	}
	backup(pcinema);
	fclose(pcinema);
	return 0;
}
