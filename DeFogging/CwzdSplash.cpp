// CwzdSplash.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "CwzdSplash.h"

// CwzdSplash
bool destroy = false;
IMPLEMENT_DYNAMIC(CwzdSplash, CWnd)

CwzdSplash::CwzdSplash()
{

}

CwzdSplash::~CwzdSplash()
{
}


BEGIN_MESSAGE_MAP(CwzdSplash, CWnd)
	ON_WM_PAINT()
	ON_WM_TIMER()
END_MESSAGE_MAP()



// CwzdSplash ��Ϣ�������




void CwzdSplash::Create(UINT nBitmapID)
{
	m_bitmap.LoadBitmap(nBitmapID);  
	BITMAP bitmap;  
	m_bitmap.GetBitmap(&bitmap);  
	//CreateEx(0,AfxRegisterWndClass(0),"",WS_POPUP|WS_VISIBLE|WS_BORDER,0,0,bitmap.bmWidth,bitmap.bmHeight,NULL,0);  
	CreateEx(0,  
		AfxRegisterWndClass(0, AfxGetApp()->LoadStandardCursor(IDC_ARROW)),  
		NULL, WS_POPUP | WS_VISIBLE, 0, 0, bitmap.bmWidth, bitmap.bmHeight, NULL , NULL);
}


void CwzdSplash::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CWnd::OnPaint()

	//CPaintDC dc(this); // device context forpainting  
	BITMAP bitmap;  
	m_bitmap.GetBitmap(&bitmap);  
	CDC dcComp;  
	dcComp.CreateCompatibleDC(&dc);  
	dcComp.SelectObject(&m_bitmap);  
	// draw bitmap 
	dc.BitBlt(0,0,bitmap.bmWidth,bitmap.bmHeight,&dcComp,0,0,SRCCOPY);
	//while (1)
	//{
	//	TextOut(this->GetDC()->m_hDC,15,15,_T("Loading."),8);
	//	Sleep(10);
	//	TextOut(this->GetDC()->m_hDC,15,15,_T("Loading.."),9);
	//	Sleep(10);
	//	TextOut(this->GetDC()->m_hDC,15,15,_T("Loading..."),10);
	//}
	
}


void CwzdSplash::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	destroy = DestroyWindow(); //���ٳ�ʼ���洰�� 
	//CWnd::OnTimer(nIDEvent);
}
