
make:
	g++ -o claim netcp_server.cpp
	g++ -o serve netcp_client.cpp

clean:
	rm serve claim 