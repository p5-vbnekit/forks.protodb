#pragma once

#include <QWidget>
#include <protodb/configurable/Configurable.h>

class QCheckBox;
class QLineEdit;

class LogFormatWidget: public QWidget, Configurable
{
public:
    explicit LogFormatWidget(QWidget* parent = nullptr);
    virtual ~LogFormatWidget() = default;

    void defaultConfig(nlohmann::json& json) const override;
    void fromJson(const nlohmann::json& json) override;
    void toJson(nlohmann::json& json) const override;

private:
    void createGui();
    void connectSignals();

private:
    QCheckBox* m_append_en;
    QCheckBox* m_timestamp_en;
    QCheckBox* m_ch1_en;
    QCheckBox* m_ch2_en;
    QLineEdit* m_timestamp_format;
    QLineEdit* m_ch1_name;
    QLineEdit* m_ch2_name;
    QLineEdit* m_separator;
};
