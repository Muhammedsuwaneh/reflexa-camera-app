#include "MediaListModel.h"

MediaListModel::MediaListModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

QVariantMap MediaListModel::get(int index) const
{
    QVariantMap map;

    if (index < 0 || index >= m_items.size())
        return map;

    const auto &item = m_items.at(index);

    map["type"] = item.type;
    map["filePath"] = item.filePath;
    map["createdAt"] = item.createdAt;

    return map;
}

int MediaListModel::rowCount(const QModelIndex &) const
{
    return m_items.count();
}

QVariant MediaListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= m_items.size())
        return {};

    const MediaItem &item = m_items.at(index.row());

    switch (role) {
    case FilePathRole: return item.filePath;
    case TypeRole: return item.type;
    case CreatedAtRole: return item.createdAt;
    }

    return {};
}

QHash<int, QByteArray> MediaListModel::roleNames() const
{
    return {
        { FilePathRole, "filePath" },
        { TypeRole, "type" },
        { CreatedAtRole, "createdAt" }
    };
}

void MediaListModel::addMedia(const QString &filePath, const QString &type)
{
    beginInsertRows(QModelIndex(), m_items.size(), m_items.size());
    m_items.push_back({ filePath, type, QDateTime::currentDateTime() });
    endInsertRows();
    this->m_count = this->m_items.count();
    emit countChanged();
}

void MediaListModel::clear()
{
    beginResetModel();
    m_items.clear();
    endResetModel();
    this->m_count = this->m_items.count();
    emit countChanged();
}
