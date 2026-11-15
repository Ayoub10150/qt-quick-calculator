#include "AppController.h"

AppController::AppController(QObject* parent)
	: QObject(parent), m_contentManager(this)
{
	connect(&m_contentManager, &ContentManager::displayTextChanged,
			this, &AppController::displayTextChanged);
}

QString AppController::displayText() const
{
	return m_contentManager.getDisplayText();
}

void AppController::onButtonClicked(const QString& value)
{
	if (value.isEmpty())
		return;

	switch (value.at(0).unicode()) {
	case 'C':
		m_contentManager.clear();
		break;
	case 0x232B:
		m_contentManager.backspace();
		break;
	case '.':
		m_contentManager.inputDecimal();
		break;
	case '=':
		m_contentManager.evaluate();
		break;
	case '%':
		m_contentManager.inputOperator(QStringLiteral("%"));
		break;
	case '+':
	case '-':
	case u'×':
	case u'÷':
		m_contentManager.inputOperator(value);
		break;
	default:
		if (value.size() == 1 && value.at(0).isDigit())
			m_contentManager.inputDigit(value);
		break;
	}
}
