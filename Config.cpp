#include "Config.h"

#include <QtCore/QtCore>

Config::Config(QString filename)
{
	if (filename.isEmpty())
		m_qstrFileName = QCoreApplication::applicationDirPath() + "/config.ini";
	else
		m_qstrFileName = filename;

	m_psetting = new QSettings(m_qstrFileName, QSettings::IniFormat);
}

Config::~Config()
{
	delete m_psetting;
	m_psetting = nullptr;
}

void Config::Set(QString qstrnodename, QString qstrkeyname, QVariant qvarvalue)
{
	m_psetting->setValue(QString("/%1/%2").arg(qstrnodename).arg(qstrkeyname), qvarvalue);
}

QVariant Config::Get(QString qstrnodename, QString qstrkeyname)
{
	QVariant qvar = m_psetting->value(QString("/%1/%2").arg(qstrnodename).arg(qstrkeyname));
	return qvar;
}