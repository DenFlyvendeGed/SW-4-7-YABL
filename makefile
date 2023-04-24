CFLAGS := -g


DESTINATION := ./.target
DATA_STRUCTURES:= $(DESTINATION)/data-structures
VISITOR:= $(DESTINATION)/visitor

OMAIN := $(DESTINATION)/main.o 
OTEST := $(DESTINATION)/test.o 

OFILES := \
	$(DESTINATION)/cfg.tab.parse.o \
	$(DESTINATION)/lex.yy.parse.o \
	$(DESTINATION)/cfgfunctions.o \
	$(DESTINATION)/cfg.test.o \
	$(DATA_STRUCTURES)/hashtable.o \
	$(DATA_STRUCTURES)/list.o \
	$(VISITOR)/typeChecker.o \
	$(VISITOR)/visitor.o 

$(DESTINATION):
	if [ ! -d $(DESTINATION) ] ; then mkdir $(DESTINATION) ; fi;
	if [ ! -d $(DATA_STRUCTURES) ] ; then mkdir $(DATA_STRUCTURES) ; fi
	if [ ! -d $(VISITOR) ] ; then mkdir $(VISITOR) ; fi


# Main	
yabl : $(DESTINATION) $(OFILES) $(OMAIN) $(OTEST)
	gcc -o $@ $(OFILES) $(OMAIN) $(OTEST) $(CFLAGS)


# Cleanup of destination folder
clean:
	rm -rf $(DESTINATION)

#Test
test : $(DESTINATION) $(OFILES) $(OTEST) 
	gcc -c -o $(DESTINATION)/main.o main.c -DTEST $(CFLAGS);
	gcc -o $@ $(OFILES) $(OMAIN) $(OTEST) $(CFLAGS)

$(DESTINATION)/cfg.tab.c : cfg.y $(DESTINATION)/lex.yy.c
	bison -d -Wcounterexamples $< -o $@

$(DESTINATION)/lex.yy.c : cfg.l 
	flex --header-file=./.target/lex.yy.h -o $@ $<

$(DESTINATION)/cfg.tab.parse.o : $(DESTINATION)/cfg.tab.c 
	gcc -c -o $@ $^ $(CFLAGS)

$(DESTINATION)/lex.yy.parse.o : $(DESTINATION)/lex.yy.c 
	gcc -c -o $@ $^ $(CFLAGS)

	
$(DESTINATION)/%.o : %.c $(DESTINATION)/cfg.tab.parse.o $(DESTINATION)/lex.yy.parse.o 
	gcc -c -o $@ $< $(CFLAGS)



