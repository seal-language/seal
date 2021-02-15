#pragma once

////////////////////////////////////////////////////////////////////
//         S E A L  P R O G R A M M I N G  L A N G U A G E
//  
//  seal.leixcal.h : 
//       @description :  This header file defines the token cutting in seal's lexical analyzer
//       @birth       :  2021/2.15
//       @operator    :  Margoo
//

#include "seal.leixcal.token.h"
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
	typedef struct _leixcal_token_type {
		// Temporary token
		_leixcal_token cache_token = NO_STATUS_TOKEN;
		// Token's string
		std::string token_string = "";
	} seal_leixcal_token;

	_leixcal_token_type lexical_token;
} seal_leixcal_type_info;

///////////////////////////////////////////////////////////////
//   _leixcal[typedef with seal_leixcal] :
//          @description :   Token split core
//          @birth       :   2021/2.15
//          @operator    :   Margoo
typedef class _leixcal_core {
private:
	// Define info class
	seal_leixcal_type_info core_info;

} seal_leixcal;