#include "CMyButton.h"
#include "json_tool.h"


CMyButton::CMyButton(QString name/*, int index*/, QWidget* parent): QPushButton(parent)
{
	m_parent = parent;
	m_act_del = nullptr;

	if("+"==name || "-"==name)
		connect(this, SIGNAL(clicked()), this, SLOT(add_sub()));
	else if("(+)" == name)
	    connect(this, SIGNAL(clicked()), this, SLOT(add()));
	else if("(-)" == name)
		connect(this, SIGNAL(clicked()), this, SLOT(sub()));
	else if("(m)" == name)
		connect(this, SIGNAL(clicked()), this, SLOT(modify()));
	else if("bt_open"==name)
		connect(this, SIGNAL(clicked()), this, SLOT(bt_open()));
	else if ("bt_close" == name)
		connect(this, SIGNAL(clicked()), this, SLOT(bt_close()));
	else if ("bt_higher" == name)
		connect(this, SIGNAL(clicked()), this, SLOT(bt_higher()));
	else if ("bt_lower" == name)
		connect(this, SIGNAL(clicked()), this, SLOT(bt_lower()));
	else if ("" == name)
		connect(this, SIGNAL(clicked()), this, SLOT(bt_template()));
	else if("preview" == name)
		connect(this, SIGNAL(clicked()), this, SLOT(bt_preview()));
	else     //导航栏按钮
	{
		connect(this, SIGNAL(clicked()), this, SLOT(deal_navi_btn()));
		m_act_del = new QAction("delete", this);
		addAction(m_act_del);
		setContextMenuPolicy(Qt::ActionsContextMenu);
		connect(m_act_del, SIGNAL(triggered(bool)), this, SLOT(del_navibtn()));
	}
		
	//m_ipos = index;
	m_itype = 0;
	m_index = 0;
	m_jt = nullptr;
}

CMyButton::~CMyButton()
{
	if (m_act_del)
	{
		delete m_act_del;
		m_act_del = nullptr;
	}
}

void CMyButton::add()
{
	((json_tool*)m_jt)->add_ele(m_itype,m_ipos);
}

void CMyButton::sub()
{
	((json_tool*)m_jt)->sub_ele(m_ipos);
}

void CMyButton::modify()
{
	((json_tool*)m_jt)->mod_ele(m_ipos);
}

void CMyButton::deal_navi_btn()
{
	//设置导航栏当前按钮背景为乳白色
	setStyleSheet("QPushButton{font-family:'宋体';font-size:22px;color:rgb(0,0,255);}QPushButton{background-color:rgb(255,251,240)}QPushButton:hover{background-color:rgb(255,251,240)}");

	((json_tool*)m_jt)->deal_navi_currbtn(m_index);
}

void CMyButton::add_sub()
{
	((json_tool*)m_jt)->add_sub(m_ipos);
}

void CMyButton::bt_open()
{
	((json_tool*)m_jt)->smooth_open(m_ipos);
}

void CMyButton::bt_close()
{
	((json_tool*)m_jt)->smooth_close(m_ipos);
}

void CMyButton::bt_higher()
{
	((json_tool*)m_jt)->smooth_higher(m_ipos);
}

void CMyButton::bt_lower()
{
	((json_tool*)m_jt)->smooth_lower(m_ipos);
}

void CMyButton::bt_template()
{
	((json_tool*)m_jt)->bt_template(m_ipos);
}

void CMyButton::bt_preview()
{
	((json_tool*)m_jt)->bt_preview(m_ipos);
}

void CMyButton::del_navibtn()
{
	((json_tool*)m_jt)->del_navibtn(m_index);
}