
gcc T01server.c -o server
gcc T01client.c -o client
gcc T02server.c ../network.c -o t02server
gcc T02client.c ../network.c -o t02client

gcc T03server.c ../network.c -o t03server
gcc T03client.c ../network.c -o t03client
