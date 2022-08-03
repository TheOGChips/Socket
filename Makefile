SERVER = server
CLIENT = client
SOCKET = Socket
SRC = $(SERVER).cpp $(SOCKET).cpp
OBJ = $(SERVER).o $(SOCKET).o
TGT = $(SERVER).exe
ALL = $(SERVER) $(CLIENT)

#all: $(ALL)

$(SERVER):	$(OBJ)
			g++ -o $(TGT) $^

$(OBJ):	$(SRC)
		g++ -c $^

$(CLIENT): $(CLIENT).cpp $(SOCKET).cpp
		g++ -o $@.exe $^

.PHONY:	clean run runclient
clean:	$(TGT) $(CLIENT).exe
		rm *.o $^

run:	$(TGT)
		./$<

runclient:	$(CLIENT).exe
			./$< $(arg1)