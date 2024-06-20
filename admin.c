#include<stdio.h>
#include"envVariable.h"
#include"basestructures.h"

int main()
{
	system("cls");
	
	FILE *activeUser;
	activeUser=fopen("activeUser.txt","rb");
	userDetails currentUser;
	fread(&currentUser, sizeof(userDetails),1,activeUser);
	
	if(currentUser.loginMode!=1)
	{
		system("menu");
	}
	
	printf("admin");
}