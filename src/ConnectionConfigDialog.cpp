#include <QFrame>
#include <QFileDialog>
#include <QDialogButtonBox>
#include <QButtonGroup>
#include <QRadioButton>
#include <QPushButton>
#include <QCheckBox>
#include <QLineEdit>
#include <QLayout>
#include <QLabel>
#include <QAction>

#include "ConnectionConfigDialog.h"
#include "SerialConfigWidget.h"
#include "log_config_widget.h"

ConnectionConfigDialog::ConnectionConfigDialog(QWidget* aParent)
    : QDialog(aParent)
{
    createGui();
    connectSignals();
}

void ConnectionConfigDialog::createGui()
{
    // --------[BUTTONS]--------- //
    m_dialog_buttons = new QDialogButtonBox( QDialogButtonBox::Ok | QDialogButtonBox::Apply | QDialogButtonBox::Cancel );

    auto serial_radio  = new QRadioButton(tr("Serial"));
        serial_radio->setChecked(true);
    auto network_radio = new QRadioButton(tr("Network"));
    auto bridge_radio  = new QRadioButton(tr("Bridge"));

    m_mode_selector = new QButtonGroup();
        m_mode_selector->addButton(serial_radio, 0);
        m_mode_selector->addButton(network_radio, 0);
        m_mode_selector->addButton(bridge_radio, 0);

    // --------[LOG FILE]--------- //
    m_file_button = new QPushButton();
        m_file_button->setIcon(QIcon(":/icons/file_search.svg"));
        m_file_button->setToolTip(tr("Show File Dialog"));

    m_logfile = new QLineEdit();
        m_logfile->setPlaceholderText(tr("Path to log file"));

    // ------[CONFIG WIDGETS] ------ //
    auto serial_config_widget = new SerialConfigWidget();
        m_widgets.append(serial_config_widget);

    // ---------[SELECTOR FRAME]---------//
    auto selector_frame = new QFrame();
        selector_frame->setFrameShape(QFrame::StyledPanel);
        selector_frame->setFrameShadow(QFrame::Raised);

    auto selector_layout = new QVBoxLayout();
        selector_layout->setAlignment(Qt::AlignTop);
        selector_layout->addWidget(new QLabel(tr("Select mode:")));
        selector_layout->addWidget(serial_radio);
        selector_layout->addWidget(network_radio);
        selector_layout->addWidget(bridge_radio);

    selector_frame->setLayout(selector_layout);

    // ---------[LOG CONFIGS FRAME]-------- //
    auto append_file_checkbox = new QCheckBox(tr("Append file"));
        append_file_checkbox->setChecked(true);

    auto enable_timestamp = new QCheckBox(tr("Enable timestamp"));
        enable_timestamp->setChecked(true);

    auto enable_channel_1 = new QCheckBox(tr("Enable channel 1"));
         enable_channel_1->setChecked(true);

    auto enable_channel_2 = new QCheckBox(tr("Enable channel 2"));
         enable_channel_2->setChecked(true);

    auto enable_user_comment = new QCheckBox(tr("Enable comments"));
         enable_user_comment->setChecked(true);

    auto date_time_format = new QLineEdit();
        date_time_format->setPlaceholderText("yyyy.mm.dd hh:mm:ss.sss");

    auto channel_1_name = new QLineEdit();
         channel_1_name->setText(">>>");

    auto channel_2_name = new QLineEdit();
         channel_2_name->setText("<<<");

    auto logcfg_frame = new QFrame();
        logcfg_frame->setFrameShape(QFrame::StyledPanel);
        logcfg_frame->setFrameShadow(QFrame::Raised);

    auto logcfg_layout = new QGridLayout();
        logcfg_layout->setAlignment(Qt::AlignTop | Qt::AlignLeft);
        logcfg_layout->addWidget(new QLabel(tr("Log configurations:")), 0, 0, 1, 1);
        logcfg_layout->addWidget(append_file_checkbox, 1, 0, 1, 1);
        logcfg_layout->addWidget(enable_timestamp, 2, 0, 1, 1);
        logcfg_layout->addWidget(enable_channel_1, 1, 1, 1, 1);
        logcfg_layout->addWidget(enable_channel_2, 2, 1, 1, 1);
        logcfg_layout->addWidget(enable_user_comment, 1, 3, 1, 1);
        logcfg_layout->addWidget(new QLabel(tr("Timestamp format")), 3, 0, 1, 1);
        logcfg_layout->addWidget(date_time_format, 3, 1, 1, 1);
        logcfg_layout->addWidget(new QLabel(tr("Channel 1 label")), 4, 0, 1, 1);
        logcfg_layout->addWidget(channel_1_name, 4, 1, 1, 1);
        logcfg_layout->addWidget(new QLabel(tr("Channel 2 label")), 5, 0, 1, 1);
        logcfg_layout->addWidget(channel_2_name, 5, 1, 1, 1);
        logcfg_layout->setColumnStretch(0, 0);
        logcfg_layout->setColumnStretch(1, 0);
        logcfg_layout->setColumnStretch(2, 0);
        logcfg_layout->setColumnStretch(3, 0);
        logcfg_layout->setColumnStretch(4, 1);

    logcfg_frame->setLayout(logcfg_layout);

    // ---------[LAYOUT]---------- //

    auto main_layout = new QGridLayout();
        main_layout->addWidget(selector_frame, 0 , 0, 3, 1);
        main_layout->addWidget(m_file_button, 0 , 2, 1, 1);
        main_layout->addWidget(m_logfile, 0, 1, 1, 1);
        main_layout->addWidget(serial_config_widget, 1 ,1, 1, 2);
        main_layout->addWidget(logcfg_frame, 2 ,1, 1, 2);
        main_layout->addWidget(m_dialog_buttons, 3, 0, 1, 3);
        main_layout->setColumnStretch(0, 0);
        main_layout->setColumnStretch(1, 1);
        main_layout->setColumnStretch(2, 0);


        setLayout(main_layout);
}

void ConnectionConfigDialog::connectSignals()
{
    connect(m_file_button, &QPushButton::released, this, [this]() {
        QFileDialog fileDialog;
        fileDialog.setAcceptMode(QFileDialog::AcceptOpen);
        fileDialog.setFileMode(QFileDialog::AnyFile);
        fileDialog.setViewMode(QFileDialog::List);

        QStringList fileNames;
        if (fileDialog.exec()) {
          fileNames = fileDialog.selectedFiles();

          if (fileNames.size() != 0) {
                m_logfile->setText(fileNames.back());
          }
        }

    });
}

