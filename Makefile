
make:
	g++ -o serve netcp_server.cpp
	g++ -o claim netcp_client.cpp

clean:
	rm serve claim 