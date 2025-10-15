#include <QLabel>
#include "analytic.hpp"

namespace pages
{
AnalyticPage::AnalyticPage(QWidget* parent) : Page(parent) {
	new QLabel(QString("Analytic"), this);
}
}

