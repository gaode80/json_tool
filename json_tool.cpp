#include "json_tool.h"
#include "mydefine.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QPainter>

/*
1、导航栏右边的那个单独 + 按钮，保存在m_qjson里节点类型为1的p_btadd变量里
2、右键导航栏按钮时，没有删除导航栏按钮的CMyButton指针，因为此按钮有右键菜单，删除时报错
*/
json_tool::json_tool(QWidget *parent): QMainWindow(parent)
{
	QIcon icon("./Resources/json_tool.png");
	this->setWindowIcon(icon);

	ui.setupUi(this);

	this->move(550, 50);
	m_navibtn_count = 0;
	m_icurrbtn_index = 0;
	m_iprebtn_index = 0;

	m_fp = nullptr;

	Config cfg;
	QString show_btn = cfg.Get("basic", "show_btn").toString();

	ui.m_navi_scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);  //水平滚动条隐藏
	ui.centralWidget->setStyleSheet("background-color: rgb(255,251,240)");
	//begin 菜单  
	m_pmenu = menuBar()->addMenu(QString::fromLocal8Bit("文件"));

	m_showbtn = new QAction(this);
	m_showbtn->setText(QString::fromLocal8Bit("隐藏按钮"));
	m_showbtn->setStatusTip(QString::fromLocal8Bit("隐藏按钮"));

	m_open = new QAction(this);
	m_open->setText(QString::fromLocal8Bit("打开"));
	m_open->setStatusTip(QString::fromLocal8Bit("打开"));

	m_save = new QAction(this);
	m_save->setText(QString::fromLocal8Bit("保存"));
	m_save->setStatusTip(QString::fromLocal8Bit("保存"));

	m_quit = new QAction(this);
	m_quit->setText(QString::fromLocal8Bit("关闭"));
	m_quit->setStatusTip(QString::fromLocal8Bit("关闭"));

	connect(m_showbtn, SIGNAL(triggered()), this, SLOT(show_btn()));
	connect(m_open, SIGNAL(triggered()), this, SLOT(open_json()));
	connect(m_save, SIGNAL(triggered()), this, SLOT(save_json()));
	connect(m_quit, SIGNAL(triggered()), this, SLOT(quit_form()));
	
	m_pmenu->addAction(m_open);
	m_pmenu->addAction(m_save);
	m_pmenu->addAction(m_quit);
	//end 菜单

	connect(ui.m_save_btn, SIGNAL(clicked()), this, SLOT(save_json()));
	ui.m_save_btn->setStyleSheet("background-color: rgb(5,150,255);border-radius:5px;padding:2px 4px;");  //;border:2px groove gray;border-radius:10px;padding:2px 4px;

	m_paddbtn = new CMyButton("(+)", ui.m_navi_scrollAreaWidget);
	assert(m_paddbtn);
	m_paddbtn->set_jsontool(this);
	m_paddbtn->setText("(+)");
	m_paddbtn->set_pos(-1);
	m_paddbtn->set_type(1);
	m_paddbtn->setGeometry(0, 0, 145, 60);    
	//m_paddbtn->setFlat(true);                //隐藏边框线
	m_paddbtn->setStyleSheet("QPushButton{font-family:'宋体';font-size:32px;color:rgb(0,0,255);}QPushButton{background-color:rgb(5,150,255)}QPushButton:hover{background-color:rgb(50,170,200)}");
	m_paddbtn->show();

	m_pnavi_dlg = new navi_dlg(this);
	m_pnavi_dlg->setWindowTitle(QString::fromLocal8Bit("添加导航按钮"));
	m_pnavi_dlg->setAttribute(Qt::WA_ShowModal, true);
	m_pnavi_dlg->setMaximumSize(270,120);
	m_pnavi_dlg->setMinimumSize(270,120);

	m_pele_dlg = new eleDlg(this);
	m_pele_dlg->setWindowTitle(QString::fromLocal8Bit("添加元素"));
	m_pele_dlg->setAttribute(Qt::WA_ShowModal, true);

	m_parray_dlg = new arrayDlg(this);
	m_parray_dlg->setWindowTitle(QString::fromLocal8Bit("添加元素"));
	m_parray_dlg->setAttribute(Qt::WA_ShowModal, true);
	m_parray_dlg->setMaximumSize(510, 450);
	m_parray_dlg->setMinimumSize(510, 450);

	m_ptemplate_dlg = new template_dlg();
	m_ptemplate_dlg->setWindowTitle(QString::fromLocal8Bit("显示模板"));
	m_ptemplate_dlg->setAttribute(Qt::WA_ShowModal, true);

	ui.m_navi_scrollAreaWidget->setAttribute(Qt::WA_StyledBackground, true);
	ui.m_navi_scrollAreaWidget->setStyleSheet("background-color: rgb(5,150,255)");   //天蓝色

	ui.m_content_saw->setAttribute(Qt::WA_StyledBackground, true);
	ui.m_content_saw->setStyleSheet("background-color: rgb(255,251,240)");           //乳白色
	ui.m_hline->setGeometry(QRect(146, 42, 761, 1));
	ui.m_hline->setStyleSheet("background-color: rgb(5,150,255)");
	ui.m_ltitle->setText("");

	if ("false" == show_btn)
	{
		m_bshow_btn = false;
		m_paddbtn->setVisible(false);
	}
	else
	{
		m_pmenu->addAction(m_showbtn);
		m_paddbtn->setVisible(true);
		m_bshow_btn = true;
	}
}

json_tool::~json_tool()
{
	//
}

//begin public slots functions
void json_tool::show_btn()
{
	if (m_bshow_btn)
	{
		m_bshow_btn = false;
		m_showbtn->setText(QString::fromLocal8Bit("显示按钮"));
		m_paddbtn->setVisible(false);
	}	
	else
	{
		m_bshow_btn = true;
		m_showbtn->setText(QString::fromLocal8Bit("隐藏按钮"));
		m_paddbtn->setVisible(true);
	}	

	int spos = 0, epos = 0;
	get_navibtn_sepos(m_icurrbtn_index,spos,epos);
	construct_window(spos,epos);
}

void json_tool::open_json()
{
	QFileDialog fileDialog(this);
	fileDialog.setWindowTitle(QStringLiteral("选中文件"));
	fileDialog.setDirectory(".");
	fileDialog.setNameFilter(tr("File(*.*)"));

	fileDialog.setViewMode(QFileDialog::Detail);            //设置视图模式
	//打印所有选择的文件的路径
	QStringList fileName;

	int ires = fileDialog.exec();
	if (ires == QDialog::Accepted)
	{
		release_vjson();
		fileName = fileDialog.selectedFiles();
	}
	else if (ires == QDialog::Rejected)
		return;

	parse_json(fileName.at(0));
	
	create_navibtn();
	int spos = 0, epos = 0;
	get_navibtn_sepos(1, spos, epos);
	construct_window(spos,epos);

	return;
}

void json_tool::save_json()
{
	QFileDialog fileDialog;
	QString source = fileDialog.getSaveFileName(this, tr("Save File"), "./", tr("File(*.json)"));
	if ("" == source)
		return;

	for (auto iter : m_qjson)
	{
		switch (iter->type)
		{
		case 10:
		{
			if (iter->pspin_edit)
			{
				QByteArray cdata = iter->pspin_edit->text().toLocal8Bit();
				iter->value = cdata.data();
			}
		}
		    break;
		case 11:
		{
			if (iter->pdspin_edit)
			{
				QByteArray cdata = iter->pdspin_edit->text().toLocal8Bit();
				iter->value = cdata.data();
			}
		}
		    break;
		case 12:
		{
			if (iter->s_items != "")
			{
				if (iter->pcombox)
				{
					QByteArray cdata = iter->pcombox->currentText().toLocal8Bit();
					iter->value = cdata.data();
				}
			}
			else
			{
				if (iter->pline_edit)
				{
					QByteArray cdata = iter->pline_edit->text().toLocal8Bit();
					iter->value = cdata.data();
				}
			}
		}
			break;
		case 13:
		{
			if (iter->pcombox)
			{
				QByteArray cdata = iter->pcombox->currentText().toLocal8Bit();
				iter->value = cdata.data();
			}
		}
		    break;
		case 14:
		{
			string svalue = "";
			if (1 == iter->ctrlnum)
			{
				if (10 == iter->type1)
					svalue = iter->pspin_edit->text().toLocal8Bit();
				else if (11 == iter->type1)
					svalue = iter->pdspin_edit->text().toLocal8Bit();
				else if (12 == iter->type1)
				{
					if (iter->ctrl1_items != "")
						svalue = iter->pcombox1->currentText().toLocal8Bit();
					else if ("palette" == iter->ctrl1_value)
						svalue = "palette";
					else
						svalue = iter->pline_edit->text().toLocal8Bit();
				}
				else if (13 == iter->type1)
					svalue = iter->pcombox1->currentText().toLocal8Bit();
			}
			else if (2 == iter->ctrlnum)
			{
				if (10 == iter->type1)
					svalue = iter->pspin_edit->text().toLocal8Bit()+"&&";
				else if (11 == iter->type1)
					svalue = iter->pdspin_edit->text().toLocal8Bit() + "&&";
				else if (12 == iter->type1)
				{
					if (iter->ctrl1_items != "")
						svalue = iter->pcombox1->currentText().toLocal8Bit() + "&&";
					else if ("palette" == iter->ctrl1_value)
						svalue = "palette&&";
					else
						svalue = iter->pline_edit->text().toLocal8Bit() + "&&";
				}
				else if (13 == iter->type1)
					svalue = iter->pcombox1->currentText().toLocal8Bit() + "&&";

				if (10 == iter->type2)
					svalue += iter->pspin_edit2->text().toLocal8Bit();
				else if (11 == iter->type2)
					svalue += iter->pdspin_edit2->text().toLocal8Bit();
				else if (12 == iter->type2)
				{
					if (iter->ctrl2_items != "")
						svalue += iter->pcombox2->currentText().toLocal8Bit();
					else if ("palette" == iter->ctrl2_value)
						svalue += "palette";
					else
						svalue += iter->pline_edit2->text().toLocal8Bit();
				}
				else if (13 == iter->type2)
					svalue += iter->pcombox2->currentText().toLocal8Bit();

			}
			else if (3 == iter->ctrlnum)
			{
				if (10 == iter->type1)
					svalue = iter->pspin_edit->text().toLocal8Bit() + "&&";
				else if (11 == iter->type1)
					svalue = iter->pdspin_edit->text().toLocal8Bit() + "&&";
				else if (12 == iter->type1)
				{
					if (iter->ctrl1_items != "")
						svalue = iter->pcombox1->currentText().toLocal8Bit() + "&&";
					else if ("palette" == iter->ctrl1_value)
						svalue = "palette&&";
					else
						svalue = iter->pline_edit->text().toLocal8Bit() + "&&";
				}
				else if (13 == iter->type1)
					svalue = iter->pcombox1->currentText().toLocal8Bit() + "&&";

				if (10 == iter->type2)
					svalue += iter->pspin_edit2->text().toLocal8Bit() + "&&";
				else if (11 == iter->type2)
					svalue += iter->pdspin_edit2->text().toLocal8Bit() + "&&";
				else if (12 == iter->type2)
				{
					if (iter->ctrl2_items != "")
						svalue += iter->pcombox2->currentText().toLocal8Bit() + "&&";
					else if ("palette" == iter->ctrl2_value)
						svalue += "palette&&";
					else
						svalue += iter->pline_edit2->text().toLocal8Bit() + "&&";
				}
				else if (13 == iter->type2)
					svalue += iter->pcombox2->currentText().toLocal8Bit() + "&&";

				if (10 == iter->type3)
					svalue += iter->pspin_edit3->text().toLocal8Bit();
				else if (11 == iter->type3)
					svalue += iter->pdspin_edit3->text().toLocal8Bit();
				else if (12 == iter->type3)
				{
					if (iter->ctrl3_items != "")
						svalue += iter->pcombox3->currentText().toLocal8Bit();
					else if ("palette" == iter->ctrl3_value)
						svalue += "palette";
					else
						svalue += iter->pline_edit3->text().toLocal8Bit();
				}
				else if (13 == iter->type3)
					svalue += iter->pcombox3->currentText().toLocal8Bit();
			}

			iter->value = svalue;
		}
			break;
		default:
			break;
		}//end switch

		if (1 == iter->type || 2 == iter->type)
		{
			if (iter->pline_edit)
			{
				QByteArray cdata = iter->pline_edit->text().toLocal8Bit();
				iter->value = cdata.data();
			}
		}
		else if (3 == iter->type)
		{
			if (iter->pcombox)
			{
				QByteArray cdata = iter->pcombox->currentText().toLocal8Bit();
				iter->value = cdata.data();
			}
		}
	}//end for (auto iter : m_qjson)

	FILE* fp = nullptr;
	fp = fopen(source.toLocal8Bit().data(), "wb");

	if (fp)
		write_json(fp);

	if (fp)
	{
		fclose(fp);
		fp = nullptr;
	}
}

void json_tool::quit_form()
{
	QMessageBox Msg(QMessageBox::Question, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("确定关闭窗口？"));
	QAbstractButton* pYesBtn = (QAbstractButton*)Msg.addButton(QString::fromLocal8Bit("是"), QMessageBox::YesRole);
	QAbstractButton* pNoBtn = (QAbstractButton*)Msg.addButton(QString::fromLocal8Bit("否"), QMessageBox::NoRole);
	Msg.exec();
	if (Msg.clickedButton() != pYesBtn)
		return;

	release_vjson();
	qApp->quit();
}

void json_tool::navidlg_OKClicked()
{
	QString skey = m_pnavi_dlg->getkey();
	QString salias = m_pnavi_dlg->getalias();
	if ("" == skey)
	{
		QMessageBox::information(m_pnavi_dlg, tr("Warning"), QString::fromLocal8Bit("key 值不能为空"));
		return;
	}
	if ("" == salias)
	{
		QMessageBox::information(m_pnavi_dlg, tr("Warning"), QString::fromLocal8Bit("alias 值不能为空"));
		return;
	}

	pm_node pnode = new struct m_node();
	pnode->blank_count = 2;
	pnode->has_symbol = false;
	pnode->key = skey.toLocal8Bit();
	pnode->s_alias = salias.toLocal8Bit();
	pnode->type = 1;
	pnode->value = "";
	pnode->parent = nullptr;
	if (0 == m_qjson.size())
		pnode->prenode = nullptr;
	else
	{
		pnode->prenode = m_qjson.at(m_qjson.size() - 1);
		pnode->prenode->has_symbol = true;
	}  
	
	CMyButton* pbtn = new CMyButton(salias, ui.m_navi_scrollAreaWidget);
	pbtn->set_type(1);
	pbtn->set_index(++m_navibtn_count);
	pbtn->setText(salias);
	pbtn->set_pos(-1);
	pbtn->set_jsontool(this);
	//pbtn->setGeometry(0, (m_navibtn_count - 1) * 60, 120, 60);
	pbtn->setGeometry(0, (m_navibtn_count - 1) * 60, 145, 60);
	//pbtn->setFlat(false);                                       //隐藏边框线
	pbtn->setStyleSheet("QPushButton{font-family:'宋体';font-size:22px;color:rgb(0,0,255);}QPushButton{background-color:rgb(255,251,240)}QPushButton:hover{background-color:rgb(255,251,240)}");
	pbtn->show();
	pnode->p_navigation_btn = pbtn;

	CMyButton* padd_btn = new CMyButton("(+)",ui.m_content_saw);
	padd_btn->set_type(2);
	padd_btn->setText("(+)");
	padd_btn->set_pos(-1);
	padd_btn->set_jsontool(this);
	padd_btn->setGeometry(10, 10, 45, 30);
	padd_btn->setStyleSheet("QPushButton{font-family:'宋体';font-size:22px;color:rgb(0,0,255);}QPushButton{background-color:rgb(5,150,255)}QPushButton:hover{background-color:rgb(50,170,200)}");
	padd_btn->show();
	pnode->p_btadd = padd_btn;
	m_qjson.push_back(pnode);
	m_navi_node.push_back(pnode);

	pm_node pnode2 = new struct m_node();
	pnode2->blank_count = pnode->blank_count;
	pnode2->key = "{";
	pnode2->parent = nullptr;
	pnode2->prenode = pnode;
	pnode2->type = 20;
	m_qjson.push_back(pnode2);

	pm_node pnode3 = new struct m_node();
	pnode3->blank_count = pnode->blank_count;
	pnode3->key = "}";
	pnode3->parent = nullptr;
	pnode3->prenode = pnode2;
	pnode3->type = 20;
	m_qjson.push_back(pnode3);

	m_paddbtn->move(0, m_navibtn_count * 60);
	m_pnavi_dlg->close();

	//m_icurrbtn_index = m_navibtn_count;

	//隐藏导航栏非当前按钮的所有元素
	deal_navi_currbtn(m_navibtn_count);

	//根据导航栏按钮个数数，计算最大高度的像素值
	int height_pix = m_navibtn_count * 60 + 100;
	if (height_pix <= 780)
		height_pix = 780;
	ui.m_navi_scrollAreaWidget->setMinimumSize(120, height_pix);

	ui.m_ltitle->setText(salias);
}

void json_tool::navidlg_CancelClicked()
{
	m_pnavi_dlg->close();
}

void json_tool::eledlg_OKClicked()
{
	QString sele_type = m_pele_dlg->get_eletype();
	bool bm_flag = m_pele_dlg->getmodify();
	QString sele_key = m_pele_dlg->get_elekey();
	QString sele_alias = m_pele_dlg->get_elealias();
	QString skey = m_pele_dlg->getkey();
	QString snode_value = m_pele_dlg->getvalue();
	QString snode_valuetype = m_pele_dlg->get_valuetype();
	QString snode_alias = m_pele_dlg->get_nodealias();
	QString snode_help = m_pele_dlg->get_nodehelp();
	QString sitems = m_pele_dlg->get_nodeitems();
	QString ctrl_key = m_pele_dlg->get_mctrl_key();

	if ("alias" == sele_key || "help" == sele_key || "items" == sele_key)
	{
		QMessageBox::information(m_pele_dlg, tr("Warning"), QString::fromLocal8Bit("alias,help,items 为关键字，请修改ele_key的内容"));
		return;
	}
	if ("alias" == skey || "help" == skey || "items" == skey)
	{
		QMessageBox::information(m_pele_dlg, tr("Warning"), QString::fromLocal8Bit("alias,help,items 为关键字，请修改key的内容"));
		return;
	}

	//if ("node" != sele_type && "m_ctrl" != sele_type && bm_flag && ("" == sele_key || "" == sele_alias))
	//if ("node" != sele_type && "m_ctrl" != sele_type && bm_flag && "" == sele_key)
	if ("object" == sele_type && "array" == sele_type && bm_flag && "" == sele_key)
	{
		QMessageBox::information(m_pele_dlg, tr("Warning"), QString::fromLocal8Bit("ele_key 不能为空"));
		return;
	}
	//if ("node" != sele_type && "m_ctrl" != sele_type && !bm_flag && ("" == sele_key || "" == sele_alias || "" == skey || "" == snode_value || "" == snode_alias || "" == snode_help))
	//if ("node" != sele_type && "m_ctrl" != sele_type && !bm_flag && ("" == sele_key || "" == skey || "" == snode_value 
	if (("object" == sele_type || "array" == sele_type) && !bm_flag && ("" == sele_key || "" == skey || "" == snode_value))
	{
		QMessageBox::information(m_pele_dlg, tr("Warning"), QString::fromLocal8Bit("ele_key,key,value 不能为空"));
		return;
	}
	//else if ("node" == sele_type && ("" == skey || "" == snode_value || "" == snode_alias || "" == snode_help))
	else if (("node" == sele_type) && ("" == skey || "" == snode_value))  // || "smooth" == sele_type
	{
		QMessageBox::information(m_pele_dlg, tr("Warning"), QString::fromLocal8Bit("key,value 不能为空"));
		return;
	}
	else if ("m_ctrl" == sele_type && "" == ctrl_key)
	{
		QMessageBox::information(m_pele_dlg, tr("Warning"), QString::fromLocal8Bit("node_key 不能为空"));
		return;
	}
	//else if ("smooth" == sele_type&& snode_valuetype!="float")
	//{
	//	QMessageBox::information(m_pele_dlg, tr("Warning"), QString::fromLocal8Bit("smooth类型的节点，数据必须为浮点型"));
	//	return;
	//}

	//如果items里有值 和value没有匹配的，不允许添加
	if (sitems != "")
	{
		QStringList ql = parse_items(string(sitems.toLocal8Bit().data()),";");
		int icount = 0;
		for (auto i : ql)
		{
			if (i == snode_value)
				break;
			++icount;
		}
		if (icount == ql.count())
		{
			QMessageBox::information(m_pele_dlg, tr("Warning"), tr("value,items unmatched!"));
			return;
		}
	}//end if (sitems != "")

	int inodetype = 0;
	if ("node" == sele_type)
		inodetype = 1;
	else if ("object" == sele_type)
		inodetype = 2;
	else if ("array" == sele_type)
		inodetype = 3;
	else if ("m_ctrl" == sele_type)
		inodetype = 4;
	else if ("smooth" == sele_type)
		inodetype = 5;
	else if ("template" == sele_type)
		inodetype = 6;

	switch (inodetype)
	{
	case 1:            //普通节点
	{
		if (bm_flag)
			mod_commonnode();
		else
			add_commonnode();
	}
	    break;
	case 2:            //JSON对象节点
	{
		if (bm_flag)
			mod_objectnode();
		else
			add_objectnode();
	}
	    break;
	case 3:            //JSON数组节点
	{
		if (bm_flag)
			mod_arraynode();
		else
			add_arraynode();
	}
	    break;
	case 4:            //一行多控件
	{
		if (bm_flag)
			mod_mctrlnode();
		else
			add_mctrlnode();
	}
		break;
	case 5:           //平滑因子节点    
	{
		if (bm_flag)
			mod_smoothnode();
		else
			add_smoothnode();
	}
		break;
	case 6:             //模板节点
	{
		if (bm_flag)
			mod_templatenode();
		else
			add_templatenode();
	}
		break;
	default:
		break;
	}

	int spos = 0, epos = 0;
	get_navibtn_sepos(m_icurrbtn_index,spos,epos);
	construct_window(spos,epos);

	m_pele_dlg->close();
}

