#pragma once

////////////////////////////////////////////////////////////////////
//         S E A L  P R O G R A M M I N G  L A N G U A G E
//  
//  seal.info.h : 
//       @description :  This header folder defines the token required by leixcal
//       @birth       :  2021/2.15
//       @operator    :  Margoo
//

///////////////////////////////////////////////////////////////
//   se_large_int, se_int[Selective definition] :
//          @description :   [se_large_int] (Large integer type in seal) 
//							 [se_int] (Common integer types in the seal source code)
//          @birth       :   2021/2.15
//          @operator    :   Margoo
#ifdef _WIN64
#	define se_large_int __int64
#	define se_int int
#else
#	define se_large_int __int32
#	define se_int size_t
#endif