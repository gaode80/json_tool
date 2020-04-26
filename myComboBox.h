#pragma once
#include <QComboBox>

class m_node;

class myComboBox : public QComboBox
{
	Q_OBJECT

public:
	myComboBox(int index,m_node* pnode, QWidget* parent);
	~myComboBox();

public slots:
	void text_change(const QString& svalue);

private:
	int m_index;
	struct m_node* m_pnode;
};