void json_tool::eledlg_CancelClicked()
{
	m_pele_dlg->close();
}

void json_tool::arraydlg_OKClicked()
{
	m_parray_dlg->update_ctltojson();
	m_parray_dlg->release_label();
	m_parray_dlg->insert_nodetojson(m_qjson);

	int spos = 0, epos = 0;
	get_navibtn_sepos(m_icurrbtn_index, spos, epos);
	construct_window(spos, epos);

	m_parray_dlg->close();
}

void json_tool::arraydlg_CancelClicked()
{
	m_parray_dlg->close();
}
//end public slots functions

//begin public functions
void json_tool::add_ele(int type,int pos)
{
	m_pele_dlg->clearCtrl();
	m_pele_dlg->setWindowTitle(QString::fromLocal8Bit("添加元素"));

	switch (type)
	{
	case 1:         //导航栏+按钮
	{
		m_pnavi_dlg->setkey("");
		m_pnavi_dlg->setalias("");
		m_pnavi_dlg->show();
	}
		break;
	case 2:         //导航栏右边的+按钮
	case 3:         //元素后面的+按钮
	{
		//如果一个空页面添加了模板类型的节点，就不允许添加别的类型的节点
		//同理，如果一个页面有了别的类型的节点，就不允许添加模板类型的节点
		int spos = 0, epos = 0;
		get_navibtn_sepos(m_icurrbtn_index, spos, epos);
		if (epos - spos <= 2)
			m_pele_dlg->deal_eletype(1);
		else
		{
			pm_node pnode = nullptr;
			pnode = m_qjson.at(spos+2);
			if (16 != pnode->type)
			    m_pele_dlg->deal_eletype(2);
			else
				m_pele_dlg->deal_eletype(3);
		}

		m_pele_dlg->set_eletype(0);
		m_pele_dlg->set_eletype_enable(true);
		m_pele_dlg->set_elekey_enable(false);
		m_pele_dlg->set_elealias_enable(false);
		m_pele_dlg->set_key_enable(true);
		m_pele_dlg->set_value_enable(true);
		m_pele_dlg->set_valuetype_enable(false);
		m_pele_dlg->set_alias_enable(true);
		if("template" == m_pele_dlg->get_eletype())
		    m_pele_dlg->set_help_enable(false);
		else
			m_pele_dlg->set_help_enable(true);

		m_pele_dlg->show_hide_nodectrl(true);
		m_pele_dlg->show_hide_mctrl(false);

		m_pele_dlg->clearCtrl();
		m_pele_dlg->setmodify(false);
		m_pele_dlg->setpos(pos);

		m_pele_dlg->show();
	}
		break;
	case 4:        //数组里面某组后面的 + 按钮
	{
		int icount = 1;
		int ibrace_count = 1;
		pm_node pnode = m_qjson.at(pos);
		pm_node temp_pre = pnode->prenode;
		while (temp_pre)
		{
			if ("}" == temp_pre->key)
				++ibrace_count;
			else if ("{" == temp_pre->key || "virtual" == temp_pre->key)
				--ibrace_count;

			if ("virtual" == temp_pre->key && 0 == ibrace_count)
			{
				++icount;
				break;
			}

			++icount;
			temp_pre = temp_pre->prenode;
		}

		m_parray_dlg->setpos(pos);
		m_parray_dlg->add_array_objectnode(icount, m_qjson);
	}
		break;
	default:
		break;
	}
}

void json_tool::sub_ele(int pos)
{
	QMessageBox Msg(QMessageBox::Question, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("确定删除此节点？"));
	QAbstractButton* pYesBtn = (QAbstractButton*)Msg.addButton(QString::fromLocal8Bit("是"), QMessageBox::YesRole);
	QAbstractButton* pNoBtn = (QAbstractButton*)Msg.addButton(QString::fromLocal8Bit("否"), QMessageBox::NoRole);
	Msg.exec();
	if (Msg.clickedButton() != pYesBtn)
		return;

	pm_node pnode = nullptr;
	pnode = m_qjson.at(pos);
	if (pnode)
	{
		int itype = pnode->type;
		switch (itype)
		{
		case 2:
		case 3:
		{
			int epos = get_epos_by_spos(pos + 1);

			change_sepos(pos,epos);

			pm_node pnode_pre = m_qjson.at(pos - 1);
			pm_node pnode_after = m_qjson.at(epos + 1);
			if (pnode_pre && pnode_after && "{" == pnode_pre->key && "}" == pnode_after->key)
				pnode_pre->prenode->has_symbol = false;
			else if (pnode_pre && pnode_after && "{" != pnode_pre->key && "}" == pnode_after->key)
				pnode_pre->has_symbol = false;

			pnode_after->prenode = pnode_pre;

			pm_node temp_pnode = nullptr;
			for (int i = pos; i <= epos; ++i)
			{
				show_hide_control(i, false);
				temp_pnode = m_qjson.at(i);
				if (temp_pnode)
				{
					delete temp_pnode;
					temp_pnode = nullptr;
				}
			}	
			m_qjson.erase(m_qjson.begin() + pos, m_qjson.begin() + (epos + 1));   //erase 为前闭后开区间
		}
			break;
		
		case 4:
		{
			//
		}
			break;
		case 10:
		case 11:
		case 12:
		case 13:
		case 14:
		case 15:
		{  
			pm_node pnode_pre = m_qjson.at(pos - 1);
			pm_node pnode_after = m_qjson.at(pos + 1);
			//导航栏节点下的内容全部删除 的情况
			if (pnode_pre && pnode_after && "{" == pnode_pre->key && "}" == pnode_after->key &&
				pnode_pre->prenode && 1 == pnode_pre->prenode->type)
			{
				pnode_pre->prenode->has_symbol = false;
				pnode_after->prenode = pnode_pre;

				show_hide_control(pos, false);
				m_qjson.erase(m_qjson.begin() + pos);
				delete pnode;
				pnode = nullptr;
			}
			//对象节点的内容全部删掉 的情况
			else if (pnode_pre && pnode_after && "{" == pnode_pre->key && "}" == pnode_after->key &&
				     pnode_pre->prenode && 2 == pnode_pre->prenode->type)
			{
				int spos = pos - 2;
				int epos = pos + 1;
				change_sepos(spos,epos);

				pnode_pre = m_qjson.at(spos - 1);
				pnode_after = m_qjson.at(epos + 1);
				if (pnode_pre && pnode_after && "{" == pnode_pre->key && "}" == pnode_after->key)
					pnode_pre->prenode->has_symbol = false;
				else if (pnode_pre && pnode_after && "{" != pnode_pre->key && "}" == pnode_after->key)
					pnode_pre->has_symbol = false;

				pnode_after->prenode = pnode_pre;

				pm_node temp_pnode = nullptr;
				for (int i = spos; i <= epos; ++i)
				{
					show_hide_control(i, false);
					temp_pnode = m_qjson.at(i);
					if (temp_pnode)
					{
						delete temp_pnode;
						temp_pnode = nullptr;
					}
				}    
				m_qjson.erase(m_qjson.begin()+spos,m_qjson.begin()+(epos+1));   //erase 为前闭后开区间
			}
			//数组节点内的某一组全部删掉 的情况
			else if (pnode_pre && pnode_after&& "virtual" == pnode_pre->key && "}" == pnode_after->key)
			{
				int spos = pos - 1;
				int epos = pos + 1;
				change_sepos(spos, epos);

				pnode_pre = m_qjson.at(spos - 1);
				pnode_after = m_qjson.at(epos + 1);
				if (pnode_pre && pnode_after && "{" == pnode_pre->key && "}" == pnode_after->key)
					pnode_pre->prenode->has_symbol = false;
				else if (pnode_pre && pnode_after && "{" != pnode_pre->key && "}" == pnode_after->key)
					pnode_pre->has_symbol = false;

				pnode_after->prenode = pnode_pre;

				pm_node temp_pnode = nullptr;
				for (int i = spos; i <= epos; ++i)
				{
					show_hide_control(i, false);
					temp_pnode = m_qjson.at(i);
					if (temp_pnode)
					{
						delete temp_pnode;
						temp_pnode = nullptr;
					}
				}	
				m_qjson.erase(m_qjson.begin() + spos, m_qjson.begin() + (epos + 1));   //erase 为前闭后开区间
			}
			else
			{
				pnode_pre->has_symbol = pnode->has_symbol;
				pnode_after->prenode = pnode_pre;

				show_hide_control(pos, false);
				m_qjson.erase(m_qjson.begin() + pos);
				delete pnode;
				pnode = nullptr;
			}
		}
			break;
		case 16:
		{
			pm_node pnode_pre = m_qjson.at(pos - 1);
			pm_node pnode_after = m_qjson.at(pos + 1);

			//导航栏节点下的内容全部删除 的情况
			if (pnode_pre && pnode_after && "{" == pnode_pre->key && "}" == pnode_after->key &&
				pnode_pre->prenode && 1 == pnode_pre->prenode->type)
			{
				pnode_pre->prenode->has_symbol = false;
				pnode_after->prenode = pnode_pre;
			}
            else if (!pnode->bcheck)
			{
				pnode_pre->has_symbol = pnode->has_symbol;
				pnode_after->prenode = pnode_pre;
			}
			else if (pnode->bcheck)
			{
				pnode_pre->has_symbol = pnode->has_symbol;
				pnode_after->prenode = pnode_pre;

				if (pnode_after->type != 20)
					pnode_after->bcheck = true;
				else if (pnode_pre->type != 20)
					pnode_pre->bcheck = true;
			}

			show_hide_control(pos, false);
			m_qjson.erase(m_qjson.begin() + pos);
			delete pnode;
			pnode = nullptr;
		}
			break;
		default:
			break;
		}

		int spos = 0, epos = 0;
		get_navibtn_sepos(m_icurrbtn_index, spos, epos);
		construct_window(spos, epos);
	}//end if (pnode)
}

void json_tool::mod_ele(int pos)
{
	m_pele_dlg->clearCtrl();
	m_pele_dlg->set_eletype_enable(false);
	m_pele_dlg->setpos(pos);
	m_pele_dlg->setmodify(true);
	m_pele_dlg->setWindowTitle(QString::fromLocal8Bit("修改元素"));
	deque<pm_node>::iterator iter = m_qjson.begin() + pos;
	pm_node pnode = *iter;

	switch (pnode->type)
	{
	case 2:
	{
		m_pele_dlg->set_eletype(1);
		m_pele_dlg->set_elekey(pnode->key);
		m_pele_dlg->set_elealias(pnode->s_alias);
		m_pele_dlg->set_elekey_enable(true);
		m_pele_dlg->set_elealias_enable(true);
		m_pele_dlg->set_key_enable(false);
		m_pele_dlg->set_value_enable(false);
		m_pele_dlg->set_valuetype_enable(false);
		m_pele_dlg->set_alias_enable(false);
		m_pele_dlg->set_help_enable(false);
		m_pele_dlg->set_items_enable(false);

		m_pele_dlg->show_hide_nodectrl(true);
		m_pele_dlg->show_hide_mctrl(false);
	}
		break;
	case 3:
	{
		m_pele_dlg->set_eletype(2);
		m_pele_dlg->set_elekey(pnode->key);
		m_pele_dlg->set_elekey(pnode->key);
		m_pele_dlg->set_elealias(pnode->s_alias);
		m_pele_dlg->set_elekey_enable(true);
		m_pele_dlg->set_elealias_enable(true);
		m_pele_dlg->set_key_enable(false);
		m_pele_dlg->set_value_enable(false);
		m_pele_dlg->set_valuetype_enable(false);
		m_pele_dlg->set_alias_enable(false);
		m_pele_dlg->set_help_enable(false);
		m_pele_dlg->set_items_enable(false);

		m_pele_dlg->show_hide_nodectrl(true);
		m_pele_dlg->show_hide_mctrl(false);
	}
		break;
	case 10:
	case 11:
	case 12:
	case 13:
	case 15:
	{
		if (15 == pnode->type)
			m_pele_dlg->set_eletype(4);
		else
			m_pele_dlg->set_eletype(0);
		    
		m_pele_dlg->setkey(pnode->key);
		m_pele_dlg->setvalue(pnode->value);
		m_pele_dlg->set_nodealias(pnode->s_alias);
		m_pele_dlg->set_nodehelp(pnode->s_help);
		m_pele_dlg->set_nodeitems(pnode->s_items);

		m_pele_dlg->set_elekey_enable(false);
		m_pele_dlg->set_elealias_enable(false);
		m_pele_dlg->set_key_enable(true);
		m_pele_dlg->set_value_enable(false);
		m_pele_dlg->set_valuetype_enable(false);
		m_pele_dlg->set_alias_enable(true);
		m_pele_dlg->set_help_enable(true);
		m_pele_dlg->set_items_enable(true);

		m_pele_dlg->show_hide_nodectrl(true);
		m_pele_dlg->show_hide_mctrl(false);
	}
		break;
	case 14:
	{
		m_pele_dlg->set_eletype(3);
		m_pele_dlg->set_mctrl_key(pnode->key);
		m_pele_dlg->set_mctrl_keyalias(pnode->s_alias);
		m_pele_dlg->set_mctrl_keyhelp(pnode->s_help);
		m_pele_dlg->set_mctrlnum(pnode->ctrlnum);
		m_pele_dlg->set_mctrl1_value(pnode->ctrl1_value);
		m_pele_dlg->set_mctrl2_value(pnode->ctrl2_value);
		m_pele_dlg->set_mctrl3_value(pnode->ctrl3_value);
		m_pele_dlg->set_mctrl1_items(pnode->ctrl1_items);
		m_pele_dlg->set_mctrl2_items(pnode->ctrl2_items);
		m_pele_dlg->set_mctrl3_items(pnode->ctrl3_items);

		m_pele_dlg->show_hide_nodectrl(false);
		m_pele_dlg->show_hide_mctrl(true);
		
		m_pele_dlg->ctlnumChanged(QString::number(pnode->ctrlnum));
	}
		break;
	case 16:
	{
		int icount = m_pele_dlg->get_eletype_count();
		if (6 == icount)
			m_pele_dlg->set_eletype(5);
		else if(1==icount)
			m_pele_dlg->set_eletype(0);

		m_pele_dlg->setkey(pnode->key);
		m_pele_dlg->setvalue(pnode->value);
		m_pele_dlg->set_nodealias(pnode->s_alias);

		m_pele_dlg->set_elekey_enable(false);
		m_pele_dlg->set_elealias_enable(false);
		m_pele_dlg->set_key_enable(true);
		m_pele_dlg->set_value_enable(true);
		m_pele_dlg->set_valuetype_enable(false);
		m_pele_dlg->set_alias_enable(true);
		m_pele_dlg->set_help_enable(false);
		m_pele_dlg->set_items_enable(false);

		m_pele_dlg->show_hide_nodectrl(true);
		m_pele_dlg->show_hide_mctrl(false);
	}
	    break;
	default:
		break;
	}//end switch (pnode->type)

	m_pele_dlg->show();
}

void json_tool::add_sub(int pos)
{
	pm_node pnode = nullptr;
	pm_node temp_pnode = nullptr;
	pnode = m_qjson.at(pos);
	if (nullptr == pnode)
		return;
	if (pnode->type != 2 && pnode->type != 3)
		return;

	int spos = pos+1,epos = 0; 
	epos = get_epos_by_spos(spos);

	if (pnode->bstatus)    //收缩状态
	{
		pnode->bstatus = false;
		pnode->p_btadd_sub->setText("-");
		for (int i = spos; i <= epos; ++i)
		{
			temp_pnode = m_qjson.at(i);
			if (temp_pnode)
				temp_pnode->bshow = true;

			show_hide_control(i, true);
		}
	}
	else                   //伸张状态
	{
		pnode->bstatus = true;
		pnode->p_btadd_sub->setText("+");
		for (int i = spos; i <= epos; ++i)
		{
			temp_pnode = m_qjson.at(i);
			if (temp_pnode)
				temp_pnode->bshow = false;

			show_hide_control(i, false);
		}
	}

	spos = 0;
	epos = 0;
	get_navibtn_sepos(m_icurrbtn_index, spos, epos);
	construct_window(spos, epos);
}

void json_tool::smooth_open(int pos)
{
	pm_node pnode = nullptr;
	pnode = m_qjson.at(pos);
	if (15 == pnode->type)
	{
		if (pnode->p_btopen)
			pnode->p_btopen->setStyleSheet("background-color: rgb(5,150,255);border-radius:15px;padding:2px 4px;color:white");
		if (pnode->p_btclose)
			pnode->p_btclose->setStyleSheet("background-color: rgb(211,211,211);border-radius:15px;padding:2px 4px;color:white");
		
		if (pnode->bhigher)
		{
			if (pnode->p_bthigher)
				pnode->p_bthigher->setStyleSheet("background-color: rgb(5,150,255);border-radius:15px;padding:2px 4px;color:white");
			if (pnode->p_btlower)
				pnode->p_btlower->setStyleSheet("background-color: rgb(211,211,211);border-radius:15px;padding:2px 4px;color:white");

			if (pnode->pdspin_edit)
				pnode->pdspin_edit->setValue(1.1);
		}
		else
		{
			if (pnode->p_bthigher)
				pnode->p_bthigher->setStyleSheet("background-color: rgb(211,211,211);border-radius:15px;padding:2px 4px;color:white");
			if (pnode->p_btlower)
				pnode->p_btlower->setStyleSheet("background-color: rgb(5,150,255);border-radius:15px;padding:2px 4px;color:white");

			if (pnode->pdspin_edit)
				pnode->pdspin_edit->setValue(0.5);
		}
		
		pnode->bopen = true;
	}
}

void json_tool::smooth_close(int pos)
{
	pm_node pnode = nullptr;
	pnode = m_qjson.at(pos);
	if (15 == pnode->type)
	{
		if (pnode->p_btopen)
			pnode->p_btopen->setStyleSheet("background-color: rgb(211,211,211);border-radius:15px;padding:2px 4px;color:white");
		if (pnode->p_btclose)
			pnode->p_btclose->setStyleSheet("background-color: rgb(5,150,255);border-radius:15px;padding:2px 4px;color:white");
		
		if (pnode->p_bthigher)
			pnode->p_bthigher->setStyleSheet("background-color: rgb(211,211,211);border-radius:15px;padding:2px 4px;color:white");
		if (pnode->p_btlower)
			pnode->p_btlower->setStyleSheet("background-color: rgb(211,211,211);border-radius:15px;padding:2px 4px;color:white");

		if (pnode->pdspin_edit)
			pnode->pdspin_edit->setValue(0);

		pnode->bopen = false;
	}
}

void json_tool::smooth_higher(int pos)
{
	pm_node pnode = nullptr;
	pnode = m_qjson.at(pos);
	if (15 == pnode->type && pnode->bopen)
	{
		if (pnode->p_bthigher)
			pnode->p_bthigher->setStyleSheet("background-color: rgb(5,150,255);border-radius:15px;padding:2px 4px;color:white");
		if (pnode->p_btlower)
			pnode->p_btlower->setStyleSheet("background-color: rgb(211,211,211);border-radius:15px;padding:2px 4px;color:white");

		if (pnode->pdspin_edit)
			pnode->pdspin_edit->setValue(1.1);

		pnode->bhigher = true;
	}
}

void json_tool::smooth_lower(int pos)
{
	pm_node pnode = nullptr;
	pnode = m_qjson.at(pos);
	if (15 == pnode->type && pnode->bopen)
	{
		if (pnode->p_bthigher)
			pnode->p_bthigher->setStyleSheet("background-color: rgb(211,211,211);border-radius:15px;padding:2px 4px;color:white");
		if (pnode->p_btlower)
			pnode->p_btlower->setStyleSheet("background-color: rgb(5,150,255);border-radius:15px;padding:2px 4px;color:white");

		if (pnode->pdspin_edit)
			pnode->pdspin_edit->setValue(0.5);

		pnode->bhigher = false;
	}
}

