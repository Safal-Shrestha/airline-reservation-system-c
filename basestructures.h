#include<stdlib.h>

typedef struct{
    char name[40];
    char uName[40];
    char pw[40];
    char favFood[40];
    char birthPlace[40];
    unsigned long long int contactNo;
    int loginMode;
}userDetails;

typedef struct{
	char name[100];
	char airline[100];
	int seatRow, seatCol;
	int flightAvailability;
}airplane;

typedef struct{
	int hour;
	int minute;
	int sec;
	int day;
	int month;
	int year;
}timeDetails;

typedef struct{
	int **seatAvailability;
	char name[100];
	char airline[100];
	char departure[100];
	char destination[100];
	timeDetails flightDuration;
	timeDetails departureTime;
	timeDetails arrivalTime;
	float ticketPrice;
}flightDetails;


void initialiseSeats(int seatRow, int seatCol, int ***seatAvailability)
{
	*seatAvailability=(int **)calloc(seatRow,sizeof(int *));
	for(int i=0;i<seatRow;i++)
	{
		(*seatAvailability)[i]=(int *)calloc(seatCol,sizeof(int));
	}
}