#include "ClipboardHelper.h"

ClipboardHelper::ClipboardHelper(QObject *parent) : QObject(parent) {}

void ClipboardHelper::copyText(const QString &text)
{
    QClipboard *clipboard = QGuiApplication::clipboard();
    if (clipboard)
        clipboard->setText(text);
}
