#pragma once

#include <QWidget>

namespace pages
{
class Page : public QWidget {
	Q_OBJECT
public:
	Page(QWidget* parent = nullptr) : QWidget(parent) {}
};
}

