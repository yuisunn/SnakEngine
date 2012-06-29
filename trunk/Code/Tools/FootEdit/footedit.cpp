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
//	QString str = "测试bate";
//	 str.append(log);  
//  
//       //  把字符串追加进去。
//	 listWidget->addItem(str);  
//  
//         //添加进List  
//	  listWidget->scrollToItem (listWidget->item(listWidget -> count() - 1));  
//	  //自动滚到最新那行，
//}
