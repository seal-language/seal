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
	_ast_expression(std::string exp_string)
		: expr_string(exp_string) {
		// Set error pointer cursor
		ast_status.error_information = new error_list;
		ast_status.error_cursor = ast_status.error_information;

		// Set warning pointer cursor
		ast_status.warning_information = new warning_list;
		ast_status.warning_cursor = ast_status.warning_information;

		ast_lexical = seal_lexcial(exp_string);
	}

	// Parse AST tree
	void parse() {
		begin:

		_lexical_info::seal_leixcal_token get_token = ast_lexical.get_token();

		// If it is an unknown token, it may be a variable or a function call
		if (get_token.cache_token == UNKNOW_TOKEN) {
			if (root_node.expression_type != "Bracket Operation Priority 1") {
				root_node.expression_type = "Variable Called";
			}

			// Initialize lvalue
			root_node.left_value_tree = new seal_ast_exp_info;

			root_node.left_value_tree->value = get_token.token_string;

			root_node.left_value_tree->expression_type = "Variable Called";

			get_token = ast_lexical.get_token();

			// If it is determined to be a variable operation
			if (get_token.cache_token >= PLUS_SIGN_TOKEN &&
				get_token.cache_token <= MODULO_SIGN_TOKEN) {
				if (root_node.expression_type != "Bracket Operation Priority 1") {
					root_node.expression_type = "Binary Expression";
				}

				root_node.operator_symbol = get_token.token_string;

				// Create child ast tree
				std::string sub_expression;

				while (!ast_lexical.is_eof()) {
					get_token = ast_lexical.get_token();

					sub_expression += get_token.token_string;
				}

				_ast_expression* sub_tree = new _ast_expression(sub_expression);

				sub_tree->parse();

				// If there is an error
				if (strcmp(sub_tree->ast_status.error_cursor->error_string, "$") != 0) {
					error_list* error_info = sub_tree->ast_status.error_information;

					_ADD_ERROR_INFO(error_info);
				}
				else {
					if (sub_tree->root_node.right_value_tree != nullptr) {
						root_node.right_value_tree->left_value_tree = sub_tree->root_node.left_value_tree;
						root_node.right_value_tree->right_value_tree = sub_tree->root_node.right_value_tree;
					}
					else {
						root_node.right_value_tree = sub_tree->root_node.left_value_tree;
					}

					root_node.right_value_tree->operator_symbol = sub_tree->root_node.operator_symbol;
					root_node.right_value_tree->expression_type = sub_tree->root_node.expression_type;

					root_node.right_value_tree->function_callee = sub_tree->root_node.function_callee;
				}
			}
			if (get_token.cache_token == LEFT_BRACKETS) { // If it is a function call
				root_node.expression_type = "Function Called";

				std::string sub_expression;

				se_int stratum = 0;

				// Create subtree resolution
				_ast_expression* sub_tree;

				bool have_terminator = false;

				while (!ast_lexical.is_eof()) {
					get_token = ast_lexical.get_token();

					if (get_token.token_string == ",") {
						sub_tree = new _ast_expression(sub_expression);

						sub_tree->parse();

						if (strcmp(sub_tree->ast_status.error_cursor->error_string, "$") != 0) {
							error_list* error_info = sub_tree->ast_status.error_information;

							_ADD_ERROR_INFO(error_info);
						}
						else {
							root_node.function_callee.calle_arguments.push_back(std::move(&sub_tree->root_node));

							sub_expression.clear();
						}
					}

					if (get_token.token_string == "(") {
						++stratum;
					}

					if (get_token.token_string == ")") {
						if (stratum == 0) {
							have_terminator = true;

							break;
						}
						else {
							--stratum;
						}
					}
					else {
						sub_expression += get_token.token_string;
					}
				}

				if (have_terminator == false) {
					ast_status.error_cursor->error_string = "The terminator \')\' not found";

					ast_status.error_cursor->next = new error_list;
					ast_status.error_cursor = ast_status.error_cursor->next;
				}

				if (sub_expression.empty() == false) {
					sub_tree = new _ast_expression(sub_expression);

					sub_tree->parse();

					if (strcmp(sub_tree->ast_status.error_cursor->error_string, "$") != 0) {
						error_list* error_info = sub_tree->ast_status.error_information;

						_ADD_ERROR_INFO(error_info);
					}
					else {
						root_node.function_callee.calle_arguments.push_back(std::move(&sub_tree->root_node));

						sub_expression.clear();
					}
				}

				root_node.function_callee.called_identifier = root_node.left_value_tree->value;
			}
			if (get_token.cache_token == MIDDLE_LEFT_BRACKETS) { // Array called
				root_node.left_value_tree->expression_type = "Array Called";

				root_node.left_value_tree->array_expr = new _ast_node_assignment_expression;

				std::string array_called_expression;

				se_int bracket_level = 0;

				bool have_terminator = false;

				while (!ast_lexical.is_eof()) {
					get_token = ast_lexical.get_token();

					if (get_token.cache_token == MIDDLE_RIGHT_BRACKETS) {
						if (bracket_level == 0) {
							have_terminator = true;

							break;
						}
						else {
							--bracket_level;
						}
					}

					if (get_token.cache_token == MIDDLE_LEFT_BRACKETS) {
						++bracket_level;
					}

					array_called_expression += get_token.token_string;
				}

				if (have_terminator == false) {
					ast_status.error_cursor->error_string = "The terminator '\]\' not found";

					ast_status.error_cursor->next = new error_list;
					ast_status.error_cursor = ast_status.error_cursor->next;
				}

				_ast_expression* sub_tree = new _ast_expression(array_called_expression);

				sub_tree->parse();

				if (strcmp(sub_tree->ast_status.error_cursor->error_string, "$") != 0) {
					error_list* error_info = sub_tree->ast_status.error_information;

					_ADD_ERROR_INFO(error_info);
				}
				else {
					if (sub_tree->root_node.right_value_tree != nullptr) {
						root_node.left_value_tree->array_expr->left_value_tree = sub_tree->root_node.left_value_tree;
						root_node.left_value_tree->array_expr->right_value_tree = sub_tree->root_node.right_value_tree;
					}
					else {
						root_node.left_value_tree->array_expr->right_value_tree = sub_tree->root_node.left_value_tree;
					}

					root_node.right_value_tree->operator_symbol = sub_tree->root_node.operator_symbol;
					root_node.right_value_tree->expression_type = sub_tree->root_node.expression_type;

					root_node.right_value_tree->function_callee = sub_tree->root_node.function_callee;
				}
			}
			else {
				std::string error_information("Unexpected identifier");
				error_information += get_token.token_string;

				ast_status.error_cursor->error_string = error_information.c_str();

				ast_status.error_cursor->next = new error_list;
				ast_status.error_cursor = ast_status.error_cursor->next;
			}
		}
		if (get_token.cache_token == LEFT_BRACKETS) { // If it is a parenthesis precedence operation
			root_node.expression_type = "Bracket Operation Priority 1";

			goto begin;
		}
		if (get_token.cache_token == CONST_NUMBER) {
			if (root_node.expression_type != "Bracket Operation Priority 1") {
				root_node.expression_type = "Const Number";
			}

			// Initialize lvalue
			root_node.left_value_tree = new seal_ast_exp_info;

			root_node.left_value_tree->value = get_token.token_string;

			root_node.left_value_tree->expression_type = "Text";

			get_token = ast_lexical.get_token();

			// If it is determined to be a variable operation
			if (get_token.cache_token >= PLUS_SIGN_TOKEN &&
				get_token.cache_token <= MODULO_SIGN_TOKEN) {
				if (root_node.expression_type != "Bracket Operation Priority 1") {
					root_node.expression_type = "Binary Expression";
				}

				root_node.operator_symbol = get_token.token_string;

				// Create child ast tree
				std::string sub_expression;

				while (!ast_lexical.is_eof()) {
					get_token = ast_lexical.get_token();

					sub_expression += get_token.token_string;
				}

				_ast_expression* sub_tree = new _ast_expression(sub_expression);

				sub_tree->parse();

				// If there is an error
				if (strcmp(sub_tree->ast_status.error_cursor->error_string, "$") != 0) {
					error_list* error_info = sub_tree->ast_status.error_information;

					_ADD_ERROR_INFO(error_info);
				}
				else {
					root_node.right_value_tree = new _ast_node_assignment_expression;

					if (sub_tree->root_node.right_value_tree != nullptr) {
						root_node.right_value_tree->left_value_tree = sub_tree->root_node.left_value_tree;
						root_node.right_value_tree->right_value_tree = sub_tree->root_node.right_value_tree;
					}
					else {
						root_node.right_value_tree = sub_tree->root_node.left_value_tree;
					}

					root_node.right_value_tree->operator_symbol = sub_tree->root_node.operator_symbol;
					root_node.right_value_tree->expression_type = sub_tree->root_node.expression_type;

					root_node.right_value_tree->function_callee = sub_tree->root_node.function_callee;
				}
			}
		}
	}
} sael_ast_Exp;
