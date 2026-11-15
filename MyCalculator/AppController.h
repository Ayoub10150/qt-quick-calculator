#pragma once
#include <QObject>
#include <QString>
#include "ContentManager.h"

class AppController : public QObject {
    Q_OBJECT
    Q_PROPERTY(QString displayText READ displayText NOTIFY displayTextChanged)

public:
    explicit AppController(QObject* parent = nullptr);

    QString displayText() const;
    Q_INVOKABLE void onButtonClicked(const QString& value);

signals:
    void displayTextChanged();

private:
    ContentManager m_contentManager;
};