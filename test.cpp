#include "parser.h"

/**
 * @brief Test
 * 
 */

 int main(){
  Parser parser;
  parser.BinopPrecedence['<'] = 10;
  parser.BinopPrecedence['+'] = 20;
  parser.BinopPrecedence['-'] = 20;
  parser.BinopPrecedence['*'] = 40;

  // Prime the first token.
  fprintf(stderr, "ready> \n");
  parser.GetNextToken();

  // Run the main "interpreter loop" now.
  parser.MainLoop();

  return 0;
 }