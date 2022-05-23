#include "mainwindow.h"
#include "SequenceTableWidget.h"
#include "LogWidget.h"
#include "PluginManagerDialog.h"
#include "ProtodbSessionManager.h"

#include <protodb/Worker.h>
#include <protodb/SequenceModel.h>
#include <protodb/utils/JsonUtils.h>
#include <protodb/gui/SessionManagerGui.h>

#include <QApplication>
#include <QLabel>
#include <QMenuBar>
#include <QToolBar>
#include <QIcon>
#include <QMessageBox>
#include <QFileDialog>
#include <QStringList>
#include <QInputDialog>
#include <QSettings>

#include <DockManager.h>

MainWindow::MainWindow(Worker* worker, QWidget *parent)
    : QMainWindow(parent)
    , m_worker(worker)
{
  createGui();
  connectSignals();
}


void MainWindow::createGui()
{
    createActions();
    createToolBar();
    createDock();

    setWindowTitle("Protocol Debugger");
    setWindowIcon(QIcon(":/icons/network.svg"));
    setIconSize({18,18});

    auto main_layout = new QHBoxLayout();
        main_layout->addWidget(m_toolbar);
        main_layout->addWidget(m_dock_man);

    auto central_widget = new QWidget();
        central_widget->setLayout(main_layout);

    setCentralWidget(central_widget);

    m_plugin_manager_dialog = new PluginManagerDialog(this);
    m_session_manager_dialog = new SessionManagerGui(this);
        m_session_manager_dialog->setSessionManager(&ProtodbSessionManager::instance());
}

void MainWindow::createDock()
{
    ads::CDockManager::setConfigFlag(ads::CDockManager::DockAreaHideDisabledButtons, true);
    ads::CDockManager::setConfigFlag(ads::CDockManager::AlwaysShowTabs, true);

    auto log_widget = new ads::CDockWidget("Log");
        log_widget->setWidget(new LogWidget());

    auto incoming_table_widget = new ads::CDockWidget("Incoming");
        incoming_table_widget->setWidget(new SequenceTableWidget(m_worker->incomingSequences()));

    auto outgoing_table_widget = new ads::CDockWidget("Outgoing");
        outgoing_table_widget->setWidget(new SequenceTableWidget(m_worker->outgoingSequences()));

    m_dock_man = new ads::CDockManager();
    m_dock_man->addDockWidget(ads::RightDockWidgetArea, log_widget);
    m_dock_man->addDockWidget(ads::RightDockWidgetArea, outgoing_table_widget);
    m_dock_man->addDockWidgetTab(ads::BottomDockWidgetArea, incoming_table_widget);
}


void MainWindow::createActions()
{
    m_new = new QAction(QIcon(":/icons/new_sessions.svg"), tr("&New..."), this);
    m_save = new QAction(QIcon(":/icons/save.svg"), tr("&Save"), this);
    m_sessions = new QAction(QIcon(), tr("&Sessions..."), this);
    m_save_as = new QAction(QIcon(":/icons/save_as.svg"), tr("&Save As..."), this);
    m_open = new QAction(QIcon(":/icons/open.svg"), tr("&Open..."), this);
    m_options = new QAction(QIcon(":/icons/options.svg"), tr("&Options..."), this);
    m_plugins = new QAction(QIcon(":/icons/plugin.svg"), tr("&Plugins..."), this);
    m_import_tables = new QAction(tr("&Import tables"), this);
    m_export_tables = new QAction(tr("&Export tables"), this);
    m_plugins = new QAction(QIcon(":/icons/plugin.svg"), tr("&Plugins..."), this);
    m_about = new QAction(tr("&About"), this);
    m_about_qt = new QAction(tr("&About Qt"), this);
    m_help_content = new QAction(tr("&Help"), this);
    m_exit = new QAction(QIcon(":/icons/close.svg"), tr("&Exit"), this);
}



