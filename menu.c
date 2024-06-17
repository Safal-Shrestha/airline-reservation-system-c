#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"envVariable.h"

void signup();
void login();
void forgotPassword(char *);

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
	printf("MENU\n1.Sign Up\n2. Log In\n3. Exit");
	printf("\nEnter Choice: ");
	scanf("%d", &menuChoice);
	
	switch(menuChoice)
	{
		case 1:
			signup();
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
	const char key[] = "ADMIN_PASSWORD";
    char *adminPassValue = get_env_variable(key);
	
	userDetails newUser, *database;
	char adminPassword[40];
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
    	fgets(adminPassword,sizeof(adminPassword),stdin);
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
    printf("Password: ");
    fgets(newUser.pw,40,stdin);
    printf("Phone Number: ");
    scanf("%lld",&newUser.contactNo);
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
	fflush(stdin);
	
	loginError:
	fflush(stdin);
	printf("User Name: ");
	fgets(loginDetails.uName,40,stdin);
	printf("Password: ");
	fgets(loginDetails.pw,40,stdin);
	if(loginTry!=0)
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
	}
	
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
			printf("Logged In");
			loginCheck=1;
		}
	}
	
	if(loginCheck==0)
	{
		printf("\nIncorrect Username or Password.\n");
		loginTry++;
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