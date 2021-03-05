#pragma once
#pragma warning(disable : 6384)

////////////////////////////////////////////////////////////////////////////////////////////
//         S E A L  P R O G R A M M I N G  L A N G U A G E (seal-lang.org)
//  
//  seal.ast.expression.h : 
//       @description :  This header file defines the expression ast tree
//       @birth       :  2021/3.5
//       @operator    :  Margoo
//

#include "seal.ast.node.h"
#include "seal.ast.conf.h"

#include "seal.lexical.h"

////////////////////////////////////////////////////////////////////////////////////////////
//   _ast_expression[typedef with seal_ast_exp] :
//          @description :   Expression analysis ast
//          @birth       :   2021/3.5
//          @operator    :   Margoo
typedef class _ast_expression {
private:
	// Basic root node
	seal_ast_exp_info root_node;
	// Expression string to be parsed
	std::string expr_string = "";

	// Ast status
	_ast_status_conf ast_status;

	seal_lexcial ast_lexical;

	// Add an error to the error report
	void _ADD_ERROR_INFO(error_list ADD_ERROR) {
		ast_status.error_cursor->error_string = ADD_ERROR.error_string;
		ast_status.error_cursor->next = new error_list;
        ast_status.error_cursor = ast_status.error_cursor->next;
	}
	void _ADD_ERROR_INFO(error_list* ADD_ERROR) {
		error_list _SINGLE_ERROR;
		
		for (int COUNT = 0; COUNT < sizeof(ADD_ERROR) / sizeof(ADD_ERROR[0]); ++COUNT) {
			_SINGLE_ERROR = *ADD_ERROR;

			ast_status.error_cursor->error_string = _SINGLE_ERROR.error_string;
			ast_status.error_cursor->next = new error_list;
			ast_status.error_cursor = ast_status.error_cursor->next;
		}
	}

public:
	bool __is_children = false;

	_ast_expression(std::string exp_string)
		: expr_string(exp_string) {
		// Set error pointer cursor
		ast_status.error_information = new error_list;
		ast_status.error_cursor = ast_status.error_information;

		// Set warning pointer cursor
		ast_status.warning_information = new warning_list;
		ast_status.warning_cursor = ast_status.warning_information;
	}

	void parse() {
		_lexical_info::seal_leixcal_token get_token = ast_lexical.get_token();

		// If it is an unknown token, it may be a variable or a function call
		if (get_token.cache_token == UNKNOW_TOKEN) {
			// Initialize lvalue
			root_node.left_value_tree = new seal_ast_exp_info;

			root_node.left_value_tree->value = get_token.token_string;

			root_node.left_value_tree->expression_type = "Variable Called";

			get_token = ast_lexical.get_token();

			// If it is determined to be a variable operation
			if (get_token.cache_token >= PLUS_SIGN_TOKEN &&
				get_token.cache_token <= MODULO_SIGN_TOKEN) {
				root_node.operator_symbol = get_token.token_string;

				// Create child ast tree
				std::string sub_expression = expr_string.substr(1, expr_string.size() - 2);

				_ast_expression* sub_tree = new _ast_expression(sub_expression);

				sub_tree->__is_children = true;

				sub_tree->parse();

				// If there is an error
				if (strcmp(sub_tree->ast_status.error_cursor->error_string, "$") == 0) {
					error_list* error_info = sub_tree->ast_status.error_information;

					_ADD_ERROR_INFO(error_info);
				}
				else {
					root_node.right_value_tree = sub_tree->root_node.left_value_tree;

				}
			}
			else if (get_token.cache_token == LEFT_BRACKETS) { // If it is a function call
				std::string sub_expression;

				se_int stratum = 0;

				for (std::string::iterator iterator = expr_string.begin();
					 iterator != expr_string.end(); ++iterator) {
					if (iterator.operator*() == '(') {
						++stratum;
					}

					if (iterator.operator*() == ')') {
						if (stratum == 1) {
							break;
						}
						else {
							--stratum;
						}
					}

					sub_expression.push_back(iterator.operator*());
				}

				// Create subtree resolution
				_ast_expression* sub_tree = new _ast_expression(sub_expression);
			}
		}
	}
} sael_ast_Exp;