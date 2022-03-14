// CPSC 441 ASSIGNMENT 3 
// File: buffer.c
// Author: Alexander Varga
// UCID: 30099000
// date: 03/12/2022

#include <stdio.h>

// Buffer size in packets
#define MAX_BUFFER 100

// Wireless link data rate
#define SPEED 3000000.0

// Debug Flag
#define DEBUG 1

int main()
{
	float now, prev, elapsed;
	int pktsin, pktsout, pktslost;
	int bytesin, bytesout, byteslost;
	int pktsinq, bytesinq;
	int maxpkts, maxbytes;
	int size, sizes[MAX_BUFFER];
	float atimes[MAX_BUFFER];
	float departures;
	float totdelay, avgdelay;
	int totpkts;
	float totKB;
	int credit;
	int i;
	
	
	
	
}