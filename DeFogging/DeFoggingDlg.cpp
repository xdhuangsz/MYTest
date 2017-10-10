
// DeFoggingDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "DeFogging.h"
#include "DeFoggingDlg.h"
#include "afxdialogex.h"
#include "PWdehanze.h"
#include "CwzdSplash.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
static  UINT indicators[]={ID_SEPARATOR,ID_INDICATOR_NUM,ID_INDICATOR_SCRL};
double ratio[6][3] = {{1.35,1.35,1.26},{1.35,1.35,1.26},{1.35,1.35,1.26},{1.35,1.35,1.17},{1.35,1.35,1.17},{1.5,1.5,1.5}};
// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CDeFoggingDlg 对话框




CDeFoggingDlg::CDeFoggingDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDeFoggingDlg::IDD, pParent)
	,state(Ismax)
	,m_fLButtonDownNotUp(false)
	,iscapture(false)
	,xferThread(NULL)
	,imgW(0)
	,imgH(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDeFoggingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK_MAX, m_check_max);
	DDX_Control(pDX, IDC_CHECK_MIN, m_check_min);
}

BEGIN_MESSAGE_MAP(CDeFoggingDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_CHECK_MAX, &CDeFoggingDlg::OnCheckMax)
	ON_BN_CLICKED(IDC_CHECK_MIN, &CDeFoggingDlg::OnCheckMin)
	ON_BN_CLICKED(IDC_CHOOSEPIC, &CDeFoggingDlg::OnChoosepic)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(IDC_CHOOSEBACKGROUND, &CDeFoggingDlg::OnChoosebackground)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_DEFOG, &CDeFoggingDlg::OnBnDefog)
	ON_WM_CLOSE()
//	ON_BN_CLICKED(IDC_SAVE, &CDeFoggingDlg::OndSave)
//ON_BN_CLICKED(IDC_SAVE, &CDeFoggingDlg::OnSave)
ON_BN_CLICKED(IDC_SAVE, &CDeFoggingDlg::OnSave)
END_MESSAGE_MAP()


// CDeFoggingDlg 消息处理程序

BOOL CDeFoggingDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。
	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}
	CwzdSplash Splash;
	Splash.Create(IDB_BITMAP1);
	Splash.CenterWindow();
	Splash.UpdateWindow();
	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	m_check_min.SetCheck(0);
	m_check_max.SetCheck(1);
	maximgname = "";
	minimgname = "";
	resultdata = new unsigned char;
	memset(rectpoint,0,sizeof(double)*4);
	if(!(mclInitializeApplication(NULL,0)))
	{
		MessageBox(L"MATLAB初始化失败!");
		return false;
	}
	if(!PWdehanzeInitialize())
	{
		MessageBox(L"初始化失败!");
		return false;
	}
	CRect rect;
	GetClientRect(&rect);
	CDstate.Create(this);
	CDstate.SetIndicators(indicators,1);
	CDstate.SetPaneInfo(0,ID_INDICATOR_SCRL,SBPS_NORMAL,300);
	//CDstate.SetPaneInfo(2,ID_INDICATOR_SCRL,SBPS_NORMAL,rect.Width()-600);
	CDstate.MoveWindow(0,rect.bottom-20,rect.Width(),20);  
	CDstate.SetPaneText(0,_T("空闲"),true);
	Splash.DestroyWindow();
	UpdateData(false);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CDeFoggingDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}


//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CDeFoggingDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

UINT xferLoop(LPVOID param)
{
	CDeFoggingDlg* dlg = (CDeFoggingDlg*)param;
	//dlg->CDstate.SetPaneText(0,_T("正忙"),true);
	dlg->defog();
	//dlg->CDstate.SetPaneText(0,_T("空闲"),true);
	dlg->xferThread = NULL;
	return 0;
}

void CDeFoggingDlg::OnCheckMax()
{
	// TODO: 在此添加控件通知处理程序代码
	if(m_check_max.GetCheck() == 1)
	{
		state = Ismax;
		m_check_min.SetCheck(0);
	}
	else if(m_check_max.GetCheck() == 0 && m_check_min.GetCheck() == 0)
	{
		state = None;
	}
}