void json_tool::bt_template(int pos)
{
	pm_node pnode = nullptr;
	pm_node temp_node = nullptr;

	pnode = m_qjson.at(pos);
	if (nullptr == pnode)
		return;
	if (pnode->bcheck)
		return;

	int spos = 0, epos = 0;
	get_navibtn_sepos(m_icurrbtn_index, spos, epos);
	for (int i = spos; i <= epos; ++i)
	{
		temp_node = m_qjson.at(i);
		if (temp_node && 16 == temp_node->type)
		{
			if (temp_node != pnode && temp_node->bcheck)
			{
				temp_node->plabel2->setVisible(false);
				temp_node->plabel2->setEnabled(false);
				temp_node->p_t_bottom->setStyleSheet("background-color: rgb(255,251,240);border-radius:5px;padding:2px 4px;border:1px solid;border-color:rgb(211,211,211)");
				temp_node->p_btpreview->setEnabled(false);
				temp_node->p_btpreview->setStyleSheet("background-color: rgb(211,211,211);border-radius:5px;padding:2px 4px;");
				temp_node->bcheck = false;
				break;
			}
		}
	}

	pnode->plabel2->setVisible(true);
	pnode->plabel2->setEnabled(true);
	pnode->p_t_bottom->setStyleSheet("background-color: rgb(255,251,240);border-radius:5px;padding:2px 4px;border:1px solid;border-color:rgb(5,150,255)");
	pnode->p_btpreview->setEnabled(true);
	pnode->p_btpreview->setStyleSheet("background-color: rgb(5,150,255);border-radius:5px;padding:2px 4px;");
	pnode->bcheck = true;
}

void json_tool::bt_preview(int pos)
{
	pm_node pnode = nullptr;
	pnode = m_qjson.at(pos);
	if (!pnode)
		return;
	if (pnode->type != 16)
		return;

	string stemplate_name = pnode->value;

	if (m_ptemplate_dlg)
	{
		delete m_ptemplate_dlg;
		m_ptemplate_dlg = nullptr;
	}

	m_ptemplate_dlg = new template_dlg();
	m_ptemplate_dlg->setWindowTitle(QString::fromLocal8Bit("显示模板"));
	m_ptemplate_dlg->setAttribute(Qt::WA_ShowModal, true);

	m_ptemplate_dlg->show_template(stemplate_name);
	m_ptemplate_dlg->show();
}

void json_tool::del_navibtn(int index)
{
	QMessageBox Msg(QMessageBox::Question, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("确定删除此节点？"));
	QAbstractButton* pYesBtn = (QAbstractButton*)Msg.addButton(QString::fromLocal8Bit("是"), QMessageBox::YesRole);
	QAbstractButton* pNoBtn = (QAbstractButton*)Msg.addButton(QString::fromLocal8Bit("否"), QMessageBox::NoRole);
	Msg.exec();
	if (Msg.clickedButton() != pYesBtn)
		return;

	pm_node pnode = nullptr;
	//pnode = m_qjson.at(pos);
	pnode = m_navi_node.at(index-1);
	if (pnode)
	{
		if (pnode->type != 1)
			return;

		if (!pnode->p_navigation_btn)
			return;

		pnode->p_navigation_btn->setVisible(false);
		int index = pnode->p_navigation_btn->get_index();
		int spos = 0, epos = 0;
		get_navibtn_sepos(index, spos, epos);

		if (index == m_icurrbtn_index)
		{
			for (int i = spos + 1; i <= epos; ++i)
				show_hide_control(i, false);
		}

		pm_node temp_pnode = nullptr;
		deque<pm_node>::iterator it_begin = m_qjson.begin();
		for (int i = spos; i <= epos; ++i)
		{
			temp_pnode = m_qjson.at(i);
			if (temp_pnode)
			{
				delete temp_pnode;
				temp_pnode = nullptr;
			}
		}

		pm_node pnode_pre = nullptr;
		pm_node pnode_after = nullptr;
		if(spos!=0)
			pnode_pre = m_qjson.at(spos - 1);
		if(epos!=m_qjson.size()-1)
			pnode_after = m_qjson.at(epos + 1);

		if (pnode_pre && pnode_after)
		{
			pnode_pre->has_symbol = true;
			pnode_after->prenode = pnode_pre;
		}
		else if (!pnode_pre && pnode_after)
			pnode_after->prenode = nullptr;
		else if (pnode_pre && !pnode_after)
			pnode_pre->has_symbol = false;

		m_qjson.erase(it_begin + spos, it_begin + epos + 1);
		m_navi_node.erase(m_navi_node.begin() + index - 1);

		int icount = m_navi_node.size();
		for (int j = index - 1; j < icount; ++j)
		{
			temp_pnode = m_navi_node.at(j);
			temp_pnode->p_navigation_btn->set_index(j + 1);
			temp_pnode->p_navigation_btn->setGeometry(0, j * 60, 145, 60);
		}

		if (index < m_icurrbtn_index)
		{
			m_icurrbtn_index = m_icurrbtn_index - 1;
			get_navibtn_sepos(m_icurrbtn_index, spos, epos);
			construct_window(spos, epos);
		}
		else if (index == m_icurrbtn_index)
		{
			//m_icurrbtn_index <= m_navi_node.size()时，m_icurrbtn_index不变
			if (m_icurrbtn_index > m_navi_node.size())
				m_icurrbtn_index = m_icurrbtn_index - 1;

			if (m_icurrbtn_index > 0)
			{
				get_navibtn_sepos(m_icurrbtn_index, spos, epos);
				construct_window(spos, epos);

				//设置导航栏当前按钮背景为乳白色
				m_navi_node.at(m_icurrbtn_index - 1)->p_navigation_btn->setStyleSheet("QPushButton{font-family:'宋体';font-size:22px;color:rgb(0,0,255);}QPushButton{background-color:rgb(255,251,240)}QPushButton:hover{background-color:rgb(255,251,240)}");
			}	
			else
				ui.m_content_saw->setMinimumSize(661, 661);
		}

		--m_navibtn_count;
		m_paddbtn->move(0, m_navibtn_count * 60);

		if (m_navibtn_count > 0)
		{
			temp_pnode = m_navi_node.at(m_icurrbtn_index - 1);
			ui.m_ltitle->setText(QString::fromLocal8Bit(temp_pnode->s_alias.c_str()));
		}
		else
			ui.m_ltitle->setText("");

		//根据导航栏按钮个数数，计算最大高度的像素值
		int height_pix = m_navibtn_count * 60 + 100;
		if (height_pix <= 780)
			height_pix = 780;
		ui.m_navi_scrollAreaWidget->setMinimumSize(120, height_pix);
	}
}

/*
index--导航栏当前按钮索引
点击导航栏上的 +按钮和节点按钮，都会调用此函数
*/
void json_tool::deal_navi_currbtn(int index)
{
	//多次点击导航栏同一个按钮，不做处理
	if (index == m_icurrbtn_index)
		return;

	m_iprebtn_index = m_icurrbtn_index;
	m_icurrbtn_index = index;

	string salias = m_navi_node.at(index-1)->s_alias;
	ui.m_ltitle->setText(QString::fromLocal8Bit(salias.data()));

	//恢复导航栏上非当前按钮的颜色
	for (auto it : m_navi_node)
	{
		//隐藏前一个当前按钮对应的界面所有元素
		if (it->p_navigation_btn && m_iprebtn_index == it->p_navigation_btn->get_index())
		{
			it->p_navigation_btn->setStyleSheet("QPushButton{font-family:'宋体';font-size:22px;color:rgb(0,0,255);}QPushButton{background-color:rgb(5,150,255)}QPushButton:hover{background-color:rgb(50,170,200)}");
			if (it->p_btadd)
				it->p_btadd->setVisible(false);

			int spos = 0, epos = 0;
			get_navibtn_sepos(m_iprebtn_index,spos,epos);
			for (int i = spos; i <= epos; ++i)
				show_hide_control(i, false);
		}	
		//显示当前按钮对应的界面所有元素
		else if (it->p_navigation_btn && m_icurrbtn_index == it->p_navigation_btn->get_index())
		{
			if (it->p_btadd)
				it->p_btadd->setVisible(true);

			int spos = 0, epos = 0;
			get_navibtn_sepos(m_icurrbtn_index, spos, epos);
			for (int i = spos; i <= epos; ++i)
				show_hide_control(i, true);

			construct_window(spos, epos);
		}
	}//end for (auto it : m_navi_node)
}
//end public functions

//begin private functions
void json_tool::release_vjson()
{
	m_navi_node.clear();

	for (auto iter : m_qjson)
	{
		delete iter;
		iter = nullptr;
	}
	m_qjson.clear();
}

void json_tool::write_json(FILE* fp)
{
	fwrite("{", 1, 1, fp);
	fwrite("\r\n", 1, 1, fp);

	int itype = 0;
	string sblank = "";
	string scontent = "";
	int iblank_count = 0;
	for (auto iter : m_qjson)
	{
		sblank = "";
		iblank_count = iter->blank_count;
		for (int i = 0; i < iblank_count; ++i)
			sblank += " ";
		sblank += "  ";

		itype = iter->type;
		switch (itype)
		{
		case 1:
		case 2:
		case 3:
		{
			iter->key.erase(0, iter->key.find_first_not_of(" "));
			iter->key.erase(iter->key.find_last_not_of(" ") + 1);

			scontent = sblank + "\"" + iter->key + "\":" + "\r\n";
		}
			break;
		case 4:
		{
			scontent = sblank + "{" + "\r\n"+sblank+"  \"alias\":\""+iter->parent->s_alias+"\"";
			if (iter->has_symbol)
				scontent += ",";
			scontent += "\r\n";
		}
			break;
		case 10:
		case 11:
		case 13:
		{
			scontent = sblank + "\"" + iter->key + "\":" + iter->value + ",\r\n";
			scontent += sblank + "    \"alias\":\"" + iter->s_alias + "\",\r\n";
			scontent += sblank + "    \"help\":\"" + iter->s_help + "\",\r\n";
			scontent += sblank + "    \"items\":\"" + iter->s_items + "\"";
			if (iter->has_symbol)
				scontent += ",";
			scontent += "\r\n";
		}
			break;
		case 12:
		{
			iter->key.erase(0, iter->key.find_first_not_of(" "));
			iter->key.erase(iter->key.find_last_not_of(" ") + 1);

			iter->value.erase(0, iter->value.find_first_not_of(" "));
			iter->value.erase(iter->value.find_last_not_of(" ") + 1);

			scontent = sblank + "\"" + iter->key + "\":" + "\"" + iter->value + "\",\r\n";
			scontent += sblank + "    \"alias\":\"" + iter->s_alias + "\",\r\n";
			scontent += sblank + "    \"help\":\"" + iter->s_help + "\",\r\n";
			scontent += sblank + "    \"items\":\"" + iter->s_items + "\"";
			if (iter->has_symbol)
				scontent += ",";
			scontent += "\r\n";
		}
			break;
		case 14:
		{
			iter->key.erase(0, iter->key.find_first_not_of(" "));
			iter->key.erase(iter->key.find_last_not_of(" ") + 1);

			iter->value.erase(0, iter->value.find_first_not_of(" "));
			iter->value.erase(iter->value.find_last_not_of(" ") + 1);

			scontent = sblank + "\"" + iter->key + "\":" + "\"" + iter->value + "\",\r\n";
			scontent += sblank + "    \"alias_mctrl\":\"" + iter->s_alias + "\",\r\n";
			scontent += sblank + "    \"help\":\"" + iter->s_help + "\",\r\n";
			scontent += sblank + "    \"ctrl_num\":\"" + std::to_string(iter->ctrlnum) + "\",\r\n";
			string stemp_name = "";
			string stemp_items = "";
			for (int i = 1; i <= iter->ctrlnum; ++i)
			{
				stemp_name = "items" + std::to_string(i);
				if (1 == i) stemp_items = iter->ctrl1_items;
				else if (2 == i) stemp_items = iter->ctrl2_items;
				else if (3 == i) stemp_items = iter->ctrl3_items;

				if (i != iter->ctrlnum)
					scontent += sblank + "    \"" + stemp_name + "\":\"" + stemp_items + "\",\r\n"; 
				else
					scontent += sblank + "    \"" + stemp_name + "\":\"" + stemp_items + "\",\r\n";
			}

			if (iter->has_symbol)
				scontent += ",";
			scontent += "\r\n";
		}
			break;
		case 15:
		{
			iter->key.erase(0, iter->key.find_first_not_of(" "));
			iter->key.erase(iter->key.find_last_not_of(" ") + 1);

			scontent = sblank + "\"" + iter->key + "\":" + iter->value + ",\r\n";
			scontent += sblank + "    \"alias_smooth\":\"" + iter->s_alias + "\",\r\n";
			scontent += sblank + "    \"help\":\"" + iter->s_help + "\",\r\n";
			scontent += sblank + "    \"items\":\"" + iter->s_items + "\",\r\n";
			scontent += sblank + "    \"bopen\":\"" + (iter->bopen?"true":"false") + "\",\r\n";
			scontent += sblank + "    \"bhigher\":\"" + (iter->bhigher?"true":"false")+"\"";
			
			if (iter->has_symbol)
				scontent += ",";
			scontent += "\r\n";
		}
			break;
		case 16:
		{
			iter->key.erase(0, iter->key.find_first_not_of(" "));
			iter->key.erase(iter->key.find_last_not_of(" ") + 1);

			if(iter->bcheck)
				scontent = sblank + "\"template\":\"" + iter->value + "\",\r\n";
			scontent += sblank + "\"" + iter->key + "\":\"" + iter->value + "\",\r\n";
			scontent += sblank + "    \"alias_template\":\"" + iter->s_alias + "\",\r\n";
			scontent += sblank + "    \"help\":\""  + "\",\r\n";
			scontent += sblank + "    \"items\":\"" + "\",\r\n";
			scontent += sblank + "    \"checked\":\"" + (iter->bcheck ? "true" : "false") + "\"";
			if (iter->has_symbol)
				scontent += ",";
			scontent += "\r\n";
		}
			break;
		case 20:
		{
			scontent = sblank + iter->key;
			if (1 == iter->prenode->type || 2 == iter->prenode->type)
			{
				scontent += sblank + "\r\n"+ sblank+"  \"alias:\"" + iter->prenode->s_alias+"\"";
				if (iter->prenode->has_symbol)
					scontent += ",";

				scontent += "\r\n";
			}
			else
			{
				scontent = sblank + iter->key;
				if (iter->has_symbol)
					scontent += ",";

				scontent += "\r\n";
			}	
		}
		    break;
		default:
			break;
		}//end switch (itype)

		fwrite(scontent.c_str(), 1, scontent.length(), fp);
		scontent = "";
	}//end for (auto iter : m_qjson)

	fwrite("}\r\n", 1, 1, fp);
}

QStringList json_tool::parse_items(string& item, string symbol)
{
	QString qitem = QString::fromLocal8Bit(item.data());
	QStringList ql = qitem.split(QString::fromLocal8Bit(symbol.c_str()));
	return ql;
}

void json_tool::erase_space(string& s)
{
	//去掉首尾空格
	s.erase(0, s.find_first_not_of(" "));
	s.erase(s.find_last_not_of(" ") + 1);

	int index = 0;
	if (!s.empty())
	{
		while ((index = s.find(' ', index)) != string::npos)
			s.erase(index, 1);

		index = 0;
		while ((index = s.find('\r', index)) != string::npos)
			s.erase(index, 1);

		index = 0;
		while ((index = s.find('\n', index)) != string::npos)
			s.erase(index, 1);
	}
}

void json_tool::parse_property(string& sjson, pm_node pnode,int type)
{
	assert(pnode);

	switch (type)
	{
	case 1:
	case 2:
	case 3:
	case 4:
	{
		int icurr_pos = sjson.find_first_of(":");
		sjson = sjson.substr(icurr_pos + 2, sjson.length());
		icurr_pos = sjson.find_first_of("\"");
		pnode->s_alias = sjson.substr(0, icurr_pos);

		sjson = sjson.substr(icurr_pos + 1, sjson.length());
	}
		break;
	case 10:
	case 11:
	case 12:
	case 13:
	{
		int icurr_pos = sjson.find_first_of("\"");
		sjson = sjson.substr(icurr_pos + 1, sjson.length());
		icurr_pos = sjson.find_first_of("\"");
		string salias = sjson.substr(0, icurr_pos);
		if ("alias_smooth" == salias)
			pnode->type = 15;
		else if ("alias_template" == salias)
			pnode->type = 16;

		icurr_pos = sjson.find_first_of(":");
		sjson = sjson.substr(icurr_pos + 2, sjson.length());
		icurr_pos = sjson.find_first_of("\"");
		pnode->s_alias = sjson.substr(0, icurr_pos);

		icurr_pos = sjson.find_first_of(":");
		sjson = sjson.substr(icurr_pos + 2, sjson.length());
		icurr_pos = sjson.find_first_of("\"");
		pnode->s_help = sjson.substr(0, icurr_pos);

		icurr_pos = sjson.find_first_of(":");
		sjson = sjson.substr(icurr_pos + 2, sjson.length());
		icurr_pos = sjson.find_first_of("\"");
		pnode->s_items = sjson.substr(0, icurr_pos);

		if (15 == pnode->type)
		{
			icurr_pos = sjson.find_first_of(":");
			sjson = sjson.substr(icurr_pos + 2, sjson.length());
			icurr_pos = sjson.find_first_of("\"");
			pnode->bopen = (sjson.substr(0, icurr_pos)=="true")?true:false;

			icurr_pos = sjson.find_first_of(":");
			sjson = sjson.substr(icurr_pos + 2, sjson.length());
			icurr_pos = sjson.find_first_of("\"");
			pnode->bhigher = (sjson.substr(0, icurr_pos) == "true") ? true : false;
		}
		else if (16 == pnode->type)
		{
			icurr_pos = sjson.find_first_of(":");
			sjson = sjson.substr(icurr_pos + 2, sjson.length());
			icurr_pos = sjson.find_first_of("\"");
			pnode->bcheck = (sjson.substr(0, icurr_pos) == "true") ? true : false;
		}

		sjson = sjson.substr(icurr_pos + 1, sjson.length());
	}
		break;
	case 14:
	{
		int icurr_pos = sjson.find_first_of("\"");
		sjson = sjson.substr(icurr_pos + 1, sjson.length());
		icurr_pos = sjson.find_first_of("\"");
		string salias = sjson.substr(0, icurr_pos);
		if ("alias_mctrl" != salias)
		{
			pnode->type = 12;

			icurr_pos = sjson.find_first_of(":");
			sjson = sjson.substr(icurr_pos + 2, sjson.length());
			icurr_pos = sjson.find_first_of("\"");
			pnode->s_alias = sjson.substr(0, icurr_pos);

			sjson = sjson.substr(icurr_pos + 1, sjson.length());
		}
		else
		{
			icurr_pos = sjson.find_first_of(":");
			sjson = sjson.substr(icurr_pos + 2, sjson.length());
			icurr_pos = sjson.find_first_of("\"");
			pnode->s_alias = sjson.substr(0, icurr_pos);

			icurr_pos = sjson.find_first_of(":");
			sjson = sjson.substr(icurr_pos + 2, sjson.length());
			icurr_pos = sjson.find_first_of("\"");
			pnode->s_help = sjson.substr(0, icurr_pos);

			icurr_pos = sjson.find_first_of(":");
			sjson = sjson.substr(icurr_pos + 2, sjson.length());
			icurr_pos = sjson.find_first_of("\"");
			string scount = sjson.substr(0, icurr_pos);
			int icount = atoi(scount.c_str());
			pnode->ctrlnum = icount;
			for (int i = 0; i < icount; ++i)
			{
				icurr_pos = sjson.find_first_of(":");
				sjson = sjson.substr(icurr_pos + 2, sjson.length());
				icurr_pos = sjson.find_first_of("\"");
				string sitems = sjson.substr(0,icurr_pos);
				if (0 == i) 
					pnode->ctrl1_items = sitems;
				else if (1 == i)
					pnode->ctrl2_items = sitems;
				else if (2 == i)
					pnode->ctrl3_items = sitems;
			}
			sjson = sjson.substr(icurr_pos + 1, sjson.length());

			QStringList ql = parse_items(pnode->value, "&");
			for (int j = 0; j < ql.count(); ++j)
			{
				QString qs_value = ql.at(j);
				if ("true" == qs_value || "false" == qs_value || "TRUE" == qs_value ||
					"FALSE" == qs_value || "True" == qs_value || "False" == qs_value)
				{
					if (0 == j)
					{
						pnode->type1 = 13;
						pnode->ctrl1_value = qs_value.toLocal8Bit();
					}
					else if (1 == j)
					{
						pnode->type2 = 13;
						pnode->ctrl2_value = qs_value.toLocal8Bit();
					}
					else if (2 == j)
					{
						pnode->type3 = 13;
						pnode->ctrl3_value = qs_value.toLocal8Bit();
					}	
				}
				else
				{
					for (int k = 0; k < qs_value.size(); ++k)
					{
						if (0 == j)
							pnode->ctrl1_value = qs_value.toLocal8Bit();
						else if (1 == j)
							pnode->ctrl2_value = qs_value.toLocal8Bit();
						else if (2 == j)
							pnode->ctrl3_value = qs_value.toLocal8Bit();

						if ((qs_value.at(k) >= '0' && qs_value.at(k) <= '9') || '.' == qs_value.at(k))
						{
							if ('.' == qs_value.at(k))
							{
								if (0 == j)
									pnode->type1 = 11;
								else if (1 == j)
									pnode->type2 = 11;
								else if (2 == j)
									pnode->type3 = 11;
							}
							else
							{
                                if (0 == j && pnode->type1!=11)
								    pnode->type1 = 10;
							    else if (1 == j && pnode->type2 != 11)
								    pnode->type2 = 10;
							    else if (2 == j && pnode->type3 != 11)
								    pnode->type3 = 10;
							}
						}
						else
						{
							if (0 == j)
								pnode->type1 = 12;
							else if (1 == j)
								pnode->type2 = 12;
							else if (2 == j)
								pnode->type3 = 12;
							break;
						}
					}//end for (int k = 0; k < qs_value.size(); ++k)
				}
			}//end for (int j = 0; j < ql.count(); ++j)
		}
	}
		break;
	default:
		break;
	}
}

