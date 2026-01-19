#include"function.h"

void openning() {
	puts("                                                     番剧收藏馆\n");
	puts("                                     1.番剧列表                        2.番剧排名\n");
	puts("                                     3.添加番剧                        4.删除番剧\n");
	puts("                                     5.重置                            6.使用说明\n");
	printf("\n");
	puts("                                            输入对应的编号即可使用相应功能");
	puts("                                  --------------------------------------------------");
	printf("                                  按你喜欢的选吧~:");
}

int load_count(FILE* fp) {
	int count = 0;
	fseek(fp, 0, SEEK_SET);
	if (fread(&count, sizeof(int), 1, fp) == 1)
		return count;
	return 0;
}

void save_count(FILE* fp,int count){
	fseek(fp, 0, SEEK_SET);
	fwrite(&count, sizeof(int), 1, fp);
	fflush(fp);
}

char* s_gets(char* st, int n) {
	char* ret_val;
	char* find;

	ret_val = fgets(st, n, stdin);
	if (ret_val)
	{
		find = strchr(st, '\n');
		if (find)
			*find = '\0';
		else
			while (getchar() != '\n')
				continue;
	}
	return ret_val;
}

void index(int* pcount, FILE* pcinema) {
	puts("番剧索引列表");
	if (*pcount == 0)
		printf("空\n\n");
	else
	{
		struct film sindex;
		fseek(pcinema, sizeof(int), SEEK_SET);
		for (int i = 0; i < *pcount; i++) {
			if (fread(&sindex, sizeof(struct film), 1, pcinema) != 1) {
				printf("读取第%d条记录失败\n", i + 1);
				break;
			}
			printf("[%02d] %15s %10s %5.1f分\n", i + 1, sindex.name, sindex.author, sindex.score);
		}
	}
	puts("————————————————————————————————————————");
	printf("按下空格键返回");
	getchar();
	getchar();
}

void rank(int* pcount, FILE* pcinema) {
	puts("番剧排行列表");
	if (*pcount == 0)
		printf("空\n\n");
	else
	{
		struct film* srank = (struct film*)malloc(*pcount * SIZE);
		struct film exchange;
		fseek(pcinema, sizeof(int), SEEK_SET);
		for (int i = 0; i < *pcount; i++)
			fread(&srank[i], SIZE, 1, pcinema);
		for (int j = 0; j < *pcount - 1; j++)
			for (int k = 0; k < *pcount - 1 - j; k++)
				if (srank[k].score < srank[k+1].score)
				{
					exchange = srank[k];
					srank[k] = srank[k + 1];
					srank[k + 1] = exchange;
				}
		for (int l = 0; l < *pcount; l++)
			printf("[%02d] %15s %10s %5.1f分\n", l + 1, srank[l].name, srank[l].author, srank[l].score);

		free(srank);
	}
	puts("————————————————————————————————————————");
	printf("按下空格键返回");
	getchar();
	getchar();
}

void add(int* pcount, struct film cinema[MAXFILM], FILE* pcinema) {
	while (getchar() != '\n') continue;
	puts("第一请输入它的名字");
	while (*pcount < MAXFILM && s_gets(cinema[*pcount].name, MAXTITL) != NULL && cinema[*pcount].name[0] != '\0') {
		puts("第二请输入导演或原作者");
		s_gets(cinema[*pcount].author, MAXAUTL);
		puts("第三请为它打上神圣的分数(0~10.0)");
		float input_score;
		while (1) {
			if (scanf("%f", &input_score) == 1) {
				if (input_score >= 0 && input_score <= 10) {
					cinema[*pcount].score = input_score;
					break;
				}
				else 
					puts("超模分数？可惜~概不受理！");
			}
			else {
				puts("数字！笨蛋");
				while (getchar() != '\n') continue; 
			}
		}
		if (*pcount == 0)
			fseek(pcinema, sizeof(int), SEEK_SET);
		else
			fseek(pcinema, sizeof(int) + (*pcount) * SIZE, SEEK_SET);
		fwrite(&cinema[*pcount], SIZE, 1, pcinema);
		fflush(pcinema);

		(*pcount)++;
		save_count(pcinema, *pcount);
		while (getchar() != '\n') continue;

		if (*pcount < MAXFILM)
			puts("再来，第一请输入它的名字");
	}
	printf("按Enter键返回");
	getchar();
}

