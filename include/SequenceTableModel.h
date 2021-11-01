#pragma once

#include <QAbstractTableModel>
#include <QVariant>
#include <QList>

#include "SequenceAbstract.h"

class SequenceTableModel: public QAbstractTableModel
{
    Q_OBJECT

public:
    enum ColumnNames {
        kColumnName        = 0,
        kColumnBindedName  = 1,
        kColumnPeriod      = 2,
        kColumnDescription = 3,
        kColumnDsl         = 4,
        kColumnActiveFlag  = 5,

        kColumnCount
    };

    enum DisplayMode {
        kIncomingDisplayMode,
        kOutgoingDisplayMode,
    };

    enum Role {
        kClickRole = Qt::UserRole,
    };

public:
    SequenceTableModel(QObject* parent = nullptr);

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    bool setData(const QModelIndex &index, const QVariant &aValue, int role = Qt::EditRole) override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;

    DisplayMode displayMode() { return m_mode; }
    void setDisplayMode(DisplayMode mode) { m_mode = mode; }

private:
    QList<Sequence> m_list;
    DisplayMode m_mode;
};



