#include "StdAfx.h"
#include "Timers.h"
#include <atlsync.h>


DWORD WINAPI TimerThreadFunc(LPVOID lpParam)
{
  HTIMER *pTimer = (HTIMER*)lpParam;

  CMutex mutex;
  CEvent event;

  if (!mutex.Create(NULL, TRUE, NULL))
    return GetLastError();  
  
  { //  lock
    pTimer->hMutex = mutex;

    if (!event.Create(NULL, TRUE, FALSE, NULL))
      return GetLastError();
    pTimer->hEvent = event;
  
    mutex.Release();
  } //  unlock  

  DWORD dwTimeStart = GetTickCount();
  DWORD dwTimeElapsed;
  DWORD dwInterval;
  HANDLE hWaitHandles[2] = {event, mutex};
  DWORD dwWaitResult;
  

  while(true)
  {
    dwWaitResult = WaitForMultipleObjects(2, hWaitHandles, FALSE, INFINITE);    

    { //  lock
      if (dwWaitResult == WAIT_OBJECT_0)  
        break;
      else if (dwWaitResult != WAIT_OBJECT_0 + 1)
        return GetLastError();

      dwInterval = pTimer->dwMilliseconds;    

      mutex.Release();
    } //  unlock

    
    dwTimeElapsed = GetTickCount() - dwTimeStart;
    if (dwTimeElapsed < dwInterval)      
    {
      
      Sleep(1);
      continue;
    }
    else
    {
      
      dwTimeStart = GetTickCount();

      
      if (!pTimer->pFunc(pTimer->lpParam))  
        break;
    }
  }

  return 0;
}


BOOL CreateTimer(TIMER_FUNC pTimerFunc, DWORD dwMilliseconds, LPVOID lpParam, HTIMER *phTimer)
{
  ATLASSERT(phTimer != NULL);

  phTimer->pFunc = pTimerFunc;
  phTimer->dwMilliseconds = dwMilliseconds;
  phTimer->lpParam = lpParam;
  phTimer->hEvent = NULL;
  phTimer->hMutex = NULL;

  phTimer->hThread = CreateThread(
    NULL, 
    0,
    &TimerThreadFunc,
    phTimer,
    0,
    &phTimer->dwThreadId);

  return (phTimer->hThread != NULL);
}


BOOL StopTimer(HTIMER *phTimer)
{
  ATLASSERT(phTimer != NULL);

  return SetEvent(phTimer->hEvent);
}


BOOL WaitTimerStopped(HTIMER *phTimer)
{
  ATLASSERT(phTimer != NULL);

  return (WaitForSingleObject(phTimer->hThread, INFINITE) != WAIT_FAILED);
}


BOOL ChangeTimerInterval(HTIMER *phTimer, DWORD dwMilliseconds)
{
  ATLASSERT(phTimer != NULL);

  if (WaitForSingleObject(phTimer->hMutex, INFINITE) == WAIT_FAILED)
    return FALSE;
  
  phTimer->dwMilliseconds = dwMilliseconds;

  return ReleaseMutex(phTimer->hMutex);
}


BOOL IsTimerStarted(HTIMER *phTimer)
{
  ATLASSERT(phTimer != NULL);

  DWORD dwWait = WaitForSingleObject(phTimer->hThread, 0);

  if (dwWait == WAIT_OBJECT_0)
    return FALSE;
  else if (dwWait == WAIT_FAILED)
    return FALSE;

  return TRUE;
}


BOOL DestroyTimer(HTIMER *phTimer)
{
  ATLASSERT(phTimer != NULL);

  return CloseHandle(phTimer->hThread);
}
