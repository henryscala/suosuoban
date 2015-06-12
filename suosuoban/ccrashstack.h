#ifndef CCRASHSTACK_H
#define CCRASHSTACK_H

#include <windows.h>
#include <QString>

//refer to here http://blog.csdn.net/lanhy999/article/details/12189375
//another precondition is that release version shall be used instead of debug version
//
//  ccrashstack.h
//
//
//  Created by Lan huaiyu on 9/27/13.
//  Copyright 2013 CCrashStack. All rights reserved.
//

class CCrashStack
{
private:
    PEXCEPTION_POINTERS m_pException;

private:
    QString GetModuleByRetAddr(PBYTE Ret_Addr, PBYTE & Module_Addr);
    QString GetCallStack(PEXCEPTION_POINTERS pException);
    QString GetVersionStr();


public:
    CCrashStack(PEXCEPTION_POINTERS pException);

    QString GetExceptionInfo();
};

#endif // CCRASHSTACK_H
