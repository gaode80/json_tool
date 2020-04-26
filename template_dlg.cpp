#include "template_dlg.h"

#include <QWebEngineView>
#include <QFile>
#include <QUrl>

template_dlg::template_dlg(QWidget *parent): QWidget(parent)
{
	ui.setupUi(this);

	m_template_name = "";
}

template_dlg::~template_dlg()
{
	//
}

void template_dlg::show_template(string sname)
{
	QString htmlPath = QCoreApplication::applicationDirPath() + "/html/" + QString::fromLocal8Bit(sname.c_str());
	QUrl baseUrl = QCoreApplication::applicationDirPath() + "/html/";

	QFile file(htmlPath);
	if (!file.open(QIODevice::ReadOnly))
	{
		return;
	}

	QString htmlData = file.readAll().constData();
	file.close();
	ui.webEngineView->setHtml(htmlData, baseUrl);
}

