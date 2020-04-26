#include "arrayDlg.h"
#include "json_tool.h"

arrayDlg::arrayDlg(json_tool* jt, QWidget *parent): QWidget(parent)
{
	ui.setupUi(this);

	connect(ui.m_ok, SIGNAL(clicked()), jt, SLOT(arraydlg_OKClicked()));                    //确定按钮
	connect(ui.m_cancel, SIGNAL(clicked()), jt, SLOT(arraydlg_CancelClicked()));            //取消按钮

	assert(jt);
	m_jt = jt;

	m_ipos = 0;
}

arrayDlg::~arrayDlg()
{
	//
}

void arrayDlg::add_array_objectnode(int icount, deque<m_node*>& m_qjson)
{
	deque<pm_node>::iterator iter = m_array_qjson.begin();
	while (iter != m_array_qjson.end())
		iter = m_array_qjson.erase(iter);

	int bpos = m_ipos - icount+1;
	int epos = m_ipos;
	int itype = 0;
	int init_x = 10;
	int curr_y = 0;
	int offset_y = 25;

	pm_node pnode = nullptr;
	//pm_node parent_node = nullptr;
	for (int i = bpos; i <= epos; ++i)
	{
		pnode = m_qjson.at(i);
		pm_node temp_pnode = new struct m_node();

		temp_pnode->blank_count = pnode->blank_count;
		temp_pnode->has_symbol = pnode->has_symbol;
		temp_pnode->key = pnode->key;
		temp_pnode->type = pnode->type;
		temp_pnode->s_alias = pnode->s_alias;
		temp_pnode->s_help = pnode->s_help;
		temp_pnode->s_items = pnode->s_items;
		temp_pnode->bshow = true;
		if (i != bpos)
			temp_pnode->prenode = m_array_qjson.at(m_array_qjson.size()-1);

		if (i == bpos || i == epos)
			temp_pnode->parent = pnode->parent;
		else if ("{" == temp_pnode->key || "[" == temp_pnode->key)
			//只有第一个节点没有给prenode赋值，而 { [ 不可能是第一个元素，所以 temp_pnode->prenode肯定不为空 
			temp_pnode->parent = temp_pnode->prenode->parent;
		else if ("}" == temp_pnode->key || "]" == temp_pnode->key)
		{
			//temp_pnode->parent = temp_pnode->prenode->parent->parent;
			temp_pnode->parent = m_parent_node.at(m_parent_node.size() - 2);
		}
		else
			temp_pnode->parent = m_parent_node.at(m_parent_node.size() - 1);

		if ("virtual" == temp_pnode->key || 2 == temp_pnode->type || 3 == temp_pnode->type)
			m_parent_node.push_back(temp_pnode);

		itype = pnode->type;
		switch (itype)
		{
		case 10:
		case 11:
		case 12:
		case 13:
		{
			QLabel* plabel = new QLabel(ui.m_arraydlgWidget);
			QString sname = QString::fromLocal8Bit(pnode->s_alias.data());
			//sname = "\"" + sname + "\"" + ":";
			plabel->setText(sname);

			int ioffset = pnode->blank_count / 2;
			int t_curr_x = init_x + pnode->blank_count * 5;
			curr_y += offset_y;

			plabel->move(t_curr_x, curr_y);
			plabel->adjustSize();
			plabel->show();
			m_dlabel.push_back(plabel);

			t_curr_x += 80;
			int t_curr_y = curr_y - 5;
		    if (10 == itype)
		    {
			     QSpinBox* pinbox = new QSpinBox(ui.m_arraydlgWidget);
			     sname = QString::fromLocal8Bit(pnode->value.data());
			     pinbox->setMaximum(65537);
			     pinbox->setMinimum(0);
			     pinbox->setValue(sname.toInt());
			     pinbox->move(t_curr_x, t_curr_y);
			     pinbox->setGeometry(t_curr_x, t_curr_y, 350, 20);
			     pinbox->show();
			     temp_pnode->pspin_edit = pinbox;
		    }
			else if (11 == itype)
			{
				QDoubleSpinBox* dpinbox = new QDoubleSpinBox(ui.m_arraydlgWidget);
				sname = QString::fromLocal8Bit(pnode->value.data());
				dpinbox->setMaximum(1000000);
				dpinbox->setMinimum(0);
				dpinbox->setSingleStep(0.1);
				dpinbox->setValue(sname.toDouble());
				dpinbox->move(t_curr_x, t_curr_y);
				dpinbox->setGeometry(t_curr_x, t_curr_y, 350, 20);
				dpinbox->show();
				temp_pnode->pdspin_edit = dpinbox;
			}
		    else if (12 == itype)
			{
				if (pnode->s_items != "")
				{
					QComboBox* pcombox = new QComboBox(ui.m_arraydlgWidget);
					QString qitem = QString::fromLocal8Bit(pnode->s_items.data());
					QStringList ql = qitem.split(";");

					for (int i = 0; i < ql.count(); ++i)
						pcombox->addItem(ql.at(i));

					sname = QString::fromLocal8Bit(pnode->value.data());
					int index = pcombox->findText(sname);
					pcombox->setCurrentIndex(index);

					pcombox->move(t_curr_x, t_curr_y);
					pcombox->setGeometry(t_curr_x, t_curr_y, 350, 20);
					pcombox->show();

					temp_pnode->pcombox = pcombox;
				}
				else
				{
					QLineEdit* pedit = new QLineEdit(ui.m_arraydlgWidget);
					sname = QString::fromLocal8Bit(pnode->value.data());
					pedit->setText(sname);
					pedit->move(t_curr_x, t_curr_y);
					pedit->setGeometry(t_curr_x, t_curr_y, 350, 20);
					pedit->show();
					temp_pnode->pline_edit = pedit;
				}
			}
			
			else if (13 == itype)
			{
				QComboBox* pcombox = new QComboBox(ui.m_arraydlgWidget);
				QString qitem = QString::fromLocal8Bit(pnode->s_items.data());
				QStringList ql = qitem.split(";");
				for (int i = 0; i < ql.count(); ++i)
					pcombox->addItem(ql.at(i));

				sname = QString::fromLocal8Bit(pnode->value.data());
				int index = pcombox->findText(sname);
				pcombox->setCurrentIndex(index);

				pcombox->move(t_curr_x, t_curr_y);
				pcombox->setGeometry(t_curr_x, t_curr_y, 350, 20);
				pcombox->show();
				temp_pnode->pcombox = pcombox;
			}
		}
			break;
		case 2:
		case 3:
		{
			QLabel* plabel = new QLabel(ui.m_arraydlgWidget);
			QString sname = "";
			sname = QString::fromLocal8Bit(pnode->s_alias.data());

			plabel->setText(sname);
			int t_curr_x = init_x + pnode->blank_count * 5;
			curr_y += offset_y;
			plabel->move(t_curr_x, curr_y);
			plabel->adjustSize();
			plabel->show();
			m_dlabel.push_back(plabel);
		}
			break;
		case 20:
		{
			if (3 == pnode->parent->type)
				curr_y += offset_y;
			/*QLabel* plabel = new QLabel(ui.m_arraydlgWidget);
			QString sname = QString::fromStdString(pnode->key);
			plabel->setText(sname);
			int ioffset = pnode->blank_count / 2;
			int t_curr_x = init_x + pnode->blank_count * 5;
			curr_y += offset_y;

			plabel->move(t_curr_x, curr_y);
			plabel->adjustSize();
			plabel->show();
			m_dlabel.push_back(plabel);*/
		}
			break;
		default:
			break;
		}//end switch (pnode->type)
		
		m_array_qjson.push_back(temp_pnode);
	}

	//根据解析完JSON文件后的行数，计算最大高度的像素值
	int height_pix = m_array_qjson.size() * 22.5 + 200;
	ui.m_arraydlgWidget->setMinimumSize(1000, height_pix);

	show();
}

