#include <QComboBox>
#include <QLineEdit>
#include <QLabel>
#include <QLayout>
#include <QSerialPortInfo>
#include <QIntValidator>

#include "qnlohmann.h"
#include "SerialConfigFrame.h"


SerialConfigFrame::SerialConfigFrame(QWidget* parent)
    : ConfigFrame(parent)
{
    createGui();
    connectSignals();
}


void SerialConfigFrame::defaultConfig(nlohmann::json &json) const
{
    json["Device"]      = "";
    json["OpenMode"]    = "Communication";
    json["Baudrate"]    = "115200";
    json["DataBits"]    = "8";
    json["Parity"]      = "None";
    json["StopBits"]    = "1";
    json["FlowControl"] = "None";
}

void SerialConfigFrame::toJson(nlohmann::json &json) const
{
    json["Device"]      = m_device->currentText();
    json["OpenMode"]    = m_open_mode->currentText();
    json["Baudrate"]    = m_baudrate->currentText();
    json["DataBits"]    = m_data_bits->currentText();
    json["Parity"]      = m_parity->currentText();
    json["StopBits"]    = m_stop_bits->currentText();
    json["FlowControl"] = m_flow_ctrl->currentText();
}

void SerialConfigFrame::fromJson(const nlohmann::json &json)
{
    QString baudrate = json["Baudrate"].get<QString>();
    if (m_baudrate->findText(baudrate) == -1) {
        m_baudrate->setItemText(m_baudrate->count() - 1,baudrate);
    }
    m_baudrate->setCurrentText(baudrate);

    m_device->setCurrentText(json["Device"].get<QString>());
    m_open_mode->setCurrentText(json["OpenMode"].get<QString>());
    m_data_bits->setCurrentText(json["DataBits"].get<QString>());
    m_parity->setCurrentText(json["Parity"].get<QString>());
    m_stop_bits->setCurrentText(json["StopBits"].get<QString>());
    m_flow_ctrl->setCurrentText(json["FlowControl"].get<QString>());
}

void SerialConfigFrame::createGui()
{
    static const QStringList baudrate_list   = { "1200", "4800", "9600", "19200", "38400", "57600", "115200", tr("Custom") };
    static const QStringList databits_list   = { "5", "6", "7", "8" };
    static const QStringList flow_ctrl_list  = { "None", "Software", "Hardware" };
    static const QStringList parity_list     = { "None", "Even", "Odd", "Space", "Mark" };
    static const QStringList stop_bits_list  = { "1", "2" };
    static const QStringList open_mode_list  = { "Communication", "Monitoring" };

    // --------[COMBO BOXES] --------- //
    m_baudrate_validator = new QIntValidator(0, 10000000, this);
    m_baudrate  = new QComboBox();
        m_baudrate->addItems(baudrate_list);
        m_baudrate->setInsertPolicy(QComboBox::InsertAtCurrent);

    m_data_bits = new QComboBox();
        m_data_bits->addItems(databits_list);
    m_flow_ctrl = new QComboBox();
        m_flow_ctrl->addItems(flow_ctrl_list);
    m_parity    = new QComboBox();
        m_parity->addItems(parity_list);
    m_stop_bits = new QComboBox();
        m_stop_bits->addItems(stop_bits_list);
    m_open_mode = new QComboBox();
        m_open_mode->addItems(open_mode_list);
    m_device    = new QComboBox();

    for (auto& port: QSerialPortInfo::availablePorts()) {
        m_device->addItem(port.portName());
    }

    // --------[LABELS]-------- //
    auto label_baudrate  = new QLabel(tr("Baudrate"));
    auto label_data_bits = new QLabel(tr("Data Bits"));
    auto label_flow_ctrl = new QLabel(tr("Flow Control"));
    auto label_parity    = new QLabel(tr("Parity"));
    auto label_stop_bits = new QLabel(tr("Stop Bits"));
    auto label_open_mode = new QLabel(tr("Open Mode"));
    auto label_device    = new QLabel(tr("Device"));


    auto cfg_layout = new QGridLayout();
        cfg_layout->setAlignment(Qt::AlignTop);
        cfg_layout->addWidget(label_device, 0, 0);
        cfg_layout->addWidget(m_device, 0, 1);
        cfg_layout->addWidget(label_baudrate, 0, 2);
        cfg_layout->addWidget(m_baudrate, 0, 3);
        cfg_layout->addWidget(label_open_mode, 1, 0);
        cfg_layout->addWidget(m_open_mode, 1, 1);
        cfg_layout->addWidget(label_data_bits, 1, 2);
        cfg_layout->addWidget(m_data_bits, 1, 3);
        cfg_layout->addWidget(label_flow_ctrl, 2, 0);
        cfg_layout->addWidget(m_flow_ctrl, 2, 1);
        cfg_layout->addWidget(label_stop_bits, 2, 2);
        cfg_layout->addWidget(m_stop_bits, 2, 3);
        cfg_layout->addWidget(label_parity, 3, 0);
        cfg_layout->addWidget(m_parity, 3, 1);

        cfg_layout->setColumnStretch(0, 0);
        cfg_layout->setColumnStretch(1, 0);
        cfg_layout->setColumnStretch(2, 0);
        cfg_layout->setColumnStretch(3, 0);
        cfg_layout->setColumnStretch(4, 1);

    auto main_layout = new QVBoxLayout();
        main_layout->setAlignment(Qt::AlignTop);
        main_layout->addWidget(new QLabel(tr("Serial configurations:")));
        main_layout->addLayout(cfg_layout);

    setLayout(main_layout);
    setFrameShape(QFrame::StyledPanel);
    setFrameShadow(QFrame::Raised);
}

void SerialConfigFrame::connectSignals()
{
    connect(m_baudrate, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [this](int index){
        if (index == m_baudrate->count() - 1) {
            m_baudrate->setEditable(true);
            m_baudrate->setValidator(m_baudrate_validator);
            m_baudrate->lineEdit()->selectAll();
        }
        else {
            m_baudrate->setEditable(false);
        }
    });


}