void CDeFoggingDlg::OnCheckMin()
{
	// TODO: 在此添加控件通知处理程序代码
	if(m_check_min.GetCheck() == 1)
	{
		state = Ismin;
		m_check_max.SetCheck(0);
	}
	else if(m_check_max.GetCheck() == 0 && m_check_min.GetCheck() == 0)
	{
		state = None;
	}
}

void CDeFoggingDlg::ShowPic(CImage res,UINT nID)
{
	CRect rect;
	GetDlgItem(nID)->GetClientRect(&rect);
	ScreenToClient(&rect);
	CWnd *pwnd = GetDlgItem(nID);
	pwnd->GetClientRect(&rect);
	CDC *pDC = pwnd->GetDC();
	res.Draw(pDC->m_hDC,rect,Gdiplus::InterpolationModeNearestNeighbor);
	ReleaseDC(pDC);
}

void CDeFoggingDlg::OnChoosepic()
{
	// TODO: 在此添加控件通知处理程序代码
	CString szFileFilter=_T("(*.bmp,*.png,*.jpg)|*.bmp;*.png;*.jpg|(* .*)|*.*|"); 
	CFileDialog dlg(true,NULL,NULL,OFN_HIDEREADONLY|OFN_ALLOWMULTISELECT|OFN_NOCHANGEDIR,szFileFilter);
	CString pathname;
	if(dlg.DoModal()==IDOK)
	{
		pathname = dlg.GetPathName();
	}
	else
	{
		return;
	}
	if(state == Ismax)	//最大值图像
	{
		if(!maximg.IsNull())
			maximg.Destroy();
		maximgname = pathname;
		maximg.Load(pathname);
		imgW = maximg.GetWidth();
		imgH = maximg.GetHeight();
		if(maximg.GetBPP()/8 == 1)
		{
			MessageBox(L"只处理24位图像");
			maximg.Destroy();
			return;
		}
		ShowPic(maximg,IDC_MAX_PIC);
	}
	else if(state == Ismin)	//最小值图像
	{
		if(!minimg.IsNull())
			minimg.Destroy();
		minimgname = pathname;
		minimg.Load(pathname);
		if(minimg.GetBPP()/8 == 1)
		{
			MessageBox(L"只处理24位图像");
			minimg.Destroy();
			return;
		}
		ShowPic(minimg,IDC_MIN_PIC);
	}
}
void CDeFoggingDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	//CRect rect;
	//GetDlgItem(IDC_MAX_PIC)->GetWindowRect(&rect);
	//ScreenToClient(rect);
	//point = PointRestict(point,rect);
	//m_chRegionRightBottomPoint= point;
	//if(point.x<rect.right && point.x>rect.left && point.y <rect.bottom && point.y >rect.top)
	// {
 //       if(m_fLButtonDownNotUp && iscapture)
	//	{
	//		CClientDC dc(this);
	//		CPen* pOldPen;
	//		COLORREF color(0x00FFFFFF);
	//		CPen newpen(PS_DASHDOT,1,color);
	//		pOldPen=dc.SelectObject(&newpen);
	//		dc.SetROP2(R2_NOT);      //此为关键!!!
	//		dc.SelectStockObject(NULL_BRUSH);    //不使用画刷
	//		dc.Rectangle(CRect(point1,pointold)); 
	//		dc.Rectangle(CRect(point1,point));
	//		pointold = point;
	//		dc.SelectObject(pOldPen);
	//	}
 //   }
	CDialogEx::OnMouseMove(nFlags, point);
}


void CDeFoggingDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	//CRect rect;
	//GetDlgItem(IDC_MAX_PIC)->GetWindowRect(&rect);
	//ScreenToClient(rect);
	//point = PointRestict(point,rect);
	//m_chRegionRightBottomPoint = point;
	//if(m_fLButtonDownNotUp && iscapture)
	//	{
	//		int clientw = rect.right - rect.left;
	//		int clienth = rect.bottom - rect.top;
	//		int w = abs((int)((m_chRegionRightBottomPoint.x-m_chRegionLeftTopPoint.x)/(double)clientw * maximg.GetWidth()));
	//		int h = abs((int)((m_chRegionRightBottomPoint.y-m_chRegionLeftTopPoint.y)/(double)clienth * maximg.GetHeight()));
	//		int x = min((int)((m_chRegionRightBottomPoint.x-rect.left)/(double)clientw*maximg.GetWidth()),(int)((m_chRegionLeftTopPoint.x-rect.left)/(double)clientw * maximg.GetWidth()));
	//		int y = min((int)((m_chRegionRightBottomPoint.y-rect.top)/(double)clienth*maximg.GetHeight()),(int)((m_chRegionLeftTopPoint.y-rect.top)/(double)clienth * maximg.GetHeight()));
	//		if(w<=0 || h<=0)
	//		{
	//			iscapture = false;
	//			return;
	//		}
	//		int ret = MessageBox(TEXT("是否选择该区域作为天空背景？"),NULL,MB_YESNO);
	//		if(ret == IDYES)
	//		{

	//			//CalPolar(x,y,w,h);
	//			rectpoint[0] = x;
	//			rectpoint[1] = y;
	//			rectpoint[2] = x+w;
	//			rectpoint[3] = y+h;
	//			iscapture = false;
	//			m_fLButtonDownNotUp = false;
	//			GetDlgItem(IDC_CHOOSEBACKGROUND)->SetWindowTextW(L"选择天空背景区域");
	//		}
	//		else 
	//		{
	//			iscapture = false;
	//		}
	//		iscapture = false;
	//		InvalidateRect(NULL);
 //   }
	CDialogEx::OnLButtonUp(nFlags, point);
}


void CDeFoggingDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	/*CRect rect;
	GetDlgItem(IDC_MAX_PIC)->GetWindowRect(&rect);
	ScreenToClient(rect);
	if(point.x<rect.right && point.x>rect.left && point.y <rect.bottom && point.y >rect.top)
	{
		iscapture = true;
		point1 = PointRestict(point,rect);
		pointold = point1;
		m_chRegionLeftTopPoint = point1;
	}*/
	CDialogEx::OnLButtonDown(nFlags, point);
}


void CDeFoggingDlg::OnChoosebackground()
{
	// TODO: 在此添加控件通知处理程序代码
	if(!maximg.IsNull() && !minimg.IsNull())
	{
		if(maximg.GetWidth() == minimg.GetWidth() && maximg.GetHeight() == minimg.GetHeight())
		{
		if(!m_fLButtonDownNotUp)
		{
			m_fLButtonDownNotUp = true;
			GetDlgItem(IDC_CHOOSEBACKGROUND)->SetWindowTextW(L"选择中……");
			CDstate.SetPaneText(0,_T("空闲"),true);
		}
		else
		{
			m_fLButtonDownNotUp = false;
			GetDlgItem(IDC_CHOOSEBACKGROUND)->SetWindowTextW(L"选择天空背景区域");
		}
		}
		else
		{
			MessageBox(L"两幅图像尺寸不同！");
			return;
		}
	}
	else 
	{
		MessageBox(L"请打开两幅图像！");
		return;
	}
}

CPoint CDeFoggingDlg::PointRestict(CPoint point,CRect rec)
{
	if(point.x<rec.left)
		point.x = rec.left+1;
	if (point.x>rec.right)
		point.x = rec.right-1;
	if (point.y<rec.top)
		point.y = rec.top+1;
	if(point.y>rec.bottom)
		point.y = rec.bottom-1;
	return point;
}

