#include "footedit.h"

FootEdit::FootEdit(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{
	
	ui.setupUi(this);
	ui.statusBar->showMessage("Ready");
	ui.propertyTabWidget->setUsesScrollButtons(true);
	ui.propertyTabWidget->setCurrentWidget(tableView);
	//ui.toolBox
 
    //ui. dockWidget->setWidget(listWidget);

	
}

FootEdit::~FootEdit()
{

}
//
//void SystemLog(const char* log)
//{
//	QString str = "����bate";
//	 str.append(log);  
//  
//       //  ���ַ���׷�ӽ�ȥ��
//	 listWidget->addItem(str);  
//  
//         //��ӽ�List  
//	  listWidget->scrollToItem (listWidget->item(listWidget -> count() - 1));  
//	  //�Զ������������У�
//}
