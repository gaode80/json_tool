#pragma once
//#include "parse_json_tool.h"

#include <QPushButton>

class json_tool;

class CMyButton :public QPushButton
{
	Q_OBJECT

public:
	CMyButton(QString name/*,int index*/,QWidget *parent=(QWidget*)nullptr);
	~CMyButton();

public:
	void set_jsontool(json_tool* jt) { m_jt = jt; }

	void set_type(int type) { m_itype = type; }
	int get_type() const { return m_itype; }

	void set_index(int index) { m_index = index; }
	int get_index() const { return m_index; }

	void set_pos(int pos) { m_ipos = pos; }
	int get_pos() const { return m_ipos; }

public slots:
	void add();
	void sub();
	void modify();
	void deal_navi_btn();
	void add_sub();
	void bt_open();
	void bt_close();
	void bt_higher();
	void bt_lower();
	void bt_template();
	void bt_preview();

	void del_navibtn();

private:
	int m_ipos;
	int m_itype;          //��ť���� 1--�������İ�ť 2--�������ұߵ�+��ť 3--Ԫ�غ����+��ť 4--��������ĳ������+��ť 5--ƽ�����ӵײ���ť
	int m_index;          //����(�������ϵİ�ť���˱�����Ч)

	json_tool *m_jt;

	QWidget* m_parent;

	QAction* m_act_del;
};

