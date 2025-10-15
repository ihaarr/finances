#include "sidebar.hpp"

Sidebar::Sidebar(QWidget* parent) : QWidget(parent) {
	category_btn = new QPushButton(tr("Click"), this);
	connect(category_btn, &QPushButton::clicked, [](){qDebug() << "Hello, world!";});
}
