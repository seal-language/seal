#pragma once

/////////////////////////////////////////////////////////////////////////////
//         S E A L  P R O G R A M M I N G  L A N G U A G E (seal-lang.org)
//  
//  seal.ast.h : 
//       @description :  Seal's AST syntax tree source code
//       @birth       :  2021/2.15
//       @operator    :  Margoo
//

#include "seal.ast.node.h"
#include "seal.ast.conf.h"

#include "seal.lexical.h"

///////////////////////////////////////////////////////////////
//   _ast_tree[typedef with seal_ast] :
//          @description :   AST grammar tree
//          @birth       :   2021/2.17
//          @operator    :   Margoo
typedef class _ast_tree {
private:
	seal_ast_sts ast_status;

	// Root node
	seal_ast_node* root_node = nullptr;

	///////////////////////////////////////////////////////////////
	//   _ast_core[typedef with seal_ast_core] :
	//          @description :   AST grammar tree
	//          @birth       :   2021/2.20
	//          @operator    :   Margoo
	typedef struct _ast_core {
		// Script code for spanning tree
		std::string script_code = "";

		// AST's lexical
		seal_lexcial ast_lexical;

		// Temporary token (to prevent too frequent memory read consumption)
		seal_leixcal_type_info::seal_leixcal_token temporary_token;
	};

	// Instantiate core
	_ast_core core;

public:
	_ast_tree(std::string script) {
		// Init core
		core.script_code = script;
		core.ast_lexical = seal_lexcial(script);

		ast_status.error_information = new error_list;
		ast_status.error_cursor = ast_status.error_information;

		ast_status.warning_information = new warning_list;
		ast_status.warning_cursor = ast_status.warning_information;
	}

	// Generate a syntax tree of one line of code
	void parse_only_one() {
		// Start to split the token
		while (!core.ast_lexical.is_eof()) {
			core.temporary_token = core.ast_lexical.get_token();

			switch (core.temporary_token.cache_token) {
			// Meaningless quantitative single line
			case CONST_NUMBER:
			case CONST_STRING:
				core.temporary_token = core.ast_lexical.get_token();

				if (core.temporary_token.cache_token != SEMICOLON_TOKEN) {
					ast_status.status_code = 0x01;
				}

				std::string error_string = std::string("Unexpected token \"") + core.temporary_token.token_string + "\"";

				ast_status.error_cursor->error_string = error_string.c_str();

				// These people deserve the time complexity of memory expansion for their bugs
				ast_status.error_cursor->next = new error_list;
				ast_status.error_cursor = ast_status.error_cursor->next;

				break;
			}
		}
	}
} seal_ast;