#pragma once

/////////////////////////////////////////////////////////////////////////////
//         S E A L  P R O G R A M M I N G  L A N G U A G E (seal-lang.org)
//  
//  seal.leixcal.h : 
//       @description :  This header file defines the token cutting in seal's lexical analyzer
//       @birth       :  2021/2.15
//       @operator    :  Margoo
//

#include "seal.lexical.token.h"
#include "seal.info.h"

#include <string>

///////////////////////////////////////////////////////////////
//   _lexical_info[typedef with seal_leixcal_type_info] :
//          @description :   Token splitter information
//          @birth       :   2021/2.15
//          @operator    :   Margoo
//
//   member :
//        leixcal_index [var]
//		  leixcal_line  [var]
//        leixcal_code  [var]
//        lexical_token [var]
//        
//        _lexical_token [struct]
typedef struct _lexical_info{
	// lexical traverses the subscripts of the source string
	se_int lexical_index = 0;
	// read the line relative to the code
	se_int lexical_line  = 0;

	// lexical temporarily stored script code to be interpreted
	std::string lexical_code;

	///////////////////////////////////////////////////////////////
	//   _leixcal_token[typedef with seal_leixcal_token] :
	//          @description :   Token type
	//          @birth       :   2021/2.15
	//          @operator    :   Margoo
	//
	typedef struct _lexical_token_type {
		// Temporary token
		_lexical_token cache_token = NO_STATUS_TOKEN;
		// Token's string
		std::string token_string = "";
	} seal_leixcal_token;

	_lexical_token_type lexical_token;

	// Is lexical eof
	bool leixcal_eof = false;
} seal_leixcal_type_info;

