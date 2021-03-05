#pragma once

////////////////////////////////////////////////////////////////////////////////////////////
//         S E A L  P R O G R A M M I N G  L A N G U A G E (seal-lang.org)
//  
//  seal.ast.conf.h : 
//       @description :  This header file define some config information of the ast syntax tree
//       @birth       :  2021/2.26
//       @operator    :  Margoo
//

#include "seal.info.h"

// Error and warning statement linked list
struct error_list {
	const char* error_string = "$";

	error_list* next = nullptr;
};

struct warning_list {
	const char* warning_string = "$";

	warning_list* next = nullptr;
};

////////////////////////////////////////////////////////////////////////////////////////////
//   _ast_status_conf[typedef with seal_ast_sts] :
//          @description :   This class stores information about assignment expression of AST nodes
//          @birth       :   2021/2.26
//          @operator    :   Margoo
typedef struct _ast_status_conf {
	/////////////////////////////////////////////////////////////
	//  status_code :
	//    @description  :  AST syntax status code
	//    @example      :  0x00 : no status
	//                     0x01 : error status
	//                     0x02 : anthor status (like warning)
	short status_code = 0x00;

	// Error and warning information list
	error_list*   error_information   = nullptr;
	warning_list* warning_information = nullptr;

	error_list*   error_cursor   = nullptr;
	warning_list* warning_cursor = nullptr;
} seal_ast_sts;
