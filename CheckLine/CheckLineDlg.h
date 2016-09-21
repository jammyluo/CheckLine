
// CheckLineDlg.h : 头文件
//

#pragma once

#include <string>
// CCheckLineDlg 对话框
class CCheckLineDlg : public CDialogEx
{
// 构造
public:
	CCheckLineDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_CHECKLINE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
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
