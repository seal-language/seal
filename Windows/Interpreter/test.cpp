#include "seal.ast.expression.h"

int main() {
	_ast_expression expression("a + (18054 + 78042 - (c - d + (8 * 7)))");

	expression.parse();

	return 0;
}