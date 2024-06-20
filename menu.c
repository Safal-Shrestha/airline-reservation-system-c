#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<conio.h>
#include"envVariable.h"

void signup();
void login();
void forgotPassword(char *);
void getPasswordSecurely(char *);

typedef struct{
    char name[40];
    char uName[40];
    char pw[40];
    char favFood[40];
    char birthPlace[40];
    unsigned long long int contactNo;
    int loginMode;
}userDetails;


int main()
{
	int menuChoice;
	menuErr:
	printf("MENU\n1. Sign Up\n2. Log In\n3. Exit");
	printf("\nEnter Choice: ");
	scanf("%d", &menuChoice);
	
	switch(menuChoice)
	{
		case 1:
			signup();
			goto menuErr;
			break;
			
		case 2:
			login();
			break;
		
		case 3:
			exit(0);
		
		default:
			printf("Out of Scope");
			goto menuErr;
	}
}

void signup()
{	
	system("cls");
	
	int validatePhoneNumber(int);
	
	const char adminPass[] = "ADMIN_PASSWORD";
    char *adminPassValue = get_env_variable(adminPass);
	
	userDetails newUser, *database;
    char passwordConfirm[40], adminPassword[40];
	int no=1, readLoop=0, newSize=1; //variable for reading data from database
	
	FILE *fp;
    fp=fopen("database.txt","ab+");
    
    fflush(stdin);
    printf("SIGN UP\n");
    printf("Admin(1)/Client(0): ");
    scanf("%d",&newUser.loginMode);
    fflush(stdin);
    if(newUser.loginMode==1)
    {
    	adminPasswordError:
    	printf("Admin Password: ");
    	getPasswordSecurely(adminPassword);
    	fflush(stdin);
    	adminPassword[strcspn(adminPassword, "\n")] = '\0';//removing trailing newline character
    	if(strcmp(adminPassword, adminPassValue)!=0)
    	{
    		printf("Unauthorised");
    		goto adminPasswordError;
		}
	}
    printf("Name: ");
    fgets(newUser.name,40,stdin);
    printf("User Name: ");
    fgets(newUser.uName,40,stdin);
    passwordEnter:
    printf("Password: ");
    getPasswordSecurely(newUser.pw);
    fflush(stdin);
    printf("Re-Enter Password: ");
    getPasswordSecurely(passwordConfirm);
    fflush(stdin);
    if(strcmp(newUser.pw,passwordConfirm)!=0)
    {
    	printf("Password does not match\n");
    	goto passwordEnter;
	}
    invalidPhone:
    printf("Phone Number: ");
    scanf("%lld",&newUser.contactNo);
    if(validatePhoneNumber(newUser.contactNo)!=10)
    {
    	printf("Invalid Phone Number\n");
    	goto invalidPhone;
	}
    fflush(stdin);
    printf("\nSecurity Questions:\n");
    printf("What is Your Favorite Food?\n");
    fgets(newUser.favFood,40,stdin);
    printf("What is Your Birth Place?\n");
    fgets(newUser.birthPlace,40,stdin);
    
    //Checking if username exists or not
    rewind(fp);
	database=(userDetails *)calloc(no,sizeof(userDetails));
	while(fread((database+readLoop), sizeof(userDetails),1,fp)==1)
	{
		newSize++;
		database=(userDetails *)realloc(database, (newSize)*sizeof(userDetails));
		readLoop++;
	}
	
	for(int i=0;i<newSize;i++)
	{
		if(strcmp(newUser.uName,database[i].uName)==0)
		{
			printf("Username Exists");
			printf("\nUser Name: ");
			fgets(newUser.uName,40,stdin);
		}
	}
    
    fwrite(&newUser,sizeof(userDetails),1,fp);
    fclose(fp);
}

