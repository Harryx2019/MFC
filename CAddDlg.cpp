// CAddDlg.cpp: 实现文件
//

#include "pch.h"
#include "SaleSystem.h"
#include "CAddDlg.h"
#include "CInfoFile.h"


// CAddDlg

IMPLEMENT_DYNCREATE(CAddDlg, CFormView)

CAddDlg::CAddDlg()
	: CFormView(DIALOG_ADD)
	, m_price1(0)
	, m_num1(0)
	, m_name2(_T(""))
	, m_price2(0)
	, m_num2(0)
{

}

CAddDlg::~CAddDlg()
{
}

void CAddDlg::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_combo);
	DDX_Text(pDX, IDC_EDIT1, m_price1);
	DDX_Text(pDX, IDC_EDIT2, m_num1);
	DDX_Text(pDX, IDC_EDIT6, m_name2);
	DDX_Text(pDX, IDC_EDIT4, m_price2);
	DDX_Text(pDX, IDC_EDIT5, m_num2);
}

BEGIN_MESSAGE_MAP(CAddDlg, CFormView)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CAddDlg::OnCbnSelchangeCombo1)
	ON_BN_CLICKED(IDC_BUTTON1, &CAddDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON6, &CAddDlg::OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON7, &CAddDlg::OnBnClickedButton7)
	ON_BN_CLICKED(IDC_BUTTON8, &CAddDlg::OnBnClickedButton8)
END_MESSAGE_MAP()


// CAddDlg 诊断

#ifdef _DEBUG
void CAddDlg::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CAddDlg::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CAddDlg 消息处理程序


void CAddDlg::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: 在此添加专用代码和/或调用基类
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


void CAddDlg::OnCbnSelchangeCombo1()
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
			m_price1 = it->price;
			UpdateData(FALSE);
			break;
		}
	}
}


void CAddDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	// 添加商品个数按钮点击事件

	UpdateData(TRUE);
	//验证数据
	if (m_num1 <= 0) {
		MessageBox(TEXT("商品数不能小于0！"));
		return;
	}

	// 获取添加的商品的名称
	CString name;
	int index = m_combo.GetCurSel();
	m_combo.GetLBText(index,name);

	CInfoFile file;
	file.ReadDocline();

	for (list<msg>::iterator it = file.ls.begin(); it != file.ls.end(); it++) {
		if (CString(it->name.c_str()) == name) {
			//增加库存量
			it->num += m_num1;
			m_num1 = 0;
			UpdateData(FALSE);
			MessageBox(TEXT("添加成功!"));
			break;
		}
	}

	file.WirteDocline();
}


void CAddDlg::OnBnClickedButton6()
{
	// TODO: 在此添加控件通知处理程序代码
	m_num1 = 0;
	UpdateData(FALSE);

	m_combo.SetCurSel(0);
	OnCbnSelchangeCombo1();
}


void CAddDlg::OnBnClickedButton7()
{
	// TODO: 在此添加控件通知处理程序代码
	//添加新商品按钮事件

	UpdateData(TRUE);

	if (m_name2.IsEmpty() || m_price2 <= 0 || m_num2 <= 0) {
		MessageBox(TEXT("输入信息有误"));
		return;
	}

	CInfoFile file;
	file.ReadDocline();
	file.Addline(m_name2, m_num2, m_price2);

	//数据同步到文件中
	file.WirteDocline();

	MessageBox(TEXT("添加新商品成功!"));
	m_combo.InsertString(0, m_name2);
	m_combo.SetCurSel(0);
	OnCbnSelchangeCombo1();

	m_name2.Empty();
	m_num2 = 0;
	m_price2 = 0;
	UpdateData(FALSE);
}


void CAddDlg::OnBnClickedButton8()
{
	// TODO: 在此添加控件通知处理程序代码
	m_name2.Empty();
	m_num2 = 0;
	m_price2 = 0;
	UpdateData(FALSE);
}
