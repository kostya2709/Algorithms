FNAME=test1
CFLAGS= -c -Wall
LFLAGS= -o


all: $(FNAME)

$(FNAME): $(FNAME).o
	@g++ $(LFLAGS) $(FNAME) main.o -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio
	@./$(FNAME)

$(FNAME).o:
	@g++ $(CFLAGS)  main.cpp

g: all
	@dot -Tps graph.gv -o graph.ps
	@xdg-open graph.ps

clean:
	@rm -rf $(FNAME) *.o

graph: clean
	
