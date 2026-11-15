#pragma once
#include <QString>
#include <QChar>

double evaluateExpression(const QString& expression);
bool tryEvaluateExpression(const QString& expression, double& result);