void CDeFoggingDlg::CalPolar(int x,int y,int w,int h)
{
	CString str = maximgname;
	CString str1 = minimgname;
	CImage max,min;
	max.Load(str);
	min.Load(str1);
	int ichannel = max.GetBPP()/8;
	int pitch = max.GetPitch();
	int pitch1 = min.GetPitch();
	unsigned char* maxSrc = (unsigned char*)max.GetBits();
	unsigned char* minSrc = (unsigned char*)min.GetBits();
	double *dif = new double[sizeof(double)*ichannel];
	double *sum = new double[sizeof(double)*ichannel];
	memset(dif,0,sizeof(double)*ichannel);
	memset(sum,0,sizeof(double)*ichannel);
	for(int i =0;i<h; i++)
	{
		for(int j=0;j<w;j++)
		{
			for (int k =0;k<ichannel;k++)
			{
				dif[k] += maxSrc[(i+y)*pitch + (j+x)*ichannel +k] - minSrc[(i+y)*pitch1 + (j+x)*ichannel +k];
				sum[k] += maxSrc[(i+y)*pitch + (j+x)*ichannel +k] + minSrc[(i+y)*pitch1 + (j+x)*ichannel +k];
			}
		}
	}
	max.Destroy();
	min.Destroy();
	delete[] dif;
	delete[] sum;
}



void CDeFoggingDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialogEx::OnPaint()
	CString str = maximgname;
	CString str1 = minimgname;
	CImage max,min;
	max.Load(str);
	ShowPic(max,IDC_MAX_PIC);
	min.Load(str1);
	ShowPic(min,IDC_MIN_PIC);
	max.Destroy();
	min.Destroy();

	CImage myimg;
	myimg.Create(imgW,imgH,24);
	unsigned char * pBit = (unsigned char*)myimg.GetBits();
	int pitch = myimg.GetPitch();
	for(int i=0;i<imgH;i++)
	{
		for(int j = 0;j<imgW;j++)
		{
			for(int k = 0;k<3;k++)
			{
				pBit[i*pitch+j*3+k] = resultdata[i*imgW*3+j*3+k];
			}
		}
	}
	ShowPic(myimg,IDC_RESULT_PIC);
	myimg.Destroy();
}

void CDeFoggingDlg::data2CIamge(unsigned char* datar,unsigned char* datag,unsigned char* datab,CImage* image,int width, int height,int channels)
{
	if(!image->IsNull())
		image->Destroy();
	image->Create(width,height,channels*8);
	unsigned char * pBit = (unsigned char*)image->GetBits();
	if(resultdata !=NULL)
	{
		delete[] resultdata;
		resultdata = NULL;
	}
	resultdata = new unsigned char[sizeof(unsigned char)*width*height*channels];
	int pitch = image->GetPitch();
	for(int i =0;i<height;i++)
	{
		for (int j=0;j<width ;j++)
		{
			resultdata[i*width*channels+j*channels] = pBit[ i*pitch+j*channels]=datab[i*width+j];
			resultdata[i*width*channels+j*channels+1] = pBit[ i*pitch+j*channels+1]=datag[i*width+j];
			resultdata[i*width*channels+j*channels+2] = pBit[ i*pitch+j*channels+2]=datar[i*width+j];
		}
	}
}

void CDeFoggingDlg::CImage2data(unsigned char* datar,unsigned char* datag,unsigned char* datab,unsigned char* data,int width, int height,int channels)
{
	for(int i=0;i<height;i++)
	{
		for(int j = 0;j<width;j++)
		{
			datab[i*width +j] = data[i + j+0];
			datag[i*width +j] = data[i + j +1];
			datar[i*width +j] = data[i + j +2];
			
			
		}
	}
	//delete[] pBits;
}

