#include <unistd.h>
#include <netdb.h> 
#include <iostream>
#include <strings.h>
#include <fstream>

using namespace std;

// GPIO
#define TEMP_PATH "/sys/class/thermal/thermal_zone0/temp"

int main(int argc, char *argv[]){

	// ------------------------------------------------------------------------
	// ------------------ On initie la connection au socket -------------------
	// ------------------------------------------------------------------------
	int sockfd, newsockfd, n;
	socklen_t clilen;
	struct sockaddr_in serv_addr, cli_addr;
	int portno = 4999; // 400x

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0){
		cout << "ERROR opening socket" << endl;
		return -1;
	}
	bzero((char *) &serv_addr, sizeof(serv_addr));

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(portno);
	if (bind(sockfd, (struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0){
		cout << "ERROR on binding" << endl;
		return -1;
	}

	listen(sockfd,5);
	clilen = sizeof(cli_addr);

	newsockfd = accept(sockfd,(struct sockaddr *) &cli_addr,&clilen);
	
	if (newsockfd < 0){
        	cout << "ERROR on accept" << endl;
		return -1;
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

	uint32_t tempRasp = 0;

	while(1){

		// Check voltage Photo Diode
		tempFile.clear();	// clear flags
		tempFile.seekg(0);	// fait revenir le pointeur au debut
		tempFile >> tempRasp;


		// envoie le uint32_t contenant l'etat du serveur
		n = send(newsockfd, &tempRasp, sizeof(uint32_t), 0);
		if(n < 0){
			cout << "Couldnt send raspberry status" << endl;
			return -1; // error
		}

		sleep(1);
	}

	// On ferme tous les services ouvert
	tempFile.close();
	close(sockfd);
	return 0;
}
