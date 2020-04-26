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

	//构造窗体界面，并生成m_array_qjson 中的节点
	void add_array_objectnode(int icount,deque<m_node*> &m_qjson);
	//更新控件的内容到m_array_qjson->value 里
	void update_ctltojson();  
	//将m_array_qjson 的内容插入到m_qjson中
	void insert_nodetojson(deque<m_node*> &m_qjson);
	//释放m_dlabel内存
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
	deque<m_node*> m_parent_node;    //保存当做父节点的节点，不需要负责释放控件
};
