#include "eleDlg.h"
#include "json_tool.h"
#include "mydefine.h"

#include <QMessageBox>

eleDlg::eleDlg(json_tool* jt, QWidget *parent) : QWidget(parent)
{
	ui.setupUi(this);

	m_ipos = 0;
	m_bmodify = false;
	m_jt = jt;

	plabel_key = new QLabel(this);
	plabel_key->setText(QString::fromLocal8Bit("名    称："));
	plabel_key->move(INIT_X, INIT_Y+CTRL_HEIGHT+ CTRL_DISTANCE);
	plabel_key->adjustSize();
	plabel_key->setVisible(false);

	plabel_keyalias = new QLabel(this);
	plabel_keyalias->setText(QString::fromLocal8Bit("别    名："));
	plabel_keyalias->move(INIT_X, INIT_Y+ (CTRL_HEIGHT+CTRL_DISTANCE)*2);
	plabel_keyalias->adjustSize();
	plabel_keyalias->setVisible(false);
    plabel_keyhelp = new QLabel(this);
	plabel_keyhelp->setText(QString::fromLocal8Bit("描    述："));
	plabel_keyhelp->move(INIT_X, INIT_Y + (CTRL_HEIGHT+CTRL_DISTANCE) * 3);
	plabel_keyhelp->adjustSize();
	plabel_keyhelp->setVisible(false);

    pedit_key = new QLineEdit(this);
	pedit_key->move(CTRL_LEFT_X, INIT_Y+ (CTRL_HEIGHT + CTRL_DISTANCE)*1 - CTRL_SUB_HEIGHT);
	pedit_key->setGeometry(CTRL_LEFT_X, INIT_Y + (CTRL_HEIGHT + CTRL_DISTANCE) * 1 - CTRL_SUB_HEIGHT, CTRL_LENGTH, CTRL_HEIGHT);
	pedit_key->setText("");
	pedit_key->setVisible(false);
	pedit_keyalias = new QLineEdit(this);
	pedit_keyalias->move(CTRL_LEFT_X, INIT_Y + (CTRL_HEIGHT + CTRL_DISTANCE) * 2 - CTRL_SUB_HEIGHT);
	pedit_keyalias->setGeometry(CTRL_LEFT_X, INIT_Y + (CTRL_HEIGHT + CTRL_DISTANCE) * 2 - CTRL_SUB_HEIGHT, CTRL_LENGTH, CTRL_HEIGHT);
	pedit_keyalias->setText("");
	pedit_keyalias->setVisible(false);
	pedit_keyhelp = new QLineEdit(this);
	pedit_keyhelp->move(CTRL_LEFT_X, INIT_Y + (CTRL_HEIGHT + CTRL_DISTANCE) * 3 - CTRL_SUB_HEIGHT);
	pedit_keyhelp->setGeometry(CTRL_LEFT_X, INIT_Y + (CTRL_HEIGHT + CTRL_DISTANCE) * 3 - CTRL_SUB_HEIGHT, CTRL_LENGTH, CTRL_HEIGHT);
	pedit_keyhelp->setText("");
	pedit_keyhelp->setVisible(false);

	plable_ctlnum = new QLabel(this);
	plable_ctlnum->setText(QString::fromLocal8Bit("ctrl_num："));
	plable_ctlnum->move(INIT_X, INIT_Y + (CTRL_HEIGHT + CTRL_DISTANCE) * 4);
	plable_ctlnum->adjustSize();
	plable_ctlnum->setVisible(false);
	pcb_ctlnum = new QComboBox(this);
	pcb_ctlnum->addItem("1");
	pcb_ctlnum->addItem("2");
	pcb_ctlnum->addItem("3");
	pcb_ctlnum->move(CTRL_LEFT_X, INIT_Y + (CTRL_HEIGHT + CTRL_DISTANCE) * 4 - CTRL_SUB_HEIGHT);
	pcb_ctlnum->setGeometry(CTRL_LEFT_X, INIT_Y + (CTRL_HEIGHT + CTRL_DISTANCE) * 4 - CTRL_SUB_HEIGHT, CTRL_LENGTH, CTRL_HEIGHT);
	pcb_ctlnum->setCurrentIndex(0);
	pcb_ctlnum->setVisible(false);

	plabel_ctrl1_2 = new QLabel(this);
	plabel_ctrl1_2->setText(QString::fromLocal8Bit("ctrl1_value："));
	plabel_ctrl1_2->move(INIT_X, INIT_Y + (CTRL_HEIGHT + CTRL_DISTANCE) * 5);
	plabel_ctrl1_2->adjustSize();
	plabel_ctrl1_2->setVisible(false);

	plabel_ctrl1 = new QLabel(this);
	plabel_ctrl1->setText(QString::fromLocal8Bit("ctrl1_type："));
	plabel_ctrl1->move(INIT_X, INIT_Y + (CTRL_HEIGHT + CTRL_DISTANCE) * 6);
	plabel_ctrl1->adjustSize();
	plabel_ctrl1->setVisible(false);

	plabel_ctrl1_1 = new QLabel(this);
	plabel_ctrl1_1->setText(QString::fromLocal8Bit("ctrl1_items："));
	plabel_ctrl1_1->move(INIT_X, INIT_Y + (CTRL_HEIGHT + CTRL_DISTANCE) * 7);
	plabel_ctrl1_1->adjustSize();
	plabel_ctrl1_1->setVisible(false);

	plabel_ctrl2_2 = new QLabel(this);
	plabel_ctrl2_2->setText(QString::fromLocal8Bit("ctrl2_value："));
	plabel_ctrl2_2->move(INIT_X, INIT_Y + (CTRL_HEIGHT + CTRL_DISTANCE) * 8+ CTRL_DISTANCE);
	plabel_ctrl2_2->adjustSize();
	plabel_ctrl2_2->setVisible(false);

	plabel_ctrl2 = new QLabel(this);
	plabel_ctrl2->setText(QString::fromLocal8Bit("ctrl2_type："));
	plabel_ctrl2->move(INIT_X, INIT_Y + (CTRL_HEIGHT + CTRL_DISTANCE) * 9 + CTRL_DISTANCE);
	plabel_ctrl2->adjustSize();
	plabel_ctrl2->setVisible(false);

	plabel_ctrl2_1 = new QLabel(this);
	plabel_ctrl2_1->setText(QString::fromLocal8Bit("ctrl2_items："));
	plabel_ctrl2_1->move(INIT_X, INIT_Y + (CTRL_HEIGHT + CTRL_DISTANCE) * 10 + CTRL_DISTANCE);
	plabel_ctrl2_1->adjustSize();
	plabel_ctrl2_1->setVisible(false);

	plabel_ctrl3_2 = new QLabel(this);
	plabel_ctrl3_2->setText(QString::fromLocal8Bit("ctrl3_value："));
	plabel_ctrl3_2->move(INIT_X, INIT_Y + (CTRL_HEIGHT + CTRL_DISTANCE) * 11 + CTRL_DISTANCE*2);
	plabel_ctrl3_2->adjustSize();
	plabel_ctrl3_2->setVisible(false);

	plabel_ctrl3 = new QLabel(this);
	plabel_ctrl3->setText(QString::fromLocal8Bit("ctrl3_type："));
	plabel_ctrl3->move(INIT_X, INIT_Y + (CTRL_HEIGHT + CTRL_DISTANCE) * 12 + CTRL_DISTANCE * 2);
	plabel_ctrl3->adjustSize();
	plabel_ctrl3->setVisible(false);

	plabel_ctrl3_1 = new QLabel(this);
	plabel_ctrl3_1->setText(QString::fromLocal8Bit("ctrl3_items："));
	plabel_ctrl3_1->move(INIT_X, INIT_Y + (CTRL_HEIGHT + CTRL_DISTANCE) * 13 + CTRL_DISTANCE * 2);
	plabel_ctrl3_1->adjustSize();
	plabel_ctrl3_1->setVisible(false);

	pedit_ctrl1_2 = new QLineEdit(this);
	pedit_ctrl1_2->move(CTRL_LEFT_X, INIT_Y + (CTRL_HEIGHT + CTRL_DISTANCE) * 5- CTRL_SUB_HEIGHT);
	pedit_ctrl1_2->setGeometry(CTRL_LEFT_X, INIT_Y + (CTRL_HEIGHT + CTRL_DISTANCE) * 5 - CTRL_SUB_HEIGHT, CTRL_LENGTH, CTRL_HEIGHT);
	pedit_ctrl1_2->setVisible(false);

	pcom_ctrl1 = new QComboBox(this);
	pcom_ctrl1->addItem("int");
	pcom_ctrl1->addItem("float");
	pcom_ctrl1->addItem("string");
	pcom_ctrl1->addItem("bool");
	pcom_ctrl1->move(CTRL_LEFT_X, INIT_Y + (CTRL_HEIGHT + CTRL_DISTANCE) * 6 - CTRL_SUB_HEIGHT);
	pcom_ctrl1->setGeometry(CTRL_LEFT_X, INIT_Y + (CTRL_HEIGHT + CTRL_DISTANCE) * 6 - CTRL_SUB_HEIGHT, CTRL_LENGTH, CTRL_HEIGHT);
	pcom_ctrl1->setVisible(false);

	pedit_ctrl1_1 = new QLineEdit(this);
	pedit_ctrl1_1->move(CTRL_LEFT_X, INIT_Y + (CTRL_HEIGHT + CTRL_DISTANCE) * 7 - CTRL_SUB_HEIGHT);
	pedit_ctrl1_1->setGeometry(CTRL_LEFT_X, INIT_Y + (CTRL_HEIGHT + CTRL_DISTANCE) * 7 - CTRL_SUB_HEIGHT, CTRL_LENGTH, CTRL_HEIGHT);
	pedit_ctrl1_1->setVisible(false);

	pedit_ctrl2_2 = new QLineEdit(this);
	pedit_ctrl2_2->move(CTRL_LEFT_X, INIT_Y + (CTRL_HEIGHT + CTRL_DISTANCE) * 8 - CTRL_SUB_HEIGHT + CTRL_DISTANCE);
	pedit_ctrl2_2->setGeometry(CTRL_LEFT_X, INIT_Y + (CTRL_HEIGHT + CTRL_DISTANCE) * 8 - CTRL_SUB_HEIGHT + CTRL_DISTANCE, CTRL_LENGTH, CTRL_HEIGHT);
	pedit_ctrl2_2->setVisible(false);

	pcom_ctrl2 = new QComboBox(this);
	pcom_ctrl2->addItem("int");
	pcom_ctrl2->addItem("float");
	pcom_ctrl2->addItem("string");
	pcom_ctrl2->addItem("bool");
	pcom_ctrl2->move(CTRL_LEFT_X, INIT_Y + (CTRL_HEIGHT + CTRL_DISTANCE) * 9 - CTRL_SUB_HEIGHT + CTRL_DISTANCE);
	pcom_ctrl2->setGeometry(CTRL_LEFT_X, INIT_Y + (CTRL_HEIGHT + CTRL_DISTANCE) * 9 - CTRL_SUB_HEIGHT + CTRL_DISTANCE, CTRL_LENGTH, CTRL_HEIGHT);
	pcom_ctrl2->setVisible(false);

	pedit_ctrl2_1 = new QLineEdit(this);
	pedit_ctrl2_1->move(CTRL_LEFT_X, INIT_Y + (CTRL_HEIGHT + CTRL_DISTANCE) * 10 - CTRL_SUB_HEIGHT + CTRL_DISTANCE);
	pedit_ctrl2_1->setGeometry(CTRL_LEFT_X, INIT_Y + (CTRL_HEIGHT + CTRL_DISTANCE) * 10 - CTRL_SUB_HEIGHT + CTRL_DISTANCE, CTRL_LENGTH, CTRL_HEIGHT);
	pedit_ctrl2_1->setVisible(false);

	pedit_ctrl3_2 = new QLineEdit(this);
	pedit_ctrl3_2->move(CTRL_LEFT_X, INIT_Y + (CTRL_HEIGHT + CTRL_DISTANCE) * 11 - CTRL_SUB_HEIGHT + CTRL_DISTANCE*2);
	pedit_ctrl3_2->setGeometry(CTRL_LEFT_X, INIT_Y + (CTRL_HEIGHT + CTRL_DISTANCE) * 11 - CTRL_SUB_HEIGHT + CTRL_DISTANCE * 2, CTRL_LENGTH, CTRL_HEIGHT);
	pedit_ctrl3_2->setVisible(false);

	pcom_ctrl3 = new QComboBox(this);
	pcom_ctrl3->addItem("int");
	pcom_ctrl3->addItem("float");
	pcom_ctrl3->addItem("string");
	pcom_ctrl3->addItem("bool");
	pcom_ctrl3->move(CTRL_LEFT_X, INIT_Y + (CTRL_HEIGHT + CTRL_DISTANCE) * 12 - CTRL_SUB_HEIGHT + CTRL_DISTANCE * 2);
	pcom_ctrl3->setGeometry(CTRL_LEFT_X, INIT_Y + (CTRL_HEIGHT + CTRL_DISTANCE) * 12 - CTRL_SUB_HEIGHT + CTRL_DISTANCE * 2, CTRL_LENGTH, CTRL_HEIGHT);
	pcom_ctrl3->setVisible(false);

	pedit_ctrl3_1 = new QLineEdit(this);
	pedit_ctrl3_1->move(CTRL_LEFT_X, INIT_Y + (CTRL_HEIGHT + CTRL_DISTANCE) * 13 - CTRL_SUB_HEIGHT + CTRL_DISTANCE * 2);
	pedit_ctrl3_1->setGeometry(CTRL_LEFT_X, INIT_Y + (CTRL_HEIGHT + CTRL_DISTANCE) * 13 - CTRL_SUB_HEIGHT + CTRL_DISTANCE * 2, CTRL_LENGTH, CTRL_HEIGHT);
	pedit_ctrl3_1->setVisible(false);

	ui.m_eletype->addItem("node");
	ui.m_eletype->addItem("object");
	ui.m_eletype->addItem("array");
	ui.m_eletype->addItem("m_ctrl");    //一行有多个控件
	ui.m_eletype->addItem("smooth");    //平滑因子 类型节点
	ui.m_eletype->addItem("template");  //模板类型节点
	
	ui.m_valuetype->addItem("int");
	ui.m_valuetype->addItem("float");
	ui.m_valuetype->addItem("string");
	ui.m_valuetype->addItem("bool");

	connect(ui.m_ok, SIGNAL(clicked()), jt, SLOT(eledlg_OKClicked()));                    //确定按钮
	connect(ui.m_cancel, SIGNAL(clicked()), jt, SLOT(eledlg_CancelClicked()));            //取消按钮

	connect(ui.m_eletype, SIGNAL(activated(int)), this, SLOT(IsTypeChanged()));
	connect(ui.m_valuetype, SIGNAL(activated(int)), this, SLOT(IsValuetypeChanged()));

	connect(ui.m_value, SIGNAL(textChanged(const QString&)), this, SLOT(valueChanged(const QString&)));
	connect(pcb_ctlnum, SIGNAL(activated(const QString&)), this, SLOT(ctlnumChanged(const QString&)));

	connect(pcom_ctrl1, SIGNAL(activated(int)), this, SLOT(comctl1Changed()));
	connect(pcom_ctrl2, SIGNAL(activated(int)), this, SLOT(comctl2Changed()));
	connect(pcom_ctrl3, SIGNAL(activated(int)), this, SLOT(comctl3Changed()));

	connect(pedit_ctrl1_2, SIGNAL(textChanged(const QString&)), this, SLOT(ctrl1_2_valueChanged(const QString&)));
	connect(pedit_ctrl2_2, SIGNAL(textChanged(const QString&)), this, SLOT(ctrl2_2_valueChanged(const QString&)));
	connect(pedit_ctrl3_2, SIGNAL(textChanged(const QString&)), this, SLOT(ctrl3_2_valueChanged(const QString&)));

	if ("node" == ui.m_eletype->currentText())
	{
		ui.m_elekey->setText("");
		ui.m_elekey->setEnabled(false);
		ui.m_elealias->setEnabled(false);
		ui.m_lele_key->setEnabled(false);
		ui.m_lele_alias->setEnabled(false);
	}
		
	QString scur_text = ui.m_valuetype->currentText();
	if ("int" == scur_text || "float" == scur_text)
	{
		ui.m_nodeitems->setText("");
		ui.m_nodeitems->setEnabled(false);
		ui.m_litems->setEnabled(false);
	}
	else if ("bool" == scur_text)
	{
		ui.m_nodeitems->setText("true;false");
		ui.m_nodeitems->setEnabled(false);
		ui.m_litems->setEnabled(false);
	}
	else if ("string" == scur_text)
		ui.m_nodeitems->setText("");
}

