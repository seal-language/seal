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
	//           
	_lexical_info::seal_leixcal_token get_token() {
		if (lexical_update_line == true) {
			++core_info.lexical_line;
		}

		// Init core_info
		core_info.lexical_token.cache_token  = NO_STATUS_TOKEN;
		core_info.lexical_token.token_string = "";

		// Traverse string
		for (; core_info.lexical_index < core_info.lexical_code.size(); ++core_info.lexical_index) {
			// Skip null characters
			if (core_info.lexical_code[core_info.lexical_index] == '\t' ||
				core_info.lexical_code[core_info.lexical_index] == ' ' ||
				core_info.lexical_code[core_info.lexical_index] == '\n') {
				++core_info.lexical_index;

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
				// Determine mandatory characters
				if (core_info.lexical_code[core_info.lexical_index] == '=') {
					++core_info.lexical_index;

					core_info.lexical_token.token_string = "=";

					// If there is a continuous
					if (core_info.lexical_index < core_info.lexical_code.size() &&
						(core_info.lexical_code[core_info.lexical_code[core_info.lexical_index]] == '=' ||
						 core_info.lexical_code[core_info.lexical_code[core_info.lexical_index]] == '>' ||
						 core_info.lexical_code[core_info.lexical_code[core_info.lexical_index]] == '<'
						)
						) {
						// Set string variable
						core_info.lexical_token.token_string += core_info.lexical_code[core_info.lexical_code[core_info.lexical_index]];

						break;
					}
					else {
						--core_info.lexical_index;

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
						core_info.lexical_token.token_string += core_info.lexical_code[core_info.lexical_code[core_info.lexical_index]];

						break;
					}
					else {
						--core_info.lexical_index;

						break;
					}
				}

				// Judge a single character
				if ((core_info.lexical_code[core_info.lexical_index] == '{' ||
					core_info.lexical_code[core_info.lexical_index] == '['  ||
					core_info.lexical_code[core_info.lexical_index] == '('  ||
					core_info.lexical_code[core_info.lexical_index] == '}'  ||
					core_info.lexical_code[core_info.lexical_index] == ']'  ||
					core_info.lexical_code[core_info.lexical_index] == ')')  &&
					core_info.lexical_token.token_string == "") {
					core_info.lexical_token.token_string = core_info.lexical_code[core_info.lexical_index];

					break;
				}

				// Processing string
				if (core_info.lexical_code[core_info.lexical_index] == '\"' ||
					core_info.lexical_code[core_info.lexical_index] == '\'') {
					core_info.lexical_token.cache_token = CONST_STRING;

					char head_character = core_info.lexical_code[core_info.lexical_index];

					for (; core_info.lexical_index < core_info.lexical_code.size(); ++core_info.lexical_index) {
						if (core_info.lexical_code[core_info.lexical_index] == '\\') {

						}

						if (core_info.lexical_code[core_info.lexical_index] == head_character) {
							break;
						}

						core_info.lexical_token.token_string += core_info.lexical_code[core_info.lexical_index];
					}
				}
			}
		}
	}
} seal_leixcal;