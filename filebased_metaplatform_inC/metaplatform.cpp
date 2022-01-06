#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<dirent.h>
int CUser=0; 
typedef struct fri{
	char email[40];
	char name[40];
	char gender[40];
};
typedef struct users{
	char email[40];
	char password[40];
	char name[40];
	char gender[40];
	char comment[40];
	int end;
	fri friends[40];
};
users user[100];
void PromptUser(){
	int dex=0;
	FILE *fp=fopen("user_data.txt","r");
	while(fscanf(fp,"%[^#]#%[^#]#%[^#]#%[^#]%[^\n]\n",user[dex].email,user[dex].password,user[dex].name,user[dex].gender,user[dex].comment)!=EOF){
		
		dex++;
	}
	CUser=dex;
	fclose(fp);
}

void writeUser(){
	FILE *fp=fopen("user_data.txt","w");
	for(int i=0;i<CUser;i++){
		fprintf(fp,"%s#%s#%s#%s%s\n",user[i].email,user[i].password,user[i].name,user[i].gender,user[i].comment);

	}
	fclose(fp); 
}
void appendUser(){
	FILE *fp=fopen("user_data.txt","a");
		fprintf(fp,"%s#%s#%s#%s%s\n",user[CUser-1].email,user[CUser-1].password,user[CUser-1].name,user[CUser-1].gender,user[CUser-1].comment);
	fclose(fp); 
}
int valUser(char usermail[],char userpass[],int L,int R){
	int M=(L+R)/2;
	if(L>R){
		return -1;
	}
	
	if(strcmp(usermail,user[M].email)==0){
		
		if(strcmp(userpass,user[M].password)==0){
			return M;
		}else{
			return -2;
		}
	}else if(strcmp(usermail,user[M].email)<0){
		valUser(usermail,userpass,L,M-1);
	}else{
		valUser(usermail,userpass,M+1,R);
	}
}

void getFriends(int mind){
	int dex=0;
	char str[255];
	strcpy(str,user[mind].email);
	strcat(str,".txt");
	int len=strlen(str);
	str[len]='\0';
		char str2[]={"User Friendlist/"};
		strcat(str2,str);
		FILE *fp=fopen(str2,"r");
		while(fscanf(fp,"%[^#]#%[^#]#%[^\n]\n",user[mind].friends[dex].email,user[mind].friends[dex].name,user[mind].friends[dex].gender)!=EOF){
		dex++;
		}
		user[mind].end=dex;
		fclose(fp);
		
}
void displayFriend(int mind,int hal){
	printf("No. Name Gender Email\n");
	for(int i=hal-1;i<hal+4;i++){
		printf("%d. %s %s %s\n",i+1,user[mind].friends[i].name,user[mind].friends[i].gender,user[mind].friends[i].email);
	}
}
void sortUserMail(){
	for(int j=CUser-1;j>=0;j--){
	for(int i=1;i<=j;i++){
		if(strcmp(user[i].email,user[i-1].email)<0){
			users temp=user[i];
			user[i]=user[i-1];
			user[i-1]=temp;
		}
	}
}}
void display(int hal,int mind){
	printf("No. Name Gender Email\n");
	for(int i=hal-1;i<hal+4&&hal+4<CUser-1;i++){
		printf("%d. %s %s %s\n",i+1,user[i].name,user[i].gender,user[i].email);
	}
}
void updateStat(int mind){
	char str[255];
	for(;;){
	printf("What's on your mind ?\n");
	scanf("%[^\n]",str);
	getchar();
	if(strlen(str)<=1){
		printf("Your status cannot be empty!\n");
		getchar();
	}else{
		char str2[]="#";
		strcat(str2,str);
		strcpy(user[mind].comment,str2);
		str2[strlen(str2)]='\0';
		break;
	}}}