eleDlg::~eleDlg()
{
	//
}

QString eleDlg::get_eletype() const { return ui.m_eletype->currentText(); }
QString eleDlg::get_elekey() const { return ui.m_elekey->text(); }
QString eleDlg::get_elealias() const { return ui.m_elealias->text(); }
int eleDlg::get_eletype_count() const { return ui.m_eletype->count(); }

QString eleDlg::getkey() const { return ui.m_key->text(); }
QString eleDlg::getvalue() const { return ui.m_value->text(); }
QString eleDlg::get_valuetype() const { return ui.m_valuetype->currentText(); }
QString eleDlg::get_nodealias() const { return ui.m_nodealias->text(); }
QString eleDlg::get_nodehelp() const { return ui.m_nodehelp->text(); }
QString eleDlg::get_nodeitems() const { return ui.m_nodeitems->text(); }

QString eleDlg::get_mctrl_key() const { return pedit_key->text(); };
QString eleDlg::get_mctrl_keyalias() const { return pedit_keyalias->text(); }
QString eleDlg::get_mctrl_keyhelp() const { return pedit_keyhelp->text(); }
QString eleDlg::get_mctrl_num() const { return pcb_ctlnum->currentText(); }
QString eleDlg::get_mctrl1_value() const { return pedit_ctrl1_2->text(); }
QString eleDlg::get_mctrl2_value() const { return pedit_ctrl2_2->text(); }
QString eleDlg::get_mctrl3_value() const { return pedit_ctrl3_2->text(); }
QString eleDlg::get_mctrl1_type() const { return pcom_ctrl1->currentText(); }
QString eleDlg::get_mctrl2_type() const { return pcom_ctrl2->currentText();}
QString eleDlg::get_mctrl3_type() const { return pcom_ctrl3->currentText();}
QString eleDlg::get_mctrl1_items() const { return pedit_ctrl1_1->text(); }
QString eleDlg::get_mctrl2_items() const { return pedit_ctrl2_1->text(); }
QString eleDlg::get_mctrl3_items() const { return pedit_ctrl3_1->text(); }

