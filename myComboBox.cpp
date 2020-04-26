#include "myComboBox.h"
#include "json_tool.h"

myComboBox::myComboBox(int index, m_node* pnode, QWidget* parent):QComboBox(parent)
{
	m_index = index;
	m_pnode = pnode;

	connect(this, SIGNAL(activated(const QString&)), this, SLOT(text_change(const QString&)));

}

myComboBox::~myComboBox()
{
	//
}

void myComboBox::text_change(const QString& svalue)
{
	QString sitems=""; 
	vector<QString>  vlist;
	QStringList ql,ql_1;
	if (1 == m_index && m_pnode->pcombox2)
	{
		m_pnode->pcombox2->clear();
		sitems = QString::fromLocal8Bit(m_pnode->ctrl2_items.c_str());
		ql = sitems.split(";");
		for (auto iter : ql)
		{
			ql_1 = iter.split("-");
			if (svalue == ql_1.at(0))
				m_pnode->pcombox2->addItem(ql_1.at(1));
		}
		m_pnode->pcombox2->setCurrentIndex(0);
	}
	else if (2 == m_index && m_pnode->pcombox3)
	{
		m_pnode->pcombox3->clear();
		sitems = QString::fromLocal8Bit(m_pnode->ctrl3_items.c_str());
		ql = sitems.split(";");
		for (auto iter : ql)
		{
			ql_1 = iter.split("-");
			if (svalue == ql_1.at(0))
				m_pnode->pcombox3->addItem(ql_1.at(1));
		}
		m_pnode->pcombox3->setCurrentIndex(0);
	}	
}