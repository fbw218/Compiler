/*Ferguson Watkins
  CSE 109
  3/28/16
  Token Class
*/

#ifndef TOKEN_H
#define TOKEN_H

#include <string>
#include <iostream>
using namespace std;

class Token {

public:
  Token();
  
  //default constructor
  Token(int type, string lex, int line, int pos);

  //descructor
  ~Token();
  
  //public getters for private vals
  int getType();
  string getLexeme();
  int getLine();
  int getPos();

  //Token types
  const static int INTLIT = 31;
  const static int IDENT = 1;
  const static int PLUS = 2;
  const static int MINUS = 3;
  const static int TIMES = 4;
  const static int DIVIDE = 5;
  const static int REM = 6;
  const static int ASSIGN = 7;
  const static int LPAREN = 8;
  const static int RPAREN = 9;
  const static int COMMA = 10;
  const static int EQ = 11;
  const static int LT = 12;
  const static int LE = 13;
  const static int GT = 14;
  const static int GE = 15;
  const static int NE = 16;
  const static int SET = 17;
  const static int PRINT = 18;
  const static int WHILE = 19;
  const static int DO = 20;
  const static int END = 21;
  const static int AND = 22;
  const static int OR = 23;
  const static int IF = 24;
  const static int THEN = 25;
  const static int ELSE = 26;
  const static int ENDIF = 27;
  const static int PROGRAM = 28;
  const static int EndOF = 29;
  const static int ERROR = 30;

private:
  //int to hold the token type
  int type;

  //string to hold lexeme
  string lexeme;

  //int to hold input file line num where token was found
  int line;

  //int to hold char pos on the line wehre token was found
  int pos;
};


#endif
