CFLAGS :=


DESTINATION := ./.target
DATA_STRUCTURES:= $(DESTINATION)/data-structures
OMAIN := $(DESTINATION)/main.o 
OTEST := $(DESTINATION)/test.o 
OFILES := \
	$(DESTINATION)/cfg.tab.o \
	$(DESTINATION)/lex.yy.o \
	$(DATA_STRUCTURES)/hashtable.o \
	$(DATA_STRUCTURES)/list.o 

$(DESTINATION):
	if [ ! -d $(DESTINATION) ] ; then mkdir $(DESTINATION) ; fi;
	if [ ! -d $(DATA_STRUCTURES) ] ; then mkdir $(DATA_STRUCTURES) ; fi



# Main	
yabl : $(DESTINATION) $(OFILES) $(OMAIN)
	gcc -o $@ $(OFILES) $(OMAIN)

clean:
	rm -rf $(DESTINATION)

#Test
test : $(DESTINATION) $(OFILES) $(OTEST) 
	gcc -o $@ $(OFILES) $(OTEST)

$(DESTINATION)/%.o : %.c 
	gcc -c -o $@ $^ $(CFLAGS)

$(DESTINATION)/%.o : $(DESTINATION)/%.c
	gcc -c -o $@ $^ $(CFLAGS)

$(DESTINATION)/cfg.tab.c : cfg.y
	bison -d -Wcounterexamples $^ -o $@

$(DESTINATION)/lex.yy.c : cfg.l $(DESTINATION)/cfg.tab.c
	flex -o $@ $<


