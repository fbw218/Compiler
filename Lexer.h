/*Ferguson Watkins
  CSE 109
  3/28/16
  LEXER Class
*/

#ifndef LEXER_H
#define LEXER_H

#include <string>
#include <iostream>
#include "Token.h"
using namespace std;

class Lexer {

public:
  //default constructor
  Lexer(istream& strm);

  char nextChar();
  Token* nextToken();

private:
  istream& in;
  int lineNum;
  int posNum;
  char ch;
};

#endif
