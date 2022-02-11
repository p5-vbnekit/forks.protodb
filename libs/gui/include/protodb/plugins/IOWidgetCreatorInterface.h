#pragma once

#include <protodb/factories/CreatorAbstract.h>

#include <QtPlugin>
#include <nlohmann/json.hpp>

class IOWidget;

class IOWidgetCreatorInterface: public CreatorAbstract
{
public:
    virtual ~IOWidgetCreatorInterface() override = default;

    virtual IOWidget* create() const = 0;
    virtual IOWidget* create(const nlohmann::json&) const = 0;
};

Q_DECLARE_INTERFACE(IOWidgetCreatorInterface, "ProtoDb.IOWidgetCreatorInterface")
