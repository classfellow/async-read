/****************************************************************************************
*
*   Copyright (c) 2013 All Rights Reserved by KingSoft Integrated Service co. Ltd.
*
*   Module: 
*
****************************************************************************************/
/**
* @file     util_tools.h
* @brief    
*
* @author   Õ¨∞‡Õ¨—ß
* @date     
* @version  0.0.8
*
* @warning  
*           
* @par 
*           
*
*
****************************************************************************************/
#pragma once

#ifndef tstring
#ifdef _UNICODE
#define tstring std::wstring
#else
#define tstring std::string
#endif
#endif

#define CPairString std::pair<tstring, tstring>
#define	LPCTSTR2(lpszData) ((lpszData == NULL) ? _T("") : lpszData)

class CriticalSection {
public:
	CriticalSection(void);
	~CriticalSection(void);

private:
#ifdef _WIN32
	CRITICAL_SECTION m_cs;
#else
	pthread_mutex_t  m_cs;
#endif

public:
	void Lock(void);
	void UnLock(void);
	BOOL TryLock(void);
};


template<typename TLock>
class AutoCritSecLock {
public:
  AutoCritSecLock( TLock& cs, bool bInitialLock = true );
  ~AutoCritSecLock() throw();

  HRESULT Lock() throw();
  void Unlock() throw();

  // Implementation
private:
  TLock& m_cs;
  bool m_bLocked;

  // Private to avoid accidental use
  AutoCritSecLock( const AutoCritSecLock& ) throw();
  AutoCritSecLock& operator=( const AutoCritSecLock& ) throw();
};

template< class TLock >
inline AutoCritSecLock< TLock >::AutoCritSecLock( TLock& cs, bool bInitialLock ) :m_cs( cs ),m_bLocked( false ) {
  if( bInitialLock )  {
    Lock();
  }
}

template< class TLock >
inline AutoCritSecLock< TLock >::~AutoCritSecLock() throw() {
  if( m_bLocked ) {
    Unlock();
  }
}

template< class TLock >
inline HRESULT AutoCritSecLock< TLock >::Lock() throw() {
  assert( !m_bLocked );
  m_cs.Lock();
  m_bLocked = true;
  return( S_OK );
}

template< class TLock >
inline void AutoCritSecLock< TLock >::Unlock() throw() {
  assert( m_bLocked );
  m_cs.UnLock();
  m_bLocked = false;
}
