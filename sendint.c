int sendInt(int a){
	int statusOfSend, ackValue=1, statusOfAck;
	statusOfSend = send(sock , &a , sizeof(int) , 0);
	if(statusOfSend < 0){
		puts("Send Failed");
		return -1;
	}
	while(1){
		statusOfAck = recv(sock , &ack , sizeof(int),0);
		if(statusOfAck < 0){
			puts("Send Failed");
			return -1;
		}else{
			if(ack == 1){
				return 1;
			}else{
				puts("Send Failed");
				return -1;
			}
		}
	}	
}
