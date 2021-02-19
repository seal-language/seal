#pragma once

////////////////////////////////////////////////////////////////////////////////////////////
//         S E A L  P R O G R A M M I N G  L A N G U A G E (seal-lang.org)
//  
//  seal.ast.node.h : 
//       @description :  This header file defines the node class of the seal syntax tree
//       @birth       :  2021/2.17
//       @operator    :  Margoo
//

#include <string>
#include <vector>

////////////////////////////////////////////////////////////////////////////////////////////
//   _ast_node_assignment_expression[typedef with seal_ast_exp_info] :
//          @description :   This class stores information about assignment expression of AST nodes
//          @birth       :   2021/2.17
//          @operator    :   Margoo
typedef struct _ast_node_assignment_expression {
	////////////////////////////////////////////////////////////////////////////////////////////
	//   _ast_node_callee_expression[typedef with seal_callee_exp_info] (sub class) :
	//          @description :   This class stores information about function call
	//          @birth       :   2021/2.17
	//          @operator    :   Margoo
	typedef struct _ast_node_callee_expression {
		// Callee identifier (i.e. function name)
		std::string called_identifier = "";

		// Callee arguments expression
		std::vector<_ast_node_assignment_expression> calle_arguments;
	} sealcallee_exp_info;

	// Inverse Polish expression lvalue
	_ast_node_assignment_expression* left_value_tree  = nullptr;
	// Inverse Polish expression rvalue
	_ast_node_assignment_expression* right_value_tree = nullptr;

	// Median (i.e. operator)
	std::string operator_symbol = "";

	// Indicates the language type
	std::string expression_type = "";

	// If there is no need to store in a tree structure, then the corresponding value is
	std::string value = "";
} seal_ast_exp_info;

/////////////////////////////////////////////////////////////////////////////////////////////////////////
//   _ast_node_variable_declaration_info[typedef with seal_ast_var_info] :
//          @description :   This class stores information about variable declarations of AST nodes
//          @birth       :   2021/2.17
//          @operator    :   Margoo
typedef struct _ast_node_variable_declaration_info {
	// Equivalent to variable name
	std::string variable_declarator = "";

	// Variable assignment expression (reverse Polish median)
	_ast_node_assignment_expression init_expression;
} seal_ast_var_info;

/////////////////////////////////////////////////////////////////////////////////////////////////////////
//   _ast_node_function_declaration_info[typedef with seal_ast_fnc_info] :
//          @description :   This class stores information about function declarations of AST nodes
//          @birth       :   2021/2.17
//          @operator    :   Margoo
typedef struct _ast_node_function_declaration_info {
	// Equivalent to function name
	std::string function_declarator = "";

	// Function parameters (with vector)
	std::vector<_ast_node_variable_declaration_info> parameter;
} seal_ast_fnc_info;

///////////////////////////////////////////////////////////
//   _ast_node[typedef with seal_ast_node] :
//          @description :   Single AST syntax tree node
//          @birth       :   2021/2.17
//          @operator    :   Margoo
typedef struct _ast_node {
	///////////////////////////////////////////////////////////
	//   _ast_core[typedef with seal_ast_core] :
	//          @description :   Core of AST syntax tree node
	//          @birth       :   2021/2.17
	//          @operator    :   Margoo
	typedef struct _ast_core {
		// Sibling node
		_ast_node* next = nullptr;
		// Lower node
		_ast_node* lower_node = nullptr;
	} seal_ast_core;

	// AST content
	seal_ast_exp_info expression_information;
	seal_ast_fnc_info function_information;
	seal_ast_var_info variable_information;

	///////////////////////////////////////////////////////////////
	// ast_iden_code   :                                      -| (Obsolete)
	//          AST type identification code, through this     |
	//          unsgiend short to identify the node type       |
	// unsigned short ast_iden_code = 0x00;                   -|
	std::string ast_iden_code = ""; // <- New solution

	// AST node's core information
	_ast_core core;
} seal_ast_node;

