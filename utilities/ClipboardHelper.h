#pragma once
#include <QObject>
#include <QGuiApplication>
#include <QClipboard>

class ClipboardHelper : public QObject
{
    Q_OBJECT
public:
    explicit ClipboardHelper(QObject *parent = nullptr);

    Q_INVOKABLE void copyText(const QString &text);
};
