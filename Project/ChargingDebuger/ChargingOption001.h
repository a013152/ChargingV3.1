
// ChargingOption001.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CChargingOption001App: 
// �йش����ʵ�֣������ ChargingOption001.cpp
//

class CChargingOption001App : public CWinApp
{
public:
	CChargingOption001App();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};

extern CChargingOption001App theApp;