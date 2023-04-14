CFLAGS := -g


DESTINATION := ./.target
DATA_STRUCTURES:= $(DESTINATION)/data-structures
VISITOR:= $(DESTINATION)/visitor

OMAIN := $(DESTINATION)/main.o 
OTEST := $(DESTINATION)/test.o 

OFILES := \
	$(DESTINATION)/cfg.tab.o \
	$(DESTINATION)/lex.yy.o \
	$(DESTINATION)/cfgfunctions.o \
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
	gcc -o $(DESTINATION)/main.o main.c -DTEST $(CFLAGS);\
	gcc -o $@ $(OFILES) $(OTEST) $(CFLAGS)

$(DESTINATION)/%.o : %.c 
	gcc -c -o $@ $^ $(CFLAGS)

$(DESTINATION)/%.o : $(DESTINATION)/%.c
	gcc -c -o $@ $^ $(CFLAGS)

$(DESTINATION)/cfg.tab.c : cfg.y
	bison -d -Wcounterexamples $^ -o $@

$(DESTINATION)/lex.yy.c : cfg.l $(DESTINATION)/cfg.tab.c
	flex -o $@ $<