void CDeFoggingDlg::defog()
{
	CDstate.SetPaneText(0,_T("正忙"),true);
	CString str = maximgname;
	CString str1 = minimgname;
	int position=str.ReverseFind('\\');
	CString exten = str.Right(str.GetLength()-position-1);
	int num = 0; 
	num= datanum(exten);
	double *ratiodata = ratio[num];
	CImage max,min;
	max.Load(str);
	min.Load(str1);
	if((max.GetWidth() != min.GetWidth()) || (max.GetHeight() != min.GetHeight()))
	{
		MessageBox(L"图像尺寸不一样！");
		GetDlgItem(IDC_DEFOG)->EnableWindow(true);
		CDstate.SetPaneText(0,_T("空闲"),true);
		max.Destroy();
		min.Destroy();
		return ;
	}
	int w = max.GetWidth();
	int h = max.GetHeight();
	int ichannel = max.GetBPP()/8;
	int pitchmax = max.GetPitch();
	int pitchmin = min.GetPitch();
	unsigned char * maxdata = (unsigned char*)max.GetBits();
	unsigned char * mindata = (unsigned char*)min.GetBits();
	unsigned char * MAX_R = new unsigned char[ w * h];
	unsigned char * MAX_G = new unsigned char[ w * h];
	unsigned char * MAX_B = new unsigned char[ w * h];
	for(int i=0;i<h;i++)
	{
		for(int j = 0;j<w;j++)
		{
			MAX_B[i*w +j] = maxdata[i*pitchmax + j*ichannel+0];
			MAX_G[i*w +j] = maxdata[i*pitchmax + j*ichannel +1];
			MAX_R[i*w +j] = maxdata[i*pitchmax + j*ichannel +2];
		}
	}
	mwArray MaxDataInr(w,h,mxUINT8_CLASS);
	mwArray MaxDataIng(w,h,mxUINT8_CLASS);
	mwArray MaxDataInb(w,h,mxUINT8_CLASS);
	MaxDataInr.SetData((mxUint8*)MAX_R,w*h);
	MaxDataIng.SetData((mxUint8*)MAX_G,w*h);
	MaxDataInb.SetData((mxUint8*)MAX_B,w*h);
	delete[] MAX_R;
	delete[] MAX_G;
	delete[] MAX_B;
	MAX_R =NULL;
	MAX_G =NULL;
	MAX_B =NULL;
	unsigned char * MIN_R = new unsigned char[ w * h];
	unsigned char * MIN_G = new unsigned char[ w * h];
	unsigned char * MIN_B = new unsigned char[ w * h];

	for(int i=0;i<h;i++)
	{
		for(int j = 0;j<w;j++)
		{
			MIN_B[i*w +j] = mindata[i*pitchmin + j*ichannel+0];
			MIN_G[i*w +j] = mindata[i*pitchmin + j*ichannel +1];
			MIN_R[i*w +j] = mindata[i*pitchmin + j*ichannel +2];
		}
	}
	mwArray MinDataInr(w,h,mxUINT8_CLASS);
	mwArray MinDataIng(w,h,mxUINT8_CLASS);
	mwArray MinDataInb(w,h,mxUINT8_CLASS);
	MinDataInr.SetData((mxUint8*)MIN_R,w*h);
	MinDataIng.SetData((mxUint8*)MIN_G,w*h);
	MinDataInb.SetData((mxUint8*)MIN_B,w*h);
	delete[] MIN_R;
	delete[] MIN_G;
	delete[] MIN_B;
	MIN_R =NULL;
	MIN_G =NULL;
	MIN_B =NULL;
	mwArray rectP(1,4,mxDOUBLE_CLASS);
	rectP.SetData(rectpoint,4);

	mwArray ResultDataOutr(w,h,mxDOUBLE_CLASS);
	mwArray ResultDataOutg(w,h,mxDOUBLE_CLASS);
	mwArray ResultDataOutb(w,h,mxDOUBLE_CLASS);
	unsigned char * RESULT_R = new unsigned char[ w * h];
	unsigned char * RESULT_G = new unsigned char[ w * h];
	unsigned char * RESULT_B = new unsigned char[ w * h];

	mwArray RATIODATA(1,3,mxDOUBLE_CLASS);
	RATIODATA.SetData(ratiodata,3);
	try{
		PWdehaze(3,ResultDataOutr,ResultDataOutg,ResultDataOutb,MaxDataInr,MaxDataIng,MaxDataInb,MinDataInr,MinDataIng,MinDataInb,rectP,RATIODATA);
	}
	catch (const mwException& erro)
	{
		max.Destroy();
		min.Destroy();
		delete[] RESULT_G;
		delete[] RESULT_B;
		delete[] RESULT_R;
		RESULT_G =NULL;
		RESULT_B =NULL;
		RESULT_R =NULL;
		CString str;
		str = erro.what();
		MessageBox(str,NULL,NULL);
		GetDlgItem(IDC_DEFOG)->EnableWindow(true);
		CDstate.SetPaneText(0,_T("空闲"),true);
		return ;
	}
	ResultDataOutr.GetData(RESULT_R,w*h);
	ResultDataOutg.GetData(RESULT_G,w*h);
	ResultDataOutb.GetData(RESULT_B,w*h);
	data2CIamge(RESULT_R,RESULT_G,RESULT_B,&resultimg,w,h,ichannel);
	ShowPic(resultimg,IDC_RESULT_PIC);
	GetDlgItem(IDC_DEFOG)->EnableWindow(true);
	CDstate.SetPaneText(0,_T("空闲"),true);
	max.Destroy();
	min.Destroy();
	delete[] RESULT_G;
	delete[] RESULT_B;
	delete[] RESULT_R;
	RESULT_G =NULL;
	RESULT_B =NULL;
	RESULT_R =NULL;
}

