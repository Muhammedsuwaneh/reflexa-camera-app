#pragma once

#include <QAbstractListModel>
#include <QDateTime>

struct MediaItem
{
    QString filePath;
    QString type;
    QDateTime createdAt;
};

class MediaListModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int count READ rowCount NOTIFY countChanged)
public:
    enum Roles {
        FilePathRole = Qt::UserRole + 1,
        TypeRole,
        CreatedAtRole
    };

    explicit MediaListModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;

    Q_INVOKABLE void addMedia(const QString &filePath, const QString &type);
    Q_INVOKABLE void clear();
    Q_INVOKABLE QVariantMap get(int index) const;

signals:
    void countChanged();

private:
    QList<MediaItem> m_items;
    int m_count;
};
