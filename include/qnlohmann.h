#pragma once

#include <nlohmann/json.hpp>
#include <QtCore>

void to_json(nlohmann::json& j, const QString& str);
void to_json(nlohmann::json& j, const QColor& color);
void to_json(nlohmann::json& j, const QFont& font);

void from_json(const nlohmann::json& j, QString& str);
void from_json(const nlohmann::json& j, QColor& color);
void from_json(const nlohmann::json& j, QFont& font);
