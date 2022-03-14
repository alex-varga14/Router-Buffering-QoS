// CPSC 441 ASSIGNMENT 3 
// File: buffer.c
// Author: Alexander Varga
// UCID: 30099000
// date: 03/12/2022

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

// Buffer size in packets
#define MAX_BUFFER 100

// Wireless link data rate
#define SPEED 3000000.0

// Debug Flag
#define DEBUG 1

#define PATH "zoom.txt"

double getArrivalTime(FILE *fp);
int getPacketSize(FILE *fp);
double serviceTime(int pktsize);
void iPlus(int* index, int max);
void printMenu();

int main(int argc, char *argv[])
{
	float now = 0.0;
	float atimes[MAX_BUFFER]; // arrival times
	int size; // packet size, and packet sizes
	float departure = 0.0; // departure time
	int totpkts = 0; // total size in packets
	long long totKB = 0; // total size in KB
	float avgdelay = 0.0;
	
	FILE *fp;
	double start = 0.0;
	int first_in = 0;
	int buffered_packets = 0;
	int last_in = 0;
	int available_space = 0;
	int dropped_packets = 0;
	
	
	
	//Start timer
	auto start_time = std::chrono::steady_clock::now();
	
	
	float prev, elapsed; // timing
	int pktsin, pktsout, pktslost; // packets in, out and lost
	int bytesin, bytesout, byteslost; // bytes in, out and lost
	int pktsinq, bytesinq; // packets in queue and bytes in queue
	int maxpkts, maxbytes; // max packets and bytes
	int sizes[MAX_BUFFER];
	float totdelay;
	int credit;
	int i;
	
	double percentpktslost, percentbyteslost;

	
	
	memset(atimes, 0, MAX_BUFFER);
	fp = fopen(PATH, "r"); //readable only
    if (fp == NULL){
        fprintf(stderr, "Cannot open File \"%s\"\n", PATH);
    }
	printf("ENTERING LOOP\n");
	while(!feof(fp))
	{
		//printf("IN LOOP\n");
		#if DEBUG
			printf("%d: %f      %d\n", totpkts, now, size);
		#endif
		now = getArrivalTime(fp); // current time
		size = getPacketSize(fp); // size of packet
		departure = serviceTime(atimes[first_in]) + start; // expected time to finish
		if(now >= departure)
		{
			//Unbuffer
			atimes[first_in] = 0; // get rid of packet
			buffered_packets--; // tell buffer that we can allow more packets in
			iPlus(&first_in, MAX_BUFFER); // start working on next packet in buffer
			start = now; // start processing the next packet at current time stamp
		}
		if(buffered_packets < MAX_BUFFER)
		{
			// Add to buffer
			buffered_packets++; // fil yp one more spot
			iPlus(&last_in, MAX_BUFFER); // will never be used bu this is the last packet to be buffered
			atimes[available_space] = size; // "buffer" the packet
			iPlus(&available_space, MAX_BUFFER); // space following this is now next available
			totKB += size; // for calculating average delay, only count buffered packets
		}
		else
		{
			// Do nothing with packetm it is lost
			dropped_packets++;
		}
		totpkts++; // keep track of all packets	
	}
	avgdelay = totKB/(totpkts - dropped_packets); // average packet size
	
	//printMenu();
	
    printf("AP Buffer Size: %d pkts\nWireless Link Speed: %f.2 bps\nEnd time: %f\n", MAX_BUFFER, SPEED, elapsed);
	printf("Incoming Traffic: %d pkts	%d bytes\nOutgoing Traffic: %d pkts	%d bytes\n", pktsin, bytesin, pktsout, bytesout);
	printf("Buffered Traffic: %d pkts	%d bytes\nDiscarded Traffic: %d pkts	%d bytes\n", pktsinq, bytesinq, pktslost, byteslost);
	printf("Peak Occupancy: %d pkts	%d bytes\nLost Traffic: %f%% pkts	%f%% bytes\n", maxpkts, maxbytes, percentpktslost, percentbyteslost);
	printf("Average Occupancy: %d pkts	%d bytes\nAverage Queuing Delay: %f seconds\n", avg_occupacny, avg_occupancy_vytes, avgdelay);
  
    fclose(fp); // clean up

    return 0;
	
	
}

void printMenu()
{
}

void iPlus(int* index, int max)
{
	(*index)++;
	if(*index >= max)
		*index = 0;
}

double getArrivalTime(FILE *fp)
{
	double time;
	fscanf(fp, "%lf\n", &time);
	return time;
}


int getPacketSize(FILE *fp)
{
	int size;
	fscanf(fp, "%d\n", &size);
	return size;
}

double serviceTime(int pktsize)
{
	double time;
	time = (double)((double)pktsize * 8)/(SPEED * 1024 * 1024);
	return time;
}
	