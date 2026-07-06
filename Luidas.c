#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define PERSON_NAME 41
#define COMPANY 20
#define WORK 6


enum CHOICE {
	LIBRARY,
	WELCOM,
	DELETE
};

enum CHOICE_UNIT {
	SINGLE_UNIT,
	ALL_UNIT,
};


typedef struct menber {
	char  sz_name[PERSON_NAME];
	bool  b_gender;
	int   n_job;
}MENBER;



MENBER ast_parties[COMPANY] = {0};


char* psz_job_List[WORK] = {
	{"勇者"},
	{"戦士"},
	{"僧侶"},
	{"魔法使い"},
	{"盗賊"},
	{"遊び人"},
};

int g_person = 0;

int show_start_screen(int catch);//スタート画
void add_menbers_screen();//仲間追加画面
void delete_parties_screen(); //仲間解雇の画面
void show_library_screen();//登録済み仲間ライブラリの画面
void show_menber(int person_number);//仲間ステータス閲覧関数
void delete_person(int choice);//単体仲間解雇の関数//必要かは微妙である



int main(void) {


	int n_you_choice = 0;
	bool end_game_flag = 0;

	do{
		n_you_choice = show_start_screen(n_you_choice);

		switch (n_you_choice) {
		case LIBRARY: {
			show_library_screen();
			break;
		}

		case WELCOM: {
			add_menbers_screen();
			break;
		}

		case DELETE: {
			delete_parties_screen();
			break;
		}
		default:
			end_game_flag = !end_game_flag;
			break;
		}
	}while (end_game_flag == 0);
}



int  show_start_screen(int catch){
	printf("ルイーダの酒場にようこそ\n");
	printf("現在の仲間の数：%d\n", g_person);
	printf("\n");

	printf("0:仲間の閲覧\n");
	printf("1:仲間の追加\n");
	printf("2:仲間の削除\n");
	printf("上記以外:終了\n");

	scanf_s("%d", &catch);
	return(catch);

}


void show_library_screen(void) {
	printf("閲覧方法を選んでください\n");
	printf("\n");
	int choice = 0;

	printf("0:特定の仲間の閲覧\n");
	printf("1:全仲間の閲覧\n");
	printf("上記以外：やっぱりやめる\n");
	scanf_s("%d", &choice);


	switch (choice) {
	case SINGLE_UNIT: {
		printf("何人目の仲間を閲覧しますか？\n");
		scanf_s("%d", &choice);
		show_menber(choice);
		break;
	}
	case ALL_UNIT: {
		for (int i = 0; i < g_person; i++) {
			  show_menber(i);
		    }
		break;
	}
	default:
		break;

	}

}

void add_menbers_screen(void) {
	printf("仲間の名前を教えて下さい\n");


	char sz_Names[PERSON_NAME] = { 0 };
	scanf_s("%s", sz_Names, PERSON_NAME);
	strcpy_s(ast_parties[g_person].sz_name, PERSON_NAME, sz_Names);

	printf("仲間の性別を教えて下さい\n");
	printf("0(男)\n");
	printf("1(女)\n");

	int push = 0;
	scanf_s("%d", &push);
	ast_parties[g_person].b_gender = push;

	printf("仲間の職業を教えて下さい\n");
	for (int i = 0; i < WORK; i++) {
		printf("%d(%s)\n", i, psz_job_List[i]);
	}

	int add_job = 0;
	scanf_s("%d", &add_job);
	ast_parties[g_person].n_job = add_job;

	printf("次の仲間が追加されました\n");
	show_menber(g_person);
	
	g_person ++;

}

void show_menber(int person_number) {
	printf("名前 : %s\n", ast_parties[person_number].sz_name);

	if (ast_parties[person_number].b_gender == 0) {
		printf("性別 : %s\n", "男");
	}
	else printf("性別 : %s\n", "女");


	int job_look = ast_parties[person_number].n_job;
	printf("職業 : %s\n", psz_job_List[job_look]);
	printf("\n");

}

void delete_parties_screen() {
	printf("特定の仲間を削除");
	printf("\n");
	int button = 0;


	printf("0:特定の仲間を削除\n");
	printf("1:全仲間を削除\n");
	printf("上記以外：やっぱりやめる\n");
	scanf_s("%d", &button);
	
	switch (button) {//todp:挙動がおかしい//修正済み
	   case SINGLE_UNIT: {
		   printf("現在%d人の仲間がいます\n",g_person);
		   printf("何番目の仲間を削除しますか？\n");
		   scanf_s("%d", &button);
		   delete_person(button-1);
		         if (button == g_person) {
			         g_person--;
			         break;
		             }
		         else
					 for (int i=button-1; i < g_person; i++) {
						 ast_parties[i] = ast_parties[i+1];
					 }
				     g_person--;
			         break;
	                 }
	   case ALL_UNIT: {
		   memset(ast_parties, 0, sizeof(ast_parties));
		   g_person = 0;
		   break;
	   }
	   default:break;
	}
    return;
}

void delete_person(int choice) {
	memset(&ast_parties[choice], 0, sizeof(ast_parties[choice]));


}