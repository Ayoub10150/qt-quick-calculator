#pragma once
#include <QObject>
#include <QString>

class ContentManager : public QObject {
    Q_OBJECT
    Q_PROPERTY(QString displayText READ getDisplayText WRITE setDisplayText NOTIFY displayTextChanged)

public:
    explicit ContentManager(QObject *parent = nullptr);

    QString getDisplayText() const;
    void setDisplayText(const QString &text);

    void clear();
    void inputDigit(const QString& digit);
    void inputDecimal();
    void inputOperator(const QString& op);
    void backspace();
    void evaluate();

signals:
    void displayTextChanged();

private:
    void updateDisplay(const QString& text);
    bool isOperator(const QChar& value) const;
    QString m_displayText = QStringLiteral("0");
    QString m_currentInput = "";
    bool m_justEvaluated = false;
};