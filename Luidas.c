#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <ctype.h>

#define PERSON_NAME 41
#define COMPANY 20
#define WORK 6
#define GENDER 2
#define INPUT_FAILED -1
#define SAVE_TITLE 11
#define SAVE_PATH 15
#define SAVE_SLOT 8
#define NEW_DATA -1
#define HEADER_SIZE 15
#define HEADER 1
#define TXT_PATH 16

enum CHOICE {
	LIBRARY,
	WELCOM,
	DELETE,
	SORT,
	SAVE_LOAD,
};

enum CHOICE_UNIT {
	SINGLE_UNIT,
	ALL_UNIT,
};

enum SAVE_LOAD {
	SAVE,
	LOAD,
};

enum TO_CHOICE_SORT {
	GENDERS,
	JOB,
};

typedef struct member {
	char  sz_name[PERSON_NAME];
	int  n_gender;
	int   n_job;
	struct member* p_next;
	struct member* p_prev;
}MEMBER;


static MEMBER* gp_head = { NULL };

char* psz_job_List[WORK] = {
	{"勇者"},
	{"戦士"},
	{"僧侶"},
	{"魔法使い"},
	{"盗賊"},
	{"遊び人"},
};

char* p_gender_list[GENDER] = {
	{"男"},
	{"女"},
};



static int g_person = 0;//現在のパーティー人数を格納する
static char (*g_MemoeyCard_copy)[SAVE_PATH] = NULL;
static int  g_save_data_count = 0;

int show_start_screen(int button);//スタート画面
void add_members_screen();//仲間追加画面
void show_member_screen();//登録済仲間ライブラリ閲覧の画面
void delete_parties_screen(); //仲間解雇の画面
void sort_member_screen();//登録済みの仲間を並べ替える画面
void save_load_screen();//現在データの保存及び保存済みデータの読込画面


//引数の番号のリストのポインタを返す関数
MEMBER* get_member(int number);
int very_safety_input(int lowest, int highest);//不正な入力を弾く関数
void show_member(MEMBER* temp);//仲間ステータス閲覧関数
//不正な入力の際に流れるセリフの関数
void to_error_reaction(void);


//仲間が居ない状態での解雇・閲覧・ソートの選択を弾く。
int member_is_enpty(int now_member);
void delete_person(int button);//仲間単体分メモリの開放を行う関数
void to_free_all_array(void);//全仲間分の取得したメモリの開放関数


//:ソート実行の為、対象リストからノードを一つずつ取り外す関数
MEMBER* insartion_sort(MEMBER* base, MEMBER* key, bool(*compare_member)(MEMBER*, MEMBER*));
//:ソートの実際の処理を行う関数
void to_member_sort(int sort_type);

bool target_gender(MEMBER* base, MEMBER* key);//比較を目的に対象の値を取り出す
bool target_job(MEMBER* base, MEMBER* key);//上記同

//入力された値に対応して、ソートの対象を取り出す為の関数を選ぶ。
bool(*target_list(int sort_type))(MEMBER*, MEMBER*);


void to_save_member();//現在のリストの中身を保存する
void to_load_member();//保存されているデータを読み込んでノードを作成する

void show_MemCard();//Memory_Cardフォルダの中身を一覧表示する
void check_error_fnf(int choice, int max_count);//ファイル喪失時にMemory_Cardフォルダの中身からpathを削除する

void make_MemCard_copy_array();//外部管理データを参照しその内容をコピーした配列を作成する関数
void write_header();//セーブデータ管理ファイルのヘッダに、全件数を書き込む関数

//プログラム内でセーブデータ管理ファイルを開く際、入力されたパスをファイルに追加し、ヘッダ部分を更新する。
void data_save(char save_path[SAVE_PATH]);
void grow_MemCard_copy(char save_path[SAVE_PATH]);//管理データのコピーに新しいデータを追加する関数
void truncate_MemCard_copy(int target_num);//管理データのコピーからデータを削除する関数


int main(void) {
	bool end_game_flag = 0;
	make_MemCard_copy_array();

	while (end_game_flag == 0) {
		int n_you_choice = INPUT_FAILED;
		n_you_choice = show_start_screen(n_you_choice);

		switch (n_you_choice) {

		case LIBRARY:
			show_member_screen();
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

		case SAVE_LOAD:
			save_load_screen();
			break;

		default:
			end_game_flag = 1;
			to_free_all_array();
			break;
		}
	}
}


