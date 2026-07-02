#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


#define person_name 41
#define company 20
#define work 6


enum choice {
	library,
	welcom,
	delete
};


struct menber {
	char  sz_name[person_name];
	bool  b_gender;
	int   n_job;
};



struct menber ast_parties[company];

char* psz_job_List[work] = {
	{"勇者"},
	{"戦士"},
	{"僧侶"},
	{"魔法使い"},
	{"盗賊"},
	{"僧侶"},
};

int Gn_person = 0;

int show_start_screen(int catch);//スタート画
void add_menbers_screen();//仲間追加画面
void delete_parties_screen(); //仲間解雇の画面
void show_library_screen();//登録済み仲間ライブラリの画面
void show_menber(int person_number);//仲間閲覧関数




int main(void) {

    
	int n_push = 0;


	while (n_push == library || n_push == welcom || n_push == delete) {
		n_push = show_start_screen(n_push);

		if (n_push == library) {
			show_library_screen();
			
		}

		else if (n_push == welcom) {
			add_menbers_screen();
		}

		else if (n_push == delete) {
			delete_parties_screen();
		}
	};


	return 0;
}



int  show_start_screen(int catch) {
	printf("ルイーダの酒場にようこそ\n");
	printf("現在の仲間の数：%d\n", Gn_person);
	printf("\n");

	printf("0:仲間の閲覧\n");
	printf("1:仲間の追加\n");
	printf("2:仲間の削除\n");

	scanf_s("%d", &catch);
	return(catch);

}


void show_library_screen(void) {
	printf("閲覧方法を選んでください\n");
	printf("\n");
	int button = 0;
	
	printf("0:特定の仲間の閲覧\n");
	printf("1:全仲間の閲覧\n");
	printf("上記以外：やっぱりやめる\n");
	scanf_s("%d", &button);
	return;
}

void add_menbers_screen(void) {
	printf("仲間の名前を教えて下さい\n");


	char sz_Names[person_name] = { 0 };
	scanf_s("%s", sz_Names, person_name);
	strcpy_s(ast_parties[Gn_person].sz_name, person_name, sz_Names);

	printf("仲間の性別を教えて下さい\n");
	printf("0(男)\n");
	printf("1(女)\n");

	int push=0;
	scanf_s("%d", &push);
	ast_parties[Gn_person].b_gender = push;

	printf("仲間の職業を教えて下さい");
	for (int i = 0; i < work; i++) {
		printf("%d(%s)\n", i, psz_job_List[i]);
	}

	int add_job=0;
	scanf_s("%d", &add_job);
	ast_parties[Gn_person].n_job = add_job;

	printf("次の仲間が追加されました\n");
	show_menber(Gn_person);

	Gn_person++;

}

void show_menber(int person_number) {
	printf("名前 : %s\n", ast_parties[person_number].sz_name);
	
	if (ast_parties[person_number].b_gender == 0) {
		printf("性別 : %s\n", "男");
	}
	else printf("性別 : %s\n", "女");


	int job_look = ast_parties[person_number].n_job;
	printf("職業 : %s\n", psz_job_List[job_look]);

}

void delete_parties_screen() {
	printf("特定の仲間を削除");
	printf("\n");
	int button=0;


	printf("0:特定の仲間を削除\n");
	printf("1:全仲間を削除\n");
	printf("上記以外：やっぱりやめる\n");
	scanf_s("%d", &button);
	return;
}