///////////////////////////////////////////////////////////////
//   _lexical_core[typedef with seal_leixcal] :
//          @description :   Token split core
//          @birth       :   2021/2.15
//          @operator    :   Margoo
typedef class _lexical_core {
private:
	// Define info class
	seal_leixcal_type_info core_info;

	// Is or not update line
	bool lexical_update_line = false;

public:
	//////////////////////////////////////////////////////////////////////////
	//    _leixcal_core[constructor]  :
	//           @description :   The default constructor of lexical core
	//           @birth       :   2021/2.15
	//           
	//           
	_lexical_core(std::string core_code = "") {
		core_info.lexical_code = core_code;
	}

	//////////////////////////////////////////////////////////////////////////
	//    get_token  :
	//           @description :   Cut a token from code_string
	//           @birth       :   2021/2.15
	//           
	_lexical_info::seal_leixcal_token get_token() {
		if (lexical_update_line == true) {
			++core_info.lexical_line;
		}

		// Init core_info
		core_info.lexical_token.cache_token = NO_STATUS_TOKEN;
		core_info.lexical_token.token_string = "";

		// If it's over
		if (!(core_info.lexical_index < core_info.lexical_code.size())) {
			// Set eof status
			core_info.leixcal_eof = true;

			return core_info.lexical_token;
		}

		// Traverse string
		for (; core_info.lexical_index < core_info.lexical_code.size(); ++core_info.lexical_index) {
			// Skip null characters
			if (core_info.lexical_code[core_info.lexical_index] == '\t' ||
				core_info.lexical_code[core_info.lexical_index] == ' ' ||
				core_info.lexical_code[core_info.lexical_index] == '\n') {
				++core_info.lexical_index;

				if (core_info.lexical_token.cache_token != NO_STATUS_TOKEN) {
					break;
				}

				// If there is a newline
				if (core_info.lexical_code[core_info.lexical_index - 1] == '\n') {
					if (core_info.lexical_token.cache_token != NO_STATUS_TOKEN) {
						lexical_update_line = true;
					}
					else {
						++lexical_update_line;

						continue;
					}
				}
			}

			// If is symbol
			if (_MACRO_IS_SYMBOL_(core_info.lexical_code[core_info.lexical_index])) {
				// If it is not UNKNOW_TOKEN, then encountering a symbol means the end of a token
				if (core_info.lexical_token.cache_token != NO_STATUS_TOKEN) {
					break;
				}

				// Determine mandatory characters
				if (core_info.lexical_code[core_info.lexical_index] == '=') {
					++core_info.lexical_index;

					core_info.lexical_token.token_string = "=";

					// If there is a continuous
					if (core_info.lexical_index < core_info.lexical_code.size() &&
						(core_info.lexical_code[core_info.lexical_index] == '=' ||
						 core_info.lexical_code[core_info.lexical_index] == '>' ||
						 core_info.lexical_code[core_info.lexical_index] == '<'
							)
						) {
						++core_info.lexical_index;
						// Set string variable
						core_info.lexical_token.token_string += core_info.lexical_code[core_info.lexical_code[core_info.lexical_index]];

						break;
					}
					else {
						break;
					}
				}
				if (core_info.lexical_code[core_info.lexical_index] == '<' ||
					core_info.lexical_code[core_info.lexical_index] == '>') {
					core_info.lexical_token.token_string = core_info.lexical_code[core_info.lexical_index];

					++core_info.lexical_index;

					// If there is a continuous
					if (core_info.lexical_index < core_info.lexical_code.size() &&
						(
							core_info.lexical_code[core_info.lexical_code[core_info.lexical_index]] == '=' ||
							(
								core_info.lexical_code[core_info.lexical_code[core_info.lexical_index]] == '-' &&
								core_info.lexical_token.token_string == "<"
								)
							)) {
						++core_info.lexical_index;

						core_info.lexical_token.token_string += core_info.lexical_code[core_info.lexical_code[core_info.lexical_index]];

						break;
					}
					else {
						break;
					}
				}

				// Judge a single character
				if ((core_info.lexical_code[core_info.lexical_index] == '{' ||
					core_info.lexical_code[core_info.lexical_index] == '[' ||
					core_info.lexical_code[core_info.lexical_index] == '(' ||
					core_info.lexical_code[core_info.lexical_index] == '}' ||
					core_info.lexical_code[core_info.lexical_index] == ']' ||
					core_info.lexical_code[core_info.lexical_index] == ')' ||
					core_info.lexical_code[core_info.lexical_index] == ',' ||
					core_info.lexical_code[core_info.lexical_index] == '%' ||
					core_info.lexical_code[core_info.lexical_index] == '~' ||
					core_info.lexical_code[core_info.lexical_index] == '&' ||
					core_info.lexical_code[core_info.lexical_index] == ':') &&
					core_info.lexical_token.token_string == "") {
					core_info.lexical_token.token_string = core_info.lexical_code[core_info.lexical_index];

					++core_info.lexical_index;

					break;
				}

				// Processing string
				if (core_info.lexical_code[core_info.lexical_index] == '\"' ||
					core_info.lexical_code[core_info.lexical_index] == '\'') {
					core_info.lexical_token.cache_token = CONST_STRING;

					char head_character = core_info.lexical_code[core_info.lexical_index];

					core_info.lexical_token.token_string = head_character;

					++core_info.lexical_index;

					for (; core_info.lexical_index < core_info.lexical_code.size(); ++core_info.lexical_index) {
						if (core_info.lexical_code[core_info.lexical_index] == '\\') {
							char insert_character = '\?';

							if (core_info.lexical_index + 1 < core_info.lexical_code.size()) {
								if (core_info.lexical_code[core_info.lexical_index + 1] == 'n') {
									insert_character = '\n';
								}
								if (core_info.lexical_code[core_info.lexical_index + 1] == 'r') {
									insert_character = '\n';
								}
								if (core_info.lexical_code[core_info.lexical_index + 1] == 'b') {
									insert_character = '\b';
								}
								if (core_info.lexical_code[core_info.lexical_index + 1] == 'a') {
									insert_character = '\a';
								}
								if (core_info.lexical_code[core_info.lexical_index + 1] == '\"') {
									insert_character = '\"';
								}
							}

							core_info.lexical_token.token_string += insert_character;
						}
						else {
							core_info.lexical_token.token_string += core_info.lexical_code[core_info.lexical_index];

							if (core_info.lexical_code[core_info.lexical_index] == head_character) {
								++core_info.lexical_index;

								break;
							}
						}
					}

					break;
				}

				if (core_info.lexical_code[core_info.lexical_index] == ';') {
					if (core_info.lexical_token.cache_token != NO_STATUS_TOKEN) {
						core_info.lexical_token.token_string = ";";

						++core_info.lexical_index;
					}

					break;
				}

				// Handling of binary and above operators
				if (core_info.lexical_code[core_info.lexical_index] == '-' ||
					core_info.lexical_code[core_info.lexical_index] == '+') {
					core_info.lexical_token.token_string = core_info.lexical_code[core_info.lexical_index];

					++core_info.lexical_index;

					if (core_info.lexical_code[core_info.lexical_index] == core_info.lexical_code[core_info.lexical_index - 1]) {
						core_info.lexical_token.token_string += core_info.lexical_code[core_info.lexical_index];

						++core_info.lexical_index;
					}
					if (core_info.lexical_code[core_info.lexical_index] == '=') {
						core_info.lexical_token.token_string += '=';

						++core_info.lexical_index;
					}

					break;
				}
				if (core_info.lexical_code[core_info.lexical_index] == '*') {
					core_info.lexical_token.token_string = core_info.lexical_code[core_info.lexical_index];

					++core_info.lexical_index;

					if (core_info.lexical_code[core_info.lexical_index] == '=') {
						core_info.lexical_token.token_string += '=';

						++core_info.lexical_index;
					}

					break;
				}
				if (core_info.lexical_code[core_info.lexical_index] == '^') {
					core_info.lexical_token.token_string = core_info.lexical_code[core_info.lexical_index];

					++core_info.lexical_index;

					if (core_info.lexical_code[core_info.lexical_index] == '=') {
						core_info.lexical_token.token_string += '=';

						++core_info.lexical_index;
					}

					break;
				}

				// Processing logical expression operators
				if (core_info.lexical_code[core_info.lexical_index] == '!') {
					core_info.lexical_token.token_string = core_info.lexical_code[core_info.lexical_index];

					++core_info.lexical_index;

					if (core_info.lexical_code[core_info.lexical_index] == '=') {
						core_info.lexical_token.token_string += '=';

						++core_info.lexical_index;
					}

					break;
				}
				if (core_info.lexical_code[core_info.lexical_index] == '|') {
					core_info.lexical_token.token_string = core_info.lexical_code[core_info.lexical_index];

					++core_info.lexical_index;

					if (core_info.lexical_code[core_info.lexical_index] == '|') {
						core_info.lexical_token.token_string += '&';

						++core_info.lexical_index;
					}
					if (core_info.lexical_code[core_info.lexical_index] == '=') {
						core_info.lexical_token.token_string += '=';

						++core_info.lexical_index;
					}

					break;
				}
				if (core_info.lexical_code[core_info.lexical_index] == '&') {
					core_info.lexical_token.token_string = core_info.lexical_code[core_info.lexical_index];

					++core_info.lexical_index;

					if (core_info.lexical_code[core_info.lexical_index] == '&') {
						core_info.lexical_token.token_string += '&';

						++core_info.lexical_index;
					}
					if (core_info.lexical_code[core_info.lexical_index] == '=') {
						core_info.lexical_token.token_string += '=';

						++core_info.lexical_index;
					}

					break;
				}
				
				// Processing comments
				if (core_info.lexical_code[core_info.lexical_index] == '/') {
					core_info.lexical_token.token_string = core_info.lexical_code[core_info.lexical_index];

					++core_info.lexical_index;
					// Is it a comment or an expression

					if (core_info.lexical_code[core_info.lexical_index] == '=') {
						core_info.lexical_token.token_string += '=';

						++core_info.lexical_index;
					}
					if (core_info.lexical_code[core_info.lexical_index] == '/') {
						core_info.lexical_token.token_string += '/';

						++core_info.lexical_index;
					}
					if (core_info.lexical_code[core_info.lexical_index] == '*') {
						core_info.lexical_token.token_string += '*';

						++core_info.lexical_index;
					}

					break;
				}

				core_info.lexical_token.cache_token = UNKNOW_SMYBOL;
			}
			else {
				// If it is not a token
				if (core_info.lexical_token.cache_token > COMMENT_TOKEN && core_info.lexical_token.cache_token <= BITWISE_OR_TOKEN) {
					break;
				}

				if (_MACRO_IS_NUMBER_(core_info.lexical_code[core_info.lexical_index]) == true && 
					core_info.lexical_token.cache_token != UNKNOW_TOKEN) {
					core_info.lexical_token.cache_token = CONST_NUMBER;
				}
				else {
					core_info.lexical_token.cache_token = UNKNOW_TOKEN;
				}
			}

			// Processing keyword token
			if (core_info.lexical_token.token_string == "int") {
				core_info.lexical_token.cache_token = INT_TOKEN;
			}
			else if (core_info.lexical_token.token_string == "bool") {
				core_info.lexical_token.cache_token = BOOL_TOKEN;
			}
			else if (core_info.lexical_token.token_string == "string") {
				core_info.lexical_token.cache_token = STRING_TOKEN;
			}
			else if (core_info.lexical_token.token_string == "char") {
				core_info.lexical_token.cache_token = CHAR_TOKEN;
			}
			else if (core_info.lexical_token.token_string == "float") {
				core_info.lexical_token.cache_token = FLOAT_TOKEN;
			}
			else if (core_info.lexical_token.token_string == "def") {
				core_info.lexical_token.cache_token = DEF_TOKEN;
			}
			else if (core_info.lexical_token.token_string == "if") {
				core_info.lexical_token.cache_token = IF_TOKEN;
			}
			else if (core_info.lexical_token.token_string == "else") {
				core_info.lexical_token.cache_token = ELSE_TOKEN;
			}
			else if (core_info.lexical_token.token_string == "elif") {
				core_info.lexical_token.cache_token = ELIF_TOKEN;
			}
			else if (core_info.lexical_token.token_string == "switch") {
				core_info.lexical_token.cache_token = SWITCH_TOKEN;
			}
			else if (core_info.lexical_token.token_string == "case") {
				core_info.lexical_token.cache_token = CASE_TOKEN;
			}
			else if (core_info.lexical_token.token_string == "default") {
				core_info.lexical_token.cache_token = DEFAULT_TOKEN;
			}
			else if (core_info.lexical_token.token_string == "while") {
				core_info.lexical_token.cache_token = WHILE_TOKEN;
			}
			else if (core_info.lexical_token.token_string == "continue") {
				core_info.lexical_token.cache_token = CONTINUE_TOKEN;
			}
			else if (core_info.lexical_token.token_string == "break") {
				core_info.lexical_token.cache_token = BREAK_TOKEN;
			}
			else if (core_info.lexical_token.token_string == "operator") {
				core_info.lexical_token.cache_token = OPERATOR_TOKEN;
			}
			else if (core_info.lexical_token.token_string == "class") {
				core_info.lexical_token.cache_token = CLASS_TOKEN;
			}
			else if (core_info.lexical_token.token_string == "public") {
				core_info.lexical_token.cache_token = PUBLIC_TOKEN;
			}
			else if (core_info.lexical_token.token_string == "private") {
				core_info.lexical_token.cache_token = PRIVATE_TOKEN;
			}
			else if (core_info.lexical_token.token_string == "return") {
				core_info.lexical_token.cache_token = RETURN_TOKEN;
			}
			else if (core_info.lexical_token.token_string == "enum") {
				core_info.lexical_token.cache_token = ENUM_TOKEN;
			}
			else if (core_info.lexical_token.token_string == "using") {
				core_info.lexical_token.cache_token = USING_TOKEN;
			}
			else if (core_info.lexical_token.token_string == "from") {
				core_info.lexical_token.cache_token = FROM_TOKEN;
			}
			else if (core_info.lexical_token.token_string == "NULL") {
				core_info.lexical_token.cache_token = NULL_TOKEN;
			}
			else if (core_info.lexical_token.token_string == "SAFENULL") {
				core_info.lexical_token.cache_token = SAFENULL_TOKEN;
			}
			else if (core_info.lexical_token.token_string == "delete") {
				core_info.lexical_token.cache_token = DELETE_TOKEN;
			}
			else if (core_info.lexical_token.token_string == "do") {
				core_info.lexical_token.cache_token = DO_TOKEN;
			}
			else if (core_info.lexical_token.token_string == "for") {
				core_info.lexical_token.cache_token = FOR_TOKEN;
			}
			else if (core_info.lexical_token.token_string == "parad") {
				core_info.lexical_token.cache_token = PARAD_TOKEN;
			}
			else if (core_info.lexical_token.token_string == "static") {
				core_info.lexical_token.cache_token = STATIC_TOKEN;
			}
			else if (core_info.lexical_token.token_string == "new_static") {
				core_info.lexical_token.cache_token = NEW_STATIC_TOKEN;
			}
			else if (core_info.lexical_token.token_string == "un_gc") {
				core_info.lexical_token.cache_token = UN_GC_TOKEN;
			}
			else if (core_info.lexical_token.token_string == "force_delete") {
				core_info.lexical_token.cache_token = FORCE_DELETE_TOKEN;
			}
			else if (core_info.lexical_token.token_string == "no_refer_count") {
				core_info.lexical_token.cache_token = NO_REFER_COUNT_TOKEN;
			}
			else if (core_info.lexical_token.token_string == "no_old") {
				core_info.lexical_token.cache_token = NO_OLD_TOKEN;
			}
			else if (core_info.lexical_token.token_string == "const") {
				core_info.lexical_token.cache_token = CONST_TOKEN;
			}
			else if (core_info.lexical_token.token_string == "seal_library_type") {
				core_info.lexical_token.cache_token = SEAL_LIBRARY_TYPE_TOKEN;
			}
			else if (core_info.lexical_token.token_string == "seal_library_function") {
				core_info.lexical_token.cache_token = SEAL_LIBRARY_FUNCTION_TOKEN;
			}
			else if (core_info.lexical_token.token_string == "seal_library_run_function") {
				core_info.lexical_token.cache_token = SEAL_LIBRARY_RUN_FUNCTION_TOKEN;
			}
			else if (core_info.lexical_token.token_string == "type_alias") {
				core_info.lexical_token.cache_token = TYPE_ALIAS_TOKEN;
			}
			else if (core_info.lexical_token.token_string == "tpye_define") {
				core_info.lexical_token.cache_token = TYPE_DEFINE_TOKEN;
			}
			else if (core_info.lexical_token.token_string == "function") {
				core_info.lexical_token.cache_token = FUNCTION_TOKEN;
			}
			else if (core_info.lexical_token.token_string == "retr_for") {
				core_info.lexical_token.cache_token = RETR_FOR_TOKEN;
			}
			else if (core_info.lexical_token.token_string == "namespace") {
				core_info.lexical_token.cache_token = NAMESPACE_TOKEN;
			}

			// Processing token
			if (core_info.lexical_token.token_string == "(") {
				core_info.lexical_token.cache_token = LEFT_BRACKETS;
			}
			else if (core_info.lexical_token.token_string == ")") {
				core_info.lexical_token.cache_token = RIGHT_BRACKETS;
			}
			else if (core_info.lexical_token.token_string == "[") {
				core_info.lexical_token.cache_token = MIDDLE_LEFT_BRACKETS;
			}
			else if (core_info.lexical_token.token_string == "]") {
				core_info.lexical_token.cache_token = MIDDLE_RIGHT_BRACKETS;
			}
			else if (core_info.lexical_token.token_string == "{") {
				core_info.lexical_token.cache_token = BIG_LEFT_BRACKETS;
			}
			else if (core_info.lexical_token.token_string == "}") {
				core_info.lexical_token.cache_token = BIG_RIGHT_BRACKETS;
			}
			else if (core_info.lexical_token.token_string == ";") {
				core_info.lexical_token.cache_token = SEMICOLON_TOKEN;
			}
			else if (core_info.lexical_token.token_string == ".") {
				core_info.lexical_token.cache_token = DOT_TOKEN;
			}
			else if (core_info.lexical_token.token_string == ",") {
				core_info.lexical_token.cache_token = COMMA_TOKEN;
			}
			else if (core_info.lexical_token.token_string == ":") {
				core_info.lexical_token.cache_token = COLON_TOKEN;
			}
			else if (core_info.lexical_token.token_string == "+") {
				core_info.lexical_token.cache_token = PLUS_SIGN_TOKEN;
			}
			else if (core_info.lexical_token.token_string == "-") {
				core_info.lexical_token.cache_token = MINUS_SIGN_TOKEN;
			}
			else if (core_info.lexical_token.token_string == "*") {
				core_info.lexical_token.cache_token = MULTIPLICATION_SIGN_TOKEN;
			}
			else if (core_info.lexical_token.token_string == "/") {
				core_info.lexical_token.cache_token = SLASH_TOKEN;
			}
			else if (core_info.lexical_token.token_string == "%") {
				core_info.lexical_token.cache_token = MODULO_SIGN_TOKEN;
			}
			else if (core_info.lexical_token.token_string == "++") {
				core_info.lexical_token.cache_token = DOUBLE_PLUS_TOKEN;
			}
			else if (core_info.lexical_token.token_string == "--") {
				core_info.lexical_token.cache_token = DOUBLE_MINUS_TOKEN;
			}
			else if (core_info.lexical_token.token_string == "_") {
				core_info.lexical_token.cache_token = UNDERSCORE_TOKEN;
			}
			else if (core_info.lexical_token.token_string == "=") {
				core_info.lexical_token.cache_token = EQUAL_SIGN_TOKEN;
			}
			else if (core_info.lexical_token.token_string == "==") {
				core_info.lexical_token.cache_token = DOUBLE_EQUAL_SIGN_TOKEN;
			}
			else if (core_info.lexical_token.token_string == "!") {
				core_info.lexical_token.cache_token = EXCLAMATION_MARK;
			}
			else if (core_info.lexical_token.token_string == "!=") {
				core_info.lexical_token.cache_token = NOT_EQUAL_TOKEN;
			}
			else if (core_info.lexical_token.token_string == ">") {
				core_info.lexical_token.cache_token = MORE_THAN_TOKEN;
			}
			else if (core_info.lexical_token.token_string == "<") {
				core_info.lexical_token.cache_token = LESS_THAN_TOKEN;
			}
			else if (core_info.lexical_token.token_string == ">=") {
				core_info.lexical_token.cache_token = MORE_THAN_EQUAL_TOKEN;
			}
			else if (core_info.lexical_token.token_string == "<=") {
				core_info.lexical_token.cache_token = LESS_THAN_EQUAL_TOKEN;
			}
			else if (core_info.lexical_token.token_string == "&") {
				core_info.lexical_token.cache_token = VERSUS_TOKEN;
			}
			else if (core_info.lexical_token.token_string == "&&") {
				core_info.lexical_token.cache_token = LOGICALAND_TOKEN;
			}
			else if (core_info.lexical_token.token_string == "|") {
				core_info.lexical_token.cache_token = VERTICAL_BAR_TOKEN;
			}
			else if (core_info.lexical_token.token_string == "||") {
				core_info.lexical_token.cache_token = LOGICAL_OR_TOKEN;
			}
			else if (core_info.lexical_token.token_string == "^") {
				core_info.lexical_token.cache_token = CARET_TOKEN;
			}
			else if (core_info.lexical_token.token_string == "~") {
				core_info.lexical_token.cache_token = TILDE_TOKEN;
			}
			else if (core_info.lexical_token.token_string == "<<") {
				core_info.lexical_token.cache_token = BR_LEFT_SHIFT_OPERATOR_TOKEN;
			}
			else if (core_info.lexical_token.token_string == ">>") {
				core_info.lexical_token.cache_token = BR_RIGHT_SHIFT_OPERATOR_TOKEN;
			}
			else if (core_info.lexical_token.token_string == "+=") {
				core_info.lexical_token.cache_token = ADDITIONAND_TOKEN;
			}
			else if (core_info.lexical_token.token_string == "-=") {
				core_info.lexical_token.cache_token = MINUSITIONAND_TOKEN;
			}
			else if (core_info.lexical_token.token_string == "*=") {
				core_info.lexical_token.cache_token = MULTIPLYAND_TOKEN;
			}
			else if (core_info.lexical_token.token_string == "/=") {
				core_info.lexical_token.cache_token = EXCEPTAND_TOKEN;
			}
			else if (core_info.lexical_token.token_string == "&=") {
				core_info.lexical_token.cache_token = BITWISEAND_TOKEN;
			}
			else if (core_info.lexical_token.token_string == "<-") {
				core_info.lexical_token.cache_token = INHERITANCE_TOKEN;
			}
			else if (core_info.lexical_token.token_string == "^=") {
				core_info.lexical_token.cache_token = BITWISEANDDIFFERENT_TOKEN;
			}
			else if (core_info.lexical_token.token_string == "|=") {
				core_info.lexical_token.cache_token = BITWISE_OR_TOKEN;
			}

			core_info.lexical_token.token_string += core_info.lexical_code[core_info.lexical_index];
		}

		return core_info.lexical_token;
	}

	//////////////////////////////////////////////////////////////////////////
	//    is_eof  :
	//           @description :   Whether it ends, if end, return true
	//           @birth       :   2021/2.16
	//           
	bool is_eof() {
		return core_info.leixcal_eof;
	}
} seal_lexcial;