int  show_start_screen(int button) {
	printf("ルイーダの酒場にようこそ\n");
	printf("現在の仲間の数：%d\n", g_person);
	printf("\n");

	printf("0:仲間の閲覧\n");
	printf("1:仲間の追加\n");
	printf("2:仲間の削除\n");
	printf("3:仲間をソート\n");
	printf("4:セーブ/ロードする\n");
	printf("上記以外:終了\n");

	scanf_s("%d", &button);
	rewind(stdin);
	return(button);
}

void add_members_screen(void) {


	MEMBER* p_new_member;

	char check_names[PERSON_NAME] = { 0 };
	int gender = INPUT_FAILED;
	int job = INPUT_FAILED;
	int check_one = INPUT_FAILED;
	int check_two = INPUT_FAILED;

	printf("仲間の名前を教えて下さい\n");


	check_one = scanf_s("%s", check_names, PERSON_NAME);
	rewind(stdin);
	if (check_one != 1) {
		printf("名前が長すぎる");
		return;
	}

	printf("仲間の性別を教えて下さい\n");
	printf("0(男)\n");
	printf("1(女)\n");

	gender = very_safety_input(0, 1);
	if (gender == INPUT_FAILED) {
		to_error_reaction();
		return;
	}

	printf("仲間の職業を教えて下さい\n");
	for (int i = 0; i < WORK; i++) {
		printf("%d(%s)\n", i, psz_job_List[i]);
	}

	job = very_safety_input(0, WORK - 1);
	if (job == INPUT_FAILED) {
		to_error_reaction();
		return;
	}

	p_new_member = (MEMBER*)malloc(sizeof(MEMBER));
	if (p_new_member == NULL) {
		printf("メモリの確保に失敗した、戻ろう");
		return;
	}


	strcpy_s(p_new_member->sz_name, PERSON_NAME, check_names);
	p_new_member->n_gender = gender;
	p_new_member->n_job = job;

	if (g_person == 0) {
		p_new_member->p_next = NULL;
		p_new_member->p_prev = NULL;

		gp_head = p_new_member;
	}
	else {
		MEMBER* p_end = get_member(g_person - 1);

		p_new_member->p_next = NULL;
		p_new_member->p_prev = p_end;

		p_end->p_next = p_new_member;
	}


	printf("次の仲間が追加されました\n");
	printf("\n");
	show_member(p_new_member);
	g_person++;

}

void show_member(MEMBER* temp) {
	printf("名前 : %s\n", temp->sz_name);

	int gender_look = temp->n_gender;
	printf("性別： %s\n", p_gender_list[gender_look]);

	int job_look = temp->n_job;
	printf("職業 : %s\n", psz_job_List[job_look]);
	printf("\n");


}

void show_member_screen(void) {
	int member_check = 0;
	member_check = member_is_enpty(g_person);
	if (member_check == INPUT_FAILED) {
		return;
	}


	printf("閲覧方法を選んでください\n");
	printf("\n");

	int button = INPUT_FAILED;

	printf("0:特定の仲間の閲覧\n");
	printf("1:全仲間の閲覧\n");
	printf("上記以外：やっぱりやめる\n");

	MEMBER* temp;
	button = very_safety_input(SINGLE_UNIT, ALL_UNIT);
	if (button == INPUT_FAILED) {
		return;
	}

	switch (button) {
	case SINGLE_UNIT:
		printf("何人目の仲間を閲覧しますか？\n");
		button = very_safety_input(1, g_person);
		if (button == INPUT_FAILED) {
			to_error_reaction();
			return;
		}
		int partys_number = button - 1;
		temp = get_member(partys_number);
		show_member(temp);
		break;

	case ALL_UNIT:
		temp = gp_head;
		while (temp != NULL) {
			show_member(temp);
			temp = temp->p_next;
		}
		break;

	default:
		break;
	}
}

