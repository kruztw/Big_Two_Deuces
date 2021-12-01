all:
	g++ card.cpp -o card
	g++ card2.cpp -o card2

clean:
	-rm card card2
