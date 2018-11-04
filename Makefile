CC = g++
CFLAGS = -std=c++14
TARGET = generator
SOURCES = Main.cpp \
          Hidato.cpp \
          Grid.cpp \
		  Model.cpp \
		  Group.cpp

all:
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCES)

clean:
	rm -rf *.o $(TARGET)