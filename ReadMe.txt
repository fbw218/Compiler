Compiler ReadMe

This project is a small complier written in C++. 
This program will parses a simple languate (sil) and all of the commands in a binary tree.
It takes this tree and translates the commands individually into machine language.
The machine language gets saved into a file and executed. 

  To Run:
  make
  silc 'ProgramName'.sil > 'ProgramName'.asm //writes out assembly code into .asm file
  a 'ProgramName'   //assemble .asm file into executable file
  'ProgramName'   //run executable --> ./'ProgramName' depending on your path
