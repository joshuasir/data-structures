#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<conio.h>
#include <unistd.h>
#include<time.h>

void delay(int msec) {
	clock_t start=clock();
	while(clock()<start+msec);
}
struct queue {
	int x, y;
	queue *parent;
	queue *next;
	queue *prev;
}*vhead,*vtail,*head,*tail,*curr;


int getMap(char map[][200]) {
	FILE *ptr=fopen("level1.txt","r");
	int i=0;
	while(!feof(ptr)) {
		fscanf(ptr,"%[^\n]\n",map[i++]);
	}
	fclose(ptr);
	return i;
}
queue *createQ(int x,int y,queue *parent) {
	queue *q=(queue *)malloc(sizeof(queue));
	q->next=q->prev=NULL;
	q->parent=parent;
	q->x=x;
	q->y=y;
}
void pushTail(int x,int y,queue *parent) {
	queue *q=createQ(x,y,parent);
	if(!head) {
		head=tail=q;
	} else {
		tail->next=q;
		q->prev=tail;
		q->next=NULL;
		tail=q;
	}
}
void popHead() {
	if(!head) {
		return;
	} else if(head==tail) {
		head->next=head->prev=NULL;
		free(head);
		head=tail=NULL;
	} else {
		queue *temp=head;
		head=head->next;
		head->prev=head->prev->next=NULL;
		free(temp);
		temp=NULL;
	}
}
void popQ(queue *curr) {
	if(!curr) {
		return;
	} else {
		while(curr->next) {
			queue *temp=curr;
			curr=curr->next;
			curr->prev=curr->prev->next=curr->parent=NULL;
			free(temp);
			temp=NULL;
		}
		head=tail=vhead=vtail=NULL;
		curr=NULL;
		free(curr);
	}
}
void pushSeen(int x,int y) {
	queue *q=createQ(x,y,NULL);
	if(!vhead) {
		vhead=vtail=q;
	} else {
		vtail->next=q;
		q->prev=vtail;
		q->next=NULL;
		vtail=q;
	}
}
int findQ(queue *rhead,int x,int y) {
	curr=rhead;
	if(!curr) {
		return 0;
	}
	while(curr->next&&(curr->x!=x||curr->y!=y)) {
		curr=curr->next;
	}
	if(curr->x==x&&curr->y==y) {
		return 1;
	} else {
		return 0;
	}
}

void printMap(char map[][200],int rlen) {
	for(int i=0; i<rlen; i++) {
		printf("%s\n",map[i]);
	}
}
int stop =0;
queue *floodfill(queue *curr,char map[][200]) {

	if(curr) {
		if(!findQ(vhead,curr->x,curr->y)) {
			pushSeen(curr->x,curr->y);
			if(map[curr->y][curr->x]=='P') {
				if(curr->parent) {
					while(curr->parent->parent) {
						curr=curr->parent;
					}
				}
				return curr;
			}
			if((curr->x)+1<75&&map[curr->y][(curr->x)+1]!='#') {
				pushTail((curr->x)+1,curr->y,curr);
			}
			if((curr->y)+1<23&&map[(curr->y)+1][curr->x]!='#') {
				pushTail(curr->x,(curr->y)+1,curr);
			}
			if((curr->x)-1>=0&&map[curr->y][(curr->x)-1]!='#') {
				pushTail((curr->x)-1,curr->y,curr);
			}
			if((curr->y)-1>=0&&map[(curr->y)-1][curr->x]!='#') {
				pushTail(curr->x,(curr->y)-1,curr);
			}
		}
		return floodfill(curr->next,map);
	}
	return NULL;
}
void initMap(char map[][200]) {
	map[2][2]='P';
	map[19][2]='E';
	map[2][71]='E';
	map[19][71]='E';
	for(int i=0; i<5; i++) {
		int ynum=rand()%22;
		int xnum=rand()%74;
		if(map[ynum][xnum]!=' ') {
			i--;
			continue;
		}
		map[ynum][xnum]='*';
	}
	map[15][73]='|';
}
int Pmovement(char grid[][200],int *locx,int *locy,int moves,int *fragments) {
	if(moves=='a') {
		if(grid[*locy][*locx-1]=='#'||grid[*locy][*locx-1]=='|') {
			goto here;
		} else if(grid[*locy][*locx-1]=='*') {
			*fragments+=1;
		}
		grid[*locy][*locx]=' ';
		grid[*locy][*locx-1]='P';
		*locx-=1;
	} else if(moves=='w') {
		if(grid[*locy-1][*locx]=='#'||grid[*locy-1][*locx]=='|') {
			goto here;
		} else if(grid[*locy-1][*locx]=='*') {
			*fragments+=1;
		}
		grid[*locy][*locx]=' ';
		grid[*locy-1][*locx]='P';
		*locy-=1;
	} else if(moves=='s') {
		if(grid[*locy+1][*locx]=='#'||grid[*locy+1][*locx]=='|') {
			goto here;
		} else if(grid[*locy+1][*locx]=='*') {
			*fragments+=1;
		}
		grid[*locy][*locx]=' ';
		grid[*locy+1][*locx]='P';
		*locy+=1;
	} else if(moves=='d') {
		if(grid[*locy][*locx+1]=='#'||grid[*locy][*locx+1]=='|') {
			goto here;
		} else if(grid[*locy][*locx+1]=='*') {
			*fragments+=1;
		}
		grid[*locy][*locx]=' ';
		grid[*locy][*locx+1]='P';
		if(*locx+1>=74) {
			return 1;
		}
		*locx+=1;
	}
	if(*fragments==5) {
		grid[15][73]=' ';
	}
here:
	system("cls");
	return 0;
}

