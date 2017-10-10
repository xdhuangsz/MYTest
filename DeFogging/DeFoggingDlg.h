
// DeFoggingDlg.h : ͷ�ļ�
//

#pragma once
#include "PWdehanze.h"
#include "Cimgclass.h"
// CDeFoggingDlg �Ի���
class CDeFoggingDlg : public CDialogEx
{
// ����
public:
	CDeFoggingDlg(CWnd* pParent = NULL);	// ��׼���캯��
	CWinThread* xferThread;
// �Ի�������
	enum { IDD = IDD_DEFOGGING_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CStatusBar CDstate; //״̬��
	enum State{Ismax,Ismin,None};
	State state;
	CImage maximg,minimg,resultimg;
	unsigned char * resultdata;
	CString maximgname,minimgname;
	bool m_fLButtonDownNotUp;
	bool iscapture;
	CPoint m_chRegionLeftTopPoint,m_chRegionRightBottomPoint,point1,pointold;
	double rectpoint[4];
	int imgW;
	int imgH;
public:
	CButton m_check_max;
	CButton m_check_min;
	afx_msg void OnCheckMax();
	afx_msg void OnCheckMin();
	void ShowPic(CImage res,UINT nID);
	afx_msg void OnChoosepic();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnChoosebackground();
	CPoint PointRestict(CPoint point,CRect rec);
	void CalPolar(int x,int y,int w,int h);
	afx_msg void OnPaint();
	void defog();
	void CImage2data(unsigned char* datar,unsigned char* datag,unsigned char* datab,unsigned char* data,int width, int height,int channels);
	void data2CIamge(unsigned char* datar,unsigned char* datag,unsigned char* datab,CImage* image,int width, int height,int channels);
	afx_msg void OnBnDefog();
	afx_msg void OnClose();
	afx_msg void OnSave();
	int datanum(CString str);
};
