int sendIntArray(int a, int *b){
	int statusOfSend, ackValue=1, statusOfAck;
	statusOfSend = send(sock , b , sizeof(int)*a , 0);
	if(statusOfSend < 0){
		puts("Send Failed");
		return -1;
	}
	while(1){
		statusOfAck = recv(sock , &ackValue , sizeof(int),0);
		if(statusOfAck < 0){
			puts("Send Failed");
			return -1;
		}else{
			if(ackValue == 1){
				return 1;
			}else{
				puts("Send Failed");
				return -1;
			}
		}
	}	
}
