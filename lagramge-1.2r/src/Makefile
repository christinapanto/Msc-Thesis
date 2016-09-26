CC = clang
LYCFLAGS = -g -O -DHOME_DIR=\"/Users/christinapantopoli/Desktop/lagramge-1.2r\"
CFLAGS = $(LYCFLAGS) -Wall -pedantic

OBJECTS1 = beam.o count.o error.o eval.o globals.o gramm_l.o gramm_y.o init.o \
	load.o main.o model.o prod.o ptree.o realloc.o search.o

OBJECTS2 = error.o rgramm_l.o rgramm_y.o rg.o

OBJECTS3 = count.o error.o globals.o gramm_l.o gramm_y.o init.o load.o \
	model.o prod.o realloc.o simulate.o

OBJECTS_ALG_717 = ./alg-717/dgletc.o ./alg-717/dglfgb.o ./alg-717/dmdc.o

all: rg g # s

rg: $(OBJECTS2)
	$(CC) -o $@ $(OBJECTS2) -ll

g: $(OBJECTS1)
	$(CC) -o $@ $(OBJECTS1) $(OBJECTS_ALG_717) -ll -lm -ldl -L/usr/local/gfortran/lib -lgfortran 
	

s: $(OBJECTS3)
	$(CC) -o $@ $(OBJECTS3) -lfl -lm -ldl

clean:
	rm -f gramm_l.c gramm_y.c gramm_y.h rgramm_l.c rgramm_y.c rgramm_y.h
	rm -f $(OBJECTS1) $(OBJECTS2) $(OBJECTS3)

gramm_l.c: gramm.l
	lex -t gramm.l > $@

gramm_l.o: gramm_l.c gramm_y.h error.h
	$(CC) $(LYCFLAGS) -c -o $@ gramm_l.c

gramm_y.c gramm_y.h: gramm.y
	yacc -d gramm.y
	mv y.tab.c gramm_y.c
	mv y.tab.h gramm_y.h

gramm_y.o: gramm_y.c globals.h gramm_t.h data_t.h realloc.h
	$(CC) $(LYCFLAGS) -c -o $@ gramm_y.c

rgramm_l.c: rgramm.l
	lex -t rgramm.l > $@

rgramm_l.o: rgramm_l.c rgramm_y.h error.h
	$(CC) $(LYCFLAGS) -c -o $@ rgramm_l.c

rgramm_y.c rgramm_y.h: rgramm.y
	yacc -d rgramm.y
	mv y.tab.c rgramm_y.c
	mv y.tab.h rgramm_y.h

rgramm_y.o: rgramm_y.c globals.h gramm_t.h data_t.h realloc.h
	$(CC) $(LYCFLAGS) -c -o $@ rgramm_y.c


# DO NOT DELETE THIS LINE -- make depend depends on it.

beam.o: beam.c beam_t.h globals.h
count.o: count.c globals.h gramm_t.h ptree_t.h data_t.h prod.h
dstep_t.o: dstep_t.c gramm_t.h
error.o: error.c error.h
eval.o: eval.c ptree_t.h gramm_t.h globals.h data_t.h prod.h
globals.o: globals.c gramm_t.h ptree_t.h data_t.h beam_t.h
gramm.o: gramm.c gramm_y.h error.h globals.h gramm_t.h ptree_t.h data_t.h \
	realloc.h
init.o: init.c globals.h gramm_t.h ptree_t.h data_t.h error.h prod.h beam_t.h
load.o: globals.h gramm_t.h
load.o: ptree_t.h data_t.h error.h
main.o: main.c load.h globals.h gramm_t.h ptree_t.h data_t.h count.h search.h \
	init.h error.h beam.h model.h
prod.o: prod.c gramm_t.h
realloc.o: realloc.c
rg.o: rg.c error.h
rgramm.o: rgramm.c rgramm_y.h error.h
rgramm_l.o: rgramm_l.c rgramm_y.h error.h
rgramm_y.o: rgramm_y.c
search.o: search.c ptree_t.h gramm_t.h globals.h data_t.h prod.h eval.h beam.h

model.o: model.c beam_t.h
simulate.o: simulate.c load.h globals.h gramm_t.h ptree_t.h data_t.h count.h \
	init.h error.h beam.h model.h
