/*Ferguson Watkins
  CSE 109
  3/28/16
  prog4.cc
*/

#include <iostream>
#include <fstream>
#include "Token.h"
#include "Lexer.h"
using namespace std;

int main(int argc, char **argv) {  
  Lexer* myLex;
  ifstream in;
  
  if(argc > 1){
    in.open(argv[1]);
    myLex = new Lexer(in);
  } else {
    myLex = new Lexer(cin);
  }
  
  Token* myToken = myLex->nextToken();
  cout << "Type\tLexeme\tLine #\tPos\n";
  while(myToken->getType() != Token::EndOF){
    cout << myToken->getType() << "\t";
    cout << myToken->getLexeme() << "\t";
    cout << myToken->getLine() << "\t";
    cout << myToken->getPos() << "\t\n";
    myToken = myLex->nextToken();
  }

  if (argc > 1){
    in.close();
  }
  return 0;
}


