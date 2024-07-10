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
			goto menuErr;
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
	flightDetails counter, *flightDatabase;
	seatBooking *bookedSeatsDatabase;
	
	FILE *flight, *bookedSeats;
	flight=fopen("operatingFlight.txt","rb");
	bookedSeats=fopen("bookedSeats.txt","ab+");
	rewind(flight);
	rewind(bookedSeats);
	int no=1, readLoop=0, newSize=1; //variable for reading data from booked flights details database
	bookedSeatsDatabase=(seatBooking *)calloc(no,sizeof(seatBooking));
	while(fread((bookedSeatsDatabase+readLoop), sizeof(seatBooking),1,bookedSeats)==1)
	{
		newSize++;
		bookedSeatsDatabase=(seatBooking *)realloc(bookedSeatsDatabase, (newSize)*sizeof(seatBooking));
		readLoop++;
	}
	
	int totalOperatingFlights=0; //reading data from operating flights
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
	int num=1;
	for(int i=0;i<readLoop;i++)
	{
		if(strcmp(currentUser.name,bookedSeatsDatabase[i].name)==0&&strcmp(currentUser.uName,bookedSeatsDatabase[i].u_name)==0)
		{
			for(int j=0;j<totalOperatingFlights;j++)
			{
				if(strcmp(flightDatabase[j].name,bookedSeatsDatabase[i].aircraft)==0&&strcmp(flightDatabase[j].airline,bookedSeatsDatabase[i].airline)==0)
				{
					printf("%d.\nAirCraft: %sAirline: %s",num,bookedSeatsDatabase[i].aircraft,bookedSeatsDatabase[i].airline);
					printf("Flight Duration: %d hrs %d min %d sec\n",flightDatabase[j].flightDuration.hour,flightDatabase[j].flightDuration.minute,flightDatabase[j].flightDuration.sec);
					printf("Departure Time (dd/mm/yyyy h:m:s): %d/%d/%d %d:%d:%d\n",flightDatabase[j].departureTime.day,flightDatabase[j].departureTime.month,flightDatabase[j].departureTime.year,flightDatabase[j].departureTime.hour,flightDatabase[j].departureTime.minute,flightDatabase[j].departureTime.sec);
					printf("Arrival Time(dd/mm/yyyy h:m:s): %d/%d/%d %d:%d:%d\n",flightDatabase[j].arrivalTime.day,flightDatabase[j].arrivalTime.month,flightDatabase[j].arrivalTime.year,flightDatabase[j].arrivalTime.hour,flightDatabase[j].arrivalTime.minute,flightDatabase[j].arrivalTime.sec);
					printf("Seat Row: %d\nSeat Column: %d\n\n",bookedSeatsDatabase[i].bookedSeat[0],bookedSeatsDatabase[i].bookedSeat[1]);
					num++;
				}
			}
		}
	}
	free(bookedSeatsDatabase);
	for(int i=0;i<readLoop;i++)
	{
		for(int j=0;j<flightDatabase[i].seatRow;j++)
		{
			free(flightDatabase[i].seatAvailability[j]);
		}
		free(flightDatabase[i].seatAvailability);
	}
	free(flightDatabase);
	fclose(flight);
	fclose(bookedSeats);
	fflush(stdin);
	getc(stdin);
}

