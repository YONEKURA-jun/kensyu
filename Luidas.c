#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>


#define PERSON_NAME 41
#define COMPANY 20
#define WORK 6

//異常な入力を弾く関数から値を受け取り、状態をスタート画面に戻す。
#define SAFETY_CHECK(button) {if(button == -1){\
                             printf("不正な入力です\n");\
                             return;}}


#define MEMBER_IS_ENPTY(button) {if(button == 0){\
                                 printf("誰も仲間がいないようだ\n"); \
                                 printf("\n");\
                                 return;}}

#define MEMBER_IS_FULL(button) {if(button == 20){\
                                printf("それ以上の仲間は必要ないようだ"); \
                                printf("\n");\
                                return;}}


enum CHOICE {
	LIBRARY,
	WELCOM,
	DELETE
};

enum CHOICE_UNIT {
	SINGLE_UNIT,
	ALL_UNIT,
};


typedef struct member {
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

static int g_person = 0;//現在のパーティー人数を格納する

int show_start_screen(int button);//スタート画
void add_menbers_screen();//仲間追加画面
void delete_parties_screen(); //仲間解雇の画面
void show_library_screen();//登録済仲間ライブラリ閲覧の画面
void show_menber(int button);//仲間ステータス閲覧関数
void delete_person(int button);//単体仲間解雇の関数//必要かは微妙である
int very_safety_input(int lowest, int highest);//異常な入力を弾く関数


int main(void) {


	
	bool end_game_flag = 0;

	while (end_game_flag == 0) {
		int n_you_choice = -1;
		n_you_choice = show_start_screen(n_you_choice);

		switch (n_you_choice) {

		case LIBRARY: 
			show_library_screen();
			break;

		case WELCOM: 
			add_menbers_screen();
			break;

		case DELETE: 
			delete_parties_screen();
			break;

		default:
			end_game_flag = !end_game_flag;
			break;
		}
	}
}



int  show_start_screen(int button){
	printf("ルイーダの酒場にようこそ\n");
	printf("現在の仲間の数：%d\n", g_person);
	printf("\n");

	printf("0:仲間の閲覧\n");
	printf("1:仲間の追加\n");
	printf("2:仲間の削除\n");
	printf("上記以外:終了\n");
	
	scanf_s("%d", &button);
	rewind(stdin);
	return(button);

}


void show_library_screen(void) {
	MEMBER_IS_ENPTY(g_person)
	
	printf("閲覧方法を選んでください\n");
	printf("\n");
	int button = -1;

	printf("0:特定の仲間の閲覧\n");
	printf("1:全仲間の閲覧\n");
	printf("上記以外：やっぱりやめる\n");
	printf("\n");
	button = very_safety_input(SINGLE_UNIT, ALL_UNIT);
	SAFETY_CHECK(button)


		switch (button) {
		case SINGLE_UNIT:
			printf("何人目の仲間を閲覧しますか？\n");
			very_safety_input(1, COMPANY);
			SAFETY_CHECK(button)
				show_menber(button);
			break;

		case ALL_UNIT:
			for (int i = 0; i < g_person; i++) {
				show_menber(i);
			}
			break;

		default:
			break;
		}
}

void add_menbers_screen(void) {
	MEMBER_IS_FULL(g_person)
	printf("仲間の名前を教えて下さい\n");
	

	char sz_Names[PERSON_NAME] = { 0 };
    int button = -1;

	scanf_s("%s", sz_Names, PERSON_NAME);//21文字を入力すると弾かれる　//maybe:要修正？？
	strcpy_s(ast_parties[g_person].sz_name, PERSON_NAME, sz_Names);
	rewind(stdin);

	printf("仲間の性別を教えて下さい\n");
	printf("0(男)\n");
	printf("1(女)\n");
	

	
	button = very_safety_input(0,1);
	SAFETY_CHECK(button)
	ast_parties[g_person].b_gender = button;
	

	printf("仲間の職業を教えて下さい\n");
	for (int i = 0; i < WORK; i++) {
		printf("%d(%s)\n", i, psz_job_List[i]);
	}

	button = very_safety_input(0,WORK);
	SAFETY_CHECK(button)
	ast_parties[g_person].n_job = button;

	printf("次の仲間が追加されました\n");
	printf("\n");
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
	MEMBER_IS_ENPTY(g_person)
	printf("特定の仲間を削除");
	printf("\n");

	int button = -1;

	printf("0:特定の仲間を削除\n");
	printf("1:全仲間を削除\n");
	printf("上記以外：やっぱりやめる\n");
	printf("\n");

	button = very_safety_input(SINGLE_UNIT,ALL_UNIT);
	SAFETY_CHECK(button)
	
	switch (button) {

	case SINGLE_UNIT: 
		printf("現在%d人の仲間がいます\n", g_person);
		printf("何番目の仲間を削除しますか？\n");

		button = very_safety_input(1, COMPANY);
		SAFETY_CHECK(button)

		delete_person(button - 1);
		g_person--;

		for (int i = button - 1; i < g_person; i++) {
			ast_parties[i] = ast_parties[i + 1];
		}
    break;

	case ALL_UNIT: 
		memset(ast_parties, 0, sizeof(ast_parties));
		g_person = 0;
	break;

	default:
		break;
	}

return;
}
	

void delete_person(int choice) {
	memset(&ast_parties[choice], 0, sizeof(ast_parties[choice]));


}

//整数型に狙いを絞る
int very_safety_input(int lowest, int highest) {
	int button;
	int input_check;
	int error_input = -1;

	
	input_check = scanf_s("%d", &button);
	rewind(stdin);

	//scanf_sが成功して、正統な入力の範囲だった時
	if (input_check == 1 && button >= lowest && button <= highest) {
			return(button);
	}

	else {
		return(error_input);
	}
}



