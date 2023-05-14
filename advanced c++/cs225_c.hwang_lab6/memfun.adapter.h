#ifndef MEMFUNADAPTER_H
#define MEMFUNADAPTER_H

template<typename ReturnType, typename ClassType, typename ArgType>
class MFA_1arg {
private:
	ReturnType (ClassType::*pfunc) (ArgType);
public:
	typedef ReturnType result_type;
	typedef ClassType first_argument_type;
	typedef ArgType const& second_argument_type;

	MFA_1arg() = default;
	MFA_1arg(ReturnType (ClassType::*_pfunc) (ArgType)) : pfunc(_pfunc) {}
	ReturnType operator()(ClassType& _class, const ArgType _arg) const {
		return (_class.*pfunc)(_arg);
	}
};

template<typename ReturnType, typename ClassType, typename ArgType>
class MFA_1arg_const {
private:
	ReturnType(ClassType::*pfunc) (ArgType) const;
public:
	typedef ReturnType result_type;
	typedef ClassType first_argument_type;
	typedef ArgType second_argument_type;

	MFA_1arg_const() = default;
	MFA_1arg_const(ReturnType(ClassType::*_pfunc)(ArgType) const) : pfunc(_pfunc) {}
	ReturnType operator()(ClassType _class, const ArgType _arg) const {
		return (_class.*pfunc)(_arg);
	}
};

template<typename ReturnType, typename ClassType, typename ArgType>
MFA_1arg<ReturnType, ClassType, ArgType> mfp_1arg(ReturnType (ClassType::*pfunc)(ArgType)) {
	return MFA_1arg<ReturnType, ClassType, ArgType>(pfunc);
}

template<typename ReturnType, typename ClassType, typename ArgType>
MFA_1arg_const<ReturnType, ClassType, ArgType> mfp_1arg(ReturnType(ClassType::*pfunc)(ArgType) const) {
	return MFA_1arg_const<ReturnType, ClassType, ArgType>(pfunc);
}
#endif