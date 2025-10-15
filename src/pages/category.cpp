#include "category.hpp"

namespace pages
{
CategoryPage::CategoryPage(std::vector<size_t> const& cats, QWidget* parent) : Page(parent) {
	auto* categories = new QTreeWidget(this);
	categories->setColumnCount(1);
	QList<QTreeWidgetItem*> items;
	for(const auto& category : cats) {
		auto list = QStringList() << QString("item %1").arg(category);
		items.append(new QTreeWidgetItem(static_cast<QTreeWidget*>(nullptr), list));
	}
	categories->insertTopLevelItems(0, items);
}
}


