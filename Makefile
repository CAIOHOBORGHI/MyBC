CFLAGS=-g -I./include

mybc: mybc.o lexer.o parser.o keywords.o db.o
	$(CC) -o $@ $^

clean:
	$(RM) *.o

mostlyclean: clean
	$(RM) *~
