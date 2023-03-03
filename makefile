DESTINATION := ./.target
OFILES := \
	$(DESTINATION)/main.o \
	$(DESTINATION)/cfg.tab.o

$(DESTINATION):
	mkdir $(DESTINATION)

yabl : $(OFILES)
	gcc -o $@ $^


$(DESTINATION)/main.o : main.c
	gcc -c -o $@ $^

$(DESTINATION)/cfg.tab.o : $(DESTINATION)/cfg.tab.c
	gcc -c -o $@ $^

$(DESTINATION)/cfg.tab.c : cfg.y
	bison -Wcounterexamples $^ -o $@


