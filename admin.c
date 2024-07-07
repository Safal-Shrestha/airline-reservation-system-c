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
	flightDetails counter, *flightDatabase;
	
	FILE *flight;
	flight=fopen("operatingFlight.txt","rb");
	rewind(flight);

	int totalOperatingFlights=0;
	while(fread(&counter,sizeof(flightDetails),1,flight)==1)
	{
		totalOperatingFlights++;
	}
	rewind(flight);
	flightDatabase=(flightDetails *)malloc(totalOperatingFlights*sizeof(flightDetails));
	for(int j=0;j<totalOperatingFlights;j++)
	{
		fread((flightDatabase+j),sizeof(flightDetails)-sizeof(int **),1,flight);
		initialiseSeats(flightDatabase[j].seatRow,flightDatabase[j].seatCol,&flightDatabase[j].seatAvailability);
		for(int k=0;k<flightDatabase[j].seatRow;k++)
		{
			for(int l=0;l<flightDatabase[j].seatCol;l++)
			{
				fread(&flightDatabase[j].seatAvailability[k][l],sizeof(int),1,flight);	
			}
		}
	}
	

	if(totalOperatingFlights==0)
	{
		printf("No Available Data for Operating Flights.");
	}
	else
	{
		printf("OPERATING FLIGHTS\n");
		for(int i=0;i<totalOperatingFlights;i++)
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

		rewind(flight);

		free(flightDatabase);
		fclose(flight);
	}
	fflush(stdin);
	getc(stdin);
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
				strcpy(info.name,craftDatabase[i].name);
				strcpy(info.airline,craftDatabase[i].airline);
				info.seatRow=details.seatRow;
				info.seatCol=details.seatCol;				
				initialiseSeats(details.seatRow, details.seatCol, &info.seatAvailability);
				fflush(stdin);
				insertFlightDetails(&info);
				fclose(flight);
				flight=fopen("operatingFlight.txt","ab+");
				fwrite(&info,sizeof(flightDetails)-sizeof(int **),1,flight);
				for(int j=0;j<details.seatRow;j++)
				{
					for(int k=0;k<details.seatCol;k++)
					{
						fwrite(&info.seatAvailability[j][k],sizeof(int),1,flight);	
					}
				}
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
				rewind(flight);
				flightDetails counter;
				int totalOperatingFlights=0;
				while(fread(&counter,sizeof(flightDetails),1,flight)==1)
				{
					totalOperatingFlights++;
				}
				rewind(flight);
				flightDatabase=(flightDetails *)malloc(totalOperatingFlights*sizeof(flightDetails));
				for(int l=0;l<totalOperatingFlights;l++)
				{
					fread((flightDatabase+l),sizeof(flightDetails)-sizeof(int **),1,flight);
					initialiseSeats(flightDatabase[l].seatRow,flightDatabase[l].seatCol,&flightDatabase[l].seatAvailability);
					for(int m=0;m<flightDatabase[l].seatRow;m++)
					{
						for(int n=0;n<flightDatabase[l].seatCol;n++)
						{
							fread(&flightDatabase[l].seatAvailability[m][n],sizeof(int),1,flight);	
						}
					}
				}
				printf("Flight Scheduled\nFlight Details:\n");
				for(int j=0;j<totalOperatingFlights;j++)
				{
					if(strcmp(details.name,flightDatabase[j].name)==0&&strcmp(details.airline,flightDatabase[j].airline)==0)
					{
						printf("Aircraft: %s",flightDatabase[j].name);
						printf("Airline: %s",flightDatabase[j].airline);
						printf("Departure: %s",flightDatabase[j].departure);
						printf("Destination: %s",flightDatabase[j].destination);
						printf("Flight Duration: %d hrs %d min %d sec\n",flightDatabase[j].flightDuration.hour,flightDatabase[j].flightDuration.minute,flightDatabase[j].flightDuration.sec);
						printf("Departure Time (dd/mm/yyyy h:m:s): %d/%d/%d %d:%d:%d\n",flightDatabase[j].departureTime.day,flightDatabase[j].departureTime.month,flightDatabase[j].departureTime.year,flightDatabase[j].departureTime.hour,flightDatabase[j].departureTime.minute,flightDatabase[j].departureTime.sec);
						printf("Arrival Time(dd/mm/yyyy h:m:s): %d/%d/%d %d:%d:%d\n",flightDatabase[j].arrivalTime.day,flightDatabase[j].arrivalTime.month,flightDatabase[j].arrivalTime.year,flightDatabase[j].arrivalTime.hour,flightDatabase[j].arrivalTime.minute,flightDatabase[j].arrivalTime.sec);
						printf("Ticket Price: %.2f\n",flightDatabase[j].ticketPrice);
						printf("Edit?(y/n): ");
						if(getc(stdin)=='y')
						{
							insertFlightDetails(&info);
							strcpy(info.name,flightDatabase[j].name);   
							strcpy(info.airline,flightDatabase[j].airline);
							info.seatRow=flightDatabase[j].seatRow;
							info.seatCol=flightDatabase[j].seatCol;
							int **seatAvailabilityTemp=flightDatabase[j].seatAvailability;
							flightDatabase[j]=info;
							flightDatabase[j].seatAvailability=seatAvailabilityTemp;
							fclose(flight);
							flight=fopen("operatingFlight.txt","wb");
							for(int k=0;k<totalOperatingFlights;k++)
							{		
								fwrite((flightDatabase+k),sizeof(flightDetails)-sizeof(int **),1,flight);
								for(int l=0;l<flightDatabase[k].seatRow;l++)
								{
									for(int m=0;m<flightDatabase[k].seatCol;m++)
									{
										fwrite(&flightDatabase[k].seatAvailability[l][m],sizeof(int),1,flight);
									}
								}
							}
							fclose(flight);
						}
					}	
				}
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
	
	fclose(flight);
	fclose(craftList);
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
	printf("Ticket Price(Rs.): ");
	scanf("%f",&info->ticketPrice);
	fflush(stdin);
}