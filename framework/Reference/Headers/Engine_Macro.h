#pragma once

#define D3DCOLOR_ARGB(a,r,g,b) \
    ((D3DCOLOR)((((a)&0xff)<<24)|(((r)&0xff)<<16)|(((g)&0xff)<<8)|((b)&0xff)))

#define	MSG_BOX(_message) MessageBox(nullptr, _message, L"System Message", MB_OK)

#define GET_INSTANCE(CLASSNAME)	[](){											\
	CLASSNAME*	pInstance = CLASSNAME::GetInstance();							\
	if(nullptr == pInstance) {													\
	char	szMessage[MAX_PATH] = "";											\
	strcpy_s(szMessage, typeid(CLASSNAME).name());								\
	strcat_s(szMessage, "is nullptr");											\
	MessageBoxA(0, szMessage, nullptr, MB_OK);}									\
	else {																		\
	pInstance->AddRef();}														\
	return pInstance;															\
	}();

#define RELEASE_INSTANCE(CLASSNAME)	[](){										\
	CLASSNAME*	pInstance = CLASSNAME::GetInstance();							\
	if(nullptr == pInstance) {													\
	char	szMessage[MAX_PATH] = "";											\
	strcpy_s(szMessage, typeid(CLASSNAME).name());								\
	strcat_s(szMessage, "is nullptr");											\
	MessageBoxA(0, szMessage, nullptr, MB_OK);}									\
	else {																		\
	pInstance->Release();}														\
	}();


#define NO_COPY(CLASSNAME)									\
		private:											\
		CLASSNAME(const CLASSNAME&);						\
		CLASSNAME& operator = (const CLASSNAME&);		

#define DECLARE_SINGLETON(CLASSNAME)						\
		NO_COPY(CLASSNAME)									\
		private:											\
		static CLASSNAME*	m_pInstance;					\
		public:												\
		static CLASSNAME*	GetInstance( void );			\
		static unsigned long DestroyInstance( void );			

#define IMPLEMENT_SINGLETON(CLASSNAME)						\
		CLASSNAME*	CLASSNAME::m_pInstance = NULL;			\
		CLASSNAME*	CLASSNAME::GetInstance( void )	{		\
			if(NULL == m_pInstance) {						\
				m_pInstance = new CLASSNAME;				\
			}												\
			return m_pInstance;								\
		}													\
		unsigned long CLASSNAME::DestroyInstance( void ) {	\
			unsigned long dwRefCnt = 0;						\
			if(nullptr != m_pInstance)	{					\
				dwRefCnt = m_pInstance->Release();			\
				if(0 == dwRefCnt)							\
					m_pInstance = nullptr;					\
			}												\
			return dwRefCnt;								\
		}

#ifdef _WINDLL
#define ENGINE_DLL _declspec(dllexport)
#else
#define ENGINE_DLL _declspec(dllimport)
#endif

#define BEGIN(NAMESPACE) namespace NAMESPACE { 
#define END }

