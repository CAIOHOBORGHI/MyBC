# Group Members
# Adriano Yoshimoto
# Bruno Domene
# Caio Borghi
# Gabriel Habberman

# This line is used to include the interfaces of the program, located at folder /include
CFLAGS=-g -I./include

# This line names the program as mybc and points to every file used in the compilation process
mybc: mybc.o lexer.o parser.o keywords.o db.o
	$(CC) -o $@ $^
	
clean:
	$(RM) *.o

mostlyclean: clean
	$(RM) *~
