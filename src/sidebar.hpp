#pragma once

#include <QtWidgets/QWidget>
#include <QtWidgets/QPushButton>

class Sidebar : public QWidget {
	Q_OBJECT

public:
	Sidebar(QWidget* parent = nullptr);

private:
	QPushButton* category_btn;
};
