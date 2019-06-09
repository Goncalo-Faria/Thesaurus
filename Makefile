CXX := gcc
EXEC := thesaurus
FLAGS := -std=c11 -g -Ofast
GLIB := `pkg-config --cflags --libs glib-2.0`
INCD := -I src/include

thesaurus: y.tab.c lex.yy.c
	$(CXX) -o $(EXEC) y.tab.c src/*.c $(FLAGS) $(GLIB) $(INCD)

dots:
	@cd out && $(MAKE)

lex.yy.c: thesaurus.l
	flex thesaurus.l

y.tab.c: thesaurus.y
	yacc -d -v thesaurus.y

clean:
	rm lex.yy.c y.tab.* $(EXEC) y.output
	rm out/html/*.html
	rm out/grafos/*.dot
