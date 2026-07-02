#include <stdio.h>
#include <stdlib.h>


int menber = 0;
int push;

int start_screen(int pu) {
 printf("ルイーダの酒場にようこそ\n");
	 printf("現在の仲間の数：%d\n",menber);
	 printf("\n");
	 
	 
	 printf("0:仲間の閲覧\n");
	 printf("1:仲間の追加\n");
	 printf("2:仲間の削除\n");
     
	 scanf_s("%d", &pu);

}

int library_screen(int pu) {
	printf("閲覧方法を選んでください\n");
	printf("\n");

	printf("0:特定の仲間の閲覧\n");
	printf("1:全仲間の閲覧\n");
	printf("上記以外：やっぱりやめる\n");
    scanf_s("%d", &pu);

}

int fire_screen(int pu) {
	printf("特定の仲間を削除");
	printf("\n");
	
	printf("0:特定の仲間を削除\n");
	printf("1:全仲間を削除\n");
	printf("上記以外：やっぱりやめる\n");
    scanf_s("%d", &pu);

}


struct menber {
	char name[13];
	int  gender;
	char job;
};


 main(void) {
	 start_screen(push);

	 if (push == 0) {
		 library_screen(push);
	 }
	 else if (push == 1) {}
	 else if (push == 2) { 
		 fire_screen(push);
	 }
	 else 

 return 0;
}