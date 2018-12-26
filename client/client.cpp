/*
 * Here we have the code running on the laptop
 *
 *  Created on: Dec 18, 2018
 *      Author: Jean-Romain Roy
 */


#include <unistd.h>
#include <netdb.h> 
#include <iostream>
#include <bitset>
#include <strings.h>
#include <ctime>

using namespace std;

// Information Carrier
uint32_t rasp1Status = 0;
uint32_t rasp2Status = 0;
uint32_t rasp3Status = 0;


bool rasp1_Enabled = true;
bool rasp2_Enabled = true;
bool rasp3_Enabled = true;

const int Pi_portno = 4999;


int main(int argc, char *argv[]){
	// --- Raspberry 1 ---
	int sockfd1;
	struct sockaddr_in rasp1_addr;
	struct hostent *rasp1;

	sockfd1 = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd1 < 0) {
		cout << "ERROR, couldn't open socket 1" << endl;
		return -1;
	}

	rasp1 = gethostbyname("10.42.0.210");
	
	if (rasp1 == NULL) {
		cout << "ERROR, Raspberry1 IP isn't valid (10.42.0.210)" << endl;
		rasp1_Enabled = false;
	}

	bzero((char *) &rasp1_addr, sizeof(rasp1_addr));
	rasp1_addr.sin_family = AF_INET;
	bcopy((char *)rasp1->h_addr, (char *)&rasp1_addr.sin_addr.s_addr, rasp1->h_length);
	rasp1_addr.sin_port = htons(Pi_portno);

	if (connect(sockfd1,(struct sockaddr *) &rasp1_addr,sizeof(rasp1_addr)) < 0) {
		cout << "ERROR, Raspberry1 script not running" << endl;
		rasp1_Enabled = false;
	}

	
	// --- Raspberry 2 ---
	int sockfd2;
	struct sockaddr_in rasp2_addr;
	struct hostent *rasp2;

	sockfd2 = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd2 < 0) {
		cout << "ERROR, couldn't open socket 2" << endl;
		return -1;
	}

	rasp2 = gethostbyname("10.42.0.254"); // the server's ip

	if (rasp2 == NULL) {
		cout << "ERROR, Raspberry2 IP isn't valid (10.42.0.254)" << endl;
		rasp2_Enabled = false;
	}	

	bzero((char *) &rasp2_addr, sizeof(rasp2_addr));
	rasp2_addr.sin_family = AF_INET;
	bcopy((char *)rasp2->h_addr, (char *)&rasp2_addr.sin_addr.s_addr, rasp2->h_length);
	rasp2_addr.sin_port = htons(Pi_portno);

	if (connect(sockfd2,(struct sockaddr *) &rasp2_addr,sizeof(rasp2_addr)) < 0) {
		cout << "ERROR, Raspberry1 script not running" << endl;
		rasp2_Enabled = false;
	}


	// --- Raspberry 3 ---
	int sockfd3;
	struct sockaddr_in rasp3_addr;
	struct hostent *rasp3;

	sockfd3 = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd3 < 0) {
		cout << "ERROR, couldn't open socket 3" << endl;
		return -1;
	}

	rasp3 = gethostbyname("10.42.0.131"); // the server's ip

	if (rasp3 == NULL) {
		cout << "ERROR, Raspberry3 IP isn't valid (10.42.0.131)" << endl;
		rasp3_Enabled = false;
	}	

	bzero((char *) &rasp3_addr, sizeof(rasp3_addr));
	rasp3_addr.sin_family = AF_INET;
	bcopy((char *)rasp3->h_addr, (char *)&rasp3_addr.sin_addr.s_addr, rasp3->h_length);
	rasp3_addr.sin_port = htons(Pi_portno);

	if (connect(sockfd3,(struct sockaddr *) &rasp3_addr,sizeof(rasp3_addr)) < 0) {
		cout << "ERROR, Raspberry3 script not running" << endl;
		rasp3_Enabled = false;
	}

	// Check if all rpi are disabled
	if(rasp1_Enabled == false && rasp2_Enabled == false && rasp3_Enabled == false){
		return -1;
	}

	// Put the sockets in an array
	int sockets [3];
	if(rasp1_Enabled){
		sockets[0] = sockfd1;
	}
	if(rasp2_Enabled){
		sockets[1] = sockfd2;
	}
	if(rasp3_Enabled){
		sockets[2] = sockfd3;
	}
	
	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------

	fd_set set;
	struct timeval timeout;
	timeout.tv_sec = 0;
	timeout.tv_usec = 0;	
	int index = 0;

	time_t tstart, tend; 
 	tstart = time(0);

	while(1){

		if(sockets[index] <= 0){

			index += 1;
			index = index % 3;
			continue;
		}

		FD_ZERO(&set); 
		FD_SET(sockets[index], &set); 
		
		int rv = select(sockets[index] + 1, &set, NULL, NULL, &timeout);
		if (rv < 0){
			cout << "ERROR, select problem" << endl;

		}else if (rv == 0){
			// timeout, socket does not have anything to read
			usleep(1000);

		}else{
			if(recv(sockets[index], &rasp1Status, sizeof(uint32_t), 0) < 0){
				cout << "Couldnt receive Raspberry 1 status" << endl;
				//return -1; // error
			}
			bitset<32> x1(rasp1Status);
			tend = time(0); 
			cout << "It took "<< difftime(tend, tstart) << " s" << endl;
			cout << " Raspberry " << index << " : " << x1 << endl << endl;		
		}

		index += 1;
		index = index % 3;
	}

	// Close the sockets
	close(sockfd1);
	close(sockfd2);
	close(sockfd3);
	
	return 0;
}
