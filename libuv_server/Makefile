CC = g++

LDFLAGS = -luv

TARGET = uv_tcp_server

SRCS = main.cpp

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CC) -o $@ $^ $(LDFLAGS)