void MainWindow::createToolBar() {
    m_toolbar = new isa_tool_bar( QBoxLayout::TopToBottom );
    m_toolbar->setButtonSize( QSize( 28, 28 ) );

    m_toolbar->addToolAction(m_new, false);
    m_toolbar->addToolAction(m_open);
    m_toolbar->addToolAction(m_save);
    m_toolbar->addToolAction(m_save_as, false);
    m_toolbar->addMenuSeparator();
    m_toolbar->addToolAction(m_export_tables, false);
    m_toolbar->addToolAction(m_import_tables, false);
    m_toolbar->addMenuSeparator();
    m_toolbar->addToolAction(m_options, false);
    m_toolbar->addToolAction(m_plugins, false);
    m_toolbar->addToolAction(m_sessions, false);
    m_toolbar->addMenuSeparator();
    m_toolbar->addToolAction(m_help_content, false);
    m_toolbar->addToolAction(m_about_qt, false);
    m_toolbar->addToolAction(m_about, false);
    m_toolbar->addMenuSeparator();
    m_toolbar->addToolAction(m_exit, false);
}

void MainWindow::connectSignals()
{
    connect(m_exit, &QAction::triggered, this, &MainWindow::exit);
    connect(m_about_qt, &QAction::triggered, &QApplication::aboutQt);

    connect(m_plugins, &QAction::triggered, this, [this]() {
        m_plugin_manager_dialog->show();
    });

    connect(m_about, &QAction::triggered, this, [this]() {
        QMessageBox box;
        box.setText("Protocol debugger is powerful terminal software.\n"
                    "(c)2021 Evgenii Fedoseev (evgeniy1294@yandex.ru)");
        box.setIcon(QMessageBox::Icon::Information);
        box.exec();
    });

    connect(m_export_tables, &QAction::triggered, this, [this]() {
        QFileDialog fileDialog;
        fileDialog.setAcceptMode(QFileDialog::AcceptSave);
        fileDialog.setFileMode(QFileDialog::AnyFile);
        fileDialog.setViewMode(QFileDialog::List);

        if (fileDialog.exec()) {
            auto fileNames = fileDialog.selectedFiles();

            if (fileNames.size() != 0) {
                auto path = fileNames.back();
                nlohmann::json j;

                nlohmann::json outgoing;
                    m_worker->outgoingSequences()->toJson(outgoing);
                    j["outgoing"] = outgoing;

                nlohmann::json incoming;
                    m_worker->incomingSequences()->toJson(incoming);
                     j["incoming"] = incoming;

                writeToFile(path, j);
            }
        }
    });

    connect(m_import_tables, &QAction::triggered, this, [this]() {
        QFileDialog fileDialog;
        fileDialog.setAcceptMode(QFileDialog::AcceptOpen);
        fileDialog.setFileMode(QFileDialog::AnyFile);
        fileDialog.setViewMode(QFileDialog::List);

        if (fileDialog.exec()) {
            auto fileNames = fileDialog.selectedFiles();

            if (fileNames.size() != 0) {
                nlohmann::json json;
                bool ok = readFromFile(fileNames.back(), json);

                if (ok) {
                    if (json["incoming"].is_array()) {
                        m_worker->incomingSequences()->fromJson(json["incoming"]);
                    }

                    if (json["outgoing"].is_array()) {
                        m_worker->outgoingSequences()->fromJson(json["outgoing"]);
                    }
                }
            }
        }
    });

    connect(m_sessions, &QAction::triggered, this, [this]() {
        m_session_manager_dialog->show();
    });
}

void MainWindow::saveState()
{
    QSettings settings;

    settings.setValue("MainWindow/Geometry", saveGeometry());
    settings.setValue("MainWindow/DockingState", m_dock_man->saveState());
}

void MainWindow::restoreState()
{
    QSettings settings;

    if (settings.contains("MainWindow/Geometry")) {
        auto geometry = settings.value("MainWindow/Geometry").toByteArray();
            restoreGeometry(geometry);
    }

    if (settings.contains("MainWindow/DockingState")) {
        auto state = settings.value("MainWindow/DockingState").toByteArray();
          m_dock_man->restoreState(state);
    }
}

void MainWindow::exit(){
  saveState();
  QApplication::exit(0);

  return;
}
