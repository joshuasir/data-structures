#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define MAX 4

int depth = 0;

struct Profile{
	char nama[255];
	char NIM[255];
};
struct Bucket{
	int ctr;
	Profile *key[MAX+1];
};
struct Folder{
	int depth;
	Bucket *bucks;
}*fold;

unsigned long djb2(const char *str){
    unsigned long hash = 5381;
    int c;
    while (c = *str++)
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    return hash;
}
int getLeastBit(unsigned long key,int depth){
	return key%(1<<(depth));
}
void reSize(){
	depth+=1;
	fold = (Folder *)realloc(fold,(1<<depth)*(sizeof *fold));
	if(depth>1){
		for(int i = 1<<(depth-1);i<(1<<(depth));i++){
			fold[i].bucks = fold[i%(1<<(depth-1))].bucks;
			fold[i].depth = depth;
		}
	}else{
		fold[0].bucks = fold[1].bucks = NULL;
		fold[0].depth = fold[1].depth = 1;
	}
}
Profile *createUser(const char *name,const char *NIM){
	Profile *toAdd = (Profile *) malloc(sizeof(Profile));
	strcpy(toAdd->nama,name);
	strcpy(toAdd->NIM,NIM);
	return toAdd;
}
Bucket *createBucket(){
	Bucket *toAdd = (Bucket *) malloc(sizeof(Bucket));
	toAdd->ctr = 0;
	memset(toAdd->key,NULL,sizeof(toAdd->key));
	return toAdd;
}
void split(int loc){
	int newDepth = fold[loc].depth+1;
	int j=0,bit;
	
	for(int i=0;i<fold[loc].bucks->ctr;i++){
	 	bit = getLeastBit(djb2(fold[loc].bucks->key[i]->nama),newDepth);
		if(bit == loc){
			fold[bit].bucks->key[j++] = fold[loc].bucks->key[i];
		}else{
			if((fold[bit].bucks == fold[loc].bucks )){
	 			fold[bit].bucks = createBucket();
			}
			fold[bit].bucks->key[fold[bit].bucks->ctr++] = fold[loc].bucks->key[i];
		}
		
	}

	fold[loc].bucks->ctr = j;
	fold[loc].depth+=1;
}
void insert(const char *name,const char *NIM){
	int loc = getLeastBit(djb2(name),depth);
	Profile *toAdd = createUser(name,NIM);
	if(!fold[loc].bucks){
		fold[loc].bucks = createBucket();
	}
	
	fold[loc].bucks->key[fold[loc].bucks->ctr++] = toAdd;
	while(fold[loc].bucks->ctr > MAX){
		if(fold[loc].depth == depth){
			reSize();
		}
		split(loc);
	}
}
void printTable(){
	for(int i=0;i<(1<<depth);i++){
		if(fold[i].bucks){
			for(int j=0;j<fold[i].bucks->ctr;j++){
				printf("%s %d ->",fold[i].bucks->key[j]->nama,getLeastBit(djb2(fold[i].bucks->key[j]->nama),fold[i].depth));
			}
			puts("");
		}
	}
}
int main(){
	reSize();
	insert("Bruce Wayne","2440022634");
	insert("Clark Kent","244231634");
	insert("Diana Prince","24403123134");
	insert("Barry Allen","244231634");
	insert("John Jones","2440023138634");
	insert("Shiera Hall","244001321334");
	insert("Joshua Sirusstara","243021321334");
	insert("John William","2430276521334");
	insert("John Doe","22330276521334");
	printTable();

}