#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_json_tool.h"
#include "CMyButton.h"
#include "navi_dlg.h"
#include "eleDlg.h"
#include "arrayDlg.h"
#include "myComboBox.h"
#include "Config.h"
#include "template_dlg.h"

#include <QSpinBox>
#include <QCloseEvent>

#include <deque>
#include <stack>
using namespace std;


typedef struct m_node
{
	string key;
	string s_alias;                       //�ڵ����(��ʾ��ҳ����)
	string s_help;
	string s_items;
	string value;
	string ctrl1_value;
	string ctrl2_value;
	string ctrl3_value;
	int blank_count;                      //�˸����
	bool has_symbol;                      //�ڴ˽ڵ�ֵд���ļ�ʱ���Ƿ���� ����
	int type;                             //1-����(navigation) 2-���� 3-���� 4-�����ڵĶ��� 10-���� 11-������ 12-�ַ��� 13-���� 14-��ؼ��ڵ� 15-ƽ���������ͽڵ� 16-ģ�����ͽڵ� 20-����
	int type1;                            //type=14ʱ������type1,type2,type3,ctrl1_items,ctrl2_itmes,ctrl3_itmes��Ч
	int type2;
	int type3;
	int ctrlnum;
	string ctrl1_items;
	string ctrl2_items;
	string ctrl3_items;
	bool bstatus;                         //false-����״̬ true-����״̬ (typeΪ2,3ʱ��Ч)
	bool bshow;                           //��ǰ�ڵ��Ƿ���Ҫ�ڽ�����ʾ false-����ʾ true-��ʾ

	m_node* parent;
	m_node* prenode;

	CMyButton* p_navigation_btn;          //��������ť��type Ϊ1ʱ����ָ�벻Ϊ��
	CMyButton* p_btadd;
	CMyButton* p_btsub;
	CMyButton* p_btmod;                   //��ӣ�ɾ�����޸İ�ť type Ϊ1ʱ��p_btadd��Ϊ�գ�typeΪ10,11,12,13ʱ��p_btadd,p_btsub,p_btmod��Ϊ��
	CMyButton* p_btadd_sub;               //object or arrayǰ���+/-��ť(typeΪ2,3ʱ��Ч)

	QLabel* plabel;
	QLabel* plabel_des;  
	QLabel* plabel1;                      //��ʾ��ɫ�忨Ƭ
	QLabel* plabel2;
	QLabel* plabel3;
	QComboBox* pcombox;
	myComboBox* pcombox1;
	myComboBox* pcombox2;
	myComboBox* pcombox3;
	QLineEdit* pline_edit;
	QLineEdit* pline_edit2;
	QLineEdit* pline_edit3;
	QSpinBox* pspin_edit;
	QSpinBox* pspin_edit2;
	QSpinBox* pspin_edit3;
	QDoubleSpinBox* pdspin_edit;
	QDoubleSpinBox* pdspin_edit2;
	QDoubleSpinBox* pdspin_edit3;

	CMyButton* p_btbottom_1;
	CMyButton* p_btbottom_2;
	CMyButton* p_btopen;
	CMyButton* p_btclose;
	CMyButton* p_bthigher;
	CMyButton* p_btlower;
	bool bopen;
	bool bhigher;

	CMyButton* p_btpreview;
	CMyButton* p_t_bottom;
	bool bcheck;

	~m_node()
	{
		//if (p_navigation_btn)
		//{
		//	delete p_navigation_btn;
		//	p_navigation_btn = nullptr;
		//}
		if (p_btadd)
		{
			delete p_btadd;
			p_btadd = nullptr;
		}
		if (p_btsub)
		{
			delete p_btsub;
			p_btsub = nullptr;
		}
		if (p_btmod)
		{
			delete p_btmod;
			p_btmod = nullptr;
		}
		if (p_btadd_sub)
		{
			delete p_btadd_sub;
			p_btadd_sub = nullptr;
		}
		if (plabel)
		{
			delete plabel;
			plabel = nullptr;
		}
		if (plabel_des)
		{
			delete plabel_des;
			plabel_des = nullptr;
		}
		if (plabel1)
		{
			delete plabel1;
			plabel1 = nullptr;
		}
		if (plabel2)
		{
			delete plabel2;
			plabel2 = nullptr;
		}
		if (plabel3)
		{
			delete plabel3;
			plabel3 = nullptr;
		}
		if (pcombox)
		{
			delete pcombox;
			pcombox = nullptr;
		}
		if (pcombox1)
		{
			delete pcombox1;
			pcombox1 = nullptr;
		}
		if (pcombox2)
		{
			delete pcombox2;
			pcombox2 = nullptr;
		}
		if (pcombox3)
		{
			delete pcombox3;
			pcombox3 = nullptr;
		}
		if (pline_edit)
		{
			delete pline_edit;
			pline_edit = nullptr;
		}
		if (pline_edit2)
		{
			delete pline_edit2;
			pline_edit2 = nullptr;
		}
		if (pline_edit3)
		{
			delete pline_edit3;
			pline_edit3 = nullptr;
		}
		if (pspin_edit)
		{
			delete pspin_edit;
			pspin_edit = nullptr;
		}
		if (pspin_edit2)
		{
			delete pspin_edit2;
			pspin_edit2 = nullptr;
		}
		if (pspin_edit3)
		{
			delete pspin_edit3;
			pspin_edit3 = nullptr;
		}
		if (pdspin_edit)
		{
			delete pdspin_edit;
			pdspin_edit = nullptr;
		}
		if (pdspin_edit2)
		{
			delete pdspin_edit2;
			pdspin_edit2 = nullptr;
		}
		if (pdspin_edit3)
		{
			delete pdspin_edit3;
			pdspin_edit3 = nullptr;
		}
		if (p_btbottom_1)
		{
			delete p_btbottom_1;
			p_btbottom_1 = nullptr;
		}
		if (p_btbottom_2)
		{
			delete p_btbottom_2;
			p_btbottom_2 = nullptr;
		}
		if (p_btopen)
		{
			delete p_btopen;
			p_btopen = nullptr;
		}
		if (p_btclose)
		{
			delete p_btclose;
			p_btclose = nullptr;
		}
		if (p_bthigher)
		{
			delete p_bthigher;
			p_bthigher = nullptr;
		}
		if (p_btlower)
		{
			delete p_btlower;
			p_btlower = nullptr;
		}
		if (p_t_bottom)
		{
			delete p_t_bottom;
			p_t_bottom = nullptr;
		}
		if (p_btpreview)
		{
			delete p_btpreview;
			p_btpreview = nullptr;
		}
	}
}*pm_node;

class json_tool : public QMainWindow
{
	Q_OBJECT

public:
	json_tool(QWidget *parent = Q_NULLPTR);
	~json_tool();

public:
	void add_ele(int type,int pos);
	void sub_ele(int pos);
	void mod_ele(int pos);

	void add_sub(int pos);                //��ʾ�����ص�ǰ�ڵ�(object,array)������Ԫ��
	void smooth_open(int pos);
	void smooth_close(int pos);
	void smooth_higher(int pos);
	void smooth_lower(int pos);
	void bt_template(int pos);
	void bt_preview(int pos);

	void del_navibtn(int index);
	void deal_navi_currbtn(int index);    //���õ������ǵ�ǰ��ť����ɫ

public slots:
	void show_btn();
	void open_json();
	void save_json();
	void quit_form();                     //�رղ˵�

	void navidlg_OKClicked();             //��ӵ��������� ȷ����ť
	void navidlg_CancelClicked();         //��ӵ��������� ȡ����ť

	void eledlg_OKClicked();
	void eledlg_CancelClicked();          

	void arraydlg_OKClicked();
	void arraydlg_CancelClicked();

private:
	void release_vjson();                 //�ͷű���JSON���������vector�е�����
	void write_json(FILE* fp);            //���޸ĵ�JSON����д���ļ���
	void get_navibtn_sepos(int index,int &spos,int &epos);      //���㵼�����ڵ���m_qjson��Ŀ�ʼ��������������
	void create_navibtn();                                      //����m_qjson��������������ť������ʾ
	void change_sepos(int& spos, int& epos);                    //ɾ��object����ʱ���ж�������Ƿ�Ҳ��object����
	void change_btnpos(int pos, int distance);                  //�޸Ľڵ������а�ť�� posֵ
	void construct_window(int spos,int epos);
	void show_hide_control(int pos, bool bvisible);             //��ʾ������m_deque,ָ���ڵ��Ӧ�Ŀؼ�
	void erase_space(string& s);                                //ȥ��string �ڵ����пո�,\r,\n
	void parse_property(string& sjson, pm_node pnode,int type); 
	void parse_json(QString path);

	int get_epos_by_spos(int spos);                             //������ʼ����������m_qjson���ҵ���ǰobject or array �Ľ���������
	QStringList parse_items(string& item,string symbol);

	void add_commonnode();                //�����ͨ�ڵ�
	void mod_commonnode();
	void add_objectnode();                //��Ӷ���ڵ�
	void mod_objectnode();
	void add_arraynode();                 //�������ڵ�
	void mod_arraynode();
	void add_mctrlnode();                 //��Ӷ�ؼ��ڵ�
	void mod_mctrlnode();
	void add_smoothnode();                //���ƽ���������ͽڵ�
	void mod_smoothnode();
	void add_templatenode();              //���ģ�����ͽڵ�       
	void mod_templatenode();

	void closeEvent(QCloseEvent* event);  //����ر���Ϣ����

private:
	Ui::json_toolClass ui;

	CMyButton* m_paddbtn;                 //�������ϵ�+�Ű�ť
	navi_dlg* m_pnavi_dlg;                //��ӵ����� �Ĵ���ָ��
	eleDlg* m_pele_dlg;
	arrayDlg* m_parray_dlg;
	template_dlg* m_ptemplate_dlg;

	QMenu* m_pmenu;
	QAction* m_open;
	QAction* m_save;
	QAction* m_showbtn;
	QAction* m_quit;

	string m_json_content;                //����JSON��ʽ�������ļ�����
	deque<pm_node> m_qjson;               //��JSON��ʽ�������ļ���deque����ʽ����
	deque<pm_node> m_navi_node;           //���浼������Ӧ��m_node�ڵ�ָ�룬�˽ṹ�������ͷ���Դ
	stack<pm_node> m_stnode;              //��ʱ��������ڵ�

	int m_navibtn_count;                  //�������ϰ�ť�ĸ���
	int m_icurrbtn_index;                 //��ǰ��ť����(������İ�ť������Ϊ1)
	int m_iprebtn_index;                  //��������ť�����ʱ����һ����ǰ��ť������

	bool m_bshow_btn;                     //�Ƿ���ʾ+��-��M��ť

	FILE* m_fp;
};
