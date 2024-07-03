#include<stdio.h>
#include<stdlib.h>
#include"envVariable.h"
#include"basestructures.h"

void currentFlightDetails();
void addFlight();
void deleteFlight();
void addAirplane();
void deleteAirplane();
void insertFlightDetails(flightDetails *);

int main()
{
	int choice;
	
	FILE *activeUser;
	activeUser=fopen("activeUser.txt","rb");
	userDetails currentUser;
	fread(&currentUser, sizeof(userDetails),1,activeUser);
	
	if(currentUser.loginMode!=1)
	{
		system("menu");
	}
	
	menuErr:
	system("cls");
	printf("Menu\n");
	printf("1. Check Flight Details\n2. Add/Edit Flight\n3. Delete Flight\n4. Add Airplane\n5. Delete Airplane\n6. Logout");
	printf("\nEnter Choice: ");
	scanf("%d",&choice);
	fflush(stdin);
	
	system("cls");
	switch(choice)
	{
		case 1:
			currentFlightDetails();
			goto menuErr;
			break;
			
		case 2:
			addFlight();
			goto menuErr;
			break;
			
		case 3:
			deleteFlight();
			goto menuErr;
			break;
			
		case 4:
			addAirplane();
			goto menuErr;
			break;
			
		case 5:
			deleteAirplane();
			goto menuErr;
			break;
			
		case 6:
			fclose(activeUser);
			activeUser=fopen("activeUser.txt","wb");
			fclose(activeUser);
			system("menu");
			break;
			
		default:
			printf("Out of Scope");
			goto menuErr;
	}
	fclose(activeUser);
}

void currentFlightDetails()
{
	airplane detail;
	flightDetails *flightDatabase;
	
	FILE *flight,*craftList;
	craftList=fopen("aircraftList.txt","ab+");
	flight=fopen("operatingFlight.txt","rb");
	
	int no=1, readLoop=0, newSize=1; //variable for reading data from flight details database
	rewind(flight);
	flightDatabase=(flightDetails *)calloc(no,sizeof(flightDetails));
	while(fread((flightDatabase+readLoop), sizeof(flightDetails),1,flight)==1)
	{
		newSize++;
		flightDatabase=(flightDetails *)realloc(flightDatabase, (newSize)*sizeof(flightDetails));
		readLoop++;
	}
	if(newSize==1)
	{
		printf("No Available Data for Operating Flights.");
		getc(stdin);
	}
	else
	{
		printf("OPERATING FLIGHTS\n");
		for(int i=0;i<newSize-1;i++)
		{
			printf("%d.\nAircraft: %sAirlines: %s\n",i+1,flightDatabase[i].name,flightDatabase[i].airline);
		}
		int flightChoice;
		printf("Check Available Seats of: ");
		scanf("%d", &flightChoice);
		flightChoice=flightChoice-1;
		system("cls");
		printf("Flight Details\n");
		printf("Aircraft: %s",flightDatabase[flightChoice].name);
		printf("Airlines: %s",flightDatabase[flightChoice].airline);
		printf("Departure: %s",flightDatabase[flightChoice].departure);
		printf("Destination: %s",flightDatabase[flightChoice].destination);
		printf("Flight Duration: %d hrs %d min %d sec\n",flightDatabase[flightChoice].flightDuration.hour,flightDatabase[flightChoice].flightDuration.minute,flightDatabase[flightChoice].flightDuration.sec);
		printf("Departure Time (dd/mm/yyyy h:m:s): %d/%d/%d %d:%d:%d\n",flightDatabase[flightChoice].departureTime.day,flightDatabase[flightChoice].departureTime.month,flightDatabase[flightChoice].departureTime.year,flightDatabase[flightChoice].departureTime.hour,flightDatabase[flightChoice].departureTime.minute,flightDatabase[flightChoice].departureTime.sec);
		printf("Arrival Time(dd/mm/yyyy h:m:s): %d/%d/%d %d:%d:%d\n",flightDatabase[flightChoice].arrivalTime.day,flightDatabase[flightChoice].arrivalTime.month,flightDatabase[flightChoice].arrivalTime.year,flightDatabase[flightChoice].arrivalTime.hour,flightDatabase[flightChoice].arrivalTime.minute,flightDatabase[flightChoice].arrivalTime.sec);
		printf("Ticket Price: %.2f\n",flightDatabase[flightChoice].ticketPrice);
		printf("\nSeats\n");
		rewind(craftList);

		for(int i=0;i<=flightChoice;i++)
		{
			fread(&detail,sizeof(airplane),1,craftList);
		}
		initialiseSeats(detail.seatRow, detail.seatCol, &flightDatabase[flightChoice].seatAvailability);
		printf("%d\n",flightDatabase[flightChoice].seatAvailability[0][0]);
		printf("%s",detail.name);
		
		fclose(craftList);
		fclose(flight);
		fflush(stdin);
		getc(stdin);
	}
}