void json_tool::parse_json(QString path)
{
	assert(path != "");

	m_fp = fopen(path.toLocal8Bit().data(), "rb");
	assert(m_fp);

	struct stat buf;
	int fdsb = fileno(m_fp);
	fstat(fdsb, &buf);
	int fsize = buf.st_size;

	char* pfile_buf = nullptr;
	pfile_buf = new char[fsize + 1];
	assert(pfile_buf);

	size_t st = fread(pfile_buf, 1, fsize, m_fp);
	if (0 == st)
		return;

	fclose(m_fp);
	m_json_content.assign(pfile_buf, pfile_buf + st);
	if (pfile_buf)
	{
		delete[]pfile_buf;
		pfile_buf = nullptr;
	}

	erase_space(m_json_content);   //去掉所有的空格和回车
	//if (m_bchange_code)
	//	m_json_content = UTF8ToGB(m_json_content.c_str());

	//去掉首尾的{}
	if (m_json_content.at(0) != '{')
		return;
	m_json_content = m_json_content.substr(1, m_json_content.length() - 2);

	int cur_pos = 0;
	string skey = "";
	pm_node pnode = nullptr;
	pm_node prenode = nullptr;
	pm_node pnode_symbol = nullptr;                   //保存符号的节点

	while (m_json_content.length() != 0)
	{
		if (',' == m_json_content.at(0))
		{
			m_json_content = m_json_content.substr(1, m_json_content.length());
			continue;
		}
		else if ('}' == m_json_content.at(0))
		{
			pnode_symbol = m_qjson.at(m_qjson.size() - 1);
			if (m_stnode.size() > 0)
			{
				pm_node temp_pnode = m_stnode.top();
				if (temp_pnode)
				{
					m_stnode.pop();
					temp_pnode = nullptr;
				}
			}

			if (pnode_symbol && pnode_symbol->has_symbol == true && pnode_symbol->key == "}")
			{
				pnode_symbol->key = "}";
				pnode_symbol->has_symbol = false;
			}
			else if (pnode_symbol && pnode_symbol->has_symbol == true && pnode_symbol->key == "]")
			{
				pnode_symbol->key = "]";
				pnode_symbol->has_symbol = false;
			}

			pnode_symbol = new struct m_node();
			if (1 == m_json_content.length())
				pnode_symbol->has_symbol = false;
			else
				pnode_symbol->has_symbol = true;

			pnode_symbol->key = "}";

			pnode_symbol->type = 20;
			pnode_symbol->prenode = prenode;
			pnode_symbol->bshow = true;
			if (m_stnode.size() > 0)
				pnode_symbol->parent = m_stnode.top();
			pnode_symbol->blank_count = prenode->blank_count - 2;
			prenode = pnode_symbol;
			m_qjson.push_back(pnode_symbol);

			m_json_content = m_json_content.substr(1, m_json_content.length());
			continue;
		}
		//数组内的{
		else if ('{' == m_json_content.at(0) && 3 == (m_qjson.at(m_qjson.size() - 1))->parent->type)
		{
			pnode = new struct m_node();
			pnode->key = "virtual";
			pnode->type = 4;
			pnode->prenode = prenode;
			pnode->parent = prenode->parent;
			pnode->has_symbol = true;
			pnode->bshow = true;
			pnode->blank_count = (m_qjson.at(m_qjson.size() - 1))->parent->blank_count + 2;
			prenode = pnode;

			m_qjson.push_back(pnode);
			m_stnode.push(pnode);
			int itemp_pos = m_json_content.find_first_of(",");
			m_json_content = m_json_content.substr(itemp_pos + 1, m_json_content.length());
			continue;
		}
		else if (']' == m_json_content.at(0))
		{
			pnode_symbol = m_qjson.at(m_qjson.size() - 1);
			if (pnode_symbol && pnode_symbol->has_symbol == true && pnode_symbol->key == "}")
				pnode_symbol->has_symbol = false;
			else if (pnode_symbol && pnode_symbol->has_symbol == true && pnode_symbol->key == "]")
				pnode_symbol->has_symbol = false;

			pnode_symbol = new struct m_node();
			pnode_symbol->key = "]";
			pnode_symbol->type = 20;
			pnode_symbol->blank_count = prenode->blank_count - 2;
			pnode_symbol->prenode = prenode;
			pnode_symbol->bshow = true;
			if (1 == m_json_content.length())
				pnode_symbol->has_symbol = false;
			else
				pnode_symbol->has_symbol = true;

			pm_node temp_pnode = nullptr;
			if (m_stnode.size() > 0)
			{
				temp_pnode = m_stnode.top();
				if (temp_pnode)
				{
					m_stnode.pop();
					temp_pnode = nullptr;
				}
			}
			if (m_stnode.size() > 0)
				pnode_symbol->parent = m_stnode.top();

			prenode = pnode_symbol;
			m_qjson.push_back(pnode_symbol);
			m_json_content = m_json_content.substr(1, m_json_content.length());
			continue;
		}

		cur_pos = m_json_content.find_first_of("\"");
		m_json_content = m_json_content.substr(cur_pos + 1, m_json_content.length());
		cur_pos = m_json_content.find_first_of("\"");
		skey = m_json_content.substr(0, cur_pos);
		m_json_content = m_json_content.substr(cur_pos + 2, m_json_content.length());
		if ("template" == skey)       //template 为模板型节点的关键字
		{
			cur_pos = m_json_content.find_first_of(",");
			m_json_content = m_json_content.substr(cur_pos + 1, m_json_content.length());
			continue;
		}
		cur_pos = 0;

		pnode = new struct m_node();
		pnode->key = skey;
		if (m_stnode.size())
			pnode->parent = m_stnode.top();
		pnode->prenode = prenode;
		pnode->has_symbol = true;
		pnode->bshow = true;
		pnode->bstatus = false;
		char chtype = m_json_content.at(0);

		if ('"' == chtype)
		{
			pnode->type = 12;
			m_json_content = m_json_content.substr(1, m_json_content.length());
			cur_pos = m_json_content.find_first_of("\"");
			pnode->value = m_json_content.substr(0, cur_pos);
			m_json_content = m_json_content.substr(cur_pos + 1, m_json_content.length());

			if (pnode->parent)
				pnode->blank_count = pnode->parent->blank_count + 2;
			else
				pnode->blank_count = 0;
			
			if (pnode->value.npos != pnode->value.find('&'))
			{
				pnode->type = 14;
				parse_property(m_json_content, pnode, 14);
			}
			else
			    parse_property(m_json_content, pnode, 12);

			prenode = pnode;
			m_qjson.push_back(pnode);
		}
		else if (chtype >= '0' && chtype <= '9')
		{
			pnode->type = 10;
			for (int i = 0; i < m_json_content.length(); ++i)
			{
				if ((m_json_content.at(i) >= '0' && m_json_content.at(i) <= '9') || m_json_content.at(i) == '.')
				{
					if (m_json_content.at(i) == '.')
						pnode->type = 11;
					++cur_pos;
				}
				else
					break;
			}
			pnode->value = m_json_content.substr(0, cur_pos);
			m_json_content = m_json_content.substr(cur_pos, m_json_content.length());

			if (pnode->parent)
				pnode->blank_count = pnode->parent->blank_count + 2;
			else
				pnode->blank_count = 0;

			parse_property(m_json_content, pnode,10);

			prenode = pnode;
			m_qjson.push_back(pnode);
		}
		else if ('T' == chtype || 't' == chtype || 'F' == chtype || 'f' == chtype)
		{
			pnode->type = 13;
			if ('T' == chtype || 't' == chtype)
			{
				pnode->value = m_json_content.substr(0, 4);
				m_json_content = m_json_content.substr(4, m_json_content.length());
			}
			else
			{
				pnode->value = m_json_content.substr(0, 5);
				m_json_content = m_json_content.substr(5, m_json_content.length());
			}

			if (pnode->parent)
				pnode->blank_count = pnode->parent->blank_count + 2;
			else
				pnode->blank_count = 0;

			parse_property(m_json_content, pnode,13);

			prenode = pnode;
			m_qjson.push_back(pnode);
		}
		else if ('{' == chtype)
		{
			if (pnode->parent)
			{
				pnode->blank_count = pnode->parent->blank_count + 2;
				pnode->type = 2;
				parse_property(m_json_content, pnode, 2);
			}	
			else
			{
				pnode->blank_count = 0;
				pnode->type = 1;
				parse_property(m_json_content, pnode, 1);
			}	
			//parse_object_property(m_json_content, pnode);

			prenode = pnode;
			m_qjson.push_back(pnode);
			m_stnode.push(pnode);

			pnode_symbol = new struct m_node();
			pnode_symbol->key = "{";
			pnode_symbol->type = 20;
			pnode_symbol->blank_count = pnode->blank_count; 
			pnode_symbol->prenode = pnode;
			pnode_symbol->parent = pnode->parent;
			pnode_symbol->bshow = true;
			m_qjson.push_back(pnode_symbol);

			prenode = pnode_symbol;
			//m_json_content = m_json_content.substr(1,m_json_content.length());
		}
		else if ('[' == chtype)
		{
			pnode->type = 3;
			if (pnode->parent)
				pnode->blank_count = pnode->parent->blank_count + 2;
			else
				pnode->blank_count = 0;

			prenode = pnode;
			m_qjson.push_back(pnode);
			m_stnode.push(pnode);

			pnode_symbol = new struct m_node();
			pnode_symbol->key = "[";
			pnode_symbol->type = 20;
			pnode_symbol->blank_count = pnode->blank_count;
			pnode_symbol->prenode = pnode;
			pnode_symbol->parent = pnode->parent;
			pnode_symbol->bshow = true;
			m_qjson.push_back(pnode_symbol);

			prenode = pnode_symbol;
			m_json_content = m_json_content.substr(1, m_json_content.length());
		}

		if (0 == m_json_content.length())
		{
			pm_node temp_node = (m_qjson.at(m_qjson.size() - 1));
			temp_node->has_symbol = false;
			break;
		}

		cur_pos = 0;
		char chtype2 = m_json_content.at(0);
		if ('"' == chtype2)
			continue;
		else if (',' == chtype2)
		{
			m_json_content = m_json_content.substr(1, m_json_content.length());
			continue;
		}
		else if ('}' == chtype2)
		{
			pm_node temp_pnode = nullptr;
			temp_pnode = m_qjson.at(m_qjson.size() - 1);
			if (temp_pnode)
				temp_pnode->has_symbol = false;
			if (temp_pnode && "{" == temp_pnode->key)
			{
				pm_node temp_pnode1 = temp_pnode->prenode;
				if (temp_pnode1)
					temp_pnode1->has_symbol = false;
			}

			temp_pnode = m_stnode.top();
			if (temp_pnode)
			{
				m_stnode.pop();
				temp_pnode = nullptr;
			}

			pnode_symbol = new struct m_node();
			if (1 == m_json_content.length())
				pnode_symbol->has_symbol = false;
			else
				pnode_symbol->has_symbol = true;

			pnode_symbol->key = "}";

			pnode_symbol->type = 20;
			pnode_symbol->prenode = prenode;
			pnode_symbol->blank_count = prenode->blank_count - 2;
			pnode_symbol->bshow = true;
			if (m_stnode.size())
				pnode_symbol->parent = m_stnode.top();

			prenode = pnode_symbol;
			m_qjson.push_back(pnode_symbol);
			m_json_content = m_json_content.substr(1, m_json_content.length());
		}
		else if ('{' == chtype2)    //只有在上一个点是JSON 数组时"["才会有此种情况
		{
			pnode = new struct m_node();
			pnode->key = "virtual";
			pnode->type = 4;
			pnode->prenode = prenode;
			pnode->has_symbol = true;
			pnode->bshow = true;
			if (m_stnode.size())
				pnode->parent = m_stnode.top();

			if (pnode->parent)
				pnode->blank_count = pnode->parent->blank_count + 2;
			else
				pnode->blank_count = 0;

			//parse_object_property(m_json_content, pnode->parent);
			parse_property(m_json_content, pnode->parent,4);

			m_qjson.push_back(pnode);
			m_stnode.push(pnode);
			prenode = pnode;
		}
	}//end while (m_json_content.length() != 0)
}

//计算导航栏节点在m_qjson里的开始，结束的索引号
void json_tool::get_navibtn_sepos(int index, int& spos, int& epos)
{
	int ipos = -1;
	int iend = -1;
	bool bnavi_btn = false;
	
	for (auto it : m_qjson)
	{
		++ipos;
		if (1 == it->type && index == it->p_navigation_btn->get_index())
		{
			spos = ipos;
			bnavi_btn = true;
		}
		else if (bnavi_btn && ("{" == it->key || "[" == it->key || "virtual"==it->key))
		{
			if (-1 == iend)
				iend = 1;
			else
				++iend;
		}	
		else if (bnavi_btn && ("}" == it->key || "]" == it->key))
			--iend;

		if (bnavi_btn && 0 == iend)
		{
			epos = ipos;
			return;
		}	
	}//end for (auto it : m_qjson)

	return;
}

void json_tool::create_navibtn()
{
	int index = 0;
	for (auto iter : m_qjson)
	{
		if (1 == iter->type)
		{
			CMyButton* pbtn = new CMyButton(QString::fromLocal8Bit(iter->s_alias.c_str()), ui.m_navi_scrollAreaWidget);
			pbtn->set_type(1);
			pbtn->set_index(++index);
			pbtn->setText(QString::fromLocal8Bit(iter->s_alias.c_str()));
			pbtn->set_pos(-1);
			pbtn->set_jsontool(this);
			pbtn->setGeometry(0, (index - 1) * 60, 145, 60);
			//pbtn->setFlat(false);  
			//隐藏边框线
			if(1==index)
			    pbtn->setStyleSheet("QPushButton{font-family:'宋体';font-size:22px;color:rgb(0,0,255);}QPushButton{background-color:rgb(255,251,240)}QPushButton:hover{background-color:rgb(255,251,240)}");
			else
				pbtn->setStyleSheet("QPushButton{font-family:'宋体';font-size:22px;color:rgb(0,0,255);}QPushButton{background-color:rgb(5,150,255)}QPushButton:hover{background-color:rgb(50,170,200)}");
			pbtn->show();
			iter->p_navigation_btn = pbtn;

			CMyButton* padd_btn = new CMyButton("(+)", ui.m_content_saw);
			padd_btn->set_type(2);
			padd_btn->setText("(+)");
			padd_btn->set_pos(-1);
			padd_btn->set_jsontool(this);
			padd_btn->setGeometry(10, 10, 45, 30);
			padd_btn->setStyleSheet("QPushButton{font-family:'宋体';font-size:22px;color:rgb(0,0,255);}QPushButton{background-color:rgb(5,150,255)}QPushButton:hover{background-color:rgb(50,170,200)}");
			//padd_btn->show();
			iter->p_btadd = padd_btn;
			//m_qjson.push_back(pnode);
			m_navi_node.push_back(iter);
		}
	}

	string salias = m_navi_node.at(0)->s_alias;
	ui.m_ltitle->setText(QString::fromLocal8Bit(salias.data()));

	m_paddbtn->move(0, index * 60);
	m_navibtn_count = index;
	m_icurrbtn_index = 1;
	m_iprebtn_index = 1;
}

int json_tool::get_epos_by_spos(int spos)
{
	pm_node pnode = m_qjson.at(spos);
	if ("{" != pnode->key && "[" != pnode->key && "virtual"!=pnode->key)
		return -1;

	string skey = "",ekey = "";
	if ("{" == pnode->key || "virtual"==pnode->key)
	{
		skey = "{";
		ekey = "}";
	}	
	else
	{
		skey = "[";
		ekey = "]";
	}	

	int icount = 1, icount2 = 0;
	deque<pm_node>::iterator it = m_qjson.begin() + spos + 1;
	while (it != m_qjson.end())
	{
		++icount2;
		if ("{" == skey)
		{
			if ("{" == (*it)->key || "virtual" == (*it)->key)
				++icount;
			else if (ekey == (*it)->key)
				--icount;
		}
		else if ("[" == skey)
		{
			if(skey == (*it)->key)
				++icount;
			else if (ekey == (*it)->key)
				--icount;
		}
			
		if (0 == icount)
			break;

		++it;
	}
	return spos + icount2;
}

void json_tool::change_sepos(int& spos, int& epos)
{
	pm_node pnode1 = m_qjson.at(spos - 1);
	pm_node pnode2 = m_qjson.at(epos + 1);
	if (("{" != pnode1->key && "[" != pnode1->key && "virtual"!=pnode1->key) || 1 == pnode1->prenode->type)
		return;
	else if("}" != pnode2->key && "]" != pnode2->key)
		return;
	else
	{
		if ("virtual" == pnode1->key)
			spos = spos - 1;
		else
		    spos = spos - 2;
		epos = epos + 1;
		change_sepos(spos, epos);
	}   
}

void json_tool::change_btnpos(int pos, int distance)
{
	int newpos = pos - distance;
	if (newpos < 0)
		return;

	pm_node pnode = nullptr;
	pnode = m_qjson.at(pos);
	if (pnode)
	{
		if (pnode->p_navigation_btn)
			pnode->p_navigation_btn->set_pos(newpos);
		 
		if (pnode->p_btadd)
			pnode->p_btadd->set_pos(newpos);

		if (pnode->p_btsub)
			pnode->p_btsub->set_pos(newpos);

		if (pnode->p_btmod)
			pnode->p_btmod->set_pos(newpos);

		if (pnode->p_btadd_sub)
			pnode->p_btadd_sub->set_pos(newpos);

		if (pnode->p_btbottom_1)
			pnode->p_btbottom_1->set_pos(newpos);

		if (pnode->p_btbottom_2)
			pnode->p_btbottom_2->set_pos(newpos);

		if (pnode->p_btopen)
			pnode->p_btopen->set_pos(newpos);

		if (pnode->p_btclose)
			pnode->p_btclose->set_pos(newpos);

		if (pnode->p_bthigher)
			pnode->p_bthigher->set_pos(newpos);

		if (pnode->p_btlower)
			pnode->p_btlower->set_pos(newpos);		
	}
}

