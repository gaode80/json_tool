#pragma once

#include <QString>
#include <QVariant>
#include <QSettings>

class Config
{
public:
	Config(QString filename="");
	~Config();

	void Set(QString qstrnodename, QString qstrkeyname, QVariant qvarvalue);
	QVariant Get(QString qstrnodename, QString qstrkeyname);

private:
	QString m_qstrFileName;
	QSettings* m_psetting;
};

