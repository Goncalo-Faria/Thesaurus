CC = gcc

thesaurus: y.tab.c lex.yy.c
	gcc -o thesaurus y.tab.c

lex.yy.c: thesaurus.l
	flex thesaurus.l

y.tab.c: thesaurus.y
	yacc -d -v thesaurus.y

clean:
	rm lex.yy.c y.tab.* thesaurus y.output