void del(int* pcount, struct film cinema[MAXFILM], FILE* pcinema) {
	while (getchar() != '\n') continue;
	puts("番剧索引列表");
	if (*pcount == 0) {
		printf("空\n\n");
	}
	else
	{
		struct film sdel;
		fseek(pcinema, sizeof(int), SEEK_SET);
		for (int i = 0; i < *pcount; i++) {
			fread(&sdel, sizeof(struct film), 1, pcinema);
			printf("[%02d] %15s %10s %5.1f分\n", i + 1, sdel.name, sdel.author, sdel.score);
		}
	}

	puts("————————————————————————————————————————");

	while (1) {
		int out = 0;
		int choice;
		puts("在此！输入想删除番剧的编号");
		
		int ch = getchar();
		if (ch == '\n' || ch == '\r')
			return;
		ungetc(ch, stdin);

		while (1) {
			if (scanf("%d", &choice) == 1)
			{
				if (choice > 0 && choice <= *pcount)
					break;
				else
					puts("皇帝的新编号？你才是笨蛋！");
				out++;
			}
			else
			{
				puts("不要拿火星上的编号糊弄我！");
				out++;
				while (getchar() != '\n') continue;
			}
			if (out == 3)
			{
				while (getchar() != '\n') continue;
				puts("————————————————————————————————————————");
				puts("恭喜你，彩蛋~~~好吧好吧，其实是我无限循环的bug懒得修了，在这里按Enter返回吧~");
				getchar();
				return;
			}
		}

		long past = sizeof(int) + (choice-1) * SIZE;
		fseek(pcinema, past, SEEK_SET);
		struct film transfer;
		for (int i = choice-1; i < *pcount; i++)
		{
			fseek(pcinema, past + SIZE, SEEK_SET);
			fread(&transfer, SIZE, 1, pcinema);

			fseek(pcinema, past, SEEK_SET);
			fwrite(&transfer, SIZE, 1, pcinema);
			past += SIZE;
		}

		if (*pcount > 0)
		{
			struct film delect;
			fseek(pcinema, sizeof(int) + (*pcount - 1) * SIZE, SEEK_SET);
			fwrite(&delect, SIZE, 1, pcinema);
		}

		(*pcount)--;
		save_count(pcinema, *pcount);
		fflush(pcinema);
		
		while (getchar() != '\n') continue;
		system("cls");

		puts("番剧索引列表");
		if (*pcount == 0)
			printf("空\n\n");
		else
		{
			struct film sdel;
			fseek(pcinema, sizeof(int), SEEK_SET);
			for (int i = 0; i < *pcount; i++) {
				fread(&sdel, sizeof(struct film), 1, pcinema);
				printf("[%02d] %15s %10s %5.1f分\n", i + 1, sdel.name, sdel.author, sdel.score);
			}
		}
		puts("————————————————————————————————————————");
	}
}

void reset(int* pcount, FILE** pcinema) {
	while (getchar() != '\n') continue;
	puts("是否确定重置列表？(此行为不可逆转)");
	puts("输入'yes'立即执行，按Enter键返回");
	puts("————————————————————————————————————————");

	char choice[10];
	if (s_gets(choice,sizeof(choice))!=NULL)
	{
		if (strlen(choice) == 0) 
			return;

		else if (strcmp(choice, "yes") == 0) {
			remove("film.dat");
			*pcinema = NULL;

			*pcinema = fopen("film.dat", "w+b");

			if (*pcinema == NULL)
			{
				puts("创建新文件失败");
				return;
			}

			*pcount = 0;
			save_count(*pcinema, *pcount);

			system("cls");
			printf("重置成功，按Enter键返回");
		}

		else
		{
			puts("若心怀犹豫，便不要勉强，我将送汝返程，切记:三思而...而后行是也！");
			getchar();
		}
	}
}

void instructions() {
	while (getchar() != '\n') continue;
	puts("使用说明");
	puts("1.任何模块的退回主界面的方式都为按下'Enter'键，退出程序的方式为在主界面输入字符，并按下'Enter'\n");
	puts("2.首次运行程序后，与存放'exe'相同的文件夹下目录中将自动生成名为'film.dat'的文件以存储数据\n");
	puts("3.添加番剧时，作品名和评分必须输入，作者栏可以按'Enter'跳过\n");
	puts("4.每次仅能录入一个番剧，写入评分后才能写入下一个番剧信息\n");
	puts("5.此程序最多能存储100条番剧信息，作品名和作者限制输入30字节，约10个汉字，或10~15个日文字符\n");
	puts("6.每次执行删除番剧程序后会刷新列表，千万注意不要删错条目，此行为不可撤销\n");
	puts("7.重置模块是将'film.dat'文件删除后重新生成并开启，注意是直接删除，不会放进回收站\n");
	puts("8.以防万一，我制作了备份程序，其位置与'filn.dat'相同，名为'film备份.dat'，每次于主界面输入任意字符退出时更新保存\n");
	puts("9.如果想在重置后找回数据，请先点击命令行界面右上角的'×'直接退出程序，不要在主界面输入字符退出。然后将'film备份.dat'重命名为'film.dat'，覆盖已有的'film.dat'空文件\n");
	puts("10.玩得愉快~");
	puts("——————————————————————————————————————————————————————————————————————————————————————————");
	puts("按Enter键退出");
	getchar();
}

void backup(FILE* pcinema) {
	FILE* pbackup = NULL;
	char package[1024];
	size_t bytes;

	pbackup = fopen("film备份.dat", "wb");
	if (!pbackup) {
		printf("无法创建备份文件！\n");
		return;
	}

	rewind(pcinema);

	while ((bytes = fread(package, 1, sizeof(package), pcinema)) > 0)
		fwrite(package, 1, bytes, pbackup);

	fclose(pbackup);
}