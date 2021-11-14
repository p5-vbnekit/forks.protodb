#pragma once

#include <QStyledItemDelegate>

class CellCheckBoxDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    enum Role {
        kClickRole = Qt::UserRole,
    };

public:
    CellCheckBoxDelegate(QObject *parent = 0);
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index) override;

    void setText(const QString& text) { m_text = text; }

signals:
    void triggered(const QModelIndex &index);

private:
    int   m_active_row;
    QString m_text;
};
