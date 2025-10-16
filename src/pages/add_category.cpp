#include <QComboBox>
#include <QTextEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>

#include "add_category.hpp"
#include "category.hpp"
#include "../models/category.hpp"

namespace pages
{
AddCategoryWindow::AddCategoryWindow(QVector<models::Category> const& categories, CategoryPage* parent) : QWidget(nullptr) {
	auto* layout = new QVBoxLayout(this);
	layout->addWidget(new QLabel("Название категории"));
	
	edit = new QTextEdit(this);
	layout->addWidget(edit);

	categoriesCombo = new QComboBox(this);
	categoriesCombo->addItem(QString("Без родителя"), QVariant::fromValue(0));
	for(const auto& category : categories) {
		categoriesCombo->addItem(category.name, QVariant::fromValue(category.id));
	}
	layout->addWidget(categoriesCombo);

	auto* createBtn = new QPushButton("Создать");
	connect(createBtn, &QPushButton::clicked, this, &AddCategoryWindow::handle_create_clicked);
	layout->addWidget(createBtn);

	connect(this, &AddCategoryWindow::create_category, parent, &CategoryPage::handle_create_category);
}

void AddCategoryWindow::handle_create_clicked() {
	size_t id = categoriesCombo->currentData().toInt();
	QString name = edit->toPlainText();
	emit create_category(id, name);
}
}