void delete_parties_screen() {
	int member_check = 0;
	member_check = member_is_enpty(g_person);
	if (member_check == INPUT_FAILED) {
		return;
	}

	int button = INPUT_FAILED;

	printf("0:特定の仲間を削除\n");
	printf("1:全仲間を削除\n");
	printf("上記以外：やっぱりやめる\n");


	button = very_safety_input(SINGLE_UNIT, ALL_UNIT);
	if (button == INPUT_FAILED) {
		return;
	}

	switch (button) {

	case SINGLE_UNIT:
		printf("現在%d人の仲間がいます\n", g_person);
		printf("何番目の仲間を削除しますか？\n");

		button = very_safety_input(1, g_person);
		if (button == INPUT_FAILED) {
			to_error_reaction();
			return;
		}
		int partys_number = button - 1;
		delete_person(partys_number);
		g_person--;
		break;

	case ALL_UNIT:
		to_free_all_array();
		break;

	default:
		break;
	}

	return;
}

void delete_person(int choice) {
	MEMBER* temp;
	temp = get_member(choice);
	if (temp == NULL) {
		return;
	}
	if (temp->p_prev != NULL) {
		temp->p_prev->p_next = temp->p_next;
	}
	if (temp->p_next != NULL) {
		temp->p_next->p_prev = temp->p_prev;
	}
	if (temp == gp_head) {
		gp_head = temp->p_next;
	}
	free(temp);

}

void sort_member_screen() {
	int member_check = 0;
	member_check = member_is_enpty(g_person);
	if (member_check == INPUT_FAILED) {
		return;
	}
	int button = INPUT_FAILED;

	printf("ソートの方法を選んでください￥ｎ");
	printf("\n");
	printf("\n");
	printf("0:性別順にソートする\n");
	printf("1:職業順にソートする\n");
	printf("上記以外:やっぱりやめる\n");

	button = very_safety_input(GENDERS, JOB);
	if (button == INPUT_FAILED) {
		return;
	}
	to_member_sort(button);

}

void to_member_sort(int sort_type) {
	MEMBER* temp_base = { NULL };
	MEMBER* temp_key = { NULL };
	MEMBER* temp_save;

	temp_key = gp_head;
	bool (*sort_target)(MEMBER*, MEMBER*) = NULL;
	sort_target = target_list(sort_type);

	while (temp_key != NULL) {
		temp_save = temp_key->p_next;
		temp_key->p_next = NULL;
		temp_key->p_prev = NULL;
		temp_base = insartion_sort(temp_base, temp_key, sort_target);
		temp_key = temp_save;
	}
	gp_head = temp_base;
}

MEMBER* insartion_sort(MEMBER* base, MEMBER* key, bool(*compare_member)(MEMBER*, MEMBER*)) {

	MEMBER* temp_base = NULL;
	temp_base = base;
	MEMBER* head;
	head = base;
	MEMBER* insertion_point = NULL;


	while (temp_base != NULL) {
		if (compare_member(temp_base, key) == true) {
			break;
		}
		insertion_point = temp_base;
		temp_base = temp_base->p_next;
	}

	if (temp_base == NULL) {
		if (insertion_point == NULL) {//最初のデータ
			temp_base = key;
			temp_base->p_prev = NULL;
			head = temp_base;
		}
		else {//終端
			insertion_point->p_next = key;
			key->p_prev = insertion_point;
			key->p_next = NULL;
		}
	}
	else if (temp_base->p_prev == NULL) {//先頭
		key->p_next = temp_base;
		key->p_prev = NULL;
		temp_base->p_prev = key;
		head = key;
	}
	else {//中間
		key->p_next = temp_base;
		key->p_prev = temp_base->p_prev;
		temp_base->p_prev->p_next = key;
		temp_base->p_prev = key;
	}
	return(head);
}

bool target_gender(MEMBER* base, MEMBER* key) {
	return(base->n_gender > key->n_gender);
}

bool target_job(MEMBER* base, MEMBER* key) {
	return(base->n_job > key->n_job);
}

bool(*target_list(int sort_type))(MEMBER*, MEMBER*) {
	bool (*target)(MEMBER*, MEMBER*) = NULL;
	switch (sort_type) {
	case GENDERS:
		target = target_gender;
		break;
	case JOB:
		target = target_job;
		break;
	}
	return(target);
}

void save_load_screen() {
	int button = INPUT_FAILED;

	printf("どうする？\n");
	printf("\n");
	printf("0:セーブする\n");
	printf("1:ロードする\n");
	printf("上記以外:やっぱり辞める\n");

	button = very_safety_input(SAVE, LOAD);
	if (button == INPUT_FAILED) {
		return;
	}
	switch (button) {
	case SAVE:
		to_save_member();
		break;
	case LOAD:
		to_load_member();
		break;
	}
}

