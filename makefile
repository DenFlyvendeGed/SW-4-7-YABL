CFLAGS := -g


DESTINATION := ./.target
DATA_STRUCTURES:= $(DESTINATION)/data-structures
VISITOR:= $(DESTINATION)/visitor
CODE_GENERATION:= $(DESTINATION)/code-generation
CFG:=$(DESTINATION)/cfg
CONFIGURATION:=$(DESTINATION)/configuration
CONST_CODE:=$(CODE_GENERATION)/const-code

OMAIN := $(DESTINATION)/main.o 
OTEST := $(DESTINATION)/test.o 

OFILES := \
	$(DESTINATION)/cfg.tab.parse.o \
	$(DESTINATION)/lex.yy.parse.o \
	$(CFG)/cfgfunctions.o \
	$(CFG)/cfg.test.o \
	$(DATA_STRUCTURES)/hashtable.o \
	$(DATA_STRUCTURES)/list.o \
	$(DATA_STRUCTURES)/stack.o \
	$(VISITOR)/typeChecker.o \
	$(VISITOR)/visitor.o \
	$(CODE_GENERATION)/code-generation.o \
	$(CONFIGURATION)/configuration.o\
	$(CONST_CODE)/const-code.o

CONST_CODE_FILES := \
	./code-generation/const-code/print.c \
	./code-generation/const-code/includes.c \
	./code-generation/const-code/yabl_main.c \
	./code-generation/const-code/list.c \
	./code-generation/const-code/string.c \
	./code-generation/const-code/globals.c \
	./code-generation/const-code/garbage_collection.c \
	./code-generation/const-code/events_initializers.c \
	./code-generation/const-code/board.c \
	./code-generation/const-code/type_cast.c \
	./code-generation/const-code/reserved_keyword.c

$(DESTINATION):
	if [ ! -d $(DESTINATION) ] ; then mkdir $(DESTINATION) ; fi;
	if [ ! -d $(DATA_STRUCTURES) ] ; then mkdir $(DATA_STRUCTURES) ; fi
	if [ ! -d $(VISITOR) ] ; then mkdir $(VISITOR) ; fi
	if [ ! -d $(CODE_GENERATION) ] ; then mkdir $(CODE_GENERATION) ; fi
	if [ ! -d $(CFG) ] ; then mkdir $(CFG) ; fi
	if [ ! -d $(CONFIGURATION) ] ; then mkdir $(CONFIGURATION) ; fi
	if [ ! -d $(CONST_CODE) ] ; then mkdir $(CONST_CODE) ; fi

# Main	
yabl : $(DESTINATION) $(OFILES) $(OMAIN) $(OTEST)
	gcc -o $@ $(OFILES) $(OMAIN) $(OTEST) $(CFLAGS)


# Cleanup of destination folder
clean:
	rm -rf $(DESTINATION); rm ./code-generation/const-code/const-code.c

#Test
test : $(DESTINATION) $(OFILES) $(OTEST) 
	gcc -c -o $(DESTINATION)/main.o main.c -DTEST $(CFLAGS);
	gcc -o $@ $(OFILES) $(OMAIN) $(OTEST) $(CFLAGS)

$(DESTINATION)/cfg.tab.c : cfg/cfg.y $(DESTINATION)/lex.yy.c
	bison -d  $< -o $@

$(DESTINATION)/lex.yy.c : cfg/cfg.l 
	flex --header-file=./.target/lex.yy.h -o $@ $<

$(DESTINATION)/cfg.tab.parse.o : $(DESTINATION)/cfg.tab.c 
	gcc -c -o $@ $^ $(CFLAGS)

$(DESTINATION)/lex.yy.parse.o : $(DESTINATION)/lex.yy.c 
	gcc -c -o $@ $^ $(CFLAGS)

./code-generation/const-code/const-code.c : $(CONST_CODE_FILES)
	python ./code-generation/const-code/run.py $^
	
$(DESTINATION)/%.o : %.c $(DESTINATION)/cfg.tab.parse.o $(DESTINATION)/lex.yy.parse.o ./code-generation/const-code/const-code.c
	gcc -c -o $@ $< $(CFLAGS)