void Emovement(char grid[][200],int locx,int locy,int newx,int newy,int loops) {
	grid[locy][locx]='E';
	grid[newy][newx]=' ';
}
int main() {
	queue *enemy;
	int choose,difficulty,row,column,fragments,locx,locy,win=0,loops=0,ex=0,ey=0,ex1=0,ey1=0,ex2=0,ey2=0;
	char grid[200][200],moves;
	do {
		printf("1. Play\n2. How to play\n3. Exit game\n");
		scanf("%d",&choose);
		getchar();
		switch(choose) {
			case 1:
				while(true) {
					system("cls");
					printf("Select Difficulty\n1. Insane\n2. Hard\n3. Easy\n");
					scanf("%d",&difficulty);
					getchar();
					system("cls");
					if(difficulty>3||difficulty<1) {
						printf("Choose a difficulty!\n");
						getchar();
						continue;
					}
					break;
				}
				row=getMap(grid); //22 x 74
				initMap(grid);
				locx=2;
				locy=2;
				ex=2,ey=19,ex1=71,ey1=2,ex2=71,ey2=19;;
				fragments=0;
				loops=difficulty;
				win=1;
				printMap(grid,row);
				printf("Key Fragments Collected : %d\n",fragments);
				moves=getch();
				win=Pmovement(grid,&locx,&locy,moves,&fragments);
				for(;; loops=((loops+1)%4)) {
					moves=NULL;
					if(kbhit()) {
						moves=getch();
					}
					win=Pmovement(grid,&locx,&locy,moves,&fragments);
					printMap(grid,row);
					printf("Key Fragments Collected : %d\n",fragments);
					if(loops&&!(loops%difficulty)) {
						pushTail(ex,ey,NULL);
						enemy=floodfill(head,grid);
						if((ex2!=enemy->x||ey2!=enemy->y)&&(ex1!=enemy->x||ey1!=enemy->y)) {
							Emovement(grid,enemy->x,enemy->y,ex,ey,loops);
							ex=enemy->x,ey=enemy->y;
						}
						popQ(head);
						popQ(vhead);
						if((ex==locx&&ey==locy)) {
							system("cls");
							printf("you lose!\n");
							getchar();
							system("cls");
							break;
						}
						pushTail(ex1,ey1,NULL);
						enemy=floodfill(head,grid);
						if((enemy->x!=ex||enemy->y!=ey)&&(enemy->x!=ex2||enemy->y!=ey2)) {
							Emovement(grid,enemy->x,enemy->y,ex1,ey1,loops);
							ex1=enemy->x,ey1=enemy->y;
						}
						popQ(head);
						popQ(vhead);
						if((ex1==locx&&ey1==locy)) {
							system("cls");
							printf("you lose!\n");
							getchar();
							system("cls");
							break;
						}
						pushTail(ex2,ey2,NULL);
						enemy=floodfill(head,grid);
						if((ex!=enemy->x||ey!=enemy->y)&&(ex1!=enemy->x||ey1!=enemy->y)) {
							Emovement(grid,enemy->x,enemy->y,ex2,ey2,loops);
							ex2=enemy->x,ey2=enemy->y;
						}
						popQ(head);
						popQ(vhead);
						if((ex2==locx&&ey2==locy)) {
							system("cls");
							printf("you lose!\n");
							getchar();
							system("cls");
							break;
						}
					}
					if(win) {
						system("cls");
						printf("you win!");
						getchar();
						system("cls");
						break;
					}
				}
				break;
			case 2:
				printf("IMPLEMENTASI ALGORITMA BFS\n");
				getchar();
				break;
			case 3:
				return 0;
			default:
				printf("Invalid\n");
				getchar();
		}
		system("cls");
	} while(choose!=3);
}
