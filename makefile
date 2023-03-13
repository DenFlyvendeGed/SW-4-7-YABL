DESTINATION := ./.target
OMAIN := $(DESTINATION)/main.o 
OTEST := $(DESTINATION)/test.o 
OFILES := \
	$(DESTINATION)/cfg.tab.o \
	$(DESTINATION)/lex.yy.o

$(DESTINATION):
	mkdir $(DESTINATION)

yabl : $(OFILES) $(OMAIN)
	gcc -o $@ $^

test : $(OFILES) $(OTEST)
	gcc -o $@ $^
	


$(DESTINATION)/main.o : main.c 
	gcc -c -o $@ $^

$(DESTINATION)/test.o : test.c 
	gcc -c -o $@ $^

$(DESTINATION)/%.o : $(DESTINATION)/%.c
	gcc -c -o $@ $^

$(DESTINATION)/cfg.tab.c : cfg.y
	bison -d -Wcounterexamples $^ -o $@

$(DESTINATION)/lex.yy.c : cfg.l $(DESTINATION)/cfg.tab.c
	flex -o $@ $<


