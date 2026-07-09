#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>


//課題の２　性別と職業順でソート

#define PERSON_NAME 41
#define COMPANY 20
#define WORK 6
#define TO_STANDBY_INPUT_ERROR -1

//異常な入力を弾く関数から値を受け取り、状態をスタート画面に戻す。
#define SAFETY_CHECK(input) {if(input == TO_STANDBY_INPUT_ERROR){\
                             printf("不正な入力です\n");\
                             return;}}


#define MEMBER_IS_ENPTY(now_member) {if(now_member == 0){\
                                 printf("誰も仲間がいないようだ\n"); \
                                 printf("\n");\
                                 return;}}

#define MEMBER_IS_FULL(now_member) {if(now_member == 20){\
                                printf("それ以上の仲間は必要ないようだ"); \
                                printf("\n");\
                                return;}}


enum CHOICE {
	LIBRARY,
	WELCOM,
	DELETE,
	SORT,
};

enum CHOICE_UNIT {
	SINGLE_UNIT,
	ALL_UNIT,
};

enum TO_CHOICE_SORT {
	GEMDER,
	JOB,
};


typedef struct member {
	char  sz_name[PERSON_NAME];
	bool  b_gender;
	int   n_job;
}MEMBER;



MEMBER* pst_parties[COMPANY] = { 0 };


char* psz_job_List[WORK] = {
	{"勇者"},
	{"戦士"},
	{"僧侶"},
	{"魔法使い"},
	{"盗賊"},
	{"遊び人"},
};

static int g_person = 0;//現在のパーティー人数を格納する

int show_start_screen(int button);//スタート画面
void add_members_screen();//仲間追加画面
void delete_parties_screen(); //仲間解雇の画面
void show_library_screen();//登録済仲間ライブラリ閲覧の画面
void sort_member_screen();//登録済みの仲間を並べ替える画面
void show_menber(int button);//仲間ステータス閲覧関数
void delete_person(int button);//単体仲間解雇の関数
int very_safety_input(int lowest, int highest);//異常な入力を弾く関数

void to_struct_array_copy(int copy_terget, int copy_source);
//tergetの構造体配列番号へsourceの構造体配列番号の中身をコピーしてくれる関数

void to_member_sort(int GENDER_OR_JOB, int lowest, int highest);
//標準入力に合わせたメンバでソートを行う、下限と上限を変えれる関数

void to_swap_member(MEMBER **swap_terget, MEMBER **swap_source);
//ターゲットの構造体配列とソース側の構造体配列を入れ替える関数

void to_malloc_array(void);//ポインタ配列の初期化関数
void to_free_array(void);//取得したメモリの開放関数