void to_save_member() {
	int member_check = 0;
	member_check = member_is_enpty(g_person);
	if (member_check == INPUT_FAILED) {
		return;
	}

	FILE* fp = NULL;
	MEMBER* temp_base = gp_head;
	char save_slot[SAVE_TITLE] = { 0 };
	char save_path[SAVE_PATH] = { 0 };
	int check_one = INPUT_FAILED;
	int check_two = INPUT_FAILED;
	int max_title_size = SAVE_TITLE - 1;
	int max_count = g_save_data_count;
	int temp_choice = 0;


	show_MemCard();
	if (max_count == 0) {
		temp_choice = 0;
	}
	else {
		printf("上記以外:新規作成\n");
		printf("上書セーブか新規作成を選んでくれ\n");
		int button = scanf_s("%d", &temp_choice);
		rewind(stdin);
		if (button == INPUT_FAILED) {
			to_error_reaction();
			return;
		}
	}
	if (1 <= temp_choice && temp_choice <= max_count) {
		printf("上書セーブだな？私がやっておこう\n");
		sprintf_s(save_path, SAVE_PATH, "%s", g_MemoeyCard_copy[temp_choice-1]);
	}
	else if (temp_choice < 1 || max_count < temp_choice) {
		printf("セーブデータの名前を入力してくれ\n");
		check_one = scanf_s("%s", save_slot, SAVE_TITLE);
		rewind(stdin);
		if (check_one != 1) {
			printf("長すぎる、ひらがなで%d文字、英語か数字なら%d文字だ\n", max_title_size / 2, max_title_size);
			return;
		}
		sprintf_s(save_path, sizeof(save_path), "%s.txt", save_slot);
		data_save(save_path);
		grow_MemCard_copy(save_path);
	}

	if (fopen_s(&fp, save_path, "wb") != 0) {
		printf("実際のファイルを開くことに失敗した\n");
		return;
	}
	for (int i = 0; i < g_person; i++) {
		fwrite(temp_base->sz_name, sizeof(temp_base->sz_name), 1, fp);
		fwrite(&temp_base->n_gender, sizeof(temp_base->n_gender), 1, fp);
		fwrite(&temp_base->n_job, sizeof(temp_base->n_job), 1, fp);
		temp_base = temp_base->p_next;
	}
	fclose(fp);
}

void to_load_member() {
	int count = 0;
	int max_count = g_save_data_count;
	FILE* fp;

	show_MemCard();
	if (max_count == 0) {
		return;
	}
	printf("全部で %d 組のパーティが記録されている、何番目のパーティを連れてくる？\n", max_count);
	count = very_safety_input(1, max_count);
	if (count == INPUT_FAILED) {
		to_error_reaction();
		return;
	}
	if (fopen_s(&fp, g_MemoeyCard_copy[count-1], "rb") != 0) {
		check_error_fnf(count, max_count);
		return;
	}
	to_free_all_array();

	MEMBER* p_new_member = NULL;
	MEMBER* temp_tail = NULL;
	MEMBER temp = { NULL };

	while ((fread(temp.sz_name, sizeof(temp.sz_name), 1, fp) == 1)
		&&
		(fread(&temp.n_gender, sizeof(temp.n_gender), 1, fp) == 1)
		&&
		(fread(&temp.n_job, sizeof(temp.n_job), 1, fp) == 1))
	{
		p_new_member = (MEMBER*)malloc(sizeof(MEMBER));
		if (p_new_member == NULL) {
			printf("メモリの確保に失敗した\n");
			break;
		}
		*p_new_member = temp;

		if (g_person == 0) {
			p_new_member->p_prev = NULL;
			gp_head = p_new_member;
		}
		else {
			temp_tail->p_next = p_new_member;
			p_new_member->p_prev = temp_tail;
		}
		p_new_member->p_next = NULL;
		temp_tail = p_new_member;
		g_person++;
	}
	if (feof(fp) == 0) {
		printf("ファイルの読込が途中で失敗した、何かおかしいようだ\n");
	}
	else {
		printf("連れて来たぞ\n");
	}
	fclose(fp);
}