void eleDlg::set_eletype(int index) { ui.m_eletype->setCurrentIndex(index); }
void eleDlg::set_elekey(string skey) { ui.m_elekey->setText(QString::fromLocal8Bit(skey.c_str())); }
void eleDlg::set_elealias(string stitle) { ui.m_elealias->setText(QString::fromLocal8Bit(stitle.c_str())); }
void eleDlg::setkey(string skey) { ui.m_key->setText(QString::fromLocal8Bit(skey.c_str())); }
void eleDlg::setvalue(string svalue) { ui.m_value->setText(QString::fromLocal8Bit(svalue.c_str())); }
void eleDlg::set_nodealias(string stitle) { ui.m_nodealias->setText(QString::fromLocal8Bit(stitle.c_str())); }
void eleDlg::set_nodehelp(string shelp) { ui.m_nodehelp->setText(QString::fromLocal8Bit(shelp.c_str())); }
void eleDlg::set_nodeitems(string sitems) { ui.m_nodeitems->setText(QString::fromLocal8Bit(sitems.c_str())); }
void eleDlg::set_mctrl_key(string skey) { pedit_key->setText(QString::fromLocal8Bit(skey.c_str())); }
void eleDlg::set_mctrl_keyalias(string salias) { pedit_keyalias->setText(QString::fromLocal8Bit(salias.c_str())); }
void eleDlg::set_mctrl_keyhelp(string shelp) { pedit_keyhelp->setText(QString::fromLocal8Bit(shelp.c_str())); }
void eleDlg::set_mctrlnum(int inum) { pcb_ctlnum->setCurrentIndex(inum-1); }
void eleDlg::set_mctrl1_value(string svalue) { pedit_ctrl1_2->setText(QString::fromLocal8Bit(svalue.c_str())); }
void eleDlg::set_mctrl2_value(string svalue){ pedit_ctrl2_2->setText(QString::fromLocal8Bit(svalue.c_str())); }
void eleDlg::set_mctrl3_value(string svalue){ pedit_ctrl3_2->setText(QString::fromLocal8Bit(svalue.c_str())); }
void eleDlg::set_mctrl1_items(string sitems) { pedit_ctrl1_1->setText(QString::fromLocal8Bit(sitems.c_str())); }
void eleDlg::set_mctrl2_items(string sitems) { pedit_ctrl2_1->setText(QString::fromLocal8Bit(sitems.c_str())); }
void eleDlg::set_mctrl3_items(string sitems) { pedit_ctrl3_1->setText(QString::fromLocal8Bit(sitems.c_str())); }

