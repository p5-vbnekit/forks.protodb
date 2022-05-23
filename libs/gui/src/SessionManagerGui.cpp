#include <protodb/gui/SessionManagerGui.h>
#include <protodb/gui/SessionCreateDialog.h>

#include <QTableView>
#include <QTextBrowser>
#include <QPushButton>
#include <QCheckBox>
#include <QGridLayout>
#include <QDialogButtonBox>
#include <QHeaderView>
#include <QLineEdit>
#include <QSortFilterProxyModel>
#include <QMessageBox>

SessionManagerGui::SessionManagerGui(QWidget *parent)
    : QDialog(parent)
{
    create_gui();
    create_connections();
}

void SessionManagerGui::create_gui()
{
    m_proxy_model = new QSortFilterProxyModel();

    m_sessions_table = new QTableView;
        m_sessions_table->setSelectionBehavior(QAbstractItemView::SelectRows);
        m_sessions_table->setShowGrid(false);
        m_sessions_table->setModel( m_proxy_model );

    m_filter_le = new QLineEdit();
        m_filter_le->setPlaceholderText( tr("Find session") );

    m_desc_browser   = new QTextBrowser();
    m_dialog_buttons = new QDialogButtonBox( QDialogButtonBox::Close );
    m_restore_chk = new QCheckBox(tr("Restore last session"));
    m_create_btn = new QPushButton(tr("Create"));
    m_change_btn = new QPushButton(tr("Change"));
    m_copy_btn = new QPushButton(tr("Copy"));
    m_rm_btn = new QPushButton(tr("Delete"));
    m_select_btn = new QPushButton(tr("Select"));

    auto btn_layout = new QVBoxLayout;
        btn_layout->setAlignment(Qt::AlignTop);
        btn_layout->addWidget(m_create_btn);
        btn_layout->addWidget(m_change_btn);
        btn_layout->addWidget(m_copy_btn);
        btn_layout->addWidget(m_rm_btn);
        btn_layout->addWidget(m_select_btn);

    auto stretch_layout = new QVBoxLayout;
        stretch_layout->addWidget(m_filter_le);
            stretch_layout->setStretch(0, 0);
        stretch_layout->addWidget(m_sessions_table);
            stretch_layout->setStretch(1, 4);
        stretch_layout->addWidget(m_desc_browser);
            stretch_layout->setStretch(2, 1);
        stretch_layout->addWidget(m_restore_chk);
            stretch_layout->setStretch(3, 0);


    auto main_layout = new QGridLayout;
        main_layout->addLayout(stretch_layout, 0, 0, Qt::AlignLeft);
        main_layout->addLayout(btn_layout, 0, 1, Qt::AlignTop);
        main_layout->addWidget(m_dialog_buttons, 1, 0, 1, 2);

    setLayout(main_layout);

    resize(640, 480);
    setWindowTitle(tr("Sessions"));
}

void SessionManagerGui::create_connections()
{
    connect(m_dialog_buttons, &QDialogButtonBox::clicked, this, &SessionManagerGui::onDialogClicked);

    connect(m_create_btn, &QPushButton::clicked, this, &SessionManagerGui::onCreateClicked);
    connect(m_change_btn, &QPushButton::clicked, this, &SessionManagerGui::onChangeClicked);
    connect(m_rm_btn, &QPushButton::clicked, this, &SessionManagerGui::onRmClicked);

    connect(m_filter_le, &QLineEdit::textChanged, m_proxy_model, &QSortFilterProxyModel::setFilterFixedString);
}


void SessionManagerGui::onDialogClicked(QAbstractButton* aBtn)
{
    switch( m_dialog_buttons->standardButton( aBtn ) )
    {
        case QDialogButtonBox::Close:
            hide();
            break;

        default:
            break;
    }
}

void SessionManagerGui::onCreateClicked()
{
    SessionCreateDialog dialog(m_sm);
        dialog.exec();

    return;
}

void SessionManagerGui::onChangeClicked()
{
    SessionCreateDialog dialog(m_sm);
        dialog.setChangeMode();
        dialog.exec();

        return;
}

void SessionManagerGui::onRmClicked()
{
    auto selected = m_sessions_table->selectionModel()->selectedRows();
    if (selected.empty())
        return;

    QString name = selected.first().data().toString();

    if ( name.isEmpty() )
        return;

    QMessageBox msgbox;
    {
        msgbox.setText( tr("Delete %1 session?").arg(name) );
        msgbox.setInformativeText( tr("Selected session will be removed") );
        msgbox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        msgbox.setDefaultButton(QMessageBox::No);
        msgbox.setIcon(QMessageBox::Icon::Warning);
        msgbox.setFixedSize( QSize(480, 240) );

            msgbox.setButtonText(QMessageBox::Yes, QObject::tr("Yes") );
            msgbox.setButtonText(QMessageBox::No, QObject::tr("No") );
    }

    if (msgbox.exec() == QMessageBox::Yes) {
        if ( !m_sm->removeSession(name) ) {
            msgbox.setStandardButtons(QMessageBox::Yes);
            msgbox.setText( tr("Error") );
            msgbox.setInformativeText( tr("Can't remove selected session") );

            msgbox.exec();
        }
    }
}

void SessionManagerGui::setSessionManager(SessionManager *sm)
{
    if (m_sm != sm) {
        m_sm = sm;

        m_proxy_model->setSourceModel(m_sm);
        m_sessions_table->hideColumn(SessionManager::kColumnDescription);

        QHeaderView* hh = m_sessions_table->horizontalHeader();
            hh->setSectionResizeMode(SessionManager::kColumnName, QHeaderView::Stretch);
            hh->setSectionResizeMode(SessionManager::kColumnLastChanged, QHeaderView::ResizeToContents);

        QHeaderView* vh = m_sessions_table->verticalHeader();
            vh->hide();
    }
}

SessionManager *SessionManagerGui::sessionManager() const
{
    return m_sm;
}


