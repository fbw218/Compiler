/*Ferguson Watkins
  parser.cc
  4/15/16
*/

#include "Lexer.h"
#include "Token.h"
#include "parser.h"
#include <cstring>

const string Parser::ops[] = {"ADD", "SUB", "MULT", "DIV", "AND", "OR", "LIT", "VAR", "STORE", "LABEL", "PRINT", "SEQ", "PRINTLN", "ISEQ", "ISLT", "ISLE", "ISGT", "ISGE", "ISNE", "JUMP", "JUMPF", "JUMPT"};

Parser::Parser(Lexer& lexerx, ostream& outx): lexer(lexerx), out(outx), lindex(1), tindex(1), index(0){
  token = lexer.nextToken();
}

Parser::~Parser() {
}

void Parser::error(string message) {
  cerr << message << " Found " << token->getLexeme() << " at line " << token->getLine() << " position " << token->getPos() << endl;
  exit(1);
}

void Parser::check(int tokenType, string message) {
  if (token->getType() != tokenType)
    error(message);
}

Parser::TreeNode* Parser::factor() {
  TreeNode* myNode;
  int myType = token->getType();
  
  if(myType == Token::LPAREN){
    check(Token::LPAREN, "Expected (");
    token = lexer.nextToken();
  
    myNode = expression();
    
    check(Token::RPAREN, "Expected )");
    token = lexer.nextToken();
  } else if (myType == Token::INTLIT){
    check(Token::INTLIT, "Expect Integer Literal");
    myNode = new TreeNode(LIT, token->getLexeme());
    token = lexer.nextToken();
  } else if (myType == Token::IDENT){
    check(Token::IDENT, "Expect Identifier");
    myNode = new TreeNode(VAR, token->getLexeme());
    token = lexer.nextToken();
  } else {
    error("Expected ( or integer or identifier");
  }
  
  return myNode;
}

Parser::TreeNode* Parser::term() {
  TreeNode* t = factor();
  TreeNode* f; 
  int myType = token->getType();
  while(myType == Token::TIMES || myType == Token::DIVIDE){
    token = lexer.nextToken();
    f = factor();
    
    if(myType == Token::TIMES){
      myType = token->getType();
      t = new TreeNode(MULT, t, f);
    } else {
      myType = token->getType();
      t = new TreeNode(DIV, t, f);
    }
  }
  
  return t;
}

Parser::TreeNode* Parser::expression() {
  TreeNode* t = term();
  TreeNode* f;
  int myType = token->getType();
  while(myType == Token::PLUS ||
	myType == Token::MINUS){
    token = lexer.nextToken();
    f = term();
    
    if(myType == Token::PLUS){
      myType = token->getType();
      t = new TreeNode(ADD, t, f);
    }else{
      myType = token->getType();
      t = new TreeNode(SUB, t, f);
    }
  }
  
  return t;
}

Parser::TreeNode* Parser::relationalExpression() {
  TreeNode* exp = expression();
  int myType = token->getType();
  
  if(myType == Token::EQ){
    token = lexer.nextToken();
    TreeNode* exp2 = expression();
    exp = new TreeNode(ISEQ, exp, exp2);
    myType = token->getType();
  }
  else if(myType == Token::LT){
    token = lexer.nextToken();
    TreeNode* exp2 = expression();
    exp = new TreeNode(ISLT, exp, exp2);
    myType = token->getType();
  }
  else if(myType == Token::LE){
    token = lexer.nextToken();
    TreeNode* exp2 = expression();
    exp = new TreeNode(ISLE, exp, exp2);
    myType = token->getType();
  }
  else if(myType == Token::GT){
    token = lexer.nextToken();
    TreeNode* exp2 = expression();
    exp = new TreeNode(ISGT, exp, exp2);
    myType = token->getType();
  }
  else if(myType == Token::GE){
    token = lexer.nextToken();
    TreeNode* exp2 = expression();
    exp = new TreeNode(ISGE, exp, exp2);
    myType = token->getType();
  }
  else if(myType == Token::NE){
    token = lexer.nextToken();
    TreeNode* exp2 = expression();
    exp = new TreeNode(ISNE, exp, exp2);
    myType = token->getType();
  } else {
    error("Expected =,<,<=,>,>=,!=");
  }
  return exp;
}

Parser::TreeNode* Parser::logicalExpression() {
  TreeNode* r = relationalExpression();
  TreeNode* r2;
  int myType = token->getType();
  while(myType == Token::AND ||
	myType == Token::OR){
    token = lexer.nextToken();
    r2 = relationalExpression();
    
    if(myType == Token::AND){
      r = new TreeNode(AND, r, r2);
    } else {
      r = new TreeNode(OR, r, r2);
    }
      myType = token->getType();
  }
  
  return r;
}