void eleDlg::set_eletype_enable(bool flag) { ui.m_eletype->setEnabled(flag); }
void eleDlg::set_elekey_enable(bool flag) { ui.m_elekey->setEnabled(flag); }
void eleDlg::set_elealias_enable(bool flag) { ui.m_elealias->setEnabled(flag); }
void eleDlg::set_key_enable(bool flag) { ui.m_key->setEnabled(flag); }
void eleDlg::set_value_enable(bool flag) { ui.m_value->setEnabled(flag); ui.m_lvalue->setEnabled(flag); }
void eleDlg::set_valuetype_enable(bool flag) { ui.m_valuetype->setEnabled(flag); ui.m_lvalue_type->setEnabled(flag); }
void eleDlg::set_alias_enable(bool flag) { ui.m_nodealias->setEnabled(flag); }
void eleDlg::set_help_enable(bool flag) { ui.m_nodehelp->setEnabled(flag); }
void eleDlg::set_items_enable(bool flag) { ui.m_nodeitems->setEnabled(flag); }

void eleDlg::set_lelekey_visible(bool visible) { ui.m_lele_key->setVisible(visible); }
void eleDlg::set_lelealias_visible(bool visible) { ui.m_lele_alias->setVisible(visible); }
void eleDlg::set_lkey_visible(bool visible) { ui.m_lkey->setVisible(visible); }
void eleDlg::set_lvalue_visible(bool visible) { ui.m_lvalue->setVisible(visible); }
void eleDlg::set_lvaluetype_visible(bool visible) { ui.m_lvalue_type->setVisible(visible); }
void eleDlg::set_lalias_visible(bool visible) { ui.m_lalias->setVisible(visible); }
void eleDlg::set_lhelp_visible(bool visible) { ui.m_lhelp->setVisible(visible); }
void eleDlg::set_litems_visible(bool visible) { ui.m_litems->setVisible(visible); }
void eleDlg::set_groupbox_visible(bool visible) { ui.m_groupBox->setVisible(visible); }
void eleDlg::set_elekey_visible(bool visible) { ui.m_elekey->setVisible(visible); }
void eleDlg::set_elealias_visible(bool visible) { ui.m_elealias->setVisible(visible); }
void eleDlg::set_key_visible(bool visible) { ui.m_key->setVisible(visible); }
void eleDlg::set_value_visible(bool visible) { ui.m_value->setVisible(visible); }
void eleDlg::set_valuetype_visible(bool visible) { ui.m_valuetype->setVisible(visible); }
void eleDlg::set_nodealias_visible(bool visible) { ui.m_nodealias->setVisible(visible); }
void eleDlg::set_nodehelp_visible(bool visible) { ui.m_nodehelp->setVisible(visible); }
void eleDlg::set_nodeitems_visible(bool visible) { ui.m_nodeitems->setVisible(visible); }