void json_tool::construct_window(int spos, int epos)
{
	if (spos == epos)
		return;

	pm_node pnode;
	int itype = 0;
	int init_x = 10;         //初始X坐标
	int init_y = 0;          //初始Y坐标
	int offset_x = 10;       //X坐标偏移量
	int offset_y = 35;       //Y坐标偏移量
	int curr_x = 10;         //当前X坐标
	int curr_y = 0;          //当前Y坐标
	int itemplate_index = 0;

	int iline_count = 0;
	int ilevel_count=0;
	for (int i = spos; i <= epos; ++i)
	{
		pnode = m_qjson.at(i);
		if (!pnode)
			continue;
		if (!pnode->bshow)
			continue;

		itype = pnode->type;
		switch (itype)
		{
		case 2:
		case 3:
		{
			++iline_count;
			if (2 == pnode->parent->type || 3 == pnode->parent->type)
				++ilevel_count;

			int ioffset = pnode->blank_count / 2;
			curr_x = init_x + offset_x * ioffset;
			curr_y = curr_y + offset_y;

			if (!pnode->p_btadd_sub)
			{
				pnode->p_btadd_sub = new CMyButton("-", ui.m_content_saw);
				pnode->p_btadd_sub->setText("-");
			}
			else
			{
				if (pnode->bstatus)    //收缩状态
					pnode->p_btadd_sub->setText("+");
				else
					pnode->p_btadd_sub->setText("-");
			}
			pnode->p_btadd_sub->set_jsontool(this);
			pnode->p_btadd_sub->set_pos(i);
			pnode->p_btadd_sub->setGeometry(curr_x-20, curr_y, 15, 15);
			pnode->p_btadd_sub->show();

			if (!pnode->plabel)
				pnode->plabel = new QLabel(ui.m_content_saw);

			QString sname = "";
			sname = QString::fromLocal8Bit(pnode->s_alias.data());
			if ("" == sname)
				sname = QString::fromLocal8Bit(pnode->key.data());
			pnode->plabel->setText(sname);
			pnode->plabel->move(curr_x, curr_y);
			pnode->plabel->adjustSize();
			pnode->plabel->show();

			QRect qrc = pnode->plabel->geometry();
			curr_x += qrc.width() + DISTANCE;
			if(!pnode->p_btsub)
			    pnode->p_btsub = new CMyButton("(-)", ui.m_content_saw);
			pnode->p_btsub->set_jsontool(this);
			pnode->p_btsub->setText("(-)");
			pnode->p_btsub->set_pos(i);
			pnode->p_btsub->setGeometry(curr_x, curr_y- CTRL_DISTANCE, BTN_WIDTH, BTN_HEIGHT);
			pnode->p_btsub->show();

			if ("virtual" != pnode->key)
			{
				curr_x += BTN_WIDTH + 5;
				if(!pnode->p_btmod)
				    pnode->p_btmod = new CMyButton("(m)", ui.m_content_saw);
				pnode->p_btmod->set_jsontool(this);
				pnode->p_btmod->setText("(m)");
				pnode->p_btmod->set_pos(i);
				pnode->p_btmod->setGeometry(curr_x, curr_y- CTRL_DISTANCE, BTN_WIDTH, BTN_HEIGHT);
				pnode->p_btmod->show();
			}
			if (m_bshow_btn)
			{
				pnode->p_btsub->setVisible(true);
				pnode->p_btmod->setVisible(true);
			}
			else
			{
				pnode->p_btsub->setVisible(false);
				pnode->p_btmod->setVisible(false);
			}
		}
		    break;
		case 10:
		case 11:
		case 12:
		case 13:
		{
			++iline_count;

			if(!pnode->plabel)
				pnode->plabel = new QLabel(ui.m_content_saw);
			
			QString sname = QString::fromLocal8Bit(pnode->s_alias.data());
			if ("" == sname)
				sname = QString::fromLocal8Bit(pnode->key.data());
			pnode->plabel->setText(sname);

			int ioffset = pnode->blank_count / 2;
			curr_x = init_x + offset_x * ioffset;
			curr_y = curr_y + offset_y;

			pnode->plabel->move(curr_x, curr_y+ CTRL_SUB_HEIGHT);
			pnode->plabel->adjustSize();
			pnode->plabel->show();

			curr_x += 120;
			int t_curr_y = curr_y - CTRL_SUB_HEIGHT;
			if (10 == itype)     //int
			{
				if(!pnode->pspin_edit)
					pnode->pspin_edit = new QSpinBox(ui.m_content_saw);

				sname = QString::fromLocal8Bit(pnode->value.data());
				pnode->pspin_edit->setMaximum(65537);
				pnode->pspin_edit->setMinimum(0);
				pnode->pspin_edit->setValue(sname.toInt());
				pnode->pspin_edit->move(curr_x, t_curr_y);
				pnode->pspin_edit->setGeometry(curr_x, t_curr_y, CTRL_LENGTH1, CTRL_HEIGHT1);
				pnode->pspin_edit->show();
			}
			else if (11 == itype)     //float
			{
				if(!pnode->pdspin_edit)
					pnode->pdspin_edit = new QDoubleSpinBox(ui.m_content_saw);

				sname = QString::fromLocal8Bit(pnode->value.data());
				pnode->pdspin_edit->setMaximum(1000000);
				pnode->pdspin_edit->setMinimum(0);
				pnode->pdspin_edit->setSingleStep(0.01);
				pnode->pdspin_edit->setValue(sname.toDouble());
				pnode->pdspin_edit->move(curr_x, t_curr_y);
				pnode->pdspin_edit->setGeometry(curr_x, t_curr_y, CTRL_LENGTH1, CTRL_HEIGHT1);
				pnode->pdspin_edit->show();
			}
			else if (12 == itype)     //string
			{
				if (pnode->s_items != "")
				{
					if(!pnode->pcombox)
						pnode->pcombox = new QComboBox(ui.m_content_saw);

					pnode->pcombox->clear();
					QStringList ql = parse_items(pnode->s_items,";");
					for (int i = 0; i < ql.count(); ++i)
						pnode->pcombox->addItem(ql.at(i));

					sname = QString::fromLocal8Bit(pnode->value.data());
					int index = pnode->pcombox->findText(sname);
					pnode->pcombox->setCurrentIndex(index);

					pnode->pcombox->move(curr_x, t_curr_y);
					pnode->pcombox->setGeometry(curr_x, t_curr_y, CTRL_LENGTH1, CTRL_HEIGHT1);
					pnode->pcombox->show();
				}
				else
				{
					if(!pnode->pline_edit)
						pnode->pline_edit = new QLineEdit(ui.m_content_saw);

					sname = QString::fromLocal8Bit(pnode->value.data());
					pnode->pline_edit->setText(sname);
					pnode->pline_edit->move(curr_x, t_curr_y);
					pnode->pline_edit->setGeometry(curr_x, t_curr_y, CTRL_LENGTH1, CTRL_HEIGHT1);
					pnode->pline_edit->show();
				}
			}
			else if (13 == itype)     //bool
			{
				if(!pnode->pcombox)
					pnode->pcombox = new QComboBox(ui.m_content_saw);

				pnode->pcombox->clear();
				QStringList ql = parse_items(pnode->s_items, ";");
				for (int i = 0; i < ql.count(); ++i)
					pnode->pcombox->addItem(ql.at(i));

				sname = QString::fromLocal8Bit(pnode->value.data());
				int index = pnode->pcombox->findText(sname);
				pnode->pcombox->setCurrentIndex(index);

				pnode->pcombox->move(curr_x, t_curr_y);
				pnode->pcombox->setGeometry(curr_x, t_curr_y, CTRL_LENGTH1, CTRL_HEIGHT1);

				pnode->pcombox->show();
			}

			curr_x += CTRL_LENGTH1 + DISTANCE;
			if(!pnode->p_btadd)
			    pnode->p_btadd = new CMyButton("(+)", ui.m_content_saw);
			pnode->p_btadd->set_jsontool(this);
			pnode->p_btadd->setText("(+)");
			pnode->p_btadd->set_pos(i);
			pnode->p_btadd->set_type(3);
			pnode->p_btadd->setGeometry(curr_x, t_curr_y, BTN_WIDTH, BTN_HEIGHT);
			pnode->p_btadd->show();

			curr_x += BTN_WIDTH + DISTANCE;
			if (!pnode->p_btsub)
			    pnode->p_btsub = new CMyButton("(-)", ui.m_content_saw);
			pnode->p_btsub->set_jsontool(this);
			pnode->p_btsub->setText("(-)");
			pnode->p_btsub->set_pos(i);
			pnode->p_btsub->setGeometry(curr_x, t_curr_y, BTN_WIDTH, BTN_HEIGHT);
			pnode->p_btsub->show();

			curr_x += BTN_WIDTH + DISTANCE;
			if (!pnode->p_btmod)
			    pnode->p_btmod = new CMyButton("(m)", ui.m_content_saw);
			pnode->p_btmod->set_jsontool(this);
			pnode->p_btmod->setText("(m)");
			pnode->p_btmod->set_pos(i);
			pnode->p_btmod->setGeometry(curr_x, t_curr_y, BTN_WIDTH, BTN_HEIGHT);
			pnode->p_btmod->show();

			if (m_bshow_btn)
			{
				pnode->p_btadd->setVisible(true);
				pnode->p_btsub->setVisible(true);
				pnode->p_btmod->setVisible(true);
				curr_x += BTN_WIDTH + DISTANCE;
			}
			else
			{
				pnode->p_btadd->setVisible(false);
				pnode->p_btsub->setVisible(false);
				pnode->p_btmod->setVisible(false);
				curr_x -= BTN_WIDTH*2 + DISTANCE * 2;
			}

			if(!pnode->plabel_des)
				pnode->plabel_des = new QLabel(ui.m_content_saw);

			sname = QString::fromLocal8Bit(pnode->s_help.data());
			if(sname!="")
			    sname = "(" + sname + ")";
			pnode->plabel_des->setText(sname);
			pnode->plabel_des->move(curr_x, curr_y+ CTRL_SUB_HEIGHT);
			pnode->plabel_des->adjustSize();
			pnode->plabel_des->show();
		}
			break;
		case 14:
		{
			++iline_count;

			if (!pnode->plabel)
				pnode->plabel = new QLabel(ui.m_content_saw);

			QString sname = QString::fromLocal8Bit(pnode->s_alias.data());
			if ("" == sname)
				sname = QString::fromLocal8Bit(pnode->key.data());
			pnode->plabel->setText(sname);

			int ioffset = pnode->blank_count / 2;
			curr_x = init_x + offset_x * ioffset;
			curr_y = curr_y + offset_y;

			pnode->plabel->move(curr_x, curr_y+ CTRL_SUB_HEIGHT);
			pnode->plabel->adjustSize();
			pnode->plabel->show();

			curr_x += 120;
			int t_curr_y = curr_y - CTRL_SUB_HEIGHT;

			if (1 == pnode->ctrlnum)
			{
				int ictrl1_type = pnode->type1;
				if (10 == ictrl1_type)     //int
				{
					if (!pnode->pspin_edit)
						pnode->pspin_edit = new QSpinBox(ui.m_content_saw);

					sname = QString::fromLocal8Bit(pnode->ctrl1_value.data());
					pnode->pspin_edit->setMaximum(65537);
					pnode->pspin_edit->setMinimum(0);
					pnode->pspin_edit->setValue(sname.toInt());
					pnode->pspin_edit->move(curr_x, t_curr_y);
					pnode->pspin_edit->setGeometry(curr_x, t_curr_y, CTRL_LENGTH1, CTRL_HEIGHT1);
					pnode->pspin_edit->show();
				}
				else if (11 == ictrl1_type)     //float
				{
					if (!pnode->pdspin_edit)
						pnode->pdspin_edit = new QDoubleSpinBox(ui.m_content_saw);

					sname = QString::fromLocal8Bit(pnode->ctrl1_value.data());
					pnode->pdspin_edit->setMaximum(1000000);
					pnode->pdspin_edit->setMinimum(0);
					pnode->pdspin_edit->setSingleStep(0.01);
					pnode->pdspin_edit->setValue(sname.toDouble());
					pnode->pdspin_edit->move(curr_x, t_curr_y);
					pnode->pdspin_edit->setGeometry(curr_x, t_curr_y, CTRL_LENGTH1, CTRL_HEIGHT1);
					pnode->pdspin_edit->show();
				}
				else if (12 == ictrl1_type)     //string
				{
					if (pnode->ctrl1_items != "")
					{
						//if (!pnode->pcombox)
						//	pnode->pcombox = new QComboBox(ui.m_content_saw);	
						if (!pnode->pcombox1)
								pnode->pcombox1 = new myComboBox(1,pnode,ui.m_content_saw);

						pnode->pcombox1->clear();
						QStringList ql = parse_items(pnode->ctrl1_items, ";");
						for (int i = 0; i < ql.count(); ++i)
							pnode->pcombox1->addItem(ql.at(i));

						sname = QString::fromLocal8Bit(pnode->ctrl1_value.data());
						int index = pnode->pcombox1->findText(sname);
						pnode->pcombox1->setCurrentIndex(index);

						pnode->pcombox1->move(curr_x, t_curr_y);
						pnode->pcombox1->setGeometry(curr_x, t_curr_y, CTRL_LENGTH1, CTRL_HEIGHT1);
						pnode->pcombox1->show();
					}
					else if ("palette" == pnode->ctrl1_value)
					{
						if(!pnode->plabel1)
						    pnode->plabel1 = new QLabel(ui.m_content_saw);
						
						QPixmap pix;
						pix.load("./Resources/palette_color1.bmp");
						pnode->plabel1->setPixmap(pix);
						pnode->plabel1->show();
						pnode->plabel1->adjustSize();
						pnode->plabel1->setGeometry(curr_x, t_curr_y, CTRL_LENGTH1, CTRL_HEIGHT1);
					}
					else
					{
						if (!pnode->pline_edit)
							pnode->pline_edit = new QLineEdit(ui.m_content_saw);

						sname = QString::fromLocal8Bit(pnode->ctrl1_value.data());
						pnode->pline_edit->setText(sname);
						pnode->pline_edit->move(curr_x, t_curr_y);
						pnode->pline_edit->setGeometry(curr_x, t_curr_y, CTRL_LENGTH1, CTRL_HEIGHT1);
						pnode->pline_edit->show();
					}
				}
				else if (13 == ictrl1_type)     //bool
				{
					if (!pnode->pcombox1)
							pnode->pcombox1 = new myComboBox(1,pnode,ui.m_content_saw);

					QStringList ql = parse_items(pnode->s_items, ";");
					for (int i = 0; i < ql.count(); ++i)
						pnode->pcombox1->addItem(ql.at(i));

					sname = QString::fromLocal8Bit(pnode->ctrl1_value.data());
					int index = pnode->pcombox1->findText(sname);
					pnode->pcombox1->setCurrentIndex(index);

					pnode->pcombox1->move(curr_x, t_curr_y);
					pnode->pcombox1->setGeometry(curr_x, t_curr_y, CTRL_LENGTH1, CTRL_HEIGHT1);
					pnode->pcombox1->show();
				}
			}
			else if (2 == pnode->ctrlnum)
			{
				int ictrl1_type = pnode->type1;
				if (10 == ictrl1_type)     //int
				{
					if (!pnode->pspin_edit)
						pnode->pspin_edit = new QSpinBox(ui.m_content_saw);

					sname = QString::fromLocal8Bit(pnode->ctrl1_value.data());
					pnode->pspin_edit->setMaximum(65537);
					pnode->pspin_edit->setMinimum(0);
					pnode->pspin_edit->setValue(sname.toInt());
					pnode->pspin_edit->move(curr_x, t_curr_y);
					pnode->pspin_edit->setGeometry(curr_x, t_curr_y, CTRL1_LENGTH, CTRL_HEIGHT1);
					pnode->pspin_edit->show();
				}
				else if (11 == ictrl1_type)     //float
				{
					if (!pnode->pdspin_edit)
						pnode->pdspin_edit = new QDoubleSpinBox(ui.m_content_saw);

					sname = QString::fromLocal8Bit(pnode->ctrl1_value.data());
					pnode->pdspin_edit->setMaximum(1000000);
					pnode->pdspin_edit->setMinimum(0);
					pnode->pdspin_edit->setSingleStep(0.01);
					pnode->pdspin_edit->setValue(sname.toDouble());
					pnode->pdspin_edit->move(curr_x, t_curr_y);
					pnode->pdspin_edit->setGeometry(curr_x, t_curr_y, CTRL1_LENGTH, CTRL_HEIGHT1);
					pnode->pdspin_edit->show();
				}
				else if (12 == ictrl1_type)     //string
				{
					if (pnode->ctrl1_items != "")
					{
						if (!pnode->pcombox1)                     
							pnode->pcombox1 = new myComboBox(1,pnode,ui.m_content_saw);

						pnode->pcombox1->clear();
						QStringList ql = parse_items(pnode->ctrl1_items, ";");
						for (int i = 0; i < ql.count(); ++i)
							pnode->pcombox1->addItem(ql.at(i));

						sname = QString::fromLocal8Bit(pnode->ctrl1_value.data());
						int index = pnode->pcombox1->findText(sname);
						pnode->pcombox1->setCurrentIndex(index);

						pnode->pcombox1->move(curr_x, t_curr_y);
						pnode->pcombox1->setGeometry(curr_x, t_curr_y, CTRL1_LENGTH, CTRL_HEIGHT1);
						pnode->pcombox1->show();
					}
					else if ("palette" == pnode->ctrl1_value)
					{
						if (!pnode->plabel1)
							pnode->plabel1 = new QLabel(ui.m_content_saw);

						QPixmap pix;
						pix.load("./Resources/palette_color2_1.bmp");
						pnode->plabel1->setPixmap(pix);
						pnode->plabel1->show();
						pnode->plabel1->adjustSize();
						pnode->plabel1->setGeometry(curr_x, t_curr_y, CTRL1_LENGTH, CTRL_HEIGHT1);
					}
					else
					{
						if (!pnode->pline_edit)
							pnode->pline_edit = new QLineEdit(ui.m_content_saw);

						sname = QString::fromLocal8Bit(pnode->ctrl1_value.data());
						pnode->pline_edit->setText(sname);
						pnode->pline_edit->move(curr_x, t_curr_y);
						pnode->pline_edit->setGeometry(curr_x, t_curr_y, CTRL1_LENGTH, CTRL_HEIGHT1);
						pnode->pline_edit->show();
					}
				}
				else if (13 == ictrl1_type)     //bool
				{
					if (!pnode->pcombox1)
							pnode->pcombox1 = new myComboBox(1,pnode,ui.m_content_saw);

					pnode->pcombox1->clear();
					QStringList ql = parse_items(pnode->ctrl1_items, ";");
					for (int i = 0; i < ql.count(); ++i)
						pnode->pcombox1->addItem(ql.at(i));

					sname = QString::fromLocal8Bit(pnode->ctrl1_value.data());
					int index = pnode->pcombox1->findText(sname);
					pnode->pcombox1->setCurrentIndex(index);

					pnode->pcombox1->move(curr_x, t_curr_y);
					pnode->pcombox1->setGeometry(curr_x, t_curr_y, CTRL1_LENGTH, CTRL_HEIGHT1);
					pnode->pcombox1->show();
				}

				int ctrl2_len = CTRL_LENGTH1 - CTRL1_LENGTH - DISTANCE;
				int ctrl2_offset = CTRL1_LENGTH + DISTANCE;
				int ictrl2_type = pnode->type2;
				if (10 == ictrl2_type)     //int
				{
					if (!pnode->pspin_edit2)
						pnode->pspin_edit2 = new QSpinBox(ui.m_content_saw);

					sname = QString::fromLocal8Bit(pnode->ctrl2_value.data());
					pnode->pspin_edit2->setMaximum(65537);
					pnode->pspin_edit2->setMinimum(0);
					pnode->pspin_edit2->setValue(sname.toInt());
					pnode->pspin_edit2->move(curr_x+ ctrl2_offset, t_curr_y);
					pnode->pspin_edit2->setGeometry(curr_x + ctrl2_offset, t_curr_y, ctrl2_len, CTRL_HEIGHT1);
					pnode->pspin_edit2->show();
				}
				else if (11 == ictrl2_type)     //float
				{
					if (!pnode->pdspin_edit2)
						pnode->pdspin_edit2 = new QDoubleSpinBox(ui.m_content_saw);

					sname = QString::fromLocal8Bit(pnode->ctrl2_value.data());
					pnode->pdspin_edit2->setMaximum(1000000);
					pnode->pdspin_edit2->setMinimum(0);
					pnode->pdspin_edit2->setSingleStep(0.01);
					pnode->pdspin_edit2->setValue(sname.toDouble());
					pnode->pdspin_edit2->move(curr_x + ctrl2_offset, t_curr_y);
					pnode->pdspin_edit2->setGeometry(curr_x + ctrl2_offset, t_curr_y, ctrl2_len, CTRL_HEIGHT1);
					pnode->pdspin_edit2->show();
				}
				else if (12 == ictrl2_type)     //string
				{
					if (pnode->ctrl2_items != "")
					{
						if (!pnode->pcombox2)
							pnode->pcombox2 = new myComboBox(2, pnode, ui.m_content_saw);	

						pnode->pcombox2->clear();
						QStringList ql = parse_items(pnode->ctrl2_items, ";");
						if (!pnode->pcombox1)
						{
							for (int i = 0; i < ql.count(); ++i)
								pnode->pcombox2->addItem(ql.at(i));
						}
						else
						{
							QString cur1_text=pnode->pcombox1->currentText();
							for (auto iter : ql)
							{
								QStringList ql_1 = iter.split("-");
								if (cur1_text == ql_1.at(0))
									pnode->pcombox2->addItem(ql_1.at(1));
							}
						}

						sname = QString::fromLocal8Bit(pnode->ctrl2_value.data());
						int index = pnode->pcombox2->findText(sname);
						if(index>=0)
						    pnode->pcombox2->setCurrentIndex(index);
						else
							pnode->pcombox2->setCurrentIndex(0);

						pnode->pcombox2->move(curr_x + ctrl2_offset, t_curr_y);
						pnode->pcombox2->setGeometry(curr_x + ctrl2_offset, t_curr_y, ctrl2_len, CTRL_HEIGHT1);
						pnode->pcombox2->show();
					}
					else if ("palette" == pnode->ctrl2_value)
					{
						if (!pnode->plabel2)
							pnode->plabel2 = new QLabel(ui.m_content_saw);

						QPixmap pix;
						pix.load("./Resources/palette_color2_2.bmp");
						pnode->plabel2->setPixmap(pix);
						pnode->plabel2->show();
						pnode->plabel2->adjustSize();
						pnode->plabel2->setGeometry(curr_x+ ctrl2_offset, t_curr_y, ctrl2_len, CTRL_HEIGHT1);
					}
					else
					{
						if (!pnode->pline_edit2)
							pnode->pline_edit2 = new QLineEdit(ui.m_content_saw);

						sname = QString::fromLocal8Bit(pnode->ctrl2_value.data());
						pnode->pline_edit2->setText(sname);
						pnode->pline_edit2->move(curr_x + ctrl2_offset, t_curr_y);
						pnode->pline_edit2->setGeometry(curr_x + ctrl2_offset, t_curr_y, ctrl2_len, CTRL_HEIGHT1);
						pnode->pline_edit2->show();
					}
				}
				else if (13 == ictrl2_type)     //bool
				{
					if (!pnode->pcombox2)
						pnode->pcombox2 = new myComboBox(2,pnode,ui.m_content_saw);

					pnode->pcombox2->clear();
					QStringList ql = parse_items(pnode->ctrl2_items, ";");
					for (int i = 0; i < ql.count(); ++i)
						pnode->pcombox2->addItem(ql.at(i));

					sname = QString::fromLocal8Bit(pnode->ctrl2_value.data());
					int index = pnode->pcombox2->findText(sname);
					pnode->pcombox2->setCurrentIndex(index);

					pnode->pcombox2->move(curr_x + ctrl2_offset, t_curr_y);
					pnode->pcombox2->setGeometry(curr_x + ctrl2_offset, t_curr_y, ctrl2_len, CTRL_HEIGHT1);
					pnode->pcombox2->show();
				}
			}
			else if (3 == pnode->ctrlnum)
			{
				int ictrl1_type = pnode->type1;
				if (10 == ictrl1_type)     //int
				{
					if (!pnode->pspin_edit)
						pnode->pspin_edit = new QSpinBox(ui.m_content_saw);

					sname = QString::fromLocal8Bit(pnode->ctrl1_value.data());
					pnode->pspin_edit->setMaximum(65537);
					pnode->pspin_edit->setMinimum(0);
					pnode->pspin_edit->setValue(sname.toInt());
					pnode->pspin_edit->move(curr_x, t_curr_y);
					pnode->pspin_edit->setGeometry(curr_x, t_curr_y, CTRL1_LENGTH, CTRL_HEIGHT1);
					pnode->pspin_edit->show();
				}
				else if (11 == ictrl1_type)     //float
				{
					if (!pnode->pdspin_edit)
						pnode->pdspin_edit = new QDoubleSpinBox(ui.m_content_saw);

					sname = QString::fromLocal8Bit(pnode->ctrl1_value.data());
					pnode->pdspin_edit->setMaximum(1000000);
					pnode->pdspin_edit->setMinimum(0);
					pnode->pdspin_edit->setSingleStep(0.01);
					pnode->pdspin_edit->setValue(sname.toDouble());
					pnode->pdspin_edit->move(curr_x, t_curr_y);
					pnode->pdspin_edit->setGeometry(curr_x, t_curr_y, CTRL1_LENGTH, CTRL_HEIGHT1);
					pnode->pdspin_edit->show();
				}
				else if (12 == ictrl1_type)     //string
				{
					if (pnode->ctrl1_items != "")
					{
						if (!pnode->pcombox1)
							pnode->pcombox1 = new myComboBox(1, pnode, ui.m_content_saw);

						pnode->pcombox1->clear();
						QStringList ql = parse_items(pnode->ctrl1_items, ";");
						for (int i = 0; i < ql.count(); ++i)
							pnode->pcombox1->addItem(ql.at(i));

						sname = QString::fromLocal8Bit(pnode->ctrl1_value.data());
						int index = pnode->pcombox1->findText(sname);
						pnode->pcombox1->setCurrentIndex(index);

						pnode->pcombox1->move(curr_x, t_curr_y);
						pnode->pcombox1->setGeometry(curr_x, t_curr_y, CTRL1_LENGTH, CTRL_HEIGHT1);
						pnode->pcombox1->show();
					}
					else if ("palette" == pnode->ctrl1_value)
					{
						if (!pnode->plabel1)
							pnode->plabel1 = new QLabel(ui.m_content_saw);

						QPixmap pix;
						pix.load("./Resources/palette_color3_1.bmp");
						pnode->plabel1->setPixmap(pix);
						pnode->plabel1->show();
						pnode->plabel1->adjustSize();
						pnode->plabel1->setGeometry(curr_x, t_curr_y, CTRL1_LENGTH, CTRL_HEIGHT1);
					}
					else
					{
						if (!pnode->pline_edit)
							pnode->pline_edit = new QLineEdit(ui.m_content_saw);

						sname = QString::fromLocal8Bit(pnode->ctrl1_value.data());
						pnode->pline_edit->setText(sname);
						pnode->pline_edit->move(curr_x, t_curr_y);
						pnode->pline_edit->setGeometry(curr_x, t_curr_y, CTRL1_LENGTH, CTRL_HEIGHT1);
						pnode->pline_edit->show();
					}
				}
				else if (13 == ictrl1_type)     //bool
				{
					if (!pnode->pcombox1)
							pnode->pcombox1 = new myComboBox(1,pnode,ui.m_content_saw);

					pnode->pcombox1->clear();
					QStringList ql = parse_items(pnode->ctrl1_items, ";");
					for (int i = 0; i < ql.count(); ++i)
						pnode->pcombox1->addItem(ql.at(i));

					sname = QString::fromLocal8Bit(pnode->ctrl1_value.data());
					int index = pnode->pcombox1->findText(sname);
					pnode->pcombox1->setCurrentIndex(index);

					pnode->pcombox1->move(curr_x, t_curr_y);
					pnode->pcombox1->setGeometry(curr_x, t_curr_y, CTRL1_LENGTH, CTRL_HEIGHT1);

					pnode->pcombox1->show();
				}

				int ctrl2_offset = CTRL1_LENGTH + DISTANCE;
				int ctrl2_len = CTRL_LENGTH1 - CTRL1_LENGTH * 2 - DISTANCE * 2;
				int ictrl2_type = pnode->type2;
				if (10 == ictrl2_type)     //int
				{
					if (!pnode->pspin_edit2)
						pnode->pspin_edit2 = new QSpinBox(ui.m_content_saw);

					sname = QString::fromLocal8Bit(pnode->ctrl2_value.data());
					pnode->pspin_edit2->setMaximum(65537);
					pnode->pspin_edit2->setMinimum(0);
					pnode->pspin_edit2->setValue(sname.toInt());
					pnode->pspin_edit2->move(curr_x + ctrl2_offset, t_curr_y);
					pnode->pspin_edit2->setGeometry(curr_x + ctrl2_offset, t_curr_y, CTRL1_LENGTH, CTRL_HEIGHT1);
					pnode->pspin_edit2->show();
				}
				else if (11 == ictrl2_type)     //float
				{
					if (!pnode->pdspin_edit2)
						pnode->pdspin_edit2 = new QDoubleSpinBox(ui.m_content_saw);

					sname = QString::fromLocal8Bit(pnode->ctrl2_value.data());
					pnode->pdspin_edit2->setMaximum(1000000);
					pnode->pdspin_edit2->setMinimum(0);
					pnode->pdspin_edit2->setSingleStep(0.01);
					pnode->pdspin_edit2->setValue(sname.toDouble());
					pnode->pdspin_edit2->move(curr_x + ctrl2_offset, t_curr_y);
					pnode->pdspin_edit2->setGeometry(curr_x + ctrl2_offset, t_curr_y, CTRL1_LENGTH, CTRL_HEIGHT1);
					pnode->pdspin_edit2->show();
				}
				else if (12 == ictrl2_type)     //string
				{
					if (pnode->ctrl2_items != "")
					{
						if (!pnode->pcombox2)
							pnode->pcombox2 = new myComboBox(2, pnode, ui.m_content_saw);	

						pnode->pcombox2->clear();
						QStringList ql = parse_items(pnode->ctrl2_items, ";");
						if (!pnode->pcombox1)
						{
							for (int i = 0; i < ql.count(); ++i)
								pnode->pcombox2->addItem(ql.at(i));
						}
						else
						{
							QString cur1_text = pnode->pcombox1->currentText();
							for (auto iter : ql)
							{
								QStringList ql_1 = iter.split("-");
								if (cur1_text == ql_1.at(0))
									pnode->pcombox2->addItem(ql_1.at(1));
							}
						}

						sname = QString::fromLocal8Bit(pnode->ctrl2_value.data());
						int index = pnode->pcombox2->findText(sname);
						if(index>=0)
						    pnode->pcombox2->setCurrentIndex(index);
						else
							pnode->pcombox2->setCurrentIndex(0);

						pnode->pcombox2->move(curr_x + ctrl2_offset, t_curr_y);
						pnode->pcombox2->setGeometry(curr_x + ctrl2_offset, t_curr_y, CTRL1_LENGTH, CTRL_HEIGHT1);
						pnode->pcombox2->show();
					}
					else if ("palette" == pnode->ctrl2_value)
					{
						if (!pnode->plabel2)
							pnode->plabel2 = new QLabel(ui.m_content_saw);

						QPixmap pix;
						pix.load("./Resources/palette_color3_2.bmp");
						pnode->plabel2->setPixmap(pix);
						pnode->plabel2->show();
						pnode->plabel2->adjustSize();
						pnode->plabel2->setGeometry(curr_x+ ctrl2_offset, t_curr_y, ctrl2_len, CTRL_HEIGHT1);
					}
					else
					{
						if (!pnode->pline_edit2)
							pnode->pline_edit2 = new QLineEdit(ui.m_content_saw);

						sname = QString::fromLocal8Bit(pnode->ctrl2_value.data());
						pnode->pline_edit2->setText(sname);
						pnode->pline_edit2->move(curr_x + ctrl2_offset, t_curr_y);
						pnode->pline_edit2->setGeometry(curr_x + ctrl2_offset, t_curr_y, CTRL1_LENGTH, CTRL_HEIGHT1);
						pnode->pline_edit2->show();
					}
				}
				else if (13 == ictrl2_type)     //bool
				{
					if (!pnode->pcombox2)
						pnode->pcombox2 = new myComboBox(2,pnode,ui.m_content_saw);

					pnode->pcombox2->clear();
					QStringList ql = parse_items(pnode->ctrl2_items, ";");
					for (int i = 0; i < ql.count(); ++i)
						pnode->pcombox2->addItem(ql.at(i));

					sname = QString::fromLocal8Bit(pnode->ctrl2_value.data());
					int index = pnode->pcombox2->findText(sname);
					pnode->pcombox2->setCurrentIndex(index);

					pnode->pcombox2->move(curr_x + ctrl2_offset, t_curr_y);
					pnode->pcombox2->setGeometry(curr_x + ctrl2_offset, t_curr_y, CTRL1_LENGTH, CTRL_HEIGHT1);
					pnode->pcombox2->show();
				}

				int offset = 0;
				int width = 0;
				if ("palette" == pnode->ctrl2_value)
				{
					offset = CTRL_LENGTH1-CTRL1_LENGTH;
					width = CTRL1_LENGTH;
				}
				else
				{
					offset = CTRL1_LENGTH * 2 + DISTANCE * 2;
					width = CTRL_LENGTH1-CTRL1_LENGTH*2-DISTANCE*2;
				}

				int ictrl3_type = pnode->type3;
				if (10 == ictrl3_type)     //int
				{
					if (!pnode->pspin_edit3)
						pnode->pspin_edit3 = new QSpinBox(ui.m_content_saw);

					sname = QString::fromLocal8Bit(pnode->ctrl3_value.data());
					pnode->pspin_edit3->setMaximum(65537);
					pnode->pspin_edit3->setMinimum(0);
					pnode->pspin_edit3->setValue(sname.toInt());
					pnode->pspin_edit3->move(curr_x + offset, t_curr_y);
					pnode->pspin_edit3->setGeometry(curr_x + offset, t_curr_y, width, CTRL_HEIGHT1);
					pnode->pspin_edit3->show();
				}
				else if (11 == ictrl3_type)     //float
				{
					if (!pnode->pdspin_edit3)
						pnode->pdspin_edit3 = new QDoubleSpinBox(ui.m_content_saw);

					sname = QString::fromLocal8Bit(pnode->ctrl3_value.data());
					pnode->pdspin_edit3->setMaximum(1000000);
					pnode->pdspin_edit3->setMinimum(0);
					pnode->pdspin_edit3->setSingleStep(0.01);
					pnode->pdspin_edit3->setValue(sname.toDouble());
					pnode->pdspin_edit3->move(curr_x + offset, t_curr_y);
					pnode->pdspin_edit3->setGeometry(curr_x + offset, t_curr_y, width, CTRL_HEIGHT1);
					pnode->pdspin_edit3->show();
				}
				else if (12 == ictrl3_type)     //string
				{
					if (pnode->ctrl3_items != "")
					{
						if (!pnode->pcombox3)
							pnode->pcombox3 = new myComboBox(3,pnode,ui.m_content_saw);	

						pnode->pcombox3->clear();
						QStringList ql = parse_items(pnode->ctrl3_items, ";");
						if (!pnode->pcombox2)
						{
							for (int i = 0; i < ql.count(); ++i)
								pnode->pcombox3->addItem(ql.at(i));
						}
						else
						{
							QString cur1_text = pnode->pcombox2->currentText();
							for (auto iter : ql)
							{
								QStringList ql_1 = iter.split("-");
								if (cur1_text == ql_1.at(0))
									pnode->pcombox3->addItem(ql_1.at(1));
							}
						}

						sname = QString::fromLocal8Bit(pnode->ctrl3_value.data());
						int index = pnode->pcombox3->findText(sname);
						if(index>=0)
						    pnode->pcombox3->setCurrentIndex(index);
						else
							pnode->pcombox3->setCurrentIndex(0);

						pnode->pcombox3->move(curr_x + offset, t_curr_y);
						pnode->pcombox3->setGeometry(curr_x + offset, t_curr_y, width, CTRL_HEIGHT1);
						pnode->pcombox3->show();
					}
					else if ("palette" == pnode->ctrl3_value)
					{
						if (!pnode->plabel3)
							pnode->plabel3 = new QLabel(ui.m_content_saw);

						QPixmap pix;
						if("palette"== pnode->ctrl2_value)
						    pix.load("./Resources/palette_color3_1.bmp");
						else
							pix.load("./Resources/palette_color3_2.bmp");
						pnode->plabel3->setPixmap(pix);
						pnode->plabel3->show();
						pnode->plabel3->adjustSize();
						pnode->plabel3->setGeometry(curr_x + offset, t_curr_y, width, CTRL_HEIGHT1);
					}
					else
					{
						if (!pnode->pline_edit3)
							pnode->pline_edit3 = new QLineEdit(ui.m_content_saw);

						sname = QString::fromLocal8Bit(pnode->ctrl3_value.data());
						pnode->pline_edit3->setText(sname);
						pnode->pline_edit3->move(curr_x + offset, t_curr_y);
						pnode->pline_edit3->setGeometry(curr_x + offset, t_curr_y, width, CTRL_HEIGHT1);
						pnode->pline_edit3->show();
					}
				}
				else if (13 == ictrl3_type)     //bool
				{
					if (!pnode->pcombox3)
						pnode->pcombox3 = new myComboBox(3,pnode,ui.m_content_saw);

					pnode->pcombox3->clear();
					QStringList ql = parse_items(pnode->ctrl3_items, ";");
					for (int i = 0; i < ql.count(); ++i)
						pnode->pcombox3->addItem(ql.at(i));

					sname = QString::fromLocal8Bit(pnode->ctrl3_value.data());
					int index = pnode->pcombox3->findText(sname);
					pnode->pcombox3->setCurrentIndex(index);

					pnode->pcombox3->move(curr_x + offset, t_curr_y);
					pnode->pcombox3->setGeometry(curr_x + offset, t_curr_y, width, CTRL_HEIGHT1);
					pnode->pcombox3->show();
				}
			}

			curr_x += CTRL_LENGTH1 + DISTANCE;
			if (!pnode->p_btadd)
				pnode->p_btadd = new CMyButton("(+)", ui.m_content_saw);
			pnode->p_btadd->set_jsontool(this);
			pnode->p_btadd->setText("(+)");
			pnode->p_btadd->set_pos(i);
			pnode->p_btadd->set_type(3);
			pnode->p_btadd->setGeometry(curr_x, t_curr_y, BTN_WIDTH, BTN_HEIGHT);
			pnode->p_btadd->show();

			curr_x += BTN_WIDTH + DISTANCE;
			if (!pnode->p_btsub)
				pnode->p_btsub = new CMyButton("(-)", ui.m_content_saw);
			pnode->p_btsub->set_jsontool(this);
			pnode->p_btsub->setText("(-)");
			pnode->p_btsub->set_pos(i);
			pnode->p_btsub->setGeometry(curr_x, t_curr_y, BTN_WIDTH, BTN_HEIGHT);
			pnode->p_btsub->show();

			curr_x += BTN_WIDTH + DISTANCE;
			if (!pnode->p_btmod)
				pnode->p_btmod = new CMyButton("(m)", ui.m_content_saw);
			pnode->p_btmod->set_jsontool(this);
			pnode->p_btmod->setText("(m)");
			pnode->p_btmod->set_pos(i);
			pnode->p_btmod->setGeometry(curr_x, t_curr_y, BTN_WIDTH, BTN_HEIGHT);
			pnode->p_btmod->show();

			if (m_bshow_btn)
			{
				pnode->p_btadd->setVisible(true);
				pnode->p_btsub->setVisible(true);
				pnode->p_btmod->setVisible(true);
				curr_x += BTN_WIDTH + DISTANCE;
			}
			else
			{
				pnode->p_btadd->setVisible(false);
				pnode->p_btsub->setVisible(false);
				pnode->p_btmod->setVisible(false);
				curr_x -= BTN_WIDTH*2+DISTANCE*2;
			}

			if (!pnode->plabel_des)
				pnode->plabel_des = new QLabel(ui.m_content_saw);

			sname = QString::fromLocal8Bit(pnode->s_help.data());
			if (sname != "")
				sname = "(" + sname + ")";
			pnode->plabel_des->setText(sname);
			pnode->plabel_des->move(curr_x, curr_y + CTRL_SUB_HEIGHT);
			pnode->plabel_des->adjustSize();
			pnode->plabel_des->show();
		}
			break;
		case 15:
		{
			++iline_count;
			++iline_count;
			if (!pnode->plabel)
				pnode->plabel = new QLabel(ui.m_content_saw);

			QString sname = QString::fromLocal8Bit(pnode->s_alias.data());
			if ("" == sname)
				sname = QString::fromLocal8Bit(pnode->key.data());
			pnode->plabel->setText(sname);

			int ioffset = pnode->blank_count / 2;
			curr_x = init_x + offset_x * ioffset;
			curr_y = curr_y + offset_y;

			pnode->plabel->move(curr_x, curr_y);
			pnode->plabel->adjustSize();
			pnode->plabel->show();

			curr_x += 120;
			int t_curr_y = curr_y - CTRL_SUB_HEIGHT;

			if (!pnode->p_btbottom_1)
				pnode->p_btbottom_1 = new CMyButton("bt1", ui.m_content_saw);

			pnode->p_btbottom_1->set_jsontool(this);
			pnode->p_btbottom_1->setText("");
			pnode->p_btbottom_1->set_pos(i);
			pnode->p_btbottom_1->set_type(5);
			pnode->p_btbottom_1->setGeometry(curr_x, t_curr_y, 65, BTN_HEIGHT);
			pnode->p_btbottom_1->setEnabled(false);
			pnode->p_btbottom_1->show();
			pnode->p_btbottom_1->setStyleSheet("background-color: rgb(211,211,211);border-radius:15px;padding:2px 4px;");

			if (!pnode->p_btopen)
				pnode->p_btopen = new CMyButton("bt_open", ui.m_content_saw);

			pnode->p_btopen->set_jsontool(this);
			pnode->p_btopen->setText(QString::fromLocal8Bit("开"));
			pnode->p_btopen->set_pos(i);
			pnode->p_btopen->set_type(5);
			pnode->p_btopen->setGeometry(curr_x, t_curr_y, BTN_WIDTH, BTN_HEIGHT);
			pnode->p_btopen->show();

			if(!pnode->p_btclose)
				pnode->p_btclose = new CMyButton("bt_close", ui.m_content_saw);

			pnode->p_btclose->set_jsontool(this);
			pnode->p_btclose->setText(QString::fromLocal8Bit("关"));
			pnode->p_btclose->set_pos(i);
			pnode->p_btclose->set_type(5);
			pnode->p_btclose->setGeometry(curr_x+35, t_curr_y, BTN_WIDTH, BTN_HEIGHT);
			pnode->p_btclose->show();

			curr_x += 75;
			if (!pnode->p_btbottom_2)
				pnode->p_btbottom_2 = new CMyButton("bt2", ui.m_content_saw);

			pnode->p_btbottom_2->set_jsontool(this);
			pnode->p_btbottom_2->setText("");
			pnode->p_btbottom_2->set_pos(i);
			pnode->p_btbottom_2->set_type(5);
			pnode->p_btbottom_2->setGeometry(curr_x, t_curr_y, 65, BTN_HEIGHT);
			pnode->p_btbottom_2->setEnabled(false);
			pnode->p_btbottom_2->show();
			pnode->p_btbottom_2->setStyleSheet("background-color: rgb(211,211,211);border-radius:15px;padding:2px 4px;");

			if (!pnode->p_bthigher)
				pnode->p_bthigher = new CMyButton("bt_higher", ui.m_content_saw);

			pnode->p_bthigher->set_jsontool(this);
			pnode->p_bthigher->setText(QString::fromLocal8Bit("高"));
			pnode->p_bthigher->set_pos(i);
			pnode->p_bthigher->set_type(5);
			pnode->p_bthigher->setGeometry(curr_x, t_curr_y, BTN_WIDTH, BTN_HEIGHT);
			pnode->p_bthigher->show();

			if (!pnode->p_btlower)
				pnode->p_btlower = new CMyButton("bt_lower", ui.m_content_saw);

			pnode->p_btlower->set_jsontool(this);
			pnode->p_btlower->setText(QString::fromLocal8Bit("低"));
			pnode->p_btlower->set_pos(i);
			pnode->p_btlower->set_type(5);
			pnode->p_btlower->setGeometry(curr_x+35, t_curr_y, BTN_WIDTH, BTN_HEIGHT);
			pnode->p_btlower->show();
		
			if (pnode->bopen)
			{
				pnode->p_btopen->setStyleSheet("background-color: rgb(5,150,255);border-radius:15px;padding:2px 4px;color:white");
				pnode->p_btclose->setStyleSheet("background-color: rgb(211,211,211);border-radius:15px;padding:2px 4px;color:white");
				if (pnode->bhigher)
				{
					pnode->p_bthigher->setStyleSheet("background-color: rgb(5,150,255);border-radius:15px;padding:2px 4px;color:white");
					pnode->p_btlower->setStyleSheet("background-color: rgb(211,211,211);border-radius:15px;padding:2px 4px;color:white");
				}
				else
				{
					pnode->p_bthigher->setStyleSheet("background-color: rgb(211,211,211);border-radius:15px;padding:2px 4px;color:white");
					pnode->p_btlower->setStyleSheet("background-color: rgb(5,150,255);border-radius:15px;padding:2px 4px;color:white");
				}
			}
			else
			{
				pnode->p_btopen->setStyleSheet("background-color: rgb(211,211,211);border-radius:15px;padding:2px 4px;color:white");
				pnode->p_btclose->setStyleSheet("background-color: rgb(5,150,255);border-radius:15px;padding:2px 4px;color:white");
				pnode->p_bthigher->setStyleSheet("background-color: rgb(211,211,211);border-radius:15px;padding:2px 4px;color:white");
				pnode->p_btlower->setStyleSheet("background-color: rgb(211,211,211);border-radius:15px;padding:2px 4px;color:white");
			}
			curr_x = init_x + offset_x * ioffset +120;
			curr_y = curr_y + offset_y;
			t_curr_y = curr_y - CTRL_SUB_HEIGHT;

			if (!pnode->pdspin_edit)
				pnode->pdspin_edit = new QDoubleSpinBox(ui.m_content_saw);

			sname = QString::fromLocal8Bit(pnode->value.data());
			pnode->pdspin_edit->setMaximum(1000000);
			pnode->pdspin_edit->setMinimum(0);
			pnode->pdspin_edit->setSingleStep(0.01);
			pnode->pdspin_edit->setValue(sname.toDouble());
			pnode->pdspin_edit->move(curr_x, t_curr_y);
			pnode->pdspin_edit->setGeometry(curr_x, t_curr_y, CTRL_LENGTH1, BTN_HEIGHT);
			pnode->pdspin_edit->show();

			curr_x += CTRL_LENGTH1 + DISTANCE;
			if (!pnode->p_btadd)
				pnode->p_btadd = new CMyButton("(+)", ui.m_content_saw);
			pnode->p_btadd->set_jsontool(this);
			pnode->p_btadd->setText("(+)");
			pnode->p_btadd->set_pos(i);
			pnode->p_btadd->set_type(3);
			pnode->p_btadd->setGeometry(curr_x, t_curr_y, BTN_WIDTH, BTN_HEIGHT);
			pnode->p_btadd->show();

			curr_x += BTN_WIDTH + DISTANCE;
			if (!pnode->p_btsub)
				pnode->p_btsub = new CMyButton("(-)", ui.m_content_saw);
			pnode->p_btsub->set_jsontool(this);
			pnode->p_btsub->setText("(-)");
			pnode->p_btsub->set_pos(i);
			pnode->p_btsub->setGeometry(curr_x, t_curr_y, BTN_WIDTH, BTN_HEIGHT);
			pnode->p_btsub->show();

			curr_x += BTN_WIDTH + DISTANCE;
			if (!pnode->p_btmod)
				pnode->p_btmod = new CMyButton("(m)", ui.m_content_saw);
			pnode->p_btmod->set_jsontool(this);
			pnode->p_btmod->setText("(m)");
			pnode->p_btmod->set_pos(i);
			pnode->p_btmod->setGeometry(curr_x, t_curr_y, BTN_WIDTH, BTN_HEIGHT);
			pnode->p_btmod->show();

			if (m_bshow_btn)
			{
				pnode->p_btadd->setVisible(true);
				pnode->p_btsub->setVisible(true);
				pnode->p_btmod->setVisible(true);
				curr_x += BTN_WIDTH + DISTANCE;
			}
			else
			{
				pnode->p_btadd->setVisible(false);
				pnode->p_btsub->setVisible(false);
				pnode->p_btmod->setVisible(false);
				curr_x -= BTN_WIDTH*2 + DISTANCE * 2;
			}

			if (!pnode->plabel_des)
				pnode->plabel_des = new QLabel(ui.m_content_saw);

			sname = QString::fromLocal8Bit(pnode->s_help.data());
			if (sname != "")
				sname = "(" + sname + ")";
			pnode->plabel_des->setText(sname);
			pnode->plabel_des->move(curr_x, curr_y+ CTRL_SUB_HEIGHT);
			pnode->plabel_des->adjustSize();
			pnode->plabel_des->show();
		}
			break;
		case 16:
		{
			int icount = (itemplate_index) / 3;
			iline_count = (icount+1)*6;
			int t_curr_y = 0;

			QString sname = "";
			
			t_curr_y = curr_y + offset_y+(icount*(offset_y * 5));
			if (icount != 0)
				t_curr_y += 40;

			icount = (itemplate_index) % 3;
			if (0 == icount)
				curr_x = 60;
			else if (1 == icount)
			{
				curr_x += 130;
				curr_x += 20;
			}
			else if (2 == icount)
			{
				curr_x += 130;
				curr_x += 20;
			}			
			
			if (!pnode->p_t_bottom)
				pnode->p_t_bottom = new CMyButton("",ui.m_content_saw);

			pnode->p_t_bottom->move(curr_x, t_curr_y);
			pnode->p_t_bottom->setGeometry(curr_x, t_curr_y, 130, offset_y*5);
			pnode->p_t_bottom->set_pos(i);
			pnode->p_t_bottom->set_jsontool(this);
			pnode->p_t_bottom->show();
			pnode->p_t_bottom->setStyleSheet("background-color: rgb(255,251,240);border-radius:5px;padding:2px 4px;border:1px solid;border-color:rgb(5,150,255)");

			if (!pnode->plabel)
				pnode->plabel = new QLabel(ui.m_content_saw);

			sname = QString::fromLocal8Bit(pnode->s_alias.data());
			if ("" == sname)
				sname = QString::fromLocal8Bit(pnode->key.data());
			pnode->plabel->setText(sname);
			pnode->plabel->move(curr_x + 10, t_curr_y + 10);
			pnode->plabel->adjustSize();
			pnode->plabel->show();
			pnode->plabel->setEnabled(false);

			if (!pnode->plabel1)
				pnode->plabel1 = new QLabel(ui.m_content_saw);
			sname = QString::fromLocal8Bit("模板");
			sname += QString::fromLocal8Bit(pnode->value.data());
			pnode->plabel1->setText(sname);
			pnode->plabel1->move(curr_x+10, t_curr_y+40);
			pnode->plabel1->adjustSize();
			pnode->plabel1->show();
			pnode->plabel1->setEnabled(false);

			if (!pnode->plabel2)
				pnode->plabel2 = new QLabel(ui.m_content_saw);
			pnode->plabel2->setText("");
			QPixmap pix;
			pix.load("./Resources/check.bmp");
			pnode->plabel2->setPixmap(pix);
			pnode->plabel2->move(curr_x+130-49, t_curr_y);
			pnode->plabel2->setGeometry(curr_x+130-49,t_curr_y,49,49);
			pnode->plabel2->show();
			pnode->plabel2->setEnabled(true);

			if (!pnode->p_btpreview)
				pnode->p_btpreview = new CMyButton("preview", ui.m_content_saw);
			pnode->p_btpreview->set_jsontool(this);
			pnode->p_btpreview->setText(QString::fromLocal8Bit("预览"));
			pnode->p_btpreview->set_pos(i);
			pnode->p_btpreview->move(curr_x+50, t_curr_y+100);
			pnode->p_btpreview->setGeometry(curr_x + 40, t_curr_y + 130, 50,30);
			pnode->p_btpreview->setStyleSheet("background-color: rgb(5,150,255);border-radius:5px;padding:2px 4px;");
			pnode->p_btpreview->show();
			pnode->p_btpreview->setEnabled(true);

			if (!pnode->p_btsub)
				pnode->p_btsub = new CMyButton("(-)", ui.m_content_saw);
			pnode->p_btsub->set_jsontool(this);
			pnode->p_btsub->setText("(-)");
			pnode->p_btsub->set_pos(i);
			pnode->p_btsub->setGeometry(curr_x+30, t_curr_y+180, BTN_WIDTH, BTN_HEIGHT);
			pnode->p_btsub->show();

			if (!pnode->p_btmod)
				pnode->p_btmod = new CMyButton("(m)", ui.m_content_saw);
			pnode->p_btmod->set_jsontool(this);
			pnode->p_btmod->setText("(m)");
			pnode->p_btmod->set_pos(i);
			pnode->p_btmod->setGeometry(curr_x + 70, t_curr_y + 180, BTN_WIDTH, BTN_HEIGHT);
			pnode->p_btmod->show();
			if (!pnode->bcheck)
			{
				pnode->plabel2->setVisible(false);
				pnode->plabel2->setEnabled(false);
				pnode->p_t_bottom->setStyleSheet("background-color: rgb(255,251,240);border-radius:5px;padding:2px 4px;border:1px solid;border-color:rgb(211,211,211)");
				pnode->p_btpreview->setEnabled(false);
				pnode->p_btpreview->setStyleSheet("background-color: rgb(211,211,211);border-radius:5px;padding:2px 4px;");
			}

			if (m_bshow_btn)
			{
				pnode->p_btsub->setVisible(true);
				pnode->p_btmod->setVisible(true);
			}
			else
			{
				pnode->p_btsub->setVisible(false);
				pnode->p_btmod->setVisible(false);
			}

			++itemplate_index;
		}
			break;
		case 20:
		{
			if (pnode->parent && 3 == pnode->parent->type)
			{
				++iline_count;
				if (!pnode->p_btadd)
					pnode->p_btadd = new CMyButton("(+)",ui.m_content_saw);

				int ioffset = pnode->blank_count / 2;
				curr_x = init_x + offset_x * ioffset;
				curr_y = curr_y + offset_y;

				pnode->p_btadd->set_jsontool(this);
				pnode->p_btadd->setText("(+)");
				pnode->p_btadd->set_pos(i);
				pnode->p_btadd->set_type(4);
				pnode->p_btadd->setGeometry(curr_x, curr_y, BTN_WIDTH, BTN_HEIGHT);
				pnode->p_btadd->show();

				if (m_bshow_btn)
					pnode->p_btadd->setVisible(true);
				else
					pnode->p_btadd->setVisible(false);
			}
		}
			break;
		default:
			break;
		}
	}//end for (int i = spos; i <= epos ++i)

	//移动单独的那个 + 按钮
	curr_y = (iline_count+1) * offset_y;
	for (auto it : m_navi_node)
	{
		if (it->p_navigation_btn->get_index() == m_icurrbtn_index)
		{
			it->p_btadd->move(10, curr_y);
			if (m_bshow_btn)
				it->p_btadd->setVisible(true);
			else
				it->p_btadd->setVisible(false);
		}	
	}
	
	//根据解析完JSON文件后的行数，计算最大高度的像素值
	int height_pix = (epos-spos) * 25 + 200;
	//int width_pix = 1000;
	int width_pix = 1000+ilevel_count*10;
	if (ilevel_count <= 2)
		width_pix = 1000;
	if (epos - spos <= 2)
		width_pix = 750;
	ui.m_content_saw->setMinimumSize(width_pix, height_pix);
}

