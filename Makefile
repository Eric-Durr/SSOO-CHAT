
make:
	g++ -o claim  socket.cpp netcp_server.cpp  -pthread
 	
	g++ -o serve  socket.cpp main.cpp -pthread
clean:
	rm serve claim 
