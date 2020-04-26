#pragma once

#include <QWidget>
#include "ui_eleDlg.h"

#include <cstring>
using namespace std;

class json_tool;

class eleDlg : public QWidget
{
	Q_OBJECT

public:
	eleDlg(json_tool *jt,QWidget *parent = Q_NULLPTR);
	~eleDlg();

	QString get_eletype() const;
	QString get_elekey() const;
	QString get_elealias() const;
	int get_eletype_count() const;

	QString getkey() const;
	QString getvalue() const;
	QString get_valuetype() const;
	QString get_nodealias() const;
	QString get_nodehelp() const;
	QString get_nodeitems() const;

	QString get_mctrl_key() const;
	QString get_mctrl_keyalias() const;
	QString get_mctrl_keyhelp() const;
	QString get_mctrl_num() const;
	QString get_mctrl1_value() const;
	QString get_mctrl2_value() const;
	QString get_mctrl3_value() const;
	QString get_mctrl1_type() const;
	QString get_mctrl2_type() const;
	QString get_mctrl3_type() const;
	QString get_mctrl1_items() const;
	QString get_mctrl2_items() const;
	QString get_mctrl3_items() const;

	void setpos(int pos) { m_ipos = pos; }
	int getpos()  const { return m_ipos; }
	void setmodify(bool flag) { m_bmodify = flag; }
	bool getmodify()  const { return m_bmodify; }

	void set_eletype(int index);
	void set_elekey(string skey);
	void set_elealias(string salias);
	void setkey(string skey);
	void setvalue(string svalue);
	void set_nodealias(string salias);
	void set_nodehelp(string shelp);
	void set_nodeitems(string sitems);
	void set_mctrl_key(string skey);
	void set_mctrl_keyalias(string salias);
	void set_mctrl_keyhelp(string shelp);
	void set_mctrlnum(int inum);
	void set_mctrl1_value(string svalue);
	void set_mctrl2_value(string svalue);
	void set_mctrl3_value(string svalue);
	void set_mctrl1_items(string sitems);
	void set_mctrl2_items(string sitems);
	void set_mctrl3_items(string sitems);

	void set_eletype_enable(bool flag);
	void set_elekey_enable(bool flag);
	void set_elealias_enable(bool flag);
	void set_key_enable(bool flag);
	void set_value_enable(bool flag);
	void set_valuetype_enable(bool flag);
	void set_alias_enable(bool flag);
	void set_help_enable(bool flag);
	void set_items_enable(bool flag);

	void set_lelekey_visible(bool visible);
	void set_lelealias_visible(bool visible);
	void set_lkey_visible(bool visible);
	void set_lvalue_visible(bool visible);
	void set_lvaluetype_visible(bool visible);
	void set_lalias_visible(bool visible);
	void set_lhelp_visible(bool visible);
	void set_litems_visible(bool visible);
	void set_groupbox_visible(bool visible);
	void set_elekey_visible(bool visible);
	void set_elealias_visible(bool visible);
	void set_key_visible(bool visible);
	void set_value_visible(bool visible);
	void set_valuetype_visible(bool visible);
	void set_nodealias_visible(bool visible);
	void set_nodehelp_visible(bool visible);
	void set_nodeitems_visible(bool visible);

	void set_label_key_visible(bool visible);
	void set_label_keyalias_visible(bool visible);
	void set_label_keyhelp_visible(bool visible);
	void set_edit_key_visible(bool visible);
	void set_edit_keyalias_visible(bool visible);
	void set_edit_keyhelp_visible(bool visible);
	void set_lctlnum_visible(bool visible);
	void set_ectlnum_visible(bool visible);
	void set_lctrl1_visible(bool visible);
	void set_lctrl1_1_visible(bool visible);
	void set_lctrl1_2_visible(bool visible);
	void set_lctrl2_visible(bool visible);
	void set_lctrl2_1_visible(bool visible);
	void set_lctrl2_2_visible(bool visible);
	void set_lctrl3_visible(bool visible);
	void set_lctrl3_1_visible(bool visible);
	void set_lctrl3_2_visible(bool visible);
	void set_cctrl1_visible(bool visible);
	void set_cctrl1_1_visible(bool visible);
	void set_cctrl1_2_visible(bool visible);
	void set_cctrl2_visible(bool visible);
	void set_cctrl2_1_visible(bool visible);
	void set_cctrl2_2_visible(bool visible);
	void set_cctrl3_visible(bool visible);
	void set_cctrl3_1_visible(bool visible);
	void set_cctrl3_2_visible(bool visible);

	void set_ctlnum_enable(bool flag);
	void set_com_ctrl1_enable(bool flag);
    void set_edit_ctrl1_1_enable(bool flag);
	void set_edit_ctrl1_2_enable(bool flag);
	void set_com_ctrl2_enable(bool flag);
	void set_edit_ctrl2_1_enable(bool flag);
	void set_edit_ctrl2_2_enable(bool flag);
	void set_com_ctrl3_enable(bool flag);
	void set_edit_ctrl3_1_enable(bool flag);
	void set_edit_ctrl3_2_enable(bool flag);

	void show_hide_nodectrl(bool flag);                  //显示或隐藏node,object,array类型时的控件
	void show_hide_mctrl(bool flag);                     //显示或隐藏m_ctrl 页面的控件

	void clearCtrl();                                    //控件内容置空
	void keyPressEvent(QKeyEvent* event);
	void deal_eletype(int flag);                         //flag 1-显示所有项 2-不显示template 3-只显示template

public slots:
	void IsTypeChanged();
	void IsValuetypeChanged();
	void valueChanged(const QString& svalue);
	void ctlnumChanged(const QString& svalue);
	void comctl1Changed();
	void comctl2Changed();
	void comctl3Changed();

	void ctrl1_2_valueChanged(const QString& svalue);
	void ctrl2_2_valueChanged(const QString& svalue);
	void ctrl3_2_valueChanged(const QString& svalue);

private:
	Ui::eleDlg ui;

	int m_ipos;
	bool m_bmodify;                  //标识当前打开的窗口是否是修改数据
	json_tool* m_jt;

	QLabel* plabel_key;
	QLabel* plabel_keyalias;
	QLabel* plabel_keyhelp;
	QLineEdit* pedit_key;
	QLineEdit* pedit_keyalias;
	QLineEdit* pedit_keyhelp;

	QLabel* plable_ctlnum;
	QComboBox* pcb_ctlnum;

	QLabel* plabel_ctrl1_2;
	QLabel* plabel_ctrl1;
	QLabel* plabel_ctrl1_1;
	QLabel* plabel_ctrl2_2;
	QLabel* plabel_ctrl2;
	QLabel* plabel_ctrl2_1;
	QLabel* plabel_ctrl3_2;
	QLabel* plabel_ctrl3;
	QLabel* plabel_ctrl3_1;

	QLineEdit* pedit_ctrl1_2;
	QComboBox* pcom_ctrl1;
	QLineEdit* pedit_ctrl1_1;
	QLineEdit* pedit_ctrl2_2;
	QComboBox* pcom_ctrl2;
	QLineEdit* pedit_ctrl2_1;
	QLineEdit* pedit_ctrl3_2;
	QComboBox* pcom_ctrl3;
	QLineEdit* pedit_ctrl3_1;
};
