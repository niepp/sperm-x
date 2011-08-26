#include "StdAfx.h"
#include "GloablFun.h"

CString GetMonth_From_CString(const CString &str)
{
	return (isdigit(str[6]) ? str.Mid(5,2) : str.Mid(5,1));
}

CString GetDay_From_CString(const CString &str) 
{
	CString cs;
	if(isdigit(str[6])) {
		if(isdigit(str[9]))
			cs = str.Mid(8,2);
		else
			cs = str.Mid(8,1);
	}
	else {
		if(isdigit(str[8]))
			cs = str.Mid(7,2);
		else
			cs = str.Mid(7,1);
	}
	return cs;
}

bool CheckRegister(const CString& str )
{
	int sum= 0;
	int i;
	if(str.GetLength()!=CODE_LEN) return false;
	for (i=0;i<CODE_LEN;i++)
	{
		int aa = str[i];
		if((aa>='0'&&aa<='9')||(aa>='A'&&aa<='Z'))
		{
			sum+=aa;
		}
		else
		{
			return false;
		}
	}
	if(sum%32==0)
		return true;
	return false;
}
