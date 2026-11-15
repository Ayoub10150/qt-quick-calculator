#include "ContentManager.h"
#include "CalculatorEngine.h"

ContentManager::ContentManager(QObject* parent) : QObject(parent) {}

QString ContentManager::getDisplayText() const
{
	return m_displayText;
}

void ContentManager::setDisplayText(const QString& text)
{
	updateDisplay(text.isEmpty() ? QStringLiteral("0") : text);
}
	
void ContentManager::updateDisplay(const QString& text)
{
	if (getDisplayText() == text)
		return;
	m_displayText = text;
	emit displayTextChanged();
}

bool ContentManager::isOperator(const QChar& value) const
{
	return value == '+' || value == '-' || value == u'×' || value == u'÷' || value == '%';
}

void ContentManager::clear()
{
	m_currentInput.clear();
	m_justEvaluated = false;
	updateDisplay(QStringLiteral("0"));
}

void ContentManager::inputDigit(const QString& digit)
{
	if (digit.size() != 1 || !digit.at(0).isDigit())
		return;

	if (m_displayText == QStringLiteral("Error") || m_justEvaluated) {
		updateDisplay(QStringLiteral("0"));
		m_currentInput.clear();
		m_justEvaluated = false;
	}

	if (m_displayText == QStringLiteral("0") && m_currentInput.isEmpty())
		m_displayText.clear();

	m_currentInput += digit;
	updateDisplay(m_displayText + digit);
}

void ContentManager::inputDecimal()
{
	if (m_displayText == QStringLiteral("Error") || m_justEvaluated) {
		updateDisplay(QStringLiteral("0"));
		m_currentInput.clear();
		m_justEvaluated = false;
	}

	if (m_currentInput.contains('.'))
		return;

	const QString decimal = m_currentInput.isEmpty() ? QStringLiteral("0.") : QStringLiteral(".");
	if (m_currentInput.isEmpty())
		m_currentInput = QStringLiteral("0.");
	else
		m_currentInput += decimal;

	if (m_displayText == QStringLiteral("0") && m_currentInput == QStringLiteral("0."))
		updateDisplay(m_currentInput);
	else
		updateDisplay(m_displayText + decimal);
}

void ContentManager::inputOperator(const QString& op)
{
	if (op.size() != 1 || !isOperator(op.at(0)))
		return;

	if (m_displayText == QStringLiteral("Error"))
		clear();
	if (m_justEvaluated)
		m_justEvaluated = false;

	if (m_currentInput.isEmpty()) {
		if (getDisplayText() == QStringLiteral("0"))
			updateDisplay(QStringLiteral("0\u200B") + op + QStringLiteral("\u200B"));
		else {
			int operatorIndex = getDisplayText().size() - 1;
			while (operatorIndex >= 0 && getDisplayText().at(operatorIndex) == QChar(0x200B))
				--operatorIndex;
			if (operatorIndex >= 0 && isOperator(getDisplayText().at(operatorIndex))) {
				updateDisplay(getDisplayText().left(operatorIndex));
				updateDisplay(getDisplayText() + QStringLiteral("\u200B") + op + QStringLiteral("\u200B"));
			}
		}
		return;
	}

	updateDisplay(getDisplayText() + QStringLiteral("\u200B") + op + QStringLiteral("\u200B"));
	m_currentInput.clear();
}

void ContentManager::backspace()
{
	if (getDisplayText() == QStringLiteral("Error") || m_justEvaluated) {
		clear();
		return;
	}
	if (getDisplayText() == QStringLiteral("0"))
		return;

	if (!m_currentInput.isEmpty()) {
		m_currentInput.chop(1);
		updateDisplay(getDisplayText().chopped(1));
		if (getDisplayText().isEmpty() || getDisplayText() == QStringLiteral("-"))
			updateDisplay(QStringLiteral("0"));
	}
	else {
		// Use a local string to track text modifications
		QString text = getDisplayText();

		int operatorIndex = text.size() - 1;
		while (operatorIndex >= 0 && text.at(operatorIndex) == QChar(0x200B))
			--operatorIndex;

		if (operatorIndex >= 0 && isOperator(text.at(operatorIndex))) {
			text = text.left(operatorIndex);
			if (text.endsWith(QChar(0x200B)))
				text.chop(1);

			// --- ADDED ITERATIONS HERE ---
			// Iterate backward from the end of the remaining text to find the start of the preceding number
			int i = text.size() - 1;
			while (i >= 0) {
				QChar ch = text.at(i);
				if (isOperator(ch) || ch == QChar(0x200B))
					break;
				--i;
			}
			// Restore m_currentInput to that preceding number
			m_currentInput = text.mid(i + 1);
			if (m_currentInput.isEmpty() || isOperator(m_currentInput.at(0))) {
				m_currentInput.clear();
			}
			// -----------------------------
			updateDisplay(text.isEmpty() ? QStringLiteral("0") : text);
		}
		else {
			// FALLBACK SAFETY: If m_currentInput was empty but the screen doesn't end in an operator,
			// extract the last number segment and delete a digit from it so it never freezes.
			int i = text.size() - 1;
			while (i >= 0) {
				QChar ch = text.at(i);
				if (isOperator(ch) || ch == QChar(0x200B))
					break;
				--i;
			}
			m_currentInput = text.mid(i + 1);
			if (!m_currentInput.isEmpty()) {
				m_currentInput.chop(1);
				text.chop(1);
				if (text.isEmpty() || text == QStringLiteral("-"))
					text = QStringLiteral("0");
				updateDisplay(text);
			}
			else {
				updateDisplay(QStringLiteral("0"));
			}
		}
	}
}


void ContentManager::evaluate()
{
	if (getDisplayText() == QStringLiteral("Error"))
		return;

	QString expression = getDisplayText();
	int lastCharacter = expression.size() - 1;
	while (lastCharacter >= 0 && expression.at(lastCharacter) == QChar(0x200B))
		--lastCharacter;
	if (lastCharacter >= 0 && isOperator(expression.at(lastCharacter)))
		expression += QStringLiteral("0");

	double result = 0.0;
	if (!tryEvaluateExpression(expression, result)) {
		m_currentInput.clear();
		m_justEvaluated = true;
		updateDisplay(QStringLiteral("Error"));
		return;
	}

	// Normalize -0.0 to 0.0 to avoid displaying "-0" on the calculator screen
	if (result == 0.0)
		result = 0.0;
	m_currentInput = QString::number(result, 'g', 15);
	m_justEvaluated = true;
	updateDisplay(m_currentInput);
}
