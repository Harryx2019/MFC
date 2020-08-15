// CSellDlg.cpp: 实现文件
//

#include "pch.h"
#include "SaleSystem.h"
#include "CSellDlg.h"
#include "CInfoFile.h"


// CSellDlg

IMPLEMENT_DYNCREATE(CSellDlg, CFormView)

CSellDlg::CSellDlg()
	: CFormView(DIALOG_SELL)
	, m_left(0)
	, m_price(0)
	, m_number(0)
	, m_sellInfo(_T(""))
{

}

CSellDlg::~CSellDlg()
{
}

void CSellDlg::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_combo);
	DDX_Text(pDX, IDC_EDIT1, m_left);
	DDX_Text(pDX, IDC_EDIT2, m_price);
	DDX_Text(pDX, IDC_EDIT3, m_number);
	DDX_Text(pDX, IDC_EDIT4, m_sellInfo);
}

BEGIN_MESSAGE_MAP(CSellDlg, CFormView)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CSellDlg::OnCbnSelchangeCombo1)
	ON_BN_CLICKED(IDC_BUTTON3, &CSellDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON5, &CSellDlg::OnBnClickedButton5)
END_MESSAGE_MAP()


// CSellDlg 诊断

#ifdef _DEBUG
void CSellDlg::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CSellDlg::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CSellDlg 消息处理程序


void CSellDlg::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: 在此添加专用代码和/或调用基类

	//初始化销售管理页面
	//初始化下拉框
	CInfoFile file;
	file.ReadDocline();

	//file.ls容器里包含所有商品
	for (list<msg>::iterator it = file.ls.begin(); it != file.ls.end(); it++) {
		m_combo.AddString(CString(it->name.c_str()));
	}

	//设置默认值
	m_combo.SetCurSel(0);
	OnCbnSelchangeCombo1();
}


void CSellDlg::OnCbnSelchangeCombo1()
{
	// TODO: 在此添加控件通知处理程序代码
	//商品切换 调用事件

	//获取当前索引值
	int index = m_combo.GetCurSel();

	CString str;
	m_combo.GetLBText(index, str);

	//根据商品名称获取这个商品的库存和单价 显示到窗口中
	CInfoFile file;
	file.ReadDocline();

	for (list<msg>::iterator it = file.ls.begin(); it != file.ls.end(); it++) {
		if (CString(it->name.c_str()) == str) {
			m_left = it->num;
			m_price = it->price;
			UpdateData(FALSE);
			break;
		}
	}
}


void CSellDlg::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
	// 购买商品 按钮点击
	UpdateData(TRUE);

	if (m_number <= 0) {
		MessageBox(TEXT("购买的个数要大于0"));
		return;
	}

	// 验证大于库存量
	if (m_left < m_number) {
		MessageBox(TEXT("库存不足！"));
		return;
	}

	//购买
	int index = m_combo.GetCurSel();
	CString str;
	m_combo.GetLBText(index, str);

	CInfoFile file;
	file.ReadDocline();

	for (list<msg>::iterator it = file.ls.begin(); it != file.ls.end(); it++) {
		if (CString(it->name.c_str()) == str) {
			it->num -= m_number;
			m_left = it->num;
			//右侧信息提示
			CString info;
			info.Format(TEXT("商品：%s \r\n单价：%d \r\n个数：%d \r\n总价：%d"), str, m_price, m_number, m_price * m_number);
			m_sellInfo = info;
			//下次再买 从0开始
			m_number = 0;

			UpdateData(FALSE);
			MessageBox(TEXT("购买成功!"));
			break;
		}
	}

	//显示的内容清空
	m_sellInfo.Empty();

	//将数据写到文件中
	file.WirteDocline();

	UpdateData(FALSE);
}


void CSellDlg::OnBnClickedButton5()
{
	// TODO: 在此添加控件通知处理程序代码
	m_number = 0;
	UpdateData(FALSE);

	m_combo.SetCurSel(0);
	OnCbnSelchangeCombo1();
}
