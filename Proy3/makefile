CC = g++
CFLAGS = -g -Wall -I. -c
STD11 = -std=c++11
LFLAGS = -L ./ga -lga -lg


GABIL: objeto experimentosGABIL
	$(CC) GABIL.o -o GABIL $(LFLAGS)
objeto:
	$(CC) $(CFLAGS) GABIL.cpp $(STD11)

experimentosGABIL: objetoEX
	$(CC) experimentosGABIL.o -o experimentosGABIL $(LFLAGS)

objetoEX: 
	$(CC) $(CFLAGS) experimentosGABIL.cpp $(STD11)
clean:
	rm *.o *~ experimentosGABIL GABIL