void addFlight()
{
	airplane details, *craftDatabase;
	flightDetails info, *flightDatabase;
	
	FILE *craftList, *flight;
	craftList=fopen("aircraftList.txt","ab+");
	flight=fopen("operatingFlight.txt","ab+");
	
	int match=0;
	
	int c_no=1, c_readLoop=0, c_newSize=1; //variable for reading data from aircraft details database
	rewind(craftList);
	craftDatabase=(airplane *)calloc(c_no,sizeof(airplane));
	while(fread((craftDatabase+c_readLoop), sizeof(airplane),1,craftList)==1)
	{
		c_newSize++;
		craftDatabase=(airplane *)realloc(craftDatabase, (c_newSize)*sizeof(airplane));
		c_readLoop++;
	}
	
	fflush(stdin);
	printf("ADD/EDIT FLIGHT\n");
	craftErr:
	printf("Aircraft Name: ");
	fgets(details.name,100,stdin);
	printf("Owning Airline Name: ");
	fgets(details.airline,100,stdin);
	
	for(int i=0;i<c_newSize;i++)
	{
		if(strcmp(details.name,craftDatabase[i].name)==0&&strcmp(details.airline,craftDatabase[i].airline)==0)
		{
			match=1;
			details=craftDatabase[i];
			if(craftDatabase[i].flightAvailability==1)
			{
				craftDatabase[i].flightAvailability=0;
				initialiseSeats(details.seatRow, details.seatCol, &info.seatAvailability);
				strcpy(info.name,craftDatabase[i].name);
				strcpy(info.airline,craftDatabase[i].airline);
				fflush(stdin);
				insertFlightDetails(&info);
				fclose(flight);
				flight=fopen("operatingFlight.txt","ab+");
				fwrite(&info,sizeof(flightDetails),1,flight);
				fclose(craftList);
				craftList=fopen("aircraftList.txt","wb");
				for(int i=0;i<c_newSize-1;i++)
				{		
					fwrite((craftDatabase+i),sizeof(airplane),1,craftList);
				}
				fclose(craftList);
				fclose(flight);	
				printf("Press Enter to Continue");
				getc(stdin);		
			}
				
			else
			{
				system("cls");
				int f_no=1, f_readLoop=0, f_newSize=1; //variable for reading data from flight details database
				rewind(flight);
				flightDatabase=(flightDetails *)calloc(f_no,sizeof(flightDetails));
				while(fread((flightDatabase+f_readLoop), sizeof(flightDetails),1,flight)==1)
				{
					f_newSize++;
					flightDatabase=(flightDetails *)realloc(flightDatabase, (f_newSize)*sizeof(flightDetails));
					f_readLoop++;
				}
				printf("Flight Scheduled\nFlight Details:\n");
				if(strcmp(details.name,flightDatabase[i].name)==0&&strcmp(details.airline,flightDatabase[i].airline)==0)
				{
					printf("Aircraft: %s",flightDatabase[i].name);
					printf("Airlines: %s",flightDatabase[i].airline);
					printf("Departure: %s",flightDatabase[i].departure);
					printf("Destination: %s",flightDatabase[i].destination);
					printf("Flight Duration: %d hrs %d min %d sec\n",flightDatabase[i].flightDuration.hour,flightDatabase[i].flightDuration.minute,flightDatabase[i].flightDuration.sec);
					printf("Departure Time (dd/mm/yyyy h:m:s): %d/%d/%d %d:%d:%d\n",flightDatabase[i].departureTime.day,flightDatabase[i].departureTime.month,flightDatabase[i].departureTime.year,flightDatabase[i].departureTime.hour,flightDatabase[i].departureTime.minute,flightDatabase[i].departureTime.sec);
					printf("Arrival Time(dd/mm/yyyy h:m:s): %d/%d/%d %d:%d:%d\n",flightDatabase[i].arrivalTime.day,flightDatabase[i].arrivalTime.month,flightDatabase[i].arrivalTime.year,flightDatabase[i].arrivalTime.hour,flightDatabase[i].arrivalTime.minute,flightDatabase[i].arrivalTime.sec);
					printf("Ticket Price: %.2f\n",flightDatabase[i].ticketPrice);
					printf("Edit?(y/n)");
					if(getc(stdin)=='y')
					{
						insertFlightDetails(&info);
						strcpy(info.name,flightDatabase[i].name);   
						strcpy(info.airline,flightDatabase[i].airline);
						flightDatabase[i]=info;
						fclose(flight);
						flight=fopen("operatingFlight.txt","wb");
						for(int i=0;i<f_newSize-1;i++)
						{		
							fwrite((flightDatabase+i),sizeof(flightDetails),1,flight);
						}
						fclose(flight);
					}
				};
				getc(stdin);
			}
		}
	}
	
	if(match==0)
	{
		printf("No such Aircraft Exists.\nRe-Enter Details\n");
		fflush(stdin);
		goto craftErr;
	}
}

