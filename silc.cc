/*Ferguson Watkins
  CSE 109
  3/28/16
  prog4.cc
*/

#include <iostream>
#include <fstream>
#include "Token.h"
#include "Lexer.h"
#include "parser.h"
using namespace std;

int main(int argc, char **argv) {  
  Lexer* myLex;
  Parser* myPars;
  ifstream in;
  
  if(argc > 1){
    in.open(argv[1]);
    myLex = new Lexer(in);
  } else {
    myLex = new Lexer(cin);
  }
  
  myPars = new Parser(*myLex, cout);
  
  cout << Parser::TreeNode::toString(myPars->program()) << endl;
  //myPars->genasm(myPars->program());
  if (argc > 1){
    in.close();
  }
  return 0;
}
