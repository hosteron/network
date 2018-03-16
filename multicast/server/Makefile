CC=gcc
TARGET=multi_server
CFLAGS=
LDFLAGS=
SRC=$(wildcard ./*c)
$(TARGET):$(SRC)
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)
.PHONY:clean
clean:
	rm -f $(TARGET)