void to_free_all_array(void) {
	for (int i = g_person; i > 0; i--) {
		delete_person(i);
	}
	g_person = 0;
}

void to_error_reaction(void) {
	printf("調子が悪い様だな、戻ろう\n");
	printf("\n");
}

int member_is_enpty(int now_member) {
	int temp = 0;
	if (now_member == 0) {
		printf("誰も仲間がいないようだ\n");
		printf("\n");
		temp = INPUT_FAILED;
	}
	return(temp);
}

MEMBER* get_member(int number) {
	MEMBER* now_point = { NULL };
	now_point = gp_head;
	if (number >= g_person || number < 0) {
		now_point = NULL;
	}

	for (int i = 0; i < number; i++) {
		if (now_point != NULL) {
			now_point = now_point->p_next;
		}
	}
	return(now_point);
}

int very_safety_input(int lowest, int highest) {
	int button;
	int input_check;

	input_check = scanf_s("%d", &button);
	rewind(stdin);
	int scan_check = (input_check != 1);
	int numbers_check = (button < lowest || button > highest);


	if (scan_check || numbers_check) {
		button = INPUT_FAILED;
	}
	return(button);
}

void check_error_fnf(int choice, int max_count) {
	int temp_check = 0;
	char save_path[TXT_PATH] = { 0 };
	FILE* base = NULL;
	FILE* copy = NULL;
	char emoty_header[] = "a0a0a0a0a0a0a0\n";
	temp_check = errno;


	if (temp_check != ENOENT) {
		return;
	}
	else {
		printf("恐ろしい事に選択したぼうけんの書は消えてしまった様だ\n");
		if (fopen_s(&base, "Memory_Card.txt", "rb") != 0 ||
			fopen_s(&copy, "Copy_Card.txt", "wb") != 0) {
			if (base != NULL) { fclose(base); }
			if (copy != NULL) { fclose(copy); }
			printf("あっ…ファイルの修正に失敗した\n");
			return;
		}
		
		fwrite(emoty_header, sizeof(char), HEADER_SIZE, copy);
		fseek(base, 15, SEEK_SET);

		for (int target = 0; target < max_count; target++) {
			fgets(save_path, TXT_PATH, base);
			if (target == choice - 1) {
				printf("%sは消えてしまった\n", save_path);
			}
			else fputs(save_path, copy);
		}
	}
	fclose(base);
	fclose(copy);
	remove("Memory_Card.txt");

	if (rename("Copy_Card.txt", "Memory_Card.txt") != 0) {
		perror("何かがおかしい、冒険の書は全て消えてしまった");
	}
	else {
		write_header();
		truncate_MemCard_copy(choice-1);
	}
}

void show_MemCard() {
	for (int i = 0; i < g_save_data_count; i++) {
		printf("%d:%s\n", i + 1, g_MemoeyCard_copy[i]);
	}
	if (g_save_data_count == 0) {
		printf("セーブデータは無いな\n");
	}
}

void make_MemCard_copy_array() {
	long how_many = 0;
	char temp_path[TXT_PATH] = { 0 };
	FILE* fp;
	int src_data_counter = 0;
	int temp;
	int checker = 0;

	if (fopen_s(&fp, "Memory_Card.txt", "rb") != 0) {
		to_error_reaction();
		return;
	}
	fscanf_s(fp, "%ld\n", &how_many);
	g_MemoeyCard_copy = calloc(how_many, SAVE_PATH);
	
	while (fgets(temp_path, TXT_PATH, fp) != NULL) {
		temp_path[strcspn(temp_path, "\r\n")] = '\0';
		sprintf_s(g_MemoeyCard_copy[src_data_counter], SAVE_PATH, "%-14.14s", temp_path);
		src_data_counter++;
	}
	fclose(fp);

	g_save_data_count = how_many;
}

void write_header() {
	FILE* fp;
	int checker = 0;
	long long file_size = 0;

	if (fopen_s(&fp, "Memory_Card.txt", "rb+") != 0) {
		printf("対象ファイルを開くのに失敗した");
		return;
	}
	checker = fseek(fp, 0, SEEK_END);
	if (checker != 0) {
		file_size = -1;
	}
	if (file_size != -1) {
		file_size = ftell(fp);
	}
	checker = fseek(fp, 0, SEEK_SET);
	if (checker != 0) {
		file_size = -1;
	}
	if (file_size != -1) {
		fprintf_s(fp, "%14lld\n", (file_size / SAVE_PATH) - HEADER);
	}
	fclose(fp);
}