void json_tool::show_hide_control(int pos, bool bvisible)
{
	int size = m_qjson.size();
	if (size <= pos)
		return;

	pm_node pnode = nullptr;
	pnode = m_qjson.at(pos);
	if (nullptr == pnode)
		return;

	if (bvisible)
	{
		if (pnode->pcombox)
			pnode->pcombox->setVisible(true);
		if (pnode->pcombox2)
			pnode->pcombox2->setVisible(true);
		if (pnode->pcombox3)
			pnode->pcombox3->setVisible(true);
		if (pnode->pdspin_edit)
			pnode->pdspin_edit->setVisible(true);
		if (pnode->pdspin_edit2)
			pnode->pdspin_edit2->setVisible(true);
		if (pnode->pdspin_edit3)
			pnode->pdspin_edit3->setVisible(true);
		if (pnode->pline_edit)
			pnode->pline_edit->setVisible(true);
		if (pnode->pline_edit2)
			pnode->pline_edit2->setVisible(true);
		if (pnode->pline_edit3)
			pnode->pline_edit3->setVisible(true);
		if (pnode->pspin_edit)
			pnode->pspin_edit->setVisible(true);
		if (pnode->pspin_edit2)
			pnode->pspin_edit2->setVisible(true);
		if (pnode->pspin_edit3)
			pnode->pspin_edit3->setVisible(true);
		if (pnode->plabel)
			pnode->plabel->setVisible(true);
		if (pnode->plabel_des)
			pnode->plabel_des->setVisible(true);
		if (pnode->plabel1)
			pnode->plabel1->setVisible(true);
		if (pnode->plabel2)
			pnode->plabel2->setVisible(true);
		if (pnode->plabel3)
			pnode->plabel3->setVisible(true);
		if (pnode->p_btadd)
			pnode->p_btadd->setVisible(true);
		if (pnode->p_btsub)
			pnode->p_btsub->setVisible(true);
		if (pnode->p_btmod)
			pnode->p_btmod->setVisible(true);
		if (pnode->p_btadd_sub)
			pnode->p_btadd_sub->setVisible(true);
		if(pnode->p_btbottom_1)
			pnode->p_btbottom_1->setVisible(true);
		if (pnode->p_btbottom_2)
			pnode->p_btbottom_2->setVisible(true);
		if (pnode->p_btopen)
			pnode->p_btopen->setVisible(true);
		if (pnode->p_btclose)
			pnode->p_btclose->setVisible(true);
		if (pnode->p_bthigher)
			pnode->p_bthigher->setVisible(true);
		if (pnode->p_btlower)
			pnode->p_btlower->setVisible(true);
		if (pnode->p_btpreview)
			pnode->p_btpreview->setVisible(true);
		if (pnode->p_t_bottom)
			pnode->p_t_bottom->setVisible(true);
	}
	else
	{
		if (pnode->pcombox)
			pnode->pcombox->setVisible(false);
		if (pnode->pcombox2)
			pnode->pcombox2->setVisible(false);
		if (pnode->pcombox3)
			pnode->pcombox3->setVisible(false);
		if (pnode->pdspin_edit)
			pnode->pdspin_edit->setVisible(false);
		if (pnode->pdspin_edit2)
			pnode->pdspin_edit2->setVisible(false);
		if (pnode->pdspin_edit3)
			pnode->pdspin_edit3->setVisible(false);
		if (pnode->pline_edit)
			pnode->pline_edit->setVisible(false);
		if (pnode->pline_edit2)
			pnode->pline_edit2->setVisible(false);
		if (pnode->pline_edit3)
			pnode->pline_edit3->setVisible(false);
		if (pnode->pspin_edit)
			pnode->pspin_edit->setVisible(false);
		if (pnode->pspin_edit2)
			pnode->pspin_edit2->setVisible(false);
		if (pnode->pspin_edit3)
			pnode->pspin_edit3->setVisible(false);
		if (pnode->plabel)
			pnode->plabel->setVisible(false);
		if (pnode->plabel_des)
			pnode->plabel_des->setVisible(false);
		if (pnode->plabel1)
			pnode->plabel1->setVisible(false);
		if (pnode->plabel2)
			pnode->plabel2->setVisible(false);
		if (pnode->plabel3)
			pnode->plabel3->setVisible(false);
		if (pnode->p_btadd)
			pnode->p_btadd->setVisible(false);
		if (pnode->p_btsub)
			pnode->p_btsub->setVisible(false);
		if (pnode->p_btmod)
			pnode->p_btmod->setVisible(false);
		if (pnode->p_btadd_sub)
			pnode->p_btadd_sub->setVisible(false);
		if (pnode->p_btbottom_1)
			pnode->p_btbottom_1->setVisible(false);
		if (pnode->p_btbottom_2)
			pnode->p_btbottom_2->setVisible(false);
		if (pnode->p_btopen)
			pnode->p_btopen->setVisible(false);
		if (pnode->p_btclose)
			pnode->p_btclose->setVisible(false);
		if (pnode->p_bthigher)
			pnode->p_bthigher->setVisible(false);
		if (pnode->p_btlower)
			pnode->p_btlower->setVisible(false);
		if (pnode->p_btpreview)
			pnode->p_btpreview->setVisible(false);
		if (pnode->p_t_bottom)
			pnode->p_t_bottom->setVisible(false);
	}
}

