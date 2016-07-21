#pragma once



typedef BOOL (WINAPI *TIMER_FUNC)(LPVOID lpParam);

typedef struct HTIMER
{
  HANDLE hThread;
  DWORD dwThreadId;
  HANDLE hEvent;
  HANDLE hMutex;
  DWORD dwMilliseconds;
  TIMER_FUNC pFunc;
  LPVOID lpParam;
}_HTIMER;



BOOL CreateTimer(TIMER_FUNC pTimerFunc, 
                DWORD dwMilliseconds, 
                LPVOID lpParam, 
                HTIMER *phTimer);


BOOL DestroyTimer(HTIMER *phTimer);

 
BOOL StopTimer(HTIMER *phTimer);

BOOL WaitTimerStopped(HTIMER *phTimer);

BOOL ChangeTimerInterval(HTIMER *phTimer, DWORD dwMilliseconds);


BOOL IsTimerStarted(HTIMER *phTimer);


//////////////////////////////////////////////////////////////////////////


#define TIMER_FUNC_NAME         TimerFunc
#define DECLARE_TIMER_FUNC      BOOL TIMER_FUNC_NAME


template<class T>
class CTimerT
{
private:

  HTIMER m_hTimer;


public:

  CTimerT()   
  {
    m_hTimer.hThread = NULL;
  }
  

  ~CTimerT()
  {
    ATLASSERT(Destroy());
  }


  operator HTIMER*()
  {
    return &m_hTimer;
  }


  BOOL Destroy()
  {
    
    if (IsStarted())
    {
      if (!Stop())
        return FALSE;
      if (!WaitStopped())
        return FALSE;
    }
    else 
    {
      DWORD dwErr = GetLastError();
      
      if (dwErr == ERROR_INVALID_HANDLE)
        return TRUE;
     
      else if (dwErr != NO_ERROR)
        return FALSE;
    }

   
    return DestroyTimer(&m_hTimer);
  }


  static BOOL WINAPI _TimerFunc(LPVOID lpParam)
  {
    T *lpThis = (T*) lpParam;
    
    return lpThis->TIMER_FUNC_NAME();
  }


  BOOL Create(DWORD dwMilliseconds)
  {
    if (IsStarted())
      return FALSE;

    return CreateTimer(&((T*)this)->_TimerFunc, dwMilliseconds, (T*)this, &m_hTimer);
  }


  BOOL Stop()
  {
    return StopTimer(&m_hTimer);
  }


  BOOL WaitStopped()
  {
    return WaitTimerStopped(&m_hTimer);
  }

  
  BOOL ChangeInterval(DWORD dwMilliseconds)
  {
    return ChangeTimerInterval(&m_hTimer, dwMilliseconds);
  }

  
  BOOL IsStarted()
  {
    return IsTimerStarted(&m_hTimer);
  }
};


class CTimerWnd : public CTimerT<CTimerWnd>
{
private:
  HWND m_hwnd;
  UINT m_id;

public:
  CTimerWnd()
  {
    m_hwnd = NULL;
    m_id = 0;
  }

  void Start(HWND hwnd, UINT id, DWORD dwInterval)
  {
    m_hwnd = hwnd;
    m_id = id;
    Create(dwInterval);
  }

  DECLARE_TIMER_FUNC()
  {
    PostMessage(m_hwnd, WM_TIMER, m_id, 0);
    return TRUE;
  }
};