void writeUserFriend(int key1,int key2){
	char str[]="User Friendlist/";
	strcat(str,user[key1].email);
	strcat(str,".txt");
	FILE *fp=fopen(str,"w");
	for(int i=0;i<user[key1].end;i++){
		fprintf(fp,"%s#%s#%s\n",user[key1].friends[i].email,user[key1].friends[i].name,user[key1].friends[i].gender);
	}
	fclose(fp);
	char str1[]="User Friendlist/";
	strcat(str1,user[key2].email);
	strcat(str1,".txt");
	FILE *fp1=fopen(str1,"w");
	for(int i=0;i<user[key2].end;i++){
		fprintf(fp1,"%s#%s#%s\n",user[key2].friends[i].email,user[key2].friends[i].name,user[key2].friends[i].gender);
	}
	fclose(fp1);
}
int main(){
	int choose,back=0;
	PromptUser();
	for(;;){
	sortUserMail();
	system("cls");
	printf("1. Login\n2. Register\n0. Exit\n");
	scanf("%d",&choose);
	getchar();
	switch(choose){
		case 1:
			int cookie;
			for(;;){
			system("cls");
			char Logname[40],Logpass[40];
			printf("Email : ");
			scanf("%[^\n]",Logname);
			getchar();
			printf("Password : ");
			scanf("%s",Logpass);
			getchar();
			cookie=valUser(Logname,Logpass,0,CUser-1);
			if(cookie>=0){
				printf("Login successfully!\n");
				getFriends(cookie);
				getchar();
				break;
			}else{
				printf("Wrong email/password\n");
				getchar();
				if(strcmp(Logname,"0")==0||strcmp(Logpass,"0")==0){
					back=1;
					break;
			}
			}
		}
		if(back){
			back=0;
			break;
		}
		for(;;){
		system("cls");
		printf("Welcome %s\nStatus :\n",user[cookie].name);
		for(int i=1;i<strlen(user[cookie].comment);i++){
			printf("%c",user[cookie].comment[i]);
		}puts("");
		printf("1. Update Status\n2. View Friends\n3. Find People\n4. Log Out\n");
		int choose2,page;
		scanf("%d",&choose2);
		getchar();
		switch(choose2){
			case 1:
				updateStat(cookie);
				writeUser();
				break;
			case 2:
				page=1;
				for(;;){
				system("cls");
				displayFriend(cookie,page);
				printf("1. Back Page\n2. Next Page\n0. Back\n");
				int choose12;
				scanf("%d",&choose12);
				getchar();
				switch(choose12){
					case 1:
						if(page>5){
						page-=5;}else{
							printf("Already first page! \n");
							getchar();
						}
					break;
					case 2:
						if(page+5<=user[cookie].end){
						page+=5;}else{
							printf("Already in the last page \n");
							getchar();
						}
						break;
					case 0:
						back=1;
						break;
				}
				if(back){
					back=0;
					break;
				}
				}
				break;
			case 3:
				page=1;
				for(;;){
					system("cls");
					char str[255];
					strcpy(str,user[cookie].email);
					int len=strlen(str);
					str[len]='\0';
					for(int i=1;i<CUser;i++){
						if(strcmp(user[i-1].email,str)==0){
							users temp=user[i-1];
							user[i-1]=user[i];
							user[i]=temp;
						}
					}
				cookie=CUser-1;
				display(page,cookie);
				printf("1. Add Friend\n2. Back Page\n3. Next Page\n0. Back\n");
				int choose31;
				scanf("%d",&choose31);
				getchar();
				switch(choose31){
					case 1:
						for(;;){
						int choose311;
						printf("Choose a person [ enter 0 to back ]\nInput Number [%d-%d]:",page,page+4);
						scanf("%d",&choose311);
						getchar();
						//choose311--;
						if(choose311<page||choose311>=page+5){
							break;
						}else if(choose311>=page&&choose311<page+5){
							choose311--;
							for(int i=0;i<user[cookie].end;i++){
								if(strcmp(user[cookie].friends[i].name,user[choose311].name)==0){
									back=1;
									printf("Already a friend!\n");
									getchar();
									break;
								}
							}
							if(back){
								back=0;
								continue;
							}
							strcpy(user[cookie].friends[user[cookie].end].email,user[choose311].email);
							strcpy(user[cookie].friends[user[cookie].end].gender,user[choose311].gender);
							strcpy(user[cookie].friends[user[cookie].end].name,user[choose311].name);
							user[cookie].end++;
							strcpy(user[choose311].friends[user[choose311].end].email,user[cookie].email);
							strcpy(user[choose311].friends[user[choose311].end].gender,user[cookie].gender);
							strcpy(user[choose311].friends[user[choose311].end].name,user[cookie].name);
							user[choose311].end++;
							printf("Successfully added!\n");
							getchar();
							writeUserFriend(cookie,choose31);
							//sortUserMail();
							break;
						}
					}
						break;
					case 2:
						if(page>5){
						page-=5;}else{
							printf("already first page!\n");
							getchar();
						}
						break;
						break;
					case 3:
						if(page+5<=CUser){
						page+=5;}else{
							printf("already last page!\n");
							getchar();
						}
						break;
					case 0:
						back=1;
					break;
				}if(back){
					back=0;
			break;
			}}
				break;
			case 4:
				back=1;
				break;
			default:
				break;
		}
		if(back){
			back=0;
			break;
		}
		}
			break;
		case 2:
		for(;;){
			int back=0;
			system("cls");
			char regname[40],regmail[40],regpass[40],regender[40];
			printf("Please insert your profile info [ enter 0 to back ]\n");
			printf("Name [ 5 - 18 ]:");
			scanf("%[^\n]",regname);
			getchar();
			if(strcmp(regname,"0")==0){
				break;
			}
			if(strlen(regname)<5||strlen(regname)>8){
				continue;;
			}
			for(;;){
			printf("Email [ Unique ]: ");
			scanf("%[^\n]",regmail);
			getchar();
			if(strcmp(regmail,"0")==0){
				back=1;
				break;
			}
			if(valUser(regmail,regmail,0,CUser-1)==-1){
				break;
			}
			}
			if(back){
				back=0;
				break;
			}
			for(;;){
			printf("Password [ 6 - 18 ]: ");
			scanf("%s",regpass);
			getchar();
			if(strcmp(regpass,"0")==0){
				back=1;
				break;
			}
			if(strlen(regpass)>=6&&strlen(regpass)<=18){
				break;
			}
			}
			if(back){
				back=0;
				break;
			}
			for(;;){
			printf("Gender [ Male/Female ]: ");
			scanf("%s",regender);
			getchar();
			if(strcmp(regender,"Male")==0||strcmp(regender,"Female")==0){
				break;
			}
			if(strcmp(regender,"0")==0){
				back=1;
				break;
			}
			}
			if(back){
				back=0;
				break;
			}
			printf("successfully regeistered an account\n");
			getchar();
			strcpy(user[CUser].name,regname);
			strcpy(user[CUser].email,regmail);
			strcpy(user[CUser].password,regpass);
			strcpy(user[CUser].gender,regender);
			strcpy(user[CUser].comment,"#");
			CUser++;
			appendUser();
			char str4[255];
			strcpy(str4,user[CUser-1].email);
			strcat(str4,".txt");
			int len=strlen(str4);
			str4[len]='\0';
			char str3[]={"User Friendlist/"};
			strcat(str3,str4);
			FILE *fp1=fopen(str3,"w");
			fclose(fp1);
			sortUserMail();
			break;
		}
			break;
		case 0:
			writeUser();
			return 0;
			break;
		default:
			
			break;
	}}
	return 0;
}
