#include "seal.ast.expression.h"

int main() {
	_ast_expression expression("!(b+780+(804848484))");

	expression.parse();

	return 0;
}