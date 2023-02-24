DESTINATION := ./.target

$(DESTINATION):
	mkdir $(DESTINATION)

yabl : $(DESTINATION)/main.o
	gcc -o $@ $(DESTINATION)/main.o


$(DESTINATION)/main.o : main.c
	gcc -c -o $@ main.c


