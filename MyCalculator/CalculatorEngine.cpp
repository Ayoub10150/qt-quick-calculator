#include "CalculatorEngine.h"
#include <cmath>
#include <stdexcept>

namespace {
constexpr int MaximumExpressionLength = 256;
constexpr double MaximumAbsoluteValue = 1.0e100;

class ExpressionParser
{
public:
    explicit ExpressionParser(const QString& expression) : m_expression(expression) {}

    double parse()
    {
        const double result = parseExpression();
        skipSpaces();
        if (m_position != m_expression.size())
            throw std::runtime_error("Invalid expression");
        return checked(result);
    }

private:
    double parseExpression()
    {
        double value = parseTerm();
        while (true) {
            skipSpaces();
            if (match('+'))
                value = checked(value + parseTerm());
            else if (match('-'))
                value = checked(value - parseTerm());
            else
                return value;
        }
    }

    double parseTerm()
    {
        double value = parseNumber();
        while (true) {
            skipSpaces();
            if (match(u'×'))
                value = checked(value * parseNumber());
            else if (match(u'÷')) {
                const double divisor = parseNumber();
                if (divisor == 0.0)
                    throw std::runtime_error("Division by zero");
                value = checked(value / divisor);
            }
            else if (match('%')) {
                const double percentage = parseNumber();
                value = checked(value * percentage / 100.0);
            }
            else
                return value;
        }
    }

    double parseNumber()
    {
        skipSpaces();

        // Handle unary minus sign for negative numbers (e.g., -6)
        bool isNegative = match('-');
        skipSpaces();

        const int start = m_position;
        bool hasDigit = false;
        bool hasDecimal = false;
        while (m_position < m_expression.size()) {
            const QChar character = m_expression.at(m_position);
            if (character.isDigit()) {
                hasDigit = true;
                ++m_position;
            }
            else if (character == '.' && !hasDecimal) {
                hasDecimal = true;
                ++m_position;
            }
            else {
                break;
            }
        }

        if (!hasDigit)
            throw std::runtime_error("Number expected");

        bool ok = false;
        const double value = m_expression.mid(start, m_position - start).toDouble(&ok);
        if (!ok)
            throw std::runtime_error("Invalid number");

        return checked(isNegative ? -value : value);
    }

    bool match(QChar expected)
    {
        if (m_position < m_expression.size() && m_expression.at(m_position) == expected) {
            ++m_position;
            return true;
        }
        return false;
    }

    void skipSpaces()
    {
        while (m_position < m_expression.size() &&
               (m_expression.at(m_position).isSpace() || m_expression.at(m_position) == QChar(0x200B)))
            ++m_position;
    }

    static double checked(double value)
    {
        if (!std::isfinite(value) || std::abs(value) > MaximumAbsoluteValue)
            throw std::runtime_error("Result is too large");
        return value;
    }

    const QString& m_expression;
    int m_position = 0;
};
}


// Main function to evaluate the expression string using QString
double evaluateExpression(const QString& expression) {
    if (expression.trimmed().isEmpty() || expression.size() > MaximumExpressionLength)
        throw std::runtime_error("Invalid expression");

    return ExpressionParser(expression).parse();
}

bool tryEvaluateExpression(const QString& expression, double& result)
{
    try {
        result = evaluateExpression(expression);
        return true;
    } catch (const std::exception&) {
        result = 0.0;
        return false;
    }
}