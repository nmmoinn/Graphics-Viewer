CC=g++
CFLAGS=-c -Wall
LDFLAGS= -lX11
SOURCES=main.cpp matrix.cpp line.cpp shape.cpp x11context.cpp gcontext.cpp triangle.cpp image.cpp drawing.cpp viewcontext.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=graphics
all: $(SOURCES) $(EXECUTABLE) 

# pull in dependency info for *existing* .o files
-include $(OBJECTS:.o=.d)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@

.cpp.o: 
	$(CC) $(CFLAGS) $< -o $@
	$(CC) -MM $(CFLAGS) $< > $*.d

clean:
	rm -rf $(OBJECTS) $(EXECUTABLE) *.d

