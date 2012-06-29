#ifndef FOOTEDIT_H
#define FOOTEDIT_H

#include <QtGui/QMainWindow>
#include "ui_footedit.h"

class FootEdit : public QMainWindow
{
	Q_OBJECT

public:
	FootEdit(QWidget *parent = 0, Qt::WFlags flags = 0);
	~FootEdit();
	void SystemLog(const char* log);

private:
	Ui::FootEditClass ui;
};

#endif // FOOTEDIT_H
