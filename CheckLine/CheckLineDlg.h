
// CheckLineDlg.h : ͷ�ļ�
//

#pragma once

#include <string>
// CCheckLineDlg �Ի���
class CCheckLineDlg : public CDialogEx
{
// ����
public:
	CCheckLineDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_CHECKLINE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonCheck();
	int CheckLine(std::string img_path);
	afx_msg void OnBnClickedButtonCheckline();
	afx_msg void OnBnClickedButtonSqware();
};
