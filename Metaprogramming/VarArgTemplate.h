#pragma once


void ProcessValues()
{

}

template <class T, class... Tn>
void ProcessValues(T Arg1, Tn... Args)
{
	HandleValue(Arg1);
	ProcessValues(Args...);
}

/*
template <class... Tn>
void ProcessValuesF(Tn... Args)
{
	(HandleValue(Args), ...);
}*/

void VarArgTemplateTest()
{
	ProcessValues((int)10, 2.3f, "Str1");
	//ProcessValuesF((int)10, 2.3f, "Str1");
}