int main(void) {
	to_malloc_array();

	
	bool end_game_flag = 0;

	while (end_game_flag == 0) {
		int n_you_choice = TO_STANDBY_INPUT_ERROR;
		n_you_choice = show_start_screen(n_you_choice);

		switch (n_you_choice) {

		case LIBRARY: 
			show_library_screen();
			break;

		case WELCOM: 
			add_members_screen();
			break;

		case DELETE: 
			delete_parties_screen();
			break;

		case SORT:
			sort_member_screen();
			break;

		default:
			end_game_flag = 1;
			to_free_array();
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
	printf("3:仲間をソート\n");
	printf("上記以外:終了\n");
	
	scanf_s("%d", &button);
	rewind(stdin);
	return(button);

}


void show_library_screen(void) {
	MEMBER_IS_ENPTY(g_person)
	
	printf("閲覧方法を選んでください\n");
	printf("\n");
	int button = TO_STANDBY_INPUT_ERROR;

	printf("0:特定の仲間の閲覧\n");
	printf("1:全仲間の閲覧\n");
	printf("上記以外：やっぱりやめる\n");
	printf("\n");
	button = very_safety_input(SINGLE_UNIT, ALL_UNIT);
	SAFETY_CHECK(button)


		switch (button) {
		case SINGLE_UNIT:
			printf("何人目の仲間を閲覧しますか？\n");
			button = very_safety_input(1, g_person);
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

void add_members_screen(void) {
	MEMBER_IS_FULL(g_person)
	printf("仲間の名前を教えて下さい\n");
	

	char sz_Names[PERSON_NAME] = { 0 };
    int button = TO_STANDBY_INPUT_ERROR;

	scanf_s("%s", sz_Names, PERSON_NAME);//21文字を入力すると弾かれる　//maybe:要修正？？
	strcpy_s(pst_parties[g_person]->sz_name, PERSON_NAME,sz_Names);
	rewind(stdin);

	printf("仲間の性別を教えて下さい\n");
	printf("0(男)\n");
	printf("1(女)\n");
	

	
	button = very_safety_input(0,1);
	SAFETY_CHECK(button)
	pst_parties[g_person]->b_gender = button;
	

	printf("仲間の職業を教えて下さい\n");
	for (int i = 0; i < WORK; i++) {
		printf("%d(%s)\n", i, psz_job_List[i]);
	}

	button = very_safety_input(0,WORK);
	SAFETY_CHECK(button)
	pst_parties[g_person]->n_job = button;

	printf("次の仲間が追加されました\n");
	printf("\n");
	show_menber(g_person);
	g_person ++;

}

void show_menber(int person_number) {
	printf("名前 : %s\n", pst_parties[person_number]->sz_name);

	if (pst_parties[person_number]->b_gender == 0) {
		printf("性別 : %s\n", "男");
	}
	else printf("性別 : %s\n", "女");


	int job_look = pst_parties[person_number]->n_job;
	printf("職業 : %s\n", psz_job_List[job_look]);
	printf("\n");

}

void delete_parties_screen() {
	MEMBER_IS_ENPTY(g_person)
	printf("特定の仲間を削除");
	printf("\n");

	int button = TO_STANDBY_INPUT_ERROR;

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

		button = very_safety_input(1, g_person);
		SAFETY_CHECK(button)
		
		int partys_number = button - 1;

		delete_person(partys_number);
		g_person--;

		for (int i = partys_number; i < g_person; i++) {
			to_struct_array_copy(partys_number, partys_number+1);
		}
		break;

	case ALL_UNIT: 
		memset(pst_parties, 0, sizeof(pst_parties));
		g_person = 0;
		break;

	default:
		break;
	}

return;
}


void sort_member_screen() {
	MEMBER_IS_ENPTY(g_person)
	int button = TO_STANDBY_INPUT_ERROR;

	printf("ソートの方法を選んでください￥ｎ");
	printf("\n");
	printf("\n");
	printf("0:性別順にソートする\n");
	printf("1:職業順にソートする\n");
	printf("上記以外:やっぱりやめる\n");

	button = very_safety_input(GEMDER,JOB);
	SAFETY_CHECK(button)

		switch (button) {
		case GEMDER:
			to_member_sort(GEMDER,0,g_person);
			break;
		
		case JOB:
			to_member_sort(JOB, 0, g_person);
			break;
		}

}
	

void delete_person(int choice) {
	memset(&pst_parties[choice], 0, sizeof(pst_parties[choice]));


}


int very_safety_input(int lowest, int highest) {
	int button;
	int input_check;

	
	input_check = scanf_s("%d", &button);
	rewind(stdin);
	int  scan_check = (input_check!= 1);
	int  numbers_check = (button < lowest || button > highest);

	//scanf_sが成功して、正統な入力の範囲だった時
	if (scan_check || numbers_check) {
		button = TO_STANDBY_INPUT_ERROR;
	}

return(button);
}

void to_struct_array_copy(int copy_terget,int copy_source) {
	strcpy_s(pst_parties[copy_terget]->sz_name,PERSON_NAME, pst_parties[copy_source]->sz_name);
	pst_parties[copy_terget]->b_gender = pst_parties[copy_source]->b_gender;
	pst_parties[copy_terget]->n_job = pst_parties[copy_source]->n_job;
}


void to_swap_member(MEMBER **swap_terget, MEMBER **swap_source) {
	MEMBER *vacation = { 0 };
	vacation = *swap_terget;
	*swap_terget = *swap_source; 
	*swap_source = vacation;
	
}

void to_member_sort(int GENDER_OR_JOB, int lowest, int highest){
	switch (GENDER_OR_JOB) {
	
	case GEMDER:
		for (int i = lowest; i < highest-1; i++) {
			for (int j = i + 1; j < highest; j++) {
				if (pst_parties[i]->b_gender > pst_parties [j]->b_gender) {
					to_swap_member(&pst_parties[i], &pst_parties[j]);
				}			
			}
		}

		break;
	
	
	case JOB:
		for (int i = lowest; i < highest - 1; i++) {
			for (int j = i + 1; j < highest; j++) {
				if (pst_parties[i]->n_job > pst_parties[j]->n_job) {
					to_swap_member(&pst_parties[i], &pst_parties[j]);
				}
			}
		}

		break;
	}

}

void to_malloc_array(void) {
	for (int i = 0; i < COMPANY; i++) {
		pst_parties[i] = (MEMBER*)malloc( sizeof(MEMBER));
	}
}

void to_free_array(void) {
	for (int i = 0; i < COMPANY; i++) {
		free(pst_parties[i]);
		pst_parties[i] = 0;
	}
}