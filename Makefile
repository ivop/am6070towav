#CXXFLAGS=-O3 -Wall -Wextra -Wno-unused-parameter -flto
CXXFLAGS=-O0 -g3 -Wall -Wextra -Wno-unused-parameter -flto
LIBS=-lm
SRC=main.cpp
OBJ=$(SRC:.cpp=.o)
LIBS=
AM6070TOWAV=am6070towav

CXXFLAGS += $(shell fltk-config --use-images --cflags)
CXXFLAGS += $(shell sdl2-config --cflags)
LIBS += $(shell fltk-config --use-images --ldstaticflags)
LIBS += $(shell sdl2-config --static-libs)

all: $(AM6070TOWAV)

$(AM6070TOWAV): $(OBJ)
	$(CXX) $(STATIC) -flto -o $@ $^ $(LIBS)

%.o: %.cpp
	$(CXX) -c -o $@ $(CXXFLAGS) $<

clean:
	rm -f *.o *.a *~ */*~ $(AM6070TOWAV) $(OBJ)

depend:
	rm -f .depend
	+make .depend

.depend:
	$(CC) -MM $(SRC) > .depend

include .depend
