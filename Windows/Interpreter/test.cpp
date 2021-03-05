#include "seal.ast.h"

int main() {
	_lexical_core core_test("int a = [8]{ \"abc\", 1, 2, 3}");

	while (!core_test.is_eof()) {
		seal_leixcal_type_info::seal_leixcal_token get_token = core_test.get_token();

		printf("%s ", get_token.token_string.c_str());
	}

	return 0;
}