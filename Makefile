FLAGS=-Wall -g
project:
	gcc image.c -c
	gcc Stego.c -c
	gcc StegoExtract.c -c
	gcc Stego.o image.o -o Stego
	gcc StegoExtract.o image.o -o StegoExtract
clean:
	rm -f Stego.o
	rm -f Stego
	rm -f StegoExtract
	rm -f StegoExtract.o
	rm -f image.o
	rm -f new.pgm
	rm -f out.txt
