#pragma once

template<typename T>
void Safe_Delete(T& Temp)
{
	if (Temp)
	{
		delete Temp;
		Temp = nullptr;
	}
}

template<typename T>
void Safe_Delete_Array(T& Temp)
{
	if (Temp)
	{
		delete[] Temp;
		Temp = nullptr;
	}
}


template<typename T>
unsigned long Safe_AddRef(T& Temp)
{
	unsigned long		dwRefCnt = 0;

	if (Temp)
		dwRefCnt = Temp->AddRef();

	return dwRefCnt;
}


template<typename T>
unsigned long Safe_Release(T& Temp)
{
	unsigned long		dwRefCnt = 0;

	if (Temp)
	{
		dwRefCnt = Temp->Release();

		if (0 == dwRefCnt)
			Temp = nullptr;
	}

	return dwRefCnt;
}

class CTagFinder
{
public:
	CTagFinder(const wchar_t* pString)
		: m_pString(pString) {}

public:
	template<typename T>
	bool		operator()(T& Pair)
	{
		if (!lstrcmp(m_pString, Pair.first))
			return true;

		return false;
	}

private:
	const	wchar_t*		m_pString;
};

static POINT		Get_Mouse(HWND hWnd)
{
	POINT	pt;
	GetCursorPos(&pt);
	ScreenToClient(hWnd, &pt);

	return pt;
}