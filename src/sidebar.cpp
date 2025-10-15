#include <QVBoxLayout>
#include "sidebar.hpp"

Sidebar::Sidebar(QWidget* parent) : QWidget(parent) {
	auto* layout = new QVBoxLayout(this);
	auto* category_btn = new QPushButton(tr("Категории"));
	connect(category_btn, &QPushButton::clicked, [this](){emit page_changed(pages::Type::Category);});
	auto* operation_btn = new QPushButton(tr("Операции"));
	connect(operation_btn, &QPushButton::clicked, [this](){emit page_changed(pages::Type::Operation);});
	auto* analytic_btn = new QPushButton(tr("Аналитика"));
	connect(analytic_btn, &QPushButton::clicked, [this](){emit page_changed(pages::Type::Analytic);});
	layout->addWidget(category_btn);
	layout->addWidget(operation_btn);
	layout->addWidget(analytic_btn);
	layout->addStretch();
	setMaximumWidth(150);
}
