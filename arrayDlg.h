#pragma once

#include <QWidget>
#include "ui_arrayDlg.h"

#include <QLabel>

#include <deque>
using namespace std;

class json_tool;
class m_node;

class arrayDlg : public QWidget
{
	Q_OBJECT

public:
	arrayDlg(json_tool* jt, QWidget *parent = Q_NULLPTR);
	~arrayDlg();

	//���촰����棬������m_array_qjson �еĽڵ�
	void add_array_objectnode(int icount,deque<m_node*> &m_qjson);
	//���¿ؼ������ݵ�m_array_qjson->value ��
	void update_ctltojson();  
	//��m_array_qjson �����ݲ��뵽m_qjson��
	void insert_nodetojson(deque<m_node*> &m_qjson);
	//�ͷ�m_dlabel�ڴ�
	void release_label();

	void keyPressEvent(QKeyEvent* event);
public:
	void setpos(int pos) { m_ipos = pos; }

private:
	Ui::arrayDlg ui;

	json_tool* m_jt;
	int m_ipos;

	deque<QLabel*> m_dlabel;
	deque<m_node*> m_array_qjson;
	deque<m_node*> m_parent_node;    //���浱�����ڵ�Ľڵ㣬����Ҫ�����ͷſؼ�
};
