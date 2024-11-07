CC = gcc
CFLAGS = -Wall -std=c11 -I$(HOME)/curl/include -I$(HOME)/openssl/include
LDFLAGS = -L$(HOME)/curl/lib -L$(HOME)/openssl/lib -lcurl -lssl -lcrypto

TARGET = upload_to_drive
SRC = upload_to_drive.c

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC) $(LDFLAGS)

clean:
	rm -f $(TARGET)
