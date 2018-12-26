#include <unistd.h>
#include <netdb.h> 
#include <iostream>
#include <strings.h>

using namespace std;

// GPIO
#define PHOTO_PATH "/sys/class/saradc/ch0"
#define BUTTON_PATH "/sys/class/gpio/gpio228/value"

// Information Carrier
uint32_t raspStatus = 0x1; // 0xX


int main(int argc, char *argv[]){

	// ------------------------------------------------------------------------
	// ------------------ On initie la connection au socket -------------------
	// ------------------------------------------------------------------------
	int sockfd, newsockfd, n;
	socklen_t clilen;
	struct sockaddr_in serv_addr, cli_addr;
	int portno = 4000; // 400x

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
	while(1){

		// envoie le uint32_t contenant l'etat du serveur
		n = send(newsockfd, &raspStatus, sizeof(uint32_t), 0);
		if(n < 0){
			cout << "Couldnt send raspberry status" << endl;
			return -1; // error
		}

		usleep(1000);			            
	}

	// On ferme tous les services ouvert
	close(sockfd);
	return 0;
}
