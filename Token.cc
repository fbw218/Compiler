/*Ferguson Watkins
  CSE 109
  3/28/16
  Token Class
*/

#include <string>
#include <stdlib.h>
#include <iostream>
#include "Token.h"
using namespace std;

Token::Token(){};

//constructor
Token::Token(int t, string lex, int ln, int position){
  type = t;
  lexeme = lex;
  line = ln;
  pos = position; 
};

Token::~Token(){
}

//public methods
int Token::getType(){
  return type;
}

string Token::getLexeme(){
  return lexeme;
}

int Token::getLine(){
  return line;
}

int Token::getPos(){
  return pos;
}