void json_tool::add_commonnode()
{
	QString stype = m_pele_dlg->get_valuetype();
	QString skey = m_pele_dlg->getkey();
	QString svalue = m_pele_dlg->getvalue();
	QString snode_alias = m_pele_dlg->get_nodealias();
	QString snode_help = m_pele_dlg->get_nodehelp();
	QString snode_items = m_pele_dlg->get_nodeitems();

	pm_node pnode = new struct m_node();
	pm_node temp_node = nullptr;

	if ("int" == stype)
		pnode->type = 10;
	else if ("float" == stype)
		pnode->type = 11;
	else if ("string" == stype)
		pnode->type = 12;
	else if ("bool" == stype)
		pnode->type = 13;

	int ipos = m_pele_dlg->getpos();
	int spos = 0, epos = 0;
	if (-1 == ipos)
	{
		get_navibtn_sepos(m_icurrbtn_index,spos,epos);
		temp_node = m_qjson.at(epos - 1);
		ipos = epos;
	}
	else
	{
		temp_node = m_qjson.at(ipos);
		++ipos;     //++ipos,是因为新节点要添加到当前节点的后面
	}

	pnode->key = skey.toLocal8Bit().data();
	pnode->value = svalue.toLocal8Bit().data();
	pnode->s_alias = snode_alias.toLocal8Bit().data();
	pnode->s_help = snode_help.toLocal8Bit().data();
	pnode->s_items = snode_items.toLocal8Bit().data();
	pnode->prenode = temp_node;
	pnode->bshow = true;
	if ("{" == temp_node->key || "[" == temp_node->key)
	{
		pnode->blank_count = temp_node->blank_count+2;
		pnode->parent = temp_node->prenode;
		pnode->has_symbol = temp_node->prenode->has_symbol;
		temp_node->prenode->has_symbol = true;
	}	
	else
	{
		pnode->blank_count = temp_node->blank_count;
		pnode->parent = temp_node->parent;
		pnode->has_symbol = temp_node->has_symbol;
		temp_node->has_symbol = true;
	}

	m_qjson.insert(m_qjson.begin() + ipos, pnode);
	
	//更新插入点后一个节点的prenode的指向
	temp_node = nullptr;
	temp_node = m_qjson.at(ipos+1);

	if (temp_node)
		temp_node->prenode = pnode;
}

void json_tool::mod_commonnode()
{
	int ipos = m_pele_dlg->getpos();
	pm_node pnode = m_qjson.at(ipos);
	if (pnode)
	{
		pnode->s_alias = m_pele_dlg->get_nodealias().toLocal8Bit().data();
		pnode->s_help = m_pele_dlg->get_nodehelp().toLocal8Bit().data();
		pnode->s_items = m_pele_dlg->get_nodeitems().toLocal8Bit().data();
		pnode->key = m_pele_dlg->getkey().toLocal8Bit().data();
	}
}

