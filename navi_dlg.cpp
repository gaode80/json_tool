#include "navi_dlg.h"
#include "json_tool.h"

navi_dlg::navi_dlg(json_tool* jt, QWidget *parent): QWidget(parent)
{
	ui.setupUi(this);
	m_jt = jt;

	connect(ui.m_ok, SIGNAL(clicked()), jt, SLOT(navidlg_OKClicked()));                    //确定按钮
	connect(ui.m_cancel, SIGNAL(clicked()), jt, SLOT(navidlg_CancelClicked()));            //取消按钮
	connect(ui.m_ok, SIGNAL(returnPressed()), jt, SLOT(navidlg_OKClicked()));

	//ui.m_ok->setShortcut(Qt::Key_Enter);
	ui.m_key->setText("");
	ui.m_alias->setText("");
}

navi_dlg::~navi_dlg()
{
	//
}

void navi_dlg::keyPressEvent(QKeyEvent* event)
{
	if (event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return)
		m_jt->navidlg_OKClicked();
}
