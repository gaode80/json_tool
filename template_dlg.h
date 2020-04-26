#pragma once

#include <QWidget>
#include "ui_template_dlg.h"

#include <string>
using namespace std;

class template_dlg : public QWidget
{
	Q_OBJECT

public:
	template_dlg(QWidget *parent = Q_NULLPTR);
	~template_dlg();

	void set_template_name(string sname) { m_template_name = sname; }
	void show_template(string sname);

private:
	Ui::template_dlg ui;

	string m_template_name;
};
