#pragma once

class QString;

namespace models
{
struct Subcategory {
	size_t id;
	QString name;
};

struct Category {
	size_t id;
	QString name;
	QVector<Subcategory> subcategories;
};
}