Parser::TreeNode* Parser::setStatement() {
  check(Token::SET, "Expected Set");
  token = lexer.nextToken();

  check(Token::IDENT, "Expected Identifier");
  TreeNode* ident = new TreeNode(STORE, token->getLexeme());
  token = lexer.nextToken();

  check(Token::ASSIGN, "Expected =");
  token = lexer.nextToken();

  TreeNode* exp = expression();
  TreeNode* s = new TreeNode(SEQ, exp, ident);
 
  return s;
}

Parser::TreeNode* Parser::printStatement() {
  check(Token::PRINT, "Expected PRINT");
  token = lexer.nextToken();

  TreeNode* e = expression();
  TreeNode* s = new TreeNode(SEQ, e, new TreeNode(PRINT));
  int myType = token->getType();
  
  while(myType == Token::COMMA){
    token = lexer.nextToken();
    e = expression();
    s = new TreeNode(SEQ, s, new TreeNode(SEQ, e, new TreeNode(PRINT)));
    myType = token->getType();
  }

  return new TreeNode(SEQ, s, new TreeNode(PRINTLN));
}

Parser::TreeNode* Parser::whileStatement() {
  string lab1 = makeLabel();
  string lab2 = makeLabel();
  TreeNode* l1 = new TreeNode(LABEL, lab1);
  TreeNode* l2 = new TreeNode(LABEL, lab2);
  
  check(Token::WHILE, "Expected WHILE");
  token = lexer.nextToken();

  TreeNode* s = new TreeNode(SEQ, l1, logicalExpression());
  s = new TreeNode(SEQ, s, new TreeNode(JUMPF, lab2));
  
  check(Token::DO, "Expected DO");
  token = lexer.nextToken();

  s = new TreeNode(SEQ, s, compoundStatement());
  s = new TreeNode(SEQ, s, new TreeNode(JUMP, lab1));
  s = new TreeNode(SEQ, s, l2);
  
  check(Token::END, "Expected END");
  token = lexer.nextToken();
  return s;
}

Parser::TreeNode* Parser::ifStatement() {
  string lab1 = makeLabel();
  string lab2 = makeLabel();
  TreeNode* l1 = new TreeNode(LABEL, lab1);
  TreeNode* l2 = new TreeNode(LABEL, lab2);
  
  check(Token::IF, "Expected IF");
  token = lexer.nextToken();

  TreeNode* s = new TreeNode(SEQ, logicalExpression(), new TreeNode(JUMPF, lab1));

  check(Token::THEN, "Expected THEN");
  token = lexer.nextToken();

  s = new TreeNode(SEQ, s, compoundStatement());
  
  if(token->getType() == Token::ELSE){
    token = lexer.nextToken();
    s = new TreeNode(SEQ, s, new TreeNode(JUMP, lab2));
    s = new TreeNode(SEQ, s, l1);
    s = new TreeNode(SEQ, s, compoundStatement());
    s = new TreeNode(SEQ, s, l2);
  } else {
    s = new TreeNode(SEQ, s, l1);
  }

  check(Token::ENDIF, "Expected ENDIF");
  token = lexer.nextToken();

  return s;
}

Parser::TreeNode* Parser::statement() {
  int myType = token->getType();
  TreeNode* s;
  
  if(myType == Token::SET){
    s = setStatement();
  } else if(myType == Token::PRINT){
    s = printStatement();
  } else if(myType == Token::WHILE){
    s = whileStatement();
  } else if(myType == Token::IF){
    s = ifStatement();
  } else {
    error("Expected Statement");
  }
  
  return s;

  }

Parser::TreeNode* Parser::compoundStatement() {
  TreeNode* s = statement();
  int myType = token->getType();
  while(myType == Token::SET ||
	myType == Token::PRINT ||
	myType == Token::WHILE ||
	myType == Token::IF){
    TreeNode* t = statement();
    s = new TreeNode(SEQ, s, t);
    myType = token->getType();
  }
  
  return s;
}

Parser::TreeNode* Parser::program() {
  check(Token::PROGRAM, "Expected PROGRAM");
  token = lexer.nextToken();
  
  check(Token::IDENT, "Expected identifier");
  token = lexer.nextToken();
  
  TreeNode* myNode = compoundStatement();
  
  check(Token::END, "Expected END");
  token = lexer.nextToken();
  
  check(Token::PROGRAM, "Expected PROGRAM");
  token = lexer.nextToken();
  
  check(Token::EndOF, "Found additional tokens after program");

  //cout << "Program Accepted\n";
  return myNode;
}

