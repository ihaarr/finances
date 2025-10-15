#pragma once

#include <QWidget>

class Page : public QWidget {
	Q_OBJECT
public:
	Page(QWidget* parent = nullptr) : QWidget(parent) {}
};
