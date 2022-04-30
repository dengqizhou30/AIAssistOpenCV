
// AIAssistDlg.h: 头文件
//

#pragma once

#include "CommUtil.h"
#include "SystemUtil.h"
#include "AssistWorker.h"
#include "opencv2/opencv.hpp"
#include<opencv2/imgproc.hpp>

using namespace cv;

// CAIAssistDlg 对话框
class CAIAssistDlg : public CDialogEx
{
// 构造
public:
	CAIAssistDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_AIASSIST_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持



// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

private:
	AssistConfig* m_AssistConfig;
	AssistWorker* m_AssistWorker;
	CWinThread* m_DrawThread;

public:

	afx_msg void OnBnClickedCancel();
	CComboBox m_processName;
	afx_msg void OnBnClickedButtonFindprocess();
	CButton m_imgDetect;
	CButton m_autoTrace;
	CButton m_autoFire;
	CButton m_autoPress;
	CStatic m_img;
	CEdit m_status;
	CEdit m_detectWidth;
	CEdit m_detectHeight;
	CButton m_traceType;
	CButton m_fireType;
	CEdit m_fireTime;
	afx_msg void OnBnClickedCheckImgdetect();
	afx_msg void OnBnClickedCheckAutotrace();
	afx_msg void OnBnClickedCheckAutofire();
	afx_msg void OnBnClickedCheckAutopress();

	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnSize(UINT nType, int cx, int cy);
};