void cancelFlight(userDetails currentUser)
{
	flightDetails counter, *flightDatabase;
	seatBooking *bookedSeatsDatabase;
	
	FILE *flight, *bookedSeats;
	flight=fopen("operatingFlight.txt","rb");
	bookedSeats=fopen("bookedSeats.txt","ab+");
	rewind(flight);
	rewind(bookedSeats);
	int no=1, readLoop=0, newSize=1; //variable for reading data from booked flights details database
	bookedSeatsDatabase=(seatBooking *)calloc(no,sizeof(seatBooking));
	while(fread((bookedSeatsDatabase+readLoop), sizeof(seatBooking),1,bookedSeats)==1)
	{
		newSize++;
		bookedSeatsDatabase=(seatBooking *)realloc(bookedSeatsDatabase, (newSize)*sizeof(seatBooking));
		readLoop++;
	}
	
	int totalOperatingFlights=0; //reading data from operating flights
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
	int num=1;
	for(int i=0;i<readLoop;i++)
	{
		if(strcmp(currentUser.name,bookedSeatsDatabase[i].name)==0&&strcmp(currentUser.uName,bookedSeatsDatabase[i].u_name)==0)
		{
			for(int j=0;j<totalOperatingFlights;j++)
			{
				if(strcmp(flightDatabase[j].name,bookedSeatsDatabase[i].aircraft)==0&&strcmp(flightDatabase[j].airline,bookedSeatsDatabase[i].airline)==0)
				{
					printf("%d.\nAirCraft: %sAirline: %s",num,bookedSeatsDatabase[i].aircraft,bookedSeatsDatabase[i].airline);
					printf("Flight Duration: %d hrs %d min %d sec\n",flightDatabase[j].flightDuration.hour,flightDatabase[j].flightDuration.minute,flightDatabase[j].flightDuration.sec);
					printf("Departure Time (dd/mm/yyyy h:m:s): %d/%d/%d %d:%d:%d\n",flightDatabase[j].departureTime.day,flightDatabase[j].departureTime.month,flightDatabase[j].departureTime.year,flightDatabase[j].departureTime.hour,flightDatabase[j].departureTime.minute,flightDatabase[j].departureTime.sec);
					printf("Arrival Time(dd/mm/yyyy h:m:s): %d/%d/%d %d:%d:%d\n",flightDatabase[j].arrivalTime.day,flightDatabase[j].arrivalTime.month,flightDatabase[j].arrivalTime.year,flightDatabase[j].arrivalTime.hour,flightDatabase[j].arrivalTime.minute,flightDatabase[j].arrivalTime.sec);
					printf("Seat Row: %d\nSeat Column: %d\n\n",bookedSeatsDatabase[i].bookedSeat[0],bookedSeatsDatabase[i].bookedSeat[1]);
					num++;
				}
			}
		}
	}
	int ticketChoice;
	fflush(stdin);
	printf("Cancel Seat of: ");
	scanf("%d",&ticketChoice);
	ticketChoice=ticketChoice-1;
	printf("Confirm Deletion?(y/n): ");
	fflush(stdin);
	if(getc(stdin)=='y')
	{
		printf(GRN "Cancellation Successfull!" reset);
		fclose(bookedSeats);
		fclose(flight);
		flight=fopen("operatingFlight.txt","wb");
		bookedSeats=fopen("bookedSeats.txt","wb");
		for(int i=0;i<totalOperatingFlights;i++)
		{
			if(strcmp(flightDatabase[i].airline,bookedSeatsDatabase[ticketChoice].airline)==0&&strcmp(flightDatabase[i].name,bookedSeatsDatabase[ticketChoice].aircraft)==0)
			{
				flightDatabase[i].seatAvailability[bookedSeatsDatabase[ticketChoice].bookedSeat[0]][bookedSeatsDatabase[ticketChoice].bookedSeat[1]]=0;
			}
		}
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
		for(int i=0;i<readLoop;i++)
		{
			if(i==ticketChoice-1)
			{
				continue;
			}
			fwrite(&bookedSeatsDatabase[i],sizeof(seatBooking),1,bookedSeats);
		}
	}
	
	free(bookedSeatsDatabase);
	for(int i=0;i<readLoop;i++)
	{
		for(int j=0;j<flightDatabase[i].seatRow;j++)
		{
			free(flightDatabase[i].seatAvailability[j]);
		}
		free(flightDatabase[i].seatAvailability);
	}
	free(flightDatabase);
	fclose(flight);
	fclose(bookedSeats);
	fflush(stdin);
	getc(stdin);
}