# Компилятор и его флаги
CC = gcc
CFLAGS = -c -Wall

# Windows API для работы с сокетами
WIN_API_SOCKETS = -l ws2_32

.PHONY: all clean

all: bin/server.exe bin/client.exe

# Правило создания директории bin 
bin:
	mkdir $@

# Правило создания директории obj
obj:
	mkdir $@

# Правило получения объектных файлов из исходников
obj/server.o: src/server.c | obj
	$(CC) $(CFLAGS) -o  $@ $<

obj/client.o: src/client.c | obj
	$(CC) $(CFLAGS) -o  $@ $<

obj/socket.o: src/socket.c src/socket.h | obj
	$(CC) $(CFLAGS) -o  $@ $< 

# Правило получения исполняемых файлов из объектных файлов
bin/server.exe: obj/server.o obj/socket.o | bin 
	$(CC) -o $@ $^ $(WIN_API_SOCKETS)

bin/client.exe: obj/client.o obj/socket.o | bin 
	$(CC) -o $@ $^ $(WIN_API_SOCKETS)

clean:
	del /Q bin obj 
	rmdir bin obj
