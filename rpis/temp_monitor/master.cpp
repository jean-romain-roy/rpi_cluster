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

bool slave1_Enabled = true;
bool slave2_Enabled = true;

const int Pi_portno = 4999;

// GPIO
#define TEMP_PATH "/sys/class/thermal/thermal_zone0/temp"

uint32_t tempMaster = 0;
uint32_t tempSlave1 = 0;
uint32_t tempSlave2 = 0;


int main(int argc, char *argv[]){
	
	// --- Slave 1 ---
	int sockfd1;
	struct sockaddr_in slave1_addr;
	struct hostent *slave1;

	sockfd1 = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd1 < 0) {
		cout << "ERROR, couldn't open socket 1" << endl;
		return -1;
	}

	slave1 = gethostbyname("192.168.0.11"); // the server's ip

	if (slave1 == NULL) {
		cout << "ERROR, Slave1 IP isn't valid (192.168.0.11)" << endl;
		slave1_Enabled = false;
	}	

	bzero((char *) &slave1_addr, sizeof(slave1_addr));
	slave1_addr.sin_family = AF_INET;
	bcopy((char *)slave1->h_addr, (char *)&slave1_addr.sin_addr.s_addr, slave1->h_length);
	slave1_addr.sin_port = htons(Pi_portno);

	if (connect(sockfd1,(struct sockaddr *) &slave1_addr,sizeof(slave1_addr)) < 0) {
		cout << "ERROR, Slave1 script not running" << endl;
		slave1_Enabled = false;
	}


	// --- Slave 1 ---
	int sockfd2;
	struct sockaddr_in slave2_addr;
	struct hostent *slave2;

	sockfd2 = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd2 < 0) {
		cout << "ERROR, couldn't open socket 2" << endl;
		return -1;
	}

	slave2 = gethostbyname("192.168.0.12"); // the server's ip

	if (slave2 == NULL) {
		cout << "ERROR, Slave2 IP isn't valid (192.168.0.11)" << endl;
		slave2_Enabled = false;
	}	

	bzero((char *) &slave2_addr, sizeof(slave2_addr));
	slave2_addr.sin_family = AF_INET;
	bcopy((char *)slave2->h_addr, (char *)&slave2_addr.sin_addr.s_addr, slave2->h_length);
	slave2_addr.sin_port = htons(Pi_portno);

	if (connect(sockfd2,(struct sockaddr *) &slave2_addr,sizeof(slave2_addr)) < 0) {
		cout << "ERROR, Slave2 script not running" << endl;
		slave2_Enabled = false;
	}
	
	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------

	// Ouvre le fichier contenant la temperature du CPU
	ifstream tempFile(TEMP_PATH);
	if (!tempFile.is_open()) {                 
		cout << " Failed to open CPU temp file." << endl;
		return -1;
	}

	
	while(1){
		
		// Check voltage Photo Diode
		tempFile.clear();	// clear flags
		tempFile.seekg(0);	// fait revenir le pointeur au debut
		tempFile >> tempMaster;
		cout << "\tMaster : "<< tempMaster << " C" << endl;

		if(slave1_Enabled){
			
			if(recv(sockfd1, &tempSlave1, sizeof(uint32_t), 0) < 0){
				cout << "Couldnt receive Slave1 temp" << endl;
			}

			cout << "\tSlave1 : " << tempSlave1 << endl;
		}


		if(slave2_Enabled){

			if(recv(sockfd2, &tempSlave2, sizeof(uint32_t), 0) < 0){
				cout << "Couldnt receive Slave2 temp" << endl;
			}

			cout << "\tSlave2 : " << tempSlave2 << " C" << endl;
		}

		cout << endl;

		sleep(5);
	}

	// Close the file
	tempFile.close();

	// Close the sockets
	close(sockfd1);
	close(sockfd2);

	return 0;
}
