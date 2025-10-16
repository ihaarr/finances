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

namespace db
{
struct Category {
	size_t id;
	QString name;
	std::optional<size_t> parent_id;
};
}

}

Q_DECLARE_METATYPE(models::db::Category)