void eleDlg::set_label_key_visible(bool visible) { plabel_key->setVisible(visible); }
void eleDlg::set_label_keyalias_visible(bool visible) { plabel_keyalias->setVisible(visible); }
void eleDlg::set_label_keyhelp_visible(bool visible) { plabel_keyhelp->setVisible(visible); }
void eleDlg::set_edit_key_visible(bool visible) { pedit_key->setVisible(visible); }
void eleDlg::set_edit_keyalias_visible(bool visible) { pedit_keyalias->setVisible(visible); }
void eleDlg::set_edit_keyhelp_visible(bool visible) { pedit_keyhelp->setVisible(visible); }
void eleDlg::set_lctlnum_visible(bool visible) { plable_ctlnum->setVisible(visible); }
void eleDlg::set_ectlnum_visible(bool visible) { pcb_ctlnum->setVisible(visible); }
void eleDlg::set_lctrl1_visible(bool visible) { plabel_ctrl1->setVisible(visible); }
void eleDlg::set_lctrl1_1_visible(bool visible) { plabel_ctrl1_1->setVisible(visible); }
void eleDlg::set_lctrl1_2_visible(bool visible) { plabel_ctrl1_2->setVisible(visible); }
void eleDlg::set_lctrl2_visible(bool visible) { plabel_ctrl2->setVisible(visible); }
void eleDlg::set_lctrl2_1_visible(bool visible) { plabel_ctrl2_1->setVisible(visible); }
void eleDlg::set_lctrl2_2_visible(bool visible) { plabel_ctrl2_2->setVisible(visible); }
void eleDlg::set_lctrl3_visible(bool visible) { plabel_ctrl3->setVisible(visible); }
void eleDlg::set_lctrl3_1_visible(bool visible) { plabel_ctrl3_1->setVisible(visible); }
void eleDlg::set_lctrl3_2_visible(bool visible) { plabel_ctrl3_2->setVisible(visible); }
void eleDlg::set_cctrl1_visible(bool visible) { pcom_ctrl1->setVisible(visible); }
void eleDlg::set_cctrl1_1_visible(bool visible) { pedit_ctrl1_1->setVisible(visible); }
void eleDlg::set_cctrl1_2_visible(bool visible) { pedit_ctrl1_2->setVisible(visible); }
void eleDlg::set_cctrl2_visible(bool visible) { pcom_ctrl2->setVisible(visible); }
void eleDlg::set_cctrl2_1_visible(bool visible) { pedit_ctrl2_1->setVisible(visible); }
void eleDlg::set_cctrl2_2_visible(bool visible) { pedit_ctrl2_2->setVisible(visible); }
void eleDlg::set_cctrl3_visible(bool visible) { pcom_ctrl3->setVisible(visible); }
void eleDlg::set_cctrl3_1_visible(bool visible) { pedit_ctrl3_1->setVisible(visible); }
void eleDlg::set_cctrl3_2_visible(bool visible) { pedit_ctrl3_2->setVisible(visible); }

void eleDlg::set_ctlnum_enable(bool flag) { pcb_ctlnum->setEnabled(flag); }
void eleDlg::set_com_ctrl1_enable(bool flag) { pcom_ctrl1->setEnabled(flag); }
void eleDlg::set_edit_ctrl1_1_enable(bool flag) { pedit_ctrl1_1->setEnabled(flag); }
void eleDlg::set_edit_ctrl1_2_enable(bool flag) { pedit_ctrl1_2->setEnabled(flag); }
void eleDlg::set_com_ctrl2_enable(bool flag) { pcom_ctrl2->setEnabled(flag); }
void eleDlg::set_edit_ctrl2_1_enable(bool flag) { pedit_ctrl2_1->setEnabled(flag); }
void eleDlg::set_edit_ctrl2_2_enable(bool flag) { pedit_ctrl2_2->setEnabled(flag); }
void eleDlg::set_com_ctrl3_enable(bool flag) { pcom_ctrl3->setEnabled(flag); }
void eleDlg::set_edit_ctrl3_1_enable(bool flag) { pedit_ctrl3_1->setEnabled(flag); }
void eleDlg::set_edit_ctrl3_2_enable(bool flag) { pedit_ctrl3_2->setEnabled(flag); }

void eleDlg::show_hide_nodectrl(bool flag)
{
	set_lelekey_visible(flag);
	set_lelealias_visible(flag);
	set_lkey_visible(flag);
	set_lvalue_visible(flag);
	set_lvaluetype_visible(flag);
	set_lalias_visible(flag);
	set_lhelp_visible(flag);
	set_litems_visible(flag);
	set_groupbox_visible(flag);
	set_elekey_visible(flag);
	set_elealias_visible(flag);
	set_key_visible(flag);
	set_value_visible(flag);
	set_valuetype_visible(flag);
	set_nodealias_visible(flag);
	set_nodehelp_visible(flag);
	set_nodeitems_visible(flag);
}

void eleDlg::show_hide_mctrl(bool flag)
{
	set_label_key_visible(flag);
	set_label_keyalias_visible(flag);
	set_label_keyhelp_visible(flag);
	set_edit_key_visible(flag);
	set_edit_keyalias_visible(flag);
	set_edit_keyhelp_visible(flag);
	set_lctlnum_visible(flag);
	set_lctrl1_visible(flag);
	set_lctrl1_1_visible(flag);
	set_lctrl1_2_visible(flag);
	set_lctrl2_visible(flag);
	set_lctrl2_1_visible(flag);
	set_lctrl2_2_visible(flag);
	set_lctrl3_visible(flag);
	set_lctrl3_1_visible(flag);
	set_lctrl3_2_visible(flag);
	
	set_ectlnum_visible(flag);
	set_cctrl1_visible(flag);
	set_cctrl1_1_visible(flag);
	set_cctrl1_2_visible(flag);
	set_cctrl2_visible(flag);
	set_cctrl2_1_visible(flag);
	set_cctrl2_2_visible(flag);
	set_cctrl3_visible(flag);
	set_cctrl3_1_visible(flag);
	set_cctrl3_2_visible(flag);
}