void arrayDlg::release_label()
{
	deque<QLabel*>::iterator iter;
	QLabel* plabel = nullptr;
	for(iter = m_dlabel.begin(); iter != m_dlabel.end();)
	{
		plabel = *iter;
		if (plabel)
		{
			delete plabel;
			plabel = nullptr;
		}

		iter = m_dlabel.erase(iter);
	}
}

void arrayDlg::update_ctltojson()
{
	pm_node pnode;
	int icount = m_array_qjson.size();
	for (int i = 0; i < icount; ++i)
	{
		pnode = m_array_qjson.at(i);
		switch (pnode->type)
		{
		case 10:
		{
			if (pnode->pspin_edit)
			{
				pnode->value = pnode->pspin_edit->text().toLocal8Bit().data();
				delete pnode->pspin_edit;
				pnode->pspin_edit = nullptr;
			}
		}
			break;
		case 11:
		{
			if (pnode->pdspin_edit)
			{
				pnode->value = pnode->pdspin_edit->text().toLocal8Bit().data();
				delete pnode->pdspin_edit;
				pnode->pdspin_edit = nullptr;
			}
		}
			break;
		case 12:
		{
			if (pnode->s_items != "")
			{
				if (pnode->pcombox)
				{
					pnode->value = pnode->pcombox->currentText().toLocal8Bit().data();
					delete pnode->pcombox;
					pnode->pcombox = nullptr;
				}	
			}
			else
			{
				if (pnode->pline_edit)
				{
					pnode->value = pnode->pline_edit->text().toLocal8Bit().data();
					delete pnode->pline_edit;
					pnode->pline_edit = nullptr;
				}	
			}	
		}
			break;
		case 13:
		{
			if (pnode->pcombox)
			{
				pnode->value = pnode->pcombox->currentText().toLocal8Bit().data();
				delete pnode->pcombox;
				pnode->pcombox = nullptr;
			}	
		}
			break;
		default:
			break;
		}
	}
}

void arrayDlg::insert_nodetojson(deque<m_node*>& m_qjson)
{
	pm_node pnode = nullptr;
	pnode = m_qjson.at(m_ipos);
	pnode->has_symbol = true;

	pnode = m_array_qjson.at(0);
	pnode->prenode = m_qjson.at(m_ipos);

	int icount = m_array_qjson.size();
	for (int i = 0; i < icount; ++i)
	{
		pnode = m_array_qjson.at(i);
		m_qjson.insert(m_qjson.begin() + (m_ipos + 1 + i),pnode);
	}
	pm_node pnode_last = m_qjson.at(m_ipos + icount + 1);
	pm_node pndoe_last_pre = m_qjson.at(m_ipos + icount);
	pnode_last->prenode = pndoe_last_pre;
}

void arrayDlg::keyPressEvent(QKeyEvent* event)
{
	if (event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return)
		m_jt->arraydlg_OKClicked();
}