void deleteFlight()
{
	
}

void addAirplane()
{
	fflush(stdin);
	
	airplane details;
	
	FILE *plane;
	plane=fopen("aircraftList.txt","ab+");
	
	printf("NEW AIRPLANE ENTRY\n");
	printf("Name: ");
	fgets(details.name,100,stdin);
	printf("Owning Airline: ");
	fgets(details.airline,100,stdin);
	printf("Available Rows: ");
	scanf("%d",&details.seatRow);
	printf("Available Columns: ");
	scanf("%d",&details.seatCol);
	details.flightAvailability=1;
	fwrite(&details,sizeof(airplane),1,plane);
	fclose(plane);
}

void deleteAirplane()
{
	printf("hello");
}

void insertFlightDetails(flightDetails* info)
{
	fflush(stdin);
	printf("Departure Point: ");
	fgets(info->departure,100,stdin);
	printf("Destination Point: ");
	fgets(info->destination,100,stdin);
	printf("Flight Duration(h min sec): ");
	scanf("%d%d%d",&info->flightDuration.hour,&info->flightDuration.minute,&info->flightDuration.sec);
	printf("Flight Departure Date and Time (dd mm yyyy h min sec): ");
	scanf("%d%d%d%d%d%d",&info->departureTime.day,&info->departureTime.month,&info->departureTime.year,&info->departureTime.hour,&info->departureTime.minute,&info->departureTime.sec);
	info->arrivalTime.sec=info->departureTime.sec+info->flightDuration.sec;
	info->arrivalTime.minute=info->departureTime.minute+info->flightDuration.minute;
	if(info->arrivalTime.sec>60)
	{
		info->arrivalTime.minute=info->arrivalTime.minute+(info->arrivalTime.sec/60);
		info->arrivalTime.sec=info->arrivalTime.sec%60;
	}
	info->arrivalTime.hour=info->departureTime.hour+info->flightDuration.hour;
	if(info->arrivalTime.minute>60)
	{
		info->arrivalTime.hour=info->arrivalTime.hour+(info->arrivalTime.minute/60);
		info->arrivalTime.minute=info->arrivalTime.minute%60;
	}
	info->arrivalTime.day=info->departureTime.day;
	if(info->arrivalTime.hour>24)
	{
		info->arrivalTime.day=info->arrivalTime.day+(info->arrivalTime.hour/24);
		info->arrivalTime.hour=info->arrivalTime.hour%24;
	}
	info->arrivalTime.month=info->departureTime.month;
	if(info->arrivalTime.day>30)
	{
		info->arrivalTime.month=info->arrivalTime.month+(info->arrivalTime.day/30);
		info->arrivalTime.day=info->arrivalTime.day%30;
	}
	info->arrivalTime.year=info->departureTime.year;
	if(info->arrivalTime.month>12)
	{
		info->arrivalTime.year=info->arrivalTime.year+(info->arrivalTime.month/12);
		info->arrivalTime.month=info->arrivalTime.month%12;
	}
	printf("Ticke Price(Rs.): ");
	scanf("%f",&info->ticketPrice);
	fflush(stdin);
}