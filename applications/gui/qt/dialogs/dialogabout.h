#ifndef DIALOGABOUT_H
#define DIALOGABOUT_H

#include <QDialog>
#include <QString>

namespace Ui {
	class DialogAbout;
}

class DialogAbout : public QDialog {
	Q_OBJECT

public:
	explicit DialogAbout(QWidget *parent = 0);
	~DialogAbout();
public:
	void setLabelVersion(QString version);
private:
	Ui::DialogAbout *ui;
};

#endif // DIALOGABOUT_H
