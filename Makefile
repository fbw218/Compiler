OPTS = -g -c -Wall -Werror

silc: lexer.o Token.o silc.o parser.o
	g++ -o silc lexer.o Token.o parser.o silc.o

parser.o: parser.cc parser.h Lexer.h Token.h
	g++ $(OPTS) parser.cc

silc.o: silc.cc parser.h Token.h Lexer.h
	g++ $(OPTS) silc.cc

lextest: lexer.o Token.o prog4.o
	g++ -o lextest lexer.o Token.o prog4.o

lexer.o: lexer.cc Lexer.h Token.h
	g++ $(OPTS) lexer.cc

token.o: Token.cc Token.h
	g++ $(OPTS) Token.cc

prog4.o: prog4.cc Token.h Lexer.h
	g++ $(OPTS) prog4.cc

clean:
	rm -f *.o *# *~ silc lextest myProg myProg.asm