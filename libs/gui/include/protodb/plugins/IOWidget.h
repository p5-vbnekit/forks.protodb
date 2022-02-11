#pragma once

#include <protodb/configurable/Configurable.h>

#include <QWidget>

class IOWidget: public QWidget, public Configurable
{
    Q_OBJECT

public:
    explicit IOWidget(QWidget* parent = nullptr)
        : QWidget(parent) {    }

    virtual ~IOWidget() = default;
};
