#include<stdio.h>
#include"envVariable.h"
#include"basestructures.h"

void bookFlight();
void bookedFlight();
void cancelFlight();

int main()
{	
	FILE *activeUser;
	activeUser=fopen("activeUser.txt","rb");
	userDetails currentUser;
	fread(&currentUser, sizeof(userDetails),1,activeUser);
	
	int choice;
	
	if(currentUser.loginMode!=0)
	{
		system("menu");
	}
	
	menuErr:
	system("cls");
	printf("Menu\n");
	printf("1. Book Flight\n2. Booked Flights\n3. Cancel Flight\n4. Logout");
	printf("\nEnter Choice: ");
	scanf("%d",&choice);
	switch(choice)
	{
		case 1:
			bookFlights();
			goto menuErr;
			break;
		
		case 2:
			bookedFlights();
			goto menuErr;
			break;
		
		case 3:
			cancelFlight();
			break;
			
		case 4:
			fclose(activeUser);
			activeUser=fopen("activeUser.txt","wb");
			fclose(activeUser);
			system("menu");
			break;
		
		default:
			printf("Out of Scope");
			goto menuErr;
	}
	getc(stdin);
}

bookFlights()
{
	getc(stdin);
}

bookedFlights()
{
	getc(stdin);
}

cancelFlight()
{
	getc(stdin);
}