
// DeFogging.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CDeFoggingApp:
// �йش����ʵ�֣������ DeFogging.cpp
//

class CDeFoggingApp : public CWinApp
{
public:
	CDeFoggingApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CDeFoggingApp theApp;