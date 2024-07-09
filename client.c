#include<stdio.h>
#include"envVariable.h"
#include"basestructures.h"
#include "ANSI-color-codes.h"

void bookFlight(userDetails);
void bookedFlight(userDetails);
void cancelFlight(userDetails);

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
	system("cls");
	switch(choice)
	{
		case 1:
			bookFlight(currentUser);
			goto menuErr;
			break;
		
		case 2:
			bookedFlight(currentUser);
			goto menuErr;
			break;
		
		case 3:
			cancelFlight(currentUser);
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

void bookFlight(userDetails currentUser)
{
	flightDetails counter, *flightDatabase;
	seatBooking info;
	
	FILE *flight, *bookedSeats;
	flight=fopen("operatingFlight.txt","rb");
	bookedSeats=fopen("bookedSeats.txt","ab+");
	rewind(flight);

	int no=1;//used in for loop to check if flights from departure to destination is available
	int totalOperatingFlights=0;
	for(int j=0;j>=0;j++)
	{
		if(fread(&counter,sizeof(flightDetails)-sizeof(int **),1,flight)!=1)
		{
			break;
		}
		initialiseSeats(counter.seatRow,counter.seatCol,&counter.seatAvailability);
		for(int l=0;l<counter.seatRow;l++)
		{
			for(int m=0;m<counter.seatCol;m++)
			{
				fread(&counter.seatAvailability[l][m],sizeof(int),1,flight);
			}
		}
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
	fflush(stdin);
	printf("SEARCH FLIGHTS\n");
	printf("Departure Point: ");
	fgets(counter.departure,100,stdin);
	printf("Destination Point: ");
	fgets(counter.destination,100,stdin);
	
	for(int i=0;i<totalOperatingFlights;i++)
	{
		if(strcmp(counter.departure,flightDatabase[i].departure)==0&&strcmp(counter.destination,flightDatabase[i].destination)==0)
		{
			printf("\n%d.\nAircraft: %sAirline: %s",no,flightDatabase[i].name,flightDatabase[i].airline);
			printf("Flight Duration: %d hrs %d min %d sec\n",flightDatabase[i].flightDuration.hour,flightDatabase[i].flightDuration.minute,flightDatabase[i].flightDuration.sec);
			printf("Departure Time (dd/mm/yyyy h:m:s): %d/%d/%d %d:%d:%d\n",flightDatabase[i].departureTime.day,flightDatabase[i].departureTime.month,flightDatabase[i].departureTime.year,flightDatabase[i].departureTime.hour,flightDatabase[i].departureTime.minute,flightDatabase[i].departureTime.sec);
			printf("Arrival Time(dd/mm/yyyy h:m:s): %d/%d/%d %d:%d:%d\n",flightDatabase[i].arrivalTime.day,flightDatabase[i].arrivalTime.month,flightDatabase[i].arrivalTime.year,flightDatabase[i].arrivalTime.hour,flightDatabase[i].arrivalTime.minute,flightDatabase[i].arrivalTime.sec);
			printf("Ticket Price: %.2f\n",flightDatabase[i].ticketPrice);
			no++;
		}
	}
	

	if(no==1)
	{
		printf(RED "No Such Flights Scheduled!" reset);
	}
	else
	{
		int flightChoice;
		int bookedSeat[2], ticketNum;
		printf("\nChoose Flight: ");
		scanf("%d", &flightChoice);
		flightChoice=flightChoice-1;
		system("cls");
		printf("Aircraft: %sAirline: %s",flightDatabase[flightChoice].name,flightDatabase[flightChoice].airline);
		printf("Flight Duration: %d hrs %d min %d sec\n",flightDatabase[flightChoice].flightDuration.hour,flightDatabase[flightChoice].flightDuration.minute,flightDatabase[flightChoice].flightDuration.sec);
		printf("Departure Time (dd/mm/yyyy h:m:s): %d/%d/%d %d:%d:%d\n",flightDatabase[flightChoice].departureTime.day,flightDatabase[flightChoice].departureTime.month,flightDatabase[flightChoice].departureTime.year,flightDatabase[flightChoice].departureTime.hour,flightDatabase[flightChoice].departureTime.minute,flightDatabase[flightChoice].departureTime.sec);
		printf("Arrival Time(dd/mm/yyyy h:m:s): %d/%d/%d %d:%d:%d\n",flightDatabase[flightChoice].arrivalTime.day,flightDatabase[flightChoice].arrivalTime.month,flightDatabase[flightChoice].arrivalTime.year,flightDatabase[flightChoice].arrivalTime.hour,flightDatabase[flightChoice].arrivalTime.minute,flightDatabase[flightChoice].arrivalTime.sec);
		printf("Ticket Price: %.2f\n",flightDatabase[flightChoice].ticketPrice);
		printf("Seats:\n");
		for(int i=0;i<flightDatabase[flightChoice].seatRow;i++)
		{
			for(int j=0;j<flightDatabase[flightChoice].seatCol;j++)
			{
				if(flightDatabase[flightChoice].seatAvailability[i][j]==0)
				{
					printf(GRN "|%d %d| "reset,i,j);
				}
				else
				{
					printf(RED "|%d %d| "reset,i,j);
				}
				if((j+1)%(flightDatabase[flightChoice].seatCol/2)==0)
				{
					printf("\t");
				}
			}
			printf("\n\n");
		}
		printf("No. of Tickets: ");
		scanf("%d",&ticketNum);
		for(int i=0;i<ticketNum;i++)
		{
			booked:
			printf("Enter Row and Column: ");
			scanf("%d%d",&bookedSeat[0],&bookedSeat[1]);
			if(flightDatabase[flightChoice].seatAvailability[bookedSeat[0]][bookedSeat[1]]==1)
			{
				printf(RED "Already Booked\n"reset);
				goto booked;
			}
			strcpy(info.name,currentUser.name);
			strcpy(info.u_name,currentUser.uName);
			strcpy(info.aircraft,flightDatabase[flightChoice].name);
			strcpy(info.airline,flightDatabase[flightChoice].airline);
			flightDatabase[flightChoice].seatAvailability[bookedSeat[0]][bookedSeat[1]]=1;
			info.bookedSeat[0]=bookedSeat[0];
			info.bookedSeat[1]=bookedSeat[1];
			fwrite(&info,sizeof(seatBooking),1,bookedSeats);
		}
		fclose(flight);
		flight=fopen("operatingFlight.txt","wb");
		for(int i=0;i<totalOperatingFlights;i++)
		{
			fwrite(&flightDatabase[i],sizeof(flightDetails)-sizeof(int **),1,flight);
			for(int j=0;j<flightDatabase[i].seatRow;j++)
			{
				for(int k=0;k<flightDatabase[i].seatCol;k++)
				{
					fwrite(&flightDatabase[i].seatAvailability[j][k],sizeof(int),1,flight);
				}
			}
		}
	}
	for(int j=0;j<totalOperatingFlights;j++)
	{
		for(int k=0;k<flightDatabase[j].seatRow;k++)
		{
			free(flightDatabase[j].seatAvailability[k]);
		}
		free(flightDatabase[j].seatAvailability);
	}
	fclose(flight);
	fclose(bookedSeats);
	free(flightDatabase);
	fflush(stdin);
	getc(stdin);
}

void bookedFlight(userDetails currentUser)
{
	getc(stdin);
}

void cancelFlight(userDetails currentUser)
{
	getc(stdin);
}