/* Start Header -------------------------------------------------------------
--
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: memfun.adapter.h
Purpose: member function adapter using template
Language: c++
Platform: Visual Studio 15.8.1 w/ C++17
Project: c.hwang_CS225_lab4
Author: c.hwang, Hwang Chan IL
Creation date: 10/29/2018
- End Header ----------------------------------------------------------------
*/
#ifndef MEMFUN_ADAPTER_H
#define MEMFUN_ADAPTER_H
template <typename RetType, typename ClassType, typename ArgType>
class MFA_1arg {
		RetType (ClassType::*pfunc) (ArgType);
	public:
		MFA_1arg( RetType (ClassType::*_pointer) (ArgType) ) : pfunc(_pointer) {}
		RetType operator() ( ClassType& _class, ArgType& _arg ) const {
			return (_class.*pfunc)(_arg); //call
		}
};

//helper functions
//for non-const member function
template <typename RetType, typename ClassType, typename ArgType>
MFA_1arg<RetType,ClassType,ArgType> mfp_1arg(RetType (ClassType::*pointer) (ArgType) ) { 
	return MFA_1arg<RetType, ClassType, ArgType>(pointer);
}
#endif