void eleDlg::IsTypeChanged()
{
	int x = geometry().x();
	int y = geometry().y();
	int w = geometry().width();

	QString snodetype = ui.m_eletype->currentText();
	if ("node" == snodetype)
	{
		show_hide_mctrl(false);
		show_hide_nodectrl(true);

		ui.m_elekey->setEnabled(false);
		ui.m_elealias->setEnabled(false);
		ui.m_lele_key->setEnabled(false);
		ui.m_lele_alias->setEnabled(false);
		ui.m_value->setEnabled(true);
		ui.m_lvalue->setEnabled(true);
		ui.m_nodehelp->setEnabled(true);
		ui.m_lhelp->setEnabled(true);
		ui.m_lvalue->setText(QString::fromLocal8Bit("  值："));
	}	
	else if("object" == snodetype)
	{
		show_hide_mctrl(false);
		show_hide_nodectrl(true);

		ui.m_elekey->setEnabled(true);
		ui.m_elealias->setEnabled(true);
		ui.m_value->setEnabled(true);
		ui.m_lvalue->setEnabled(true);
		ui.m_lvalue->setText(QString::fromLocal8Bit("  值："));

		ui.m_lele_key->setEnabled(true);
		ui.m_lele_alias->setEnabled(true);
		ui.m_lele_key->setText(QString::fromLocal8Bit("对象名称："));
		ui.m_lele_alias->setText(QString::fromLocal8Bit("对象别名："));
		ui.m_nodehelp->setEnabled(true);
		ui.m_lhelp->setEnabled(true);
	}	
	else if ("array" == snodetype)
	{
		show_hide_mctrl(false);
		show_hide_nodectrl(true);

		ui.m_elekey->setEnabled(true);
		ui.m_elealias->setEnabled(true);
		ui.m_value->setEnabled(true);
		ui.m_lvalue->setEnabled(true);
		ui.m_lvalue->setText(QString::fromLocal8Bit("  值："));

		ui.m_lele_key->setEnabled(true);
		ui.m_lele_alias->setEnabled(true);
		ui.m_lele_key->setText(QString::fromLocal8Bit("数组名称："));
		ui.m_lele_alias->setText(QString::fromLocal8Bit("数组别名："));
		ui.m_nodehelp->setEnabled(true);
		ui.m_lhelp->setEnabled(true);
	}
	else if ("m_ctrl" == snodetype)
	{
		show_hide_mctrl(true);
		show_hide_nodectrl(false);

		pcb_ctlnum->setEnabled(true);
		pedit_ctrl1_2->setEnabled(true);
		pedit_ctrl2_2->setEnabled(true);
		pedit_ctrl3_2->setEnabled(true);

		ctlnumChanged("1");
	}
	else if ("smooth" == snodetype)
	{
		show_hide_mctrl(false);
		show_hide_nodectrl(true);

		ui.m_elekey->setEnabled(false);
		ui.m_elealias->setEnabled(false);
		ui.m_lele_key->setEnabled(false);
		ui.m_lele_alias->setEnabled(false);
		ui.m_value->setEnabled(false);
		ui.m_lvalue->setEnabled(false);
		ui.m_nodehelp->setEnabled(true);
		ui.m_lhelp->setEnabled(true);
		ui.m_lvalue->setText(QString::fromLocal8Bit("  值："));
	}
	else if ("template" == snodetype)
	{
		show_hide_mctrl(false);
		show_hide_nodectrl(true);

		ui.m_elekey->setEnabled(false);
		ui.m_elealias->setEnabled(false);
		ui.m_lele_key->setEnabled(false);
		ui.m_lele_alias->setEnabled(false);
		ui.m_value->setEnabled(true);
		ui.m_lvalue->setEnabled(true);	
		ui.m_nodehelp->setEnabled(false);
		ui.m_lhelp->setEnabled(false);
		ui.m_lvalue->setText(QString::fromLocal8Bit("模板："));
	}

	if ("m_ctrl" != snodetype)
	{
		setGeometry(x, y, w, 375);
		ui.m_ok->move(102, 345);
		ui.m_cancel->move(243, 345);
	}

	ui.m_elekey->setText("");
	IsValuetypeChanged();
}

void eleDlg::IsValuetypeChanged()
{
	if ("template" == ui.m_eletype->currentText())
		return;

	QString snodetype = ui.m_valuetype->currentText();
	if ("int" == snodetype || "float" == snodetype)
	{
		ui.m_nodeitems->setText("");
		ui.m_nodeitems->setEnabled(false);
		ui.m_litems->setEnabled(false);
	}
	else if ("bool" == snodetype)
	{
		ui.m_nodeitems->setText("true;false");
		ui.m_nodeitems->setEnabled(false);
		ui.m_litems->setEnabled(false);
	}
	else if ("string" == snodetype)
	{
		ui.m_nodeitems->setText("");
		ui.m_nodeitems->setEnabled(true);
		ui.m_litems->setEnabled(true);
	}		
}

void eleDlg::valueChanged(const QString& svalue)
{
	int number_count = 0;
	int char_count = 0;
	int symbol_count = 0;
	QChar ch_ele = '\0';

	if ("true" == svalue || "TRUE" == svalue || "True" == svalue ||
		"false" == svalue || "FALSE" == svalue || "False" == svalue)
	{
		ui.m_valuetype->setCurrentIndex(3);
		IsValuetypeChanged();
		return;
	}

	int ilen = svalue.length();
	for (int i = 0; i < ilen; ++i)
	{
		ch_ele = svalue.at(i);
		if (ch_ele >= '0' && ch_ele <= '9')
			++number_count;
		else if ('.' == ch_ele)
			++symbol_count;
		else
			++char_count;
	}

	if (number_count == ilen)
		ui.m_valuetype->setCurrentIndex(0);
	else if (number_count + symbol_count == ilen)
		ui.m_valuetype->setCurrentIndex(1);
	else 
		ui.m_valuetype->setCurrentIndex(2);

	IsValuetypeChanged();
}

void eleDlg::ctrl1_2_valueChanged(const QString& svalue)
{
	int number_count = 0;
	int char_count = 0;
	int symbol_count = 0;
	QChar ch_ele = '\0';

	if ("true" == svalue || "TRUE" == svalue || "True" == svalue ||
		"false" == svalue || "FALSE" == svalue || "False" == svalue)
	{
		pcom_ctrl1->setCurrentIndex(3);
		comctl1Changed();
		return;
	}

	int ilen = svalue.length();
	for (int i = 0; i < ilen; ++i)
	{
		ch_ele = svalue.at(i);
		if (ch_ele >= '0' && ch_ele <= '9')
			++number_count;
		else if ('.' == ch_ele)
			++symbol_count;
		else
			++char_count;
	}

	if (number_count == ilen)
		pcom_ctrl1->setCurrentIndex(0);
	else if (number_count + symbol_count == ilen)
		pcom_ctrl1->setCurrentIndex(1);
	else
		pcom_ctrl1->setCurrentIndex(2);

	comctl1Changed();
}