void data_save(char save_path[SAVE_PATH]) {
	FILE* fp;
	long long temp = 0;
	char emoty_header[] = "a0a0a0a0a0a0a0\n";

	if (fopen_s(&fp, "Memory_Card.txt", "rb+") != 0) {
		printf("対象ファイルを開くのに失敗した");
		return;
	}
	if (fscanf_s(fp, "%014lld\n", &temp) != 1) {
		fwrite(emoty_header, sizeof(char), HEADER_SIZE, fp);
	}
	fseek(fp, 0, SEEK_END);
	fprintf(fp, "%-14.14s\n", save_path);
	fclose(fp);

	write_header();
}

void grow_MemCard_copy(char save_path[SAVE_PATH]) {
	char* temp;
	int max_count = g_save_data_count;

	temp = realloc(g_MemoeyCard_copy, (max_count + 1) * SAVE_PATH);
	if (temp == NULL) {
		printf("MemoeyCard_copyの拡張に失敗した");
		return;
	}
	g_MemoeyCard_copy = temp;
	sprintf_s(g_MemoeyCard_copy + (max_count), SAVE_PATH, "%-14.14s", save_path);
	g_save_data_count++;
}

void truncate_MemCard_copy(int target_num) {
	char* temp;
	int max_count = g_save_data_count;
	int move_range = 0;

	if (max_count <= 1) {
		free(g_MemoeyCard_copy);
		g_MemoeyCard_copy = NULL;
		g_save_data_count = 0;
		return;
	}
	move_range = (max_count-1) - target_num;
	memmove(g_MemoeyCard_copy + (target_num), g_MemoeyCard_copy + (target_num + 1), move_range * SAVE_PATH);

	temp = realloc(g_MemoeyCard_copy, (max_count - 1) * SAVE_PATH);
	if (temp == NULL) {
		printf("MemoeyCard_copyの縮小に失敗した");
		return;
	}
	g_MemoeyCard_copy = temp;
	g_save_data_count--;
}
/*
void temp_bogo_sort(int target) {
	MEMBER* temp;
	MEMBER* bogo_head;
	MEMBER* bogo_tail;
	MEMBER* check_head;

	int* Bingo_Card = (int*)calloc(g_person, sizeof(int));
	bool Bingo = false;
	srand((unsigned int)time(NULL));

	while (Bingo == false) {
		temp = NULL;
		bogo_head = NULL;
		bogo_tail = NULL;
		check_head = NULL;

		int try = 0;
		int balls = g_person;


		for (int i = 0; i < g_person; i++) {
			Bingo_Card[i] = i;
		}


		while (try < g_person) {
			int Bingo_Ball = rand() % balls;
			int lottery = Bingo_Card[Bingo_Ball];
			temp = gp_head;

			for (int i = 0; i < Bingo_Ball; i++) {
				temp = temp->p_next;
			}
			if (bogo_head == NULL) {
				bogo_head = temp;
				bogo_tail = temp;
				temp->p_next = NULL;
				temp->p_prev = NULL;
			}
			else {
				bogo_tail->p_next = temp;
				temp->p_prev = bogo_tail;
				temp->p_next = NULL;
				bogo_tail = temp;
			}
			try++;
			Bingo_Card[Bingo_Ball] = Bingo_Card[balls - 1];
			balls--;
		}
		check_head = bogo_head;

		switch (target) {
		case GENDERS:
			for (int check = 0; check < g_person - 1; check++) {
				if (check_head->n_gender > check_head->p_next->n_gender) {
					break;
				}
				check_head = check_head->p_next;
			}
			if (check_head->p_next == NULL) {
				Bingo = true;

			}
			break;
		case JOB:
			for (int check = 0; check < g_person - 1; check++) {
				if (check_head->n_job > check_head->p_next->n_job) {
					break;
				}
				check_head = check_head->p_next;

			}
			if (check_head->p_next == NULL) {
				Bingo = true;
			}
			break;
		}

	}
	free(Bingo_Card);
	gp_head = bogo_head;
}*/