int main(){
	int statusOfSend, statusOfRead, queryResult, port, clientID = 1, ackValue=1, statusOfAck;
  	int ip[4];
	struct sockaddr_in server;
	
    sock = socket(AF_INET , SOCK_STREAM , 0);
    if (sock == -1)
    {
        puts("Could not create socket");
		return 1;
    }
    puts("Socket created");

	//Connect to Directory Register and get ip and port for service
	server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_family = AF_INET;
    server.sin_port = htons( 8000 );
	
	//Connect to Directory Service
	puts("Trying to connect to Directory Service");
	int connected = -1;
	while(connected == -1){
    	connected = connect(sock , (struct sockaddr *)&server , sizeof(server));
	}
    
	//Connected to Directory Service
    puts("Connected");
	
	//Let Directory know you are the client
	statusOfSend = send(sock, &clientID, sizeof(int), 0);
	if(statusOfSend < 0){
		puts("Send programID failed");
		close(sock);
		return 0;
	}
	
	//Send Lookup Request
	
	//Send Program ID
	statusOfSend = send(sock, &programID, sizeof(programID), 0);
	if(statusOfSend < 0){
		puts("Send programID failed");
		close(sock);
		return 0;
	}
	
	//Send Version
	statusOfSend = send(sock, &version, sizeof(version), 0);
	if(statusOfSend < 0){
		puts("Send Version failed");
		close(sock);
		return 0;
	}
	
	//Get Status of Server Found or Not
	statusOfRead = recv(sock , &queryResult , sizeof(queryResult),0);
	if(statusOfRead < 0){
		puts("Receive Query Result failed");
		close(sock);
		return 0;
	}else{
		if(queryResult == 0){
			puts("Server Not Found");
			close(sock);
			return 0;
		}
	}

	//Send the ACK
	statusOfAck = send(sock , &ackValue , sizeof(int) , 0);
	if(statusOfAck < 0){
		puts("Send ACK failed");
		close(sock);
		return 0;
	}

	//Get IP
	statusOfRead = recv(sock , &ip , sizeof(int)*4,0);
	if(statusOfRead < 0){
		puts("Receive IP failed");
		close(sock);
		return 0;
	}
	
	//Send the ACK
	statusOfAck = send(sock , &ackValue , sizeof(int) , 0);
	if(statusOfAck < 0){
		puts("Send ACK failed");
		close(sock);
		return 0;
	}

	//Get Port
	statusOfRead = recv(sock , &port , sizeof(port),0);
	if(statusOfRead < 0){
		puts("Receive Port failed");
		close(sock);
		return 0;
	}

	//Convert ip to string
	char address[50];
	sprintf(address, "%d.%d.%d.%d", ip[0], ip[1], ip[2], ip[3]);
	
	//Close connection to Directory Service
	close(sock);
	
	//Create socket
    sock = socket(AF_INET , SOCK_STREAM , 0);
    if (sock == -1)
    {
        puts("Could not create socket");
		return 1;
    }
    puts("Socket created");
	
	//Now setup Server connection
	server.sin_addr.s_addr = inet_addr(address);
    server.sin_family = AF_INET;
    server.sin_port = htons( port );
	
	//Connect to Remote Server
	puts("Trying to connect to Remote Server");
	connected = -1;
	while(connected == -1){
    	connected = connect(sock , (struct sockaddr *)&server , sizeof(server));
	}
    
	//Connected to Remote Server
    puts("Connected");

	while(1){
		//Call input in client file
		if(input() == -1){
			break;
		}
	}
	
	puts("Connection terminated.");
	close(sock);
	return 0;
}
