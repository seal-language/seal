#include "seal.ast.expression.h"

int main() {
	_ast_expression expression("80 + c(80 + 70)");

	expression.parse();

	return 0;
}