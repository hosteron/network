CC=gcc
TARGET=multi_client
CFLAGS=
LDFLAGS=
SRC=$(wildcard ./*c)
$(TARGET):$(SRC)
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)
.PHONY:clean
clean:
	rm -f $(TARGET)