void eleDlg::ctrl2_2_valueChanged(const QString& svalue)
{
	int number_count = 0;
	int char_count = 0;
	int symbol_count = 0;
	QChar ch_ele = '\0';

	if ("true" == svalue || "TRUE" == svalue || "True" == svalue ||
		"false" == svalue || "FALSE" == svalue || "False" == svalue)
	{
		pcom_ctrl2->setCurrentIndex(3);
		comctl2Changed();
		return;
	}

	int ilen = svalue.length();
	for (int i = 0; i < ilen; ++i)
	{
		ch_ele = svalue.at(i);
		if (ch_ele >= '0' && ch_ele <= '9')
			++number_count;
		else if ('.' == ch_ele)
			++symbol_count;
		else
			++char_count;
	}

	if (number_count == ilen)
		pcom_ctrl2->setCurrentIndex(0);
	else if (number_count + symbol_count == ilen)
		pcom_ctrl2->setCurrentIndex(1);
	else
		pcom_ctrl2->setCurrentIndex(2);

	comctl2Changed();
}

void eleDlg::ctrl3_2_valueChanged(const QString& svalue)
{
	int number_count = 0;
	int char_count = 0;
	int symbol_count = 0;
	QChar ch_ele = '\0';

	if ("true" == svalue || "TRUE" == svalue || "True" == svalue ||
		"false" == svalue || "FALSE" == svalue || "False" == svalue)
	{
		pcom_ctrl3->setCurrentIndex(3);
		comctl3Changed();
		return;
	}

	int ilen = svalue.length();
	for (int i = 0; i < ilen; ++i)
	{
		ch_ele = svalue.at(i);
		if (ch_ele >= '0' && ch_ele <= '9')
			++number_count;
		else if ('.' == ch_ele)
			++symbol_count;
		else
			++char_count;
	}

	if (number_count == ilen)
		pcom_ctrl3->setCurrentIndex(0);
	else if (number_count + symbol_count == ilen)
		pcom_ctrl3->setCurrentIndex(1);
	else
		pcom_ctrl3->setCurrentIndex(2);

	comctl3Changed();
}

void eleDlg::clearCtrl()
{
	ui.m_elekey->setText("");
	ui.m_elealias->setText("");
	ui.m_value->setText("");
	ui.m_key->setText("");
	ui.m_nodealias->setText("");
	ui.m_nodehelp->setText("");
	ui.m_nodeitems->setText("");

	pedit_key->setText("");
	pedit_keyalias->setText("");
	pedit_keyhelp->setText("");

	pcom_ctrl1->setCurrentIndex(0);
	pedit_ctrl1_1->setText("");
	pedit_ctrl1_2->setText("");
	pcom_ctrl2->setCurrentIndex(0);
	pedit_ctrl2_1->setText("");
	pedit_ctrl2_2->setText("");
	pcom_ctrl3->setCurrentIndex(0);
	pedit_ctrl3_1->setText("");
	pedit_ctrl3_2->setText("");
}

