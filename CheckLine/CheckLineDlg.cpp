
// CheckLineDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "CheckLine.h"
#include "CheckLineDlg.h"
#include "afxdialogex.h"
#include "Transform.h"
#include "Recognition.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CCheckLineDlg �Ի���



CCheckLineDlg::CCheckLineDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CCheckLineDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCheckLineDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CCheckLineDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_CHECK, &CCheckLineDlg::OnBnClickedButtonCheck)
	ON_BN_CLICKED(IDC_BUTTON_CHECKLINE, &CCheckLineDlg::OnBnClickedButtonCheckline)
	ON_BN_CLICKED(IDC_BUTTON_SQWARE, &CCheckLineDlg::OnBnClickedButtonSqware)
END_MESSAGE_MAP()


// CCheckLineDlg ��Ϣ�������

BOOL CCheckLineDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO:  �ڴ���Ӷ���ĳ�ʼ������

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CCheckLineDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CCheckLineDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CCheckLineDlg::OnBnClickedButtonCheck()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CTransform oTransform;
	oTransform.doTransform("F:\\development\\CheckLine\\1.jpg", 50);
}


int CCheckLineDlg::CheckLine(string img_path)
{
	return 0;
}


void CCheckLineDlg::OnBnClickedButtonCheckline()
{
	CTransform oTransform;
	oTransform.checkLine("F:\\development\\CheckLine\\1.jpg");
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}


void CCheckLineDlg::OnBnClickedButtonSqware()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	Recognition oRecognition;
	oRecognition.RecognitionSquares("F:\\development\\CheckLine\\3.5.jpg");
}
