#pragma once

/*********************************************
//author: gaode
//create date: 2019-12-20
//function: 添加导航栏的窗体
*********************************************/

#include <QWidget>
#include "ui_navi_dlg.h"

class json_tool;

class navi_dlg : public QWidget
{
	Q_OBJECT

public:
	navi_dlg(json_tool *jt,QWidget *parent = Q_NULLPTR);
	~navi_dlg();

	void setkey(QString key) { ui.m_key->setText(key); }
	void setalias(QString alias) { ui.m_alias->setText(alias); }

	QString getkey() { return ui.m_key->text(); }
	QString getalias() { return ui.m_alias->text(); }

	void keyPressEvent(QKeyEvent* event);

private:
	Ui::navi_dlg ui;
	json_tool* m_jt;
};
