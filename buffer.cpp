// CPSC 441 ASSIGNMENT 3 
// File: buffer.c
// Author: Alexander Varga
// UCID: 30099000
// date: 03/20/2022
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <queue>
using namespace std;

#ifndef MAX
	#define MAX(a,b) ((a) > (b) ? (a) : (b))
#endif

// Buffer size in packets
#define MAX_BUFFER 100

// Wireless link data rate bp
#define SPEED 5000000.0
#define SPEED1 6000000.0
#define SPEED2 10000000.0

// Debug Flag
//#define DEBUG 1
//Trace files
#define PATH "starwars.txt"
#define PATH1 "soccer.txt"
#define PATH2 "zoom.txt"

//Function Prototypes
void printMenu(char* file, int64_t speed);
void printQueue(queue<int> q);
int routerSimulation(char *, int64_t speed);
void clearVars();

ifstream file; //input file stream object
std::queue<int> buffer; // AP buffer queue
float now = 0.0, elapsed = 0.0;
float pre = 0.0, pred = 0.0;
int size; // packet size, and packet sizes
float departure = 0.0; // departure time
int totpkts = 0; // total size in packets
long long totKB = 0; // total size in KB
float avgdelay = 0.0;
int pktsin = 0, pktsout = 0, pktslost = 0; // packets in, out and lost
int bytesin = 0, bytesout = 0, byteslost = 0; // bytes in, out and lost
float start = 0.0;
float totdelay;
int i = 0, k = 0;
float t = 0.0;
float delay;

int main(int argc, char *argv[])
{
	printf("Welcome to Router Simulation Program\n");
	printf("All Traffic Stats are in the form pkts - bytes\n");
    printf("-------------------------------------------------------------------------------------------------------------------------------------------------------\n");
	printf("| Filename     | APBuf | Link (bps) | End Time | Incoming Traffic    | Outgoing Traffic    | Lost Traffic     |   Loss %%   |       Avg. Queueing Delay |\n");
	printf("--------------------------------------------------------------------------------------------------------------------------------------------------------\n");
	char FILE[1024];

	strcpy(FILE, PATH);
	routerSimulation(FILE, SPEED);
	routerSimulation(FILE, SPEED1);
	routerSimulation(FILE, SPEED2);
	
	
	strcpy(FILE, PATH1);
	routerSimulation(FILE, SPEED);
	routerSimulation(FILE, SPEED1);
	routerSimulation(FILE, SPEED2);
	
	strcpy(FILE, PATH2);
	routerSimulation(FILE, SPEED);
	routerSimulation(FILE, SPEED1);
	routerSimulation(FILE, SPEED2);
	
    return 0;
}
//my algo is not great, doesnt take into account departure queuing delay, wrong output
int routerSimulation(char * path, int64_t speed)
{
	//clean variables
	clearVars();
	file.open(path);
	file >> now;
	pre = now;
	file >> size;
	buffer.push(size);
	departure =(float)((float)size * 8)/(speed) + start;
	pred = departure;
	//read file input until end of file
	while(!file.eof())
	{
		#if DEBUG
			printf("%d: %f     D: %f    %d\n", totpkts, now, departure, size);
			printQueue(buffer);
		#endif
		
		file >> now;
		file >> size;
		pktsin++;
		bytesin += size;
		//trying to work with packets arriving at the same time
		if(pre == now)
			now += delay;
		//front packet in queue is ready to leave
		if(now >= departure)
		{
			int j = 0;
			//determine how many packets can be processed until next packet arrives
			while(t <= now)
			{
				t += (float)(((float)buffer.front()+j) * 8)/(speed);
				j++;
			}
			start = t;
			if(t != now)
			{
				delay = t-now;
				avgdelay += delay;
				k++;
			}
			departure =(float)((float)buffer.front() * 8)/(speed) + start;
			while(j > 0 && !buffer.empty())
			{
				pktsout++;
				bytesout += buffer.front();
				if(buffer.size() == 99 )
				{
					#if DEBUG
					printf("%d: PACKET UNBUFFERED		%f     D: %f    BUFFER SIZE: %ld\n", totpkts,  now, departure, buffer.size());
					#endif
				}
				buffer.pop();
				j--;
			}
		}   
		pre = now;
		if(buffer.size() < MAX_BUFFER)
		{
			//printf("%d: PACKET BUFFERED		%f     D: %f    %d\n", totpkts,  now, departure, size);
			buffer.push(size);
		}
		else
		{
			//printf("%d: PACKET LOST 	BUFFER FULL: %ld\n", totpkts,  buffer.size());
			pktslost++;
			byteslost += size;
		}
		totpkts++; // keep track of all packets	
		i++;
		elapsed = now;
	}
	avgdelay = avgdelay/k; // average packet size
	buffer.pop();
	printMenu(path, speed); // Print Menu
    file.close(); // clean up
    return 0;
	
}

void printQueue(queue<int> q)
{
	while(!q.empty())
	{
		cout << " " << q.front();
		q.pop();
	}
	cout << endl;
}

void clearVars()
{
	pktsin = 0;
	bytesin = 0;
	pktsout = 0;
	bytesout = 0;
	pktslost = 0;
	byteslost = 0;
	avgdelay = 0.0;
	totpkts = 0;
	t = 0.0;
	i = 0;
	delay = 0.0;
	elapsed = 0.0;
	while(!buffer.empty())
	{
		buffer.pop();
	}
	departure = 0.0;
	start = 0.0;
	now = 0.0;
	pre = 0.0;
}

void printMenu(char* file, int64_t speed)
{
	printf("| %s  | %d | %.1ld |  %.3f  | %d - %d  | %d - %d | %d - %d    | %f%%  %f%%  | %f   |\n", file, MAX_BUFFER, speed, elapsed
	, pktsin, bytesin, pktsout, bytesout, pktslost, byteslost, avgdelay, (((double)(pktslost)/(double)(totpkts))*100), (((double)(byteslost)/(double)(bytesin))*100));
	printf("--------------------------------------------------------------------------------------------------------------------------------------------------------\n");
}