//checks to see if a value is already in the variable array
bool Parser::valExists(string value){
  for(int i = 0; i <= index; i++){
    if(vars[i] == value){
      return true;
    }
  }
  return false;
}

void Parser::vardefs(TreeNode* node){
  if(node == NULL){
    return;
  }
  vardefs(node->leftChild);
  vardefs(node->rightChild);

  if(node->op == VAR || node->op == STORE){
     if(!valExists(node->val)){    
       vars[index] = node->val;
       index++;
     }
  }
  
}

void Parser::emit(string str){
  cout << str << endl;
}

void Parser::instr(Operation op, string val){
  switch(op){
  case VAR:
    cout << "push qword[" << val << "]" << endl;
    emit("");
    break;
  case LIT:
    cout << "mov rax," << val << endl;
    emit("push rax");
    emit("");
    break;
  case ADD:
    emit("pop rbx");
    emit("pop rax");
    emit("add rax,rbx");
    emit("push rax");
    emit("\n");
    break;
  case SUB:
    emit("pop rbx");
    emit("pop rax");
    emit("sub rax,rbx");
    emit("push rax");
    emit("");
    break;
  case AND:
    emit("pop rbx");
    emit("pop rax");
    emit("and rax,rbx");
    emit("push rax");
    emit("");
    break;
  case OR:
    emit("pop rbx");
    emit("pop rax");
    emit("or rax,rbx");
    emit("push rax");
    break;
  case MULT:
    emit("pop rbx");
    emit("pop rax");
    emit("imul rbx");
    emit("push rax");
    emit("");
    break;
  case DIV:
    emit("mov rdx,0");
    emit("pop rbx");
    emit("pop rax");
    emit("idiv rbx");
    emit("push rax");
    emit("");
    break;
  case STORE:
    cout << "pop qword[" << val << "]" << endl;
    emit("");
    break;
  case LABEL:
    cout << val << ":" << endl;
    emit("");
    break;
  case ISEQ: case ISNE: case ISLT: case ISLE: case ISGT: case ISGE:
    relational(op);
    emit("\n");
    break;
  case JUMP:
    cout << "jmp " << val << endl;
    emit("");
    break;
  case JUMPF:
    emit("pop rax");
    emit("cmp rax,0");
    cout << "je " << val << endl;
    emit("");
    break;
  case PRINT:
    emit("pop rax");
    emit("push rbp");
    emit("mov rdi,fmt");
    emit("mov rsi,rax");
    emit("mov rax,0");
    emit("call printf");
    emit("pop rbp");
    emit("mov rax,0");
    emit("");
    break;
  case PRINTLN:
    emit("push rbp");
    emit("mov rdi,endl");
    emit("mov rax,0");
    emit("call printf");
    emit("pop rbp");
    emit("mov rax,0");
    emit("");
    break;
  default:
    break;
  }
}

void Parser::relational(Operation op){
  string jmp1 = makeLabel();
  string jmp2 = makeLabel();
  
  emit("pop rbx");
  emit("pop rax");
  emit("cmp rax,rbx");
  switch(op){
  case ISEQ:
    cout << "je " << jmp1 << endl;
    break;
  case ISNE:
    cout << "jne " << jmp1 << endl;
    break;
  case ISLT:
    cout << "jl " << jmp1 << endl;
    break;
  case ISLE:
    cout << "jle " << jmp1 << endl;
    break;
  case ISGT:
    cout << "jg " << jmp1 << endl;
    break;
  case ISGE:
    cout << "jge " << jmp1 << endl;
    break;
  default:
    break;
  }
  emit("mov rax,0");
  cout << "jmp " << jmp2 << endl;
  cout << jmp1 << ":" << endl;
  emit("mov rax,1");
  cout << jmp2 << ":" << endl;
  emit("push rax");
}

void Parser::geninst(TreeNode* node){
  if(node == NULL){
    return;
  }
  geninst(node->leftChild);
  geninst(node->rightChild);
  instr(node->op, node->val);
}

void Parser::genasm(TreeNode* node){
  emit("   global main");
  emit("   extern printf");
  emit("   segment .bss");

  vardefs(node);
  for(int i = 0; i < index; i++){
    cout << vars[i] << " resq 1" << endl;
  }

  emit("   section .data ");
  emit("   fmt:    db '%ld ', 0");
  emit("   endl:    db 10, 0");
  emit("   section .text");
  emit("   main:");

  geninst(node);

  emit("ret");
}
