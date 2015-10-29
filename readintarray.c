int readIntArray(int a, int *b){
	int statusOfRead, ackValue=1, statusOfAck;
	while(1){
		statusOfRead = recv(sock , b , sizeof(int)*a,0);
		if(statusOfRead < 0){
			puts("Receive Failed");
			return -1;
		}else{
			break;
		}
	}
	statusOfAck = send(sock , &ackValue , sizeof(int) , 0);
	if(statusOfAck < 0){
		puts("Receive Failed");
		return -1;
	}
	return 1;
}
