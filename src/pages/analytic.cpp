#include <QLabel>
#include "analytic.hpp"

namespace pages
{
AnalyticPage::AnalyticPage(QWidget* parent) : QWidget(parent) {
	new QLabel(QString("Analytic"), this);
}
}