void CDeFoggingDlg::OnBnDefog()
{
	// TODO: 在此添加控件通知处理程序代码
	rectpoint[0] = 10;
	rectpoint[1] = 10;
	rectpoint[2] = 100;
	rectpoint[3] = 100;
	int w = rectpoint[2] - rectpoint[0];
	int h = rectpoint[3] - rectpoint[1];
	if(w == 0 || h == 0)
	{
		MessageBox(L"请选择区域");
		return;
	}
	if(xferThread == NULL)
	{
		GetDlgItem(IDC_DEFOG)->EnableWindow(false);
		xferThread = AfxBeginThread(xferLoop,this);
	}
}


void CDeFoggingDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if(xferThread)
	{
		xferThread->PostThreadMessageW(WM_QUIT,NULL,NULL);
	}
	if(resultdata != NULL)
	delete[] resultdata;
	CDialogEx::OnClose();
}

void CDeFoggingDlg::OnSave()
{
	// TODO: 在此添加控件通知处理程序代码
	CString szFileFilter = L"位图文件|*.bmp|JPEG 图像文件|*.jpg|PNG 图像文件|*.png|";; 
	CFileDialog dlg(FALSE,NULL,L"test.bmp",OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,szFileFilter);
	////CFileDialog dlg(false,NULL,NULL,OFN_HIDEREADONLY|OFN_ALLOWMULTISELECT|OFN_NOCHANGEDIR,szFileFilter);
	CString pathname;
	if(dlg.DoModal()==IDOK)
	{
		pathname = dlg.GetPathName();
	}
	else
	{
		return;
	}
	CImage myimg;
	
	myimg.Create(imgW,imgH,24);
	unsigned char * pBit = (unsigned char*)myimg.GetBits();
	
	int pitch = myimg.GetPitch();
	for(int i=0;i<imgH;i++)
	{
		for(int j = 0;j<imgW;j++)
		{
			for(int k = 0;k<3;k++)
			{
				pBit[i*pitch+j*3+k] = resultdata[i*imgW*3+j*3+k];
			}
		}
	}
	HRESULT hResult = myimg.Save(pathname);
	if(FAILED(hResult))
	{
		MessageBox(L"保存失败！");
	}
	else
		MessageBox(L"保存成功！");
}

int CDeFoggingDlg::datanum(CString str)
{
	if(str == "1max.jpg")
	{
		return 0;
	}
	else if(str == "2max.jpg")
	{
		return 1;
	}
	else if(str == "3max.jpg")
	{
		return 2;
	}
	else if(str == "4max.jpg")
	{
		return 3;
	}
	else if(str == "5max.jpg")
	{
		return 4;
	}
	else 
		return 5;
}