void json_tool::add_objectnode()
{
	int ipos = m_pele_dlg->getpos();
	int itype = 2;

	QString sele_key = m_pele_dlg->get_elekey();
	QString sele_alias = m_pele_dlg->get_elealias();
	QString skey = m_pele_dlg->getkey();
	QString svalue = m_pele_dlg->getvalue();
	QString stype = m_pele_dlg->get_valuetype();
	QString snode_alias = m_pele_dlg->get_nodealias();
	QString snode_help = m_pele_dlg->get_nodehelp();
	QString snode_items = m_pele_dlg->get_nodeitems();

	int spos = 0, epos = 0;
	pm_node temp_node = nullptr;
	if (-1 == ipos)             //单独的+按钮
	{
		get_navibtn_sepos(m_icurrbtn_index, spos, epos);
		temp_node = m_qjson.at(epos - 1);
		ipos = epos;
	}
	else
	{
		temp_node = m_qjson.at(ipos);
		++ipos;                 //++ipos,是因为新节点要添加到当前节点的后面
	}

	pm_node pnode1 = new struct m_node();
	//第一次添加导航栏节点内的对象时
	if ("{" == temp_node->key/* || "[" == temp_node->key*/)
	{
		pnode1->blank_count = temp_node->blank_count + 2;
		if (temp_node->prenode)
		{
			temp_node->prenode->has_symbol = true;
			pnode1->parent = temp_node->prenode;
		}	
	}	
	else
	{
		pnode1->blank_count = temp_node->blank_count;
		pnode1->parent = temp_node->parent;
	}
	    
	pnode1->key = sele_key.toLocal8Bit().data();
	pnode1->prenode = temp_node;
	pnode1->type = 2;
	pnode1->has_symbol = true;
	pnode1->s_alias = sele_alias.toLocal8Bit().data();
	pnode1->bshow = true;
	m_qjson.insert(m_qjson.begin() + ipos, pnode1);

	pm_node pnode2 = new struct m_node();
	pnode2->blank_count = pnode1->blank_count;
	pnode2->key = "{";
	pnode2->prenode = pnode1;
	pnode2->parent = pnode1->parent;
	pnode2->type = 20;
	pnode2->bshow = true;
	m_qjson.insert(m_qjson.begin() + (ipos+1), pnode2);

	pm_node pnode3 = new struct m_node();
	pnode3->blank_count = pnode2->blank_count + 2;
	pnode3->key = skey.toLocal8Bit().data();
	pnode3->prenode = pnode2;
	pnode3->parent = pnode1;
	if ("int" == stype)
		pnode3->type = 10;
	else if ("float" == stype)
		pnode3->type = 11;
	else if ("string" == stype)
		pnode3->type = 12;
	else if ("bool" == stype)
		pnode3->type = 13;
	pnode3->has_symbol = false;
	pnode3->bshow = true;
	pnode3->value = svalue.toLocal8Bit().data();
	pnode3->s_alias = snode_alias.toLocal8Bit().data();
	pnode3->s_help = snode_help.toLocal8Bit().data();
	pnode3->s_items = snode_items.toLocal8Bit().data();
	m_qjson.insert(m_qjson.begin() + (ipos + 2), pnode3);

	pm_node pnode4 = new struct m_node();
	pnode4->blank_count = pnode2->blank_count;
	pnode4->key = "}";
	pnode4->prenode = pnode3;
	pnode4->parent = pnode2->parent;
	pnode4->type = 20;
	pnode4->bshow = true;
	if ("{" == temp_node->key)  //这种情况，肯定是导航栏节点第一次添加节点，而且类型是object
		pnode4->has_symbol = false;  //temp_node->prenode->has_symbol;
	else
	    pnode4->has_symbol = temp_node->has_symbol;
	temp_node->has_symbol = true;
	m_qjson.insert(m_qjson.begin() + (ipos + 3), pnode4);

	//更新插入点后一个节点的prenode的指向
	temp_node = nullptr;
	temp_node = m_qjson.at(ipos + 4);

	if (temp_node)
		temp_node->prenode = pnode4;
}

void json_tool::mod_objectnode()
{
	int ipos = m_pele_dlg->getpos();
	pm_node pnode = m_qjson.at(ipos);
	if (pnode)
	{
		pnode->key = m_pele_dlg->get_elekey().toLocal8Bit().data();
		pnode->s_alias = m_pele_dlg->get_elealias().toLocal8Bit().data();
	}
}

void json_tool::add_arraynode()
{
	int ipos = m_pele_dlg->getpos();

	QString sele_key = m_pele_dlg->get_elekey();
	QString sele_alias = m_pele_dlg->get_elealias();
	QString skey = m_pele_dlg->getkey();
	QString svalue = m_pele_dlg->getvalue();
	QString stype = m_pele_dlg->get_valuetype();
	QString snode_alias = m_pele_dlg->get_nodealias();
	QString snode_help = m_pele_dlg->get_nodehelp();
	QString snode_items = m_pele_dlg->get_nodeitems();

	int spos = 0, epos = 0;
	pm_node temp_node = nullptr;
	if (-1 == ipos)             //单独的+按钮
	{
		get_navibtn_sepos(m_icurrbtn_index, spos, epos);
		temp_node = m_qjson.at(epos - 1);
		ipos = epos;
	}
	else
	{
		temp_node = m_qjson.at(ipos);
		++ipos;                 //++ipos,是因为新节点要添加到当前节点的后面
	}

	pm_node pnode1 = new struct m_node();
	//第一次添加导航栏节点内的对象时
	if ("{" == temp_node->key/* || "[" == temp_node->key*/)
	{
		pnode1->blank_count = temp_node->blank_count + 2;
		if (temp_node->prenode)
		{
			temp_node->prenode->has_symbol = true;
			pnode1->parent = temp_node->prenode;
		}
	}
	else
	{
		pnode1->blank_count = temp_node->blank_count;
		pnode1->parent = temp_node->parent;
	}

	pnode1->key = sele_key.toLocal8Bit().data();
	pnode1->prenode = temp_node;
	pnode1->type = 3;
	//pnode1->has_symbol = true;
	pnode1->s_alias = sele_alias.toLocal8Bit().data();
	pnode1->bshow = true;
	m_qjson.insert(m_qjson.begin() + ipos, pnode1);

	pm_node pnode2 = new struct m_node();
	pnode2->blank_count = pnode1->blank_count;
	pnode2->key = "[";
	pnode2->prenode = pnode1;
	pnode2->parent = pnode1->parent;
	pnode2->type = 20;
	pnode2->bshow = true;
	m_qjson.insert(m_qjson.begin() + (ipos + 1), pnode2);

	pm_node pnode3 = new struct m_node();
	pnode3->blank_count = pnode2->blank_count+2;
	pnode3->key = "virtual";
	pnode3->prenode = pnode2;
	pnode3->parent = pnode1;
	pnode3->type = 4;
	pnode3->bshow = true; 
	pnode3->has_symbol = true;
	m_qjson.insert(m_qjson.begin() + (ipos + 2), pnode3);

	pm_node pnode4 = new struct m_node();
	pnode4->blank_count = pnode3->blank_count + 2;
	pnode4->key = skey.toLocal8Bit().data();
	pnode4->value = svalue.toLocal8Bit().data();
	pnode4->prenode = pnode3;
	pnode4->parent = pnode3;
	if ("int" == stype)
		pnode4->type = 10;
	else if ("float" == stype)
		pnode4->type = 11;
	else if ("string" == stype)
		pnode4->type = 12;
	else if ("bool" == stype)
		pnode4->type = 13;
	pnode4->has_symbol = false;
	pnode4->bshow = true;
	pnode4->s_alias = snode_alias.toLocal8Bit().data();
	pnode4->s_help = snode_help.toLocal8Bit().data();
	pnode4->s_items = snode_items.toLocal8Bit().data();
	m_qjson.insert(m_qjson.begin() + (ipos + 3), pnode4);

	pm_node pnode5 = new struct m_node();
	pnode5->blank_count = pnode3->blank_count;
	pnode5->key = "}";
	pnode5->prenode = pnode4;
	pnode5->parent = pnode1;
	pnode5->type = 20;
	pnode5->bshow = true;
	pnode5->has_symbol = false;
	m_qjson.insert(m_qjson.begin() + (ipos + 4), pnode5);

	pm_node pnode6 = new struct m_node();
	pnode6->blank_count = pnode2->blank_count;
	pnode6->key = "]";
	pnode6->prenode = pnode5;
	pnode6->parent = pnode1->parent;
	pnode6->type = 20;
	pnode6->bshow = true;

	if ("{" == temp_node->key)  //这种情况，肯定是导航栏节点第一次添加节点
		pnode6->has_symbol = false;  //temp_node->prenode->has_symbol;
	else
	{
		pnode6->has_symbol = temp_node->has_symbol;
		temp_node->has_symbol = true;
	}
	m_qjson.insert(m_qjson.begin() + (ipos + 5), pnode6);

	//更新插入点后一个节点的prenode的指向
	temp_node = nullptr;
	temp_node = m_qjson.at(ipos + 6);

	if (temp_node)
		temp_node->prenode = pnode6;
}

void json_tool::mod_arraynode()
{
	int ipos = m_pele_dlg->getpos();
	pm_node pnode = m_qjson.at(ipos);
	if (pnode)
	{
		pnode->key = m_pele_dlg->get_elekey().toLocal8Bit().data();
		pnode->s_alias = m_pele_dlg->get_elealias().toLocal8Bit().data();
	}
}

void json_tool::add_mctrlnode()
{
	QString skey = m_pele_dlg->get_mctrl_key();
	QString smctl_alias = m_pele_dlg->get_mctrl_keyalias();
	QString smctl_help = m_pele_dlg->get_mctrl_keyhelp();
	QString smctl_num = m_pele_dlg->get_mctrl_num();
	QString smctl1_value = m_pele_dlg->get_mctrl1_value();
	QString smctl2_value = m_pele_dlg->get_mctrl2_value();
	QString smctl3_value = m_pele_dlg->get_mctrl3_value();
	QString smctl1_type = m_pele_dlg->get_mctrl1_type();
	QString smctl2_type = m_pele_dlg->get_mctrl2_type();
	QString smctl3_type = m_pele_dlg->get_mctrl3_type();
	QString smctl1_items = m_pele_dlg->get_mctrl1_items();
	QString smctl2_items = m_pele_dlg->get_mctrl2_items();
	QString smctl3_items = m_pele_dlg->get_mctrl3_items();

	pm_node pnode = new struct m_node();
	pm_node temp_node = nullptr;

	int ipos = m_pele_dlg->getpos();
	int spos = 0, epos = 0;
	if (-1 == ipos)
	{
		get_navibtn_sepos(m_icurrbtn_index, spos, epos);
		temp_node = m_qjson.at(epos - 1);
		ipos = epos;
	}
	else
	{
		temp_node = m_qjson.at(ipos);
		++ipos;     //++ipos,是因为新节点要添加到当前节点的后面
	}

	pnode->type = 14;
	pnode->prenode = temp_node;
	pnode->bshow = true;
	pnode->key = skey.toLocal8Bit().data();
	pnode->s_alias = smctl_alias.toLocal8Bit().data();
	pnode->s_help = smctl_help.toLocal8Bit().data();
	if ("{" == temp_node->key || "[" == temp_node->key)
	{
		pnode->blank_count = temp_node->blank_count + 2;
		pnode->parent = temp_node->prenode;
		pnode->has_symbol = temp_node->prenode->has_symbol;
		temp_node->prenode->has_symbol = true;
	}
	else
	{
		pnode->blank_count = temp_node->blank_count;
		pnode->parent = temp_node->parent;
		pnode->has_symbol = temp_node->has_symbol;
		temp_node->has_symbol = true;
	}

	switch (smctl_num.toInt())
	{
	case 1:
	{
		if ("int" == smctl1_type)
			pnode->type1 = 10;
		else if ("float" == smctl1_type)
			pnode->type1 = 11;
		else if ("string" == smctl1_type)
			pnode->type1 = 12;
		else if ("bool" == smctl1_type)
			pnode->type1 = 13;
		pnode->ctrl1_value = smctl1_value.toLocal8Bit().data();
		pnode->ctrl1_items = smctl1_items.toLocal8Bit().data();
		pnode->ctrlnum = 1;
	}
		break;
	case 2:
	{
		if ("int" == smctl1_type)
			pnode->type1 = 10;
		else if ("float" == smctl1_type)
			pnode->type1 = 11;
		else if ("string" == smctl1_type)
			pnode->type1 = 12;
		else if ("bool" == smctl1_type)
			pnode->type1 = 13;

		if ("int" == smctl2_type)
			pnode->type2 = 10;
		else if ("float" == smctl2_type)
			pnode->type2 = 11;
		else if ("string" == smctl2_type)
			pnode->type2 = 12;
		else if ("bool" == smctl2_type)
			pnode->type2 = 13;

		pnode->ctrl1_value = smctl1_value.toLocal8Bit().data();
		pnode->ctrl2_value = smctl2_value.toLocal8Bit().data();
		pnode->ctrl1_items = smctl1_items.toLocal8Bit().data();
		pnode->ctrl2_items = smctl2_items.toLocal8Bit().data();
		pnode->ctrlnum = 2;
	}          
		break;
	case 3:
	{
		if ("int" == smctl1_type)
			pnode->type1 = 10;
		else if ("float" == smctl1_type)
			pnode->type1 = 11;
		else if ("string" == smctl1_type)
			pnode->type1 = 12;
		else if ("bool" == smctl1_type)
			pnode->type1 = 13;

		if ("int" == smctl2_type)
			pnode->type2 = 10;
		else if ("float" == smctl2_type)
			pnode->type2 = 11;
		else if ("string" == smctl2_type)
			pnode->type2 = 12;
		else if ("bool" == smctl2_type)
			pnode->type2 = 13;

		if ("int" == smctl3_type)
			pnode->type3 = 10;
		else if ("float" == smctl3_type)
			pnode->type3 = 11;
		else if ("string" == smctl3_type)
			pnode->type3 = 12;
		else if ("bool" == smctl3_type)
			pnode->type3 = 13;

		pnode->ctrl1_value = smctl1_value.toLocal8Bit().data();
		pnode->ctrl2_value = smctl2_value.toLocal8Bit().data();
		pnode->ctrl3_value = smctl3_value.toLocal8Bit().data();
		pnode->ctrl1_items = smctl1_items.toLocal8Bit().data();
		pnode->ctrl2_items = smctl2_items.toLocal8Bit().data();
		pnode->ctrl3_items = smctl3_items.toLocal8Bit().data();
		pnode->ctrlnum = 3;
	}
		break;
	default:
		break;
	}

	m_qjson.insert(m_qjson.begin() + ipos, pnode);

	//更新插入点后一个节点的prenode的指向
	temp_node = nullptr;
	temp_node = m_qjson.at(ipos + 1);
	if (temp_node)
		temp_node->prenode = pnode;
}

void json_tool::mod_mctrlnode()
{
	int ipos = m_pele_dlg->getpos();
	pm_node pnode = m_qjson.at(ipos);
	if (pnode)
	{
		pnode->key = m_pele_dlg->get_mctrl_key().toLocal8Bit().data();
		pnode->s_alias = m_pele_dlg->get_mctrl_keyalias().toLocal8Bit().data();
		pnode->s_help = m_pele_dlg->get_mctrl_keyhelp().toLocal8Bit().data();
		pnode->ctrl1_items = m_pele_dlg->get_mctrl1_items().toLocal8Bit().data();
		pnode->ctrl2_items = m_pele_dlg->get_mctrl2_items().toLocal8Bit().data();
		pnode->ctrl3_items = m_pele_dlg->get_mctrl3_items().toLocal8Bit().data();
	}
}

void json_tool::add_smoothnode()
{
	QString skey = m_pele_dlg->getkey();
	QString svalue = m_pele_dlg->getvalue();
	QString snode_alias = m_pele_dlg->get_nodealias();
	QString snode_help = m_pele_dlg->get_nodehelp();
	QString snode_items = m_pele_dlg->get_nodeitems();

	pm_node pnode = new struct m_node();
	pm_node temp_node = nullptr;

	int ipos = m_pele_dlg->getpos();
	int spos = 0, epos = 0;
	if (-1 == ipos)
	{
		get_navibtn_sepos(m_icurrbtn_index, spos, epos);
		temp_node = m_qjson.at(epos - 1);
		ipos = epos;
	}
	else
	{
		temp_node = m_qjson.at(ipos);
		++ipos;     //++ipos,是因为新节点要添加到当前节点的后面
	}

	pnode->type = 15;
	pnode->key = skey.toLocal8Bit().data();
	pnode->value = "0.5";  //svalue.toLocal8Bit().data();
	pnode->s_alias = snode_alias.toLocal8Bit().data();
	pnode->s_help = snode_help.toLocal8Bit().data();
	pnode->s_items = snode_items.toLocal8Bit().data();
	pnode->prenode = temp_node;
	pnode->bopen = true;
	pnode->bhigher = false;
	pnode->bshow = true;

	if ("{" == temp_node->key || "[" == temp_node->key)
	{
		pnode->blank_count = temp_node->blank_count + 2;
		pnode->parent = temp_node->prenode;
		pnode->has_symbol = temp_node->prenode->has_symbol;
		temp_node->prenode->has_symbol = true;
	}
	else
	{
		pnode->blank_count = temp_node->blank_count;
		pnode->parent = temp_node->parent;
		pnode->has_symbol = temp_node->has_symbol;
		temp_node->has_symbol = true;
	}

	m_qjson.insert(m_qjson.begin() + ipos, pnode);

	//更新插入点后一个节点的prenode的指向
	temp_node = nullptr;
	temp_node = m_qjson.at(ipos + 1);

	if (temp_node)
		temp_node->prenode = pnode;
}

void json_tool::mod_smoothnode()
{
	int ipos = m_pele_dlg->getpos();
	pm_node pnode = m_qjson.at(ipos);
	if (pnode)
	{
		pnode->s_alias = m_pele_dlg->get_nodealias().toLocal8Bit().data();
		pnode->s_help = m_pele_dlg->get_nodehelp().toLocal8Bit().data();
		pnode->key = m_pele_dlg->getkey().toLocal8Bit().data();
	}
}

void json_tool::add_templatenode()
{
	QString skey = m_pele_dlg->getkey();
	QString svalue = m_pele_dlg->getvalue();
	QString snode_alias = m_pele_dlg->get_nodealias();
	
	pm_node pnode = new struct m_node();
	pm_node temp_node = nullptr;

	int spos = 0, epos = 0;
	get_navibtn_sepos(m_icurrbtn_index, spos, epos);
	temp_node = m_qjson.at(epos - 1);
	int ipos = epos;

	pnode->type = 16;
	pnode->key = skey.toLocal8Bit().data();
	pnode->value = svalue.toLocal8Bit().data();
	pnode->s_alias = snode_alias.toLocal8Bit().data();
	pnode->prenode = temp_node;
	pnode->bshow = true;
	pnode->bcheck = true;
	
	if ("{" == temp_node->key)
	{
		pnode->blank_count = temp_node->blank_count + 2;
		pnode->parent = temp_node->prenode;
		pnode->has_symbol = false;
		temp_node->prenode->has_symbol = true;
	}
	else
	{
		pnode->blank_count = temp_node->blank_count;
		pnode->parent = temp_node->parent;
		pnode->has_symbol = temp_node->has_symbol;
		temp_node->has_symbol = true;
		//temp_node->bcheck = false;

		int spos = 0, epos = 0;
		get_navibtn_sepos(m_icurrbtn_index, spos, epos);
		for (int i = spos; i <= epos; ++i)
		{
			temp_node = m_qjson.at(i);
			if (temp_node && 16 == temp_node->type)
			{
				if (temp_node != pnode && temp_node->bcheck)
				{
					temp_node->bcheck = false;
					break;
				}
			}
		}
	}

	m_qjson.insert(m_qjson.begin() + ipos, pnode);

	//更新插入点后一个节点的prenode的指向
	temp_node = nullptr;
	temp_node = m_qjson.at(ipos + 1);
	if (temp_node)
		temp_node->prenode = pnode;
}

void json_tool::mod_templatenode()
{
	int ipos = m_pele_dlg->getpos();
	pm_node pnode = m_qjson.at(ipos);
	if (pnode)
	{
		pnode->s_alias = m_pele_dlg->get_nodealias().toLocal8Bit().data();
		pnode->value = m_pele_dlg->getvalue().toLocal8Bit().data();
		pnode->key = m_pele_dlg->getkey().toLocal8Bit().data();
	}
}

void json_tool::closeEvent(QCloseEvent* event)
{
	QMessageBox Msg(QMessageBox::Question, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("确定关闭窗口？"));
	QAbstractButton* pYesBtn = (QAbstractButton*)Msg.addButton(QString::fromLocal8Bit("是"), QMessageBox::YesRole);
	QAbstractButton* pNoBtn = (QAbstractButton*)Msg.addButton(QString::fromLocal8Bit("否"), QMessageBox::NoRole);
	Msg.exec();
	if (Msg.clickedButton() != pYesBtn)
		event->ignore();      //忽略退出信号，程序继续进行  
	else
		event->accept();      //接受退出信号，程序退出

	return;
}
//end private functions