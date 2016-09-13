/*Ferguson Watkins
  CSE 109
  3/28/16
  Lexer Class
*/

#include <string>
#include <stdlib.h>
#include <iostream>
#include "Lexer.h"
#include "Token.h"
using namespace std;

Lexer::Lexer(istream& strm):in(strm){
  lineNum = 1;
  posNum = 0;
  ch = ' ';
}

char Lexer::nextChar(){
  if(in.eof())
    return '$';  
  char ch = in.get();
  if(ch =='\n'){
    posNum = 0;
    lineNum++;
  } else posNum++;
  
  if(ch == '#'){
    ch = in.get();
    if(ch =='\n'){
      posNum = 0;
      lineNum++;
    } else posNum++;
    while (ch != '#'){
      ch = in.get();
      if(in.eof()){
	return '?';
      }
        if(ch =='\n'){
	  posNum = 0;
	  lineNum++;
	} else posNum++;
    }
    ch = in.get();
    if(in.eof())
       return '$';
  }
  if(in.eof())
    return '$';
  return ch;
}

Token* Lexer::nextToken(){
  int line = lineNum;
  int pos = posNum;
  string lexeme = "";
  
  while(ch == ' ' || ch == '\n' || ch == '\t') { //skip spaces
    ch = nextChar();
    if(ch == '$'){ //eof
      return new Token(Token::EndOF, lexeme, line, pos);
    }
  }
  lexeme += ch; 

  //INTLITS
  if(ch >= '0' && ch <= '9'){
    if(ch == '0'){
      ch = nextChar();
      if(ch == ' ' || ch == '\n' || ch == '\t'){
	return new Token(Token::INTLIT, lexeme, line, pos);
      }
      if(ch == 'x'){
	lexeme += ch;
	ch = nextChar();
	while((ch >= 'a' && ch <= 'f') || (ch >= '0' && ch <= '9')){
	  lexeme += ch;
	  ch = nextChar();
	}
	return new Token(Token::INTLIT, lexeme, line, pos);
      } else if (ch == ' '){
	return new Token(Token::INTLIT, lexeme, line, pos);
      } else {
	lexeme += ch;
	return new Token(Token::ERROR, lexeme, line, pos);
      }
    } 
    while(ch >= '0' && ch <= '9'){
      ch = nextChar();
      if(ch >= '0' && ch <= '9'){
	lexeme += ch;
      }
    }
    return new Token(Token::INTLIT, lexeme, line, pos);
  }

  //IDENT/Special opeators
  if((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z')){
    ch = nextChar();
    while((ch >= 'a' && ch <= 'z')
	  || (ch >= 'A' && ch <= 'Z')
	  || (ch == '_')
	  || (ch >= '0' && ch <= '9')){
      lexeme += ch;
      ch = nextChar();
    }

    if(lexeme.compare("set") == 0){
      return new Token(Token::SET, lexeme, line, pos);
    } else if(lexeme.compare("print") == 0) {
	return new Token(Token::PRINT, lexeme, line, pos);
    } else if(lexeme.compare("while") == 0) {
	return new Token(Token::WHILE, lexeme, line, pos);
    } else if(lexeme.compare("do") == 0) {
	return new Token(Token::DO, lexeme, line, pos);
    } else if(lexeme.compare("end") == 0) {
	return new Token(Token::END, lexeme, line, pos);
    } else if(lexeme.compare("and") == 0) {
	return new Token(Token::AND, lexeme, line, pos);
    } else if(lexeme.compare("or") == 0) {
	return new Token(Token::OR, lexeme, line, pos);
    } else if(lexeme.compare("if") == 0) {
	return new Token(Token::IF, lexeme, line, pos);
    } else if(lexeme.compare("then") == 0) {
	return new Token(Token::THEN, lexeme, line, pos);
    } else if(lexeme.compare("else") == 0) {
	return new Token(Token::ELSE, lexeme, line, pos);
    } else if(lexeme.compare("endif") == 0) {
	return new Token(Token::ENDIF, lexeme, line, pos);
    } else if(lexeme.compare("program") == 0) {
	return new Token(Token::PROGRAM, lexeme, line, pos);
    } else {
      return new Token(Token::IDENT, lexeme, line, pos);
    }
  }
  
  //OPERATORS
  if (ch == '+') {
    ch = nextChar();
    return new Token(Token::PLUS, lexeme, line, pos);
  } else if (ch == '-') {
    ch = nextChar();
    return new Token(Token::MINUS, lexeme, line, pos);
  } else if (ch == '*') {
    ch = nextChar();
    return new Token(Token::TIMES, lexeme, line, pos);
  } else if (ch == '/') {
    ch = nextChar();
    return new Token(Token::DIVIDE, lexeme, line, pos);
  } else if (ch == '%') {
    ch = nextChar();
    return new Token(Token::REM, lexeme, line, pos);
  } else if (ch == '=') {
    ch = nextChar();
    if(ch == '='){
      lexeme += ch;
      ch = nextChar();
      return new Token(Token::EQ, lexeme, line, pos);
    }
    return new Token(Token::ASSIGN, lexeme, line, pos);
  } else if (ch == '(') {
    ch = nextChar();
    return new Token(Token::LPAREN, lexeme, line, pos);
  } else if (ch == ')') {
    ch = nextChar();
    return new Token(Token::RPAREN, lexeme, line, pos);
  } else if (ch == ',') {
    ch = nextChar();
    return new Token(Token::COMMA, lexeme, line, pos);
  } else if (ch == '<') {
    ch = nextChar();
    if(ch == '='){
      lexeme += ch;
      ch = nextChar();
      return new Token(Token::LE, lexeme, line, pos);
    }
    return new Token(Token::LT, lexeme, line, pos);
  } else if (ch == '>') {
    ch = nextChar();
    if(ch == '='){
      lexeme += ch;
      ch = nextChar();
      return new Token(Token::GE, lexeme, line, pos);
    }
    return new Token(Token::GT, lexeme, line, pos);
  } else if (ch == '!') {
    ch = nextChar();
    if(ch == '='){
      lexeme += ch;
      ch = nextChar();
      return new Token(Token::NE, lexeme, line, pos);
    }else {
      ch = nextChar();
      return new Token(Token::ERROR, lexeme, line, pos);
    }
  }
  ch = nextChar();
  return new Token(Token::ERROR, lexeme, line, pos);
}