void eleDlg::ctlnumChanged(const QString& svalue)
{
	/*int slen = svalue.length();
	if (slen <= 0)
		return;

	for (int i = 0; i < slen; ++i)
	{
		QChar chnum = svalue.at(i);
		if (chnum < '0' || chnum>'9')
		{
			QMessageBox::information(this, tr("Warning"), QString::fromLocal8Bit("请输入数字"));
			return;
		}
	}

	int icount = svalue.toInt();
	if (icount >= 4)
	{
		QMessageBox::information(this, tr("Warning"), QString::fromLocal8Bit("控件个数最多为3个"));
		return;
	}*/

	int x = geometry().x();
	int y = geometry().y();
	int w = geometry().width();
	//int h = geometry().height();

	int icount = svalue.toInt();
	switch (icount)
	{
	case 1:
	{
		pcb_ctlnum->setCurrentIndex(0);

		plabel_ctrl2->setVisible(false);
		plabel_ctrl2_1->setVisible(false);
		plabel_ctrl2_2->setVisible(false);
		pcom_ctrl2->setVisible(false);
		pedit_ctrl2_1->setVisible(false);
		pedit_ctrl2_2->setVisible(false);

		plabel_ctrl3->setVisible(false);
		plabel_ctrl3_1->setVisible(false);
		plabel_ctrl3_2->setVisible(false);
		pcom_ctrl3->setVisible(false);
		pedit_ctrl3_1->setVisible(false);
		pedit_ctrl3_2->setVisible(false);

		plabel_ctrl1->setVisible(true);
		pcom_ctrl1->setVisible(true);
		plabel_ctrl1_1->setVisible(true);
		pedit_ctrl1_1->setVisible(true);
		plabel_ctrl1_2->setVisible(true);
		pedit_ctrl1_2->setVisible(true);	
		pcom_ctrl1->setEnabled(false);
		if (m_bmodify)
		{
			pcb_ctlnum->setEnabled(false);
			//pedit_ctrl1_1->setEnabled(false);
			pedit_ctrl1_2->setEnabled(false);
		}
		else
		{
			pcb_ctlnum->setEnabled(true);
			pedit_ctrl1_2->setEnabled(true);
		}
		comctl1Changed();
		
		setGeometry(x, y, w, 391);
		ui.m_ok->move(102, 345);
		ui.m_cancel->move(243, 345);
	}
		break;
	case 2:
	{
		pcb_ctlnum->setCurrentIndex(1);

		plabel_ctrl3->setVisible(false);
		plabel_ctrl3_1->setVisible(false);
		plabel_ctrl3_2->setVisible(false);
		pcom_ctrl3->setVisible(false);
		pedit_ctrl3_1->setVisible(false);
		pedit_ctrl3_2->setVisible(false);

		plabel_ctrl1->setVisible(true);
		pcom_ctrl1->setVisible(true);
		plabel_ctrl1_1->setVisible(true);
		pedit_ctrl1_1->setVisible(true);
		plabel_ctrl1_2->setVisible(true);
		pedit_ctrl1_2->setVisible(true);

		plabel_ctrl2->setVisible(true);
		pcom_ctrl2->setVisible(true);
		plabel_ctrl2_1->setVisible(true);
		pedit_ctrl2_1->setVisible(true);
		plabel_ctrl2_2->setVisible(true);
		pedit_ctrl2_2->setVisible(true);
		
		pcom_ctrl1->setEnabled(false);
		pcom_ctrl2->setEnabled(false);
		if (m_bmodify)
		{
			pcb_ctlnum->setEnabled(false);
			//pedit_ctrl1_1->setEnabled(false);
			//pedit_ctrl2_1->setEnabled(false);
			pedit_ctrl1_2->setEnabled(false);
			pedit_ctrl2_2->setEnabled(false);
		}		
		else
		{
			pcb_ctlnum->setEnabled(true);
			pedit_ctrl1_2->setEnabled(true);
			pedit_ctrl2_2->setEnabled(true);		
		}
		comctl1Changed();
		comctl2Changed();

		setGeometry(x, y, w, 391);
		ui.m_ok->move(102, 345+8);
		ui.m_cancel->move(243, 345+8);
	}
		break;
	case 3:
	{
		pcb_ctlnum->setCurrentIndex(2);

		plabel_ctrl1->setVisible(true);
		pcom_ctrl1->setVisible(true);
		plabel_ctrl1_1->setVisible(true);
		pedit_ctrl1_1->setVisible(true);
		plabel_ctrl1_2->setVisible(true);
		pedit_ctrl1_2->setVisible(true);

		plabel_ctrl2->setVisible(true);
		pcom_ctrl2->setVisible(true);
		plabel_ctrl2_1->setVisible(true);
		pedit_ctrl2_1->setVisible(true);
		plabel_ctrl2_2->setVisible(true);
		pedit_ctrl2_2->setVisible(true);

		plabel_ctrl3->setVisible(true);
		pcom_ctrl3->setVisible(true);
		plabel_ctrl3_1->setVisible(true);
		pedit_ctrl3_1->setVisible(true);
		plabel_ctrl3_2->setVisible(true);
		pedit_ctrl3_2->setVisible(true);
		
		pcom_ctrl1->setEnabled(false);
		pcom_ctrl2->setEnabled(false);
		pcom_ctrl3->setEnabled(false);
		if (m_bmodify)
		{
			pcb_ctlnum->setEnabled(false);
			//pedit_ctrl1_1->setEnabled(false);
			//pedit_ctrl2_1->setEnabled(false);
			//pedit_ctrl3_1->setEnabled(false);
			pedit_ctrl1_2->setEnabled(false);
			pedit_ctrl2_2->setEnabled(false);
			pedit_ctrl3_2->setEnabled(false);
		}
		else
		{
			pcb_ctlnum->setEnabled(true);
			pedit_ctrl1_2->setEnabled(true);
			pedit_ctrl2_2->setEnabled(true);
			pedit_ctrl3_2->setEnabled(true);	
		}
		comctl1Changed();
		comctl2Changed();
		comctl3Changed();

		setGeometry(x, y, w,391+90+16);
		ui.m_ok->move(102, 345+90+16);
		ui.m_cancel->move(243,345+90+16);
	}
		break;
	default:
		break;
	}	
}

void eleDlg::comctl1Changed()
{
	QString snodetype = pcom_ctrl1->currentText();
	if ("int" == snodetype || "float" == snodetype)
	{
		pedit_ctrl1_1->setEnabled(false);
		pedit_ctrl1_1->setText("");
	}
	else if ("bool" == snodetype)
	{
		pedit_ctrl1_1->setEnabled(false);
		pedit_ctrl1_1->setText("true;false");
	}
	else if ("string" == snodetype)
	{
		pedit_ctrl1_1->setEnabled(true);
		if(!m_bmodify)
		    pedit_ctrl1_1->setText("");
	}	
}

void eleDlg::comctl2Changed()
{
	QString snodetype = pcom_ctrl2->currentText();
	if ("int" == snodetype || "float" == snodetype)
	{
		pedit_ctrl2_1->setEnabled(false);
		pedit_ctrl2_1->setText("");
	}
		
	else if ("bool" == snodetype)
	{
		pedit_ctrl2_1->setEnabled(false);
		pedit_ctrl2_1->setText("true;false");
	}
	else if ("string" == snodetype)
	{
		pedit_ctrl2_1->setEnabled(true);
		if(!m_bmodify)
		    pedit_ctrl2_1->setText("");
	}	
}

void eleDlg::comctl3Changed()
{
	QString snodetype = pcom_ctrl3->currentText();
	if ("int" == snodetype || "float" == snodetype)
	{
		pedit_ctrl3_1->setEnabled(false);
		pedit_ctrl3_1->setText("");
	}
		
	else if ("bool" == snodetype)
	{
		pedit_ctrl3_1->setEnabled(false);
		pedit_ctrl3_1->setText("true;false");
	}
	else if ("string" == snodetype)
	{
		pedit_ctrl3_1->setEnabled(true);
		if (!m_bmodify)
		    pedit_ctrl3_1->setText("");
	}		
}

void eleDlg::keyPressEvent(QKeyEvent* event)
{	
	if (event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return)
		m_jt->eledlg_OKClicked();
}

void eleDlg::deal_eletype(int flag)
{
	ui.m_eletype->clear();
	if (1 == flag)
	{
		ui.m_eletype->addItem("node");
		ui.m_eletype->addItem("object");
		ui.m_eletype->addItem("array");
		ui.m_eletype->addItem("m_ctrl");    //一行有多个控件
		ui.m_eletype->addItem("smooth");    //平滑因子 类型节点
		ui.m_eletype->addItem("template");  //模板类型节点
	}
	else if (2 == flag)
	{
		ui.m_eletype->addItem("node");
		ui.m_eletype->addItem("object");
		ui.m_eletype->addItem("array");
		ui.m_eletype->addItem("m_ctrl");    //一行有多个控件
		ui.m_eletype->addItem("smooth");    //平滑因子 类型节点
	}
	else if(3==flag)
		ui.m_eletype->addItem("template");  //模板类型节点
}