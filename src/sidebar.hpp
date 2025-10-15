#pragma once

#include <QtWidgets/QWidget>
#include <QtWidgets/QPushButton>
#include "pages/type.hpp"

class Sidebar : public QWidget {
	Q_OBJECT

public:
	Sidebar(QWidget* parent = nullptr);

signals:
	void page_changed(pages::Type page);
};
