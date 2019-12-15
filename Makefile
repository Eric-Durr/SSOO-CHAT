
make:
	g++ -o claim  socket.cpp netcp_server.cpp
 	
	g++ -o serve  socket.cpp netcp_client.cpp
clean:
	rm serve claim 