void login(){
	system("cls");
	userDetails loginDetails, *database;
	
	int no=1, readLoop=0, newSize=1; //variable for reading data from database
	int loginTry=0, loginCheck=0;
	char forgotPasswordChoice;
	
	FILE *fptr;
	fptr=fopen("database.txt","ab+");
	
	printf("LOG IN\n");
	
	loginError:
	fflush(stdin);
	printf("User Name: ");
	fgets(loginDetails.uName,40,stdin);
	printf("Password: ");
	getPasswordSecurely(loginDetails.pw);
	fflush(stdin);
	
	rewind(fptr);
	database=(userDetails *)calloc(no,sizeof(userDetails));
	while(fread((database+readLoop), sizeof(userDetails),1,fptr)==1)
	{
		newSize++;
		database=(userDetails *)realloc(database, (newSize)*sizeof(userDetails));
		readLoop++;
	}
	
	for(int i=0;i<newSize;i++)
	{
		if(strcmp(loginDetails.uName,database[i].uName)==0&&strcmp(loginDetails.pw,database[i].pw)==0)
		{
			loginDetails=database[i];
			FILE *loginUser;
			loginUser=fopen("activeUser.txt","wb");
			fwrite(&loginDetails,sizeof(loginDetails),1,loginUser);
			if(loginDetails.loginMode==1)
			{
				system("admin");
			}
			else
			{
				system("client");
			}
			loginCheck=1;
		}
	}
	
	if(loginCheck==0)
	{
		printf("Incorrect Username or Password.\n\n");
		loginTry++;
		if(loginTry<2)
		{
			goto loginError;
		}
	}
//	printf("Debug %d",loginCheck);
	
	if(loginTry!=0&&loginCheck==0)
	{
		printf("Forgot Password? (Y/N): ");
		scanf("%c", &forgotPasswordChoice);
		if(forgotPasswordChoice=='y'||forgotPasswordChoice=='Y')
		{
			forgotPassword(loginDetails.uName);
			loginTry=0;
			fclose(fptr);
			fptr=fopen("database.txt","ab+");
			goto loginError;
		}
		goto loginError;
	}
}

void forgotPassword(char *uname)
{
	userDetails securityQuestions, *database;
	
	int no=1, readLoop=0, newSize=1; //variable for reading data from database
	
	FILE *fp;
    fp=fopen("database.txt","ab+");
	fflush(stdin);
	
//	puts(uname);//debug
	
	printf("\nSecurity Questions:\n");
	printf("What is Your Favorite Food?\n");
    fgets(securityQuestions.favFood,40,stdin);
    printf("What is Your Birth Place?\n");
    fgets(securityQuestions.birthPlace,40,stdin);
    
    //checking if security question matches
    rewind(fp);
	database=(userDetails *)calloc(no,sizeof(userDetails));
	while(fread((database+readLoop), sizeof(userDetails),1,fp)==1)
	{
		newSize++;
		database=(userDetails *)realloc(database, (newSize)*sizeof(userDetails));
		readLoop++;
	}
	
	for(int i=0;i<newSize;i++)
	{
		if(strcmp(uname,database[i].uName)==0&&strcmp(securityQuestions.favFood,database[i].favFood)==0&&strcmp(securityQuestions.birthPlace,database[i].birthPlace)==0)
		{
			printf("New Password: ");
			fgets(securityQuestions.pw,40,stdin);
			strcpy(database[i].pw,securityQuestions.pw);
		}
	}
	FILE *freplace;
	fclose(fp);
	freplace=fopen("database.txt","wb+");
	fclose(freplace);
	fp=fopen("database.txt","ab+");
	for(int i=0;i<newSize;i++)
	{	
		fwrite((database+i),sizeof(userDetails),1,fp);
	}
	fclose(fp);
}

int validatePhoneNumber(int number){
	int temp=number, digits=0;
	while(temp!=0)
	{
		digits++;
		temp=temp/10;
	}
	return digits;
}

void getPasswordSecurely(char *password)
{
	int p=0;
	do{
		char ch=getch();
		
		if(ch==13)
		{
			break;
		}
		else if(ch ==8)
		{
			if(p>0)
			{
				p--;
				printf("\b \b");
			}
		}
		else
		{
			password[p]=ch;
			p++;
			printf("*");
		}
	}while(1);
	password[p]='\0';
	printf("\n");
}