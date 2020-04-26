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
	string s_alias;                       //节点别名(显示在页面上)
	string s_help;
	string s_items;
	string value;
	string ctrl1_value;
	string ctrl2_value;
	string ctrl3_value;
	int blank_count;                      //退格个数
	bool has_symbol;                      //在此节点值写入文件时，是否添加 逗号
	int type;                             //1-对象(navigation) 2-对象 3-数组 4-数组内的对象 10-整形 11-浮点型 12-字符串 13-布尔 14-多控件节点 15-平滑因子类型节点 16-模板类型节点 20-符号
	int type1;                            //type=14时，变量type1,type2,type3,ctrl1_items,ctrl2_itmes,ctrl3_itmes有效
	int type2;
	int type3;
	int ctrlnum;
	string ctrl1_items;
	string ctrl2_items;
	string ctrl3_items;
	bool bstatus;                         //false-伸张状态 true-收缩状态 (type为2,3时有效)
	bool bshow;                           //当前节点是否需要在界面显示 false-不显示 true-显示

	m_node* parent;
	m_node* prenode;

	CMyButton* p_navigation_btn;          //导航栏按钮，type 为1时，此指针不为空
	CMyButton* p_btadd;
	CMyButton* p_btsub;
	CMyButton* p_btmod;                   //添加，删除，修改按钮 type 为1时，p_btadd不为空，type为10,11,12,13时，p_btadd,p_btsub,p_btmod不为空
	CMyButton* p_btadd_sub;               //object or array前面的+/-按钮(type为2,3时有效)

	QLabel* plabel;
	QLabel* plabel_des;  
	QLabel* plabel1;                      //显示调色板卡片
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

	void add_sub(int pos);                //显示或隐藏当前节点(object,array)所属的元素
	void smooth_open(int pos);
	void smooth_close(int pos);
	void smooth_higher(int pos);
	void smooth_lower(int pos);
	void bt_template(int pos);
	void bt_preview(int pos);

	void del_navibtn(int index);
	void deal_navi_currbtn(int index);    //设置导航栏非当前按钮的颜色

public slots:
	void show_btn();
	void open_json();
	void save_json();
	void quit_form();                     //关闭菜单

	void navidlg_OKClicked();             //添加导航栏窗体 确定按钮
	void navidlg_CancelClicked();         //添加导航栏窗体 取消按钮

	void eledlg_OKClicked();
	void eledlg_CancelClicked();          

	void arraydlg_OKClicked();
	void arraydlg_CancelClicked();

private:
	void release_vjson();                 //释放保存JSON解析结果的vector中的内容
	void write_json(FILE* fp);            //将修改的JSON配置写到文件中
	void get_navibtn_sepos(int index,int &spos,int &epos);      //计算导航栏节点在m_qjson里的开始，结束的索引号
	void create_navibtn();                                      //遍历m_qjson，创建导航栏按钮，并显示
	void change_sepos(int& spos, int& epos);                    //删除object对象时，判断其外层是否也是object对象
	void change_btnpos(int pos, int distance);                  //修改节点中所有按钮的 pos值
	void construct_window(int spos,int epos);
	void show_hide_control(int pos, bool bvisible);             //显示或隐藏m_deque,指定节点对应的控件
	void erase_space(string& s);                                //去掉string 内的所有空格,\r,\n
	void parse_property(string& sjson, pm_node pnode,int type); 
	void parse_json(QString path);

	int get_epos_by_spos(int spos);                             //根据起始点索引，在m_qjson中找到当前object or array 的结束点索引
	QStringList parse_items(string& item,string symbol);

	void add_commonnode();                //添加普通节点
	void mod_commonnode();
	void add_objectnode();                //添加对象节点
	void mod_objectnode();
	void add_arraynode();                 //添加数组节点
	void mod_arraynode();
	void add_mctrlnode();                 //添加多控件节点
	void mod_mctrlnode();
	void add_smoothnode();                //添加平滑因子类型节点
	void mod_smoothnode();
	void add_templatenode();              //添加模板类型节点       
	void mod_templatenode();

	void closeEvent(QCloseEvent* event);  //窗体关闭消息函数

private:
	Ui::json_toolClass ui;

	CMyButton* m_paddbtn;                 //导航栏上的+号按钮
	navi_dlg* m_pnavi_dlg;                //添加导航栏 的窗体指针
	eleDlg* m_pele_dlg;
	arrayDlg* m_parray_dlg;
	template_dlg* m_ptemplate_dlg;

	QMenu* m_pmenu;
	QAction* m_open;
	QAction* m_save;
	QAction* m_showbtn;
	QAction* m_quit;

	string m_json_content;                //保存JSON格式的配置文件内容
	deque<pm_node> m_qjson;               //把JSON格式的配置文件以deque的形式保存
	deque<pm_node> m_navi_node;           //保存导航栏对应的m_node节点指针，此结构不负责释放资源
	stack<pm_node> m_stnode;              //临时保存数组节点

	int m_navibtn_count;                  //导航栏上按钮的个数
	int m_icurrbtn_index;                 //当前按钮索引(最上面的按钮，索引为1)
	int m_iprebtn_index;                  //导航栏按钮被点击时，上一个当前按钮的索引

	bool m_bshow_btn;                     //是否显示+，-，M按钮

	FILE* m_fp;
};
