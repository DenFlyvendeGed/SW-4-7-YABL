DESTINATION := ./.target
export DESTINATION
OMAIN := $(DESTINATION)/main.o 
OTEST := $(DESTINATION)/test.o 
OFILES := $(shell find . -name "*.c" |  grep -o "[^/]*\.c" | grep -v -e "main\.c" -e "text\.c" | sed -E "s/\b(.*)\.c/\.target\/\1\.o/g")
CFILES := $(shell find . -name "*.c" |  grep -o "[^/]*\.c" | grep -v -e "cfg.tab.c" -e "lex.yy.c" ) 

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


