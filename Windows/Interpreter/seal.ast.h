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
#include "seal.lexical.h"

///////////////////////////////////////////////////////////////
//   _ast_tree[typedef with seal_ast] :
//          @description :   AST grammar tree
//          @birth       :   2021/2.17
//          @operator    :   Margoo
typedef class _ast_tree {
private:
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
	}

	// Generate a syntax tree of one line of code
	void parse_only_one() {
		// Start to split the token
		while (!core.ast_lexical.is_eof()) {
			core.temporary_token = core.ast_lexical.get_token();


		}
	}
} seal_ast;