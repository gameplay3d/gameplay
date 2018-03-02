#include "ProjectWizard.h"
#include "ProjectWizardItemDelegate.h"
#include "Project.h"
#include "EditorWindow.h"
#include "ui_ProjectWizard.h"
#include <QtWidgets>

#define PROJECT_SETTINGS_ORGANIZATION "gameplay"
#define PROJECT_SETTINGS_APPLICATION "GamePlay"
#define PROJECT_SETTINGS_RECENTS_PROECTS "recentProjects"
#define PROJECT_SETTINGS_LAST_PROJECT_PATH "lastProjectPath"
#define PROJECT_NEW "NewProject"
#define PROJECT_FILE "game.project"


ProjectWizard::ProjectWizard(QWidget* parent) : QDialog(parent), 
    _ui(new Ui::ProjectWizard),
    _editor(NULL)
{
    _ui->setupUi(this);    
}

ProjectWizard::~ProjectWizard()
{
    delete _ui;
}

bool ProjectWizard::initialize()
{
    qApp->setStyle(QStyleFactory::create("Fusion"));
    QPalette darkPalette;
    darkPalette.setColor(QPalette::Window, QColor(37, 41, 45));
    darkPalette.setColor(QPalette::WindowText, Qt::white);
    darkPalette.setColor(QPalette::Base, QColor(32, 36, 40));
    darkPalette.setColor(QPalette::AlternateBase, QColor(35, 35, 35));
    darkPalette.setColor(QPalette::ToolTipBase, Qt::white);
    darkPalette.setColor(QPalette::ToolTipText, Qt::white);
    darkPalette.setColor(QPalette::Text, Qt::white);
    darkPalette.setColor(QPalette::Button, QColor(53, 53, 53));
    darkPalette.setColor(QPalette::ButtonText, Qt::white);
    darkPalette.setColor(QPalette::BrightText, Qt::red);
    darkPalette.setColor(QPalette::Link, QColor(42, 130, 218));
    darkPalette.setColor(QPalette::Highlight, QColor(61, 142, 201));
    darkPalette.setColor(QPalette::HighlightedText, Qt::black);
    qApp->setPalette(darkPalette);
    qApp->setStyleSheet("QToolTip { color: #ffffff; background-color: #25292d; border: 1px solid #3d8ec9; } "
                        "QDockWidget { titlebar-close-icon: url(:/res/images/close.png); } "
                        "QTabBar { qproperty-drawBase: 0; } "
                        "QTabBar::close-button { image: url(:/res/images/close.png); background: transparent; subcontrol-origin: padding; padding-left: 10px; } "
                        "QToolBar { border: 0px } "
                        "QToolBar::separator { color: white; background-color: #25292d; width: 15; height: 30px; } "
                        "QPushButton::pressed { background-color: rgb(61, 142, 201); } "
                        "QPushButton::checked { background-color: rgb(61, 142, 201); }");

    _ui->listWidgetProjects->setItemDelegate(&_itemDelegate);

    connect(_ui->pushButtonNew, SIGNAL(pressed()), this, SLOT(onNewProjectPressed()));
    connect(_ui->pushButtonOpen, SIGNAL(pressed()), this, SLOT(onOpenProjectPressed()));
    connect(_ui->pushButtonCreate, SIGNAL(pressed()), this, SLOT(onCreateProjectPressed()));
    connect(_ui->pushButtonCancel, SIGNAL(pressed()), this, SLOT(onCancelProjectPressed()));
    connect(_ui->lineEditProjectName, SIGNAL(textChanged(QString)), this, SLOT(onProjectNameTextChanged()));
    connect(_ui->pushButtonProjectDirectory, SIGNAL(pressed()), this, SLOT(onProjectDirectoryPressed()));
    connect(_ui->lineEditProjectDirectory, SIGNAL(textChanged(QString)), this, SLOT(onProjectDirectoryTextChanged()));

    readProjects();

    return true;
}

void ProjectWizard::setEditor(EditorWindow* editor)
{
    _editor = editor;
}

void ProjectWizard::readProjects()
{
    _ui->frameCreateProject->hide();
    _ui->pushButtonNew->show();
    _ui->pushButtonOpen->show();

    // Restore previously created projects
    QSettings settings(PROJECT_SETTINGS_ORGANIZATION, PROJECT_SETTINGS_APPLICATION);
    _recentProjects = settings.value(PROJECT_SETTINGS_RECENTS_PROECTS).toStringList();
    if (_recentProjects.size() > 0)
    {
        _ui->listWidgetProjects->clear();
        for (size_t i = 0; i < _recentProjects.size(); ++i)
        {
            const QString path = _recentProjects.at(i);
            std::shared_ptr<Project> project = Project::open(path);
            if (project)
            {
                QListWidgetItem* item = new QListWidgetItem();
                item->setData(Qt::DisplayRole, project->getName());
                item->setData(Qt::UserRole, path);
                _ui->listWidgetProjects->addItem(item);
            }
            else
            {
                _recentProjects.removeAt(i);
                i--;
            }
        }
    }
    settings.setValue(QString(QLatin1String(PROJECT_SETTINGS_RECENTS_PROECTS)), _recentProjects);

    // Updte the ui frame components
    if (_recentProjects.size() == 0)
    {
        _ui->frameCreateProjectTip->show();
        _ui->listWidgetProjects->hide();
    }
    else
    {
        _ui->pushButtonOpen->setEnabled(true);
        _ui->frameCreateProjectTip->hide();
        _ui->listWidgetProjects->show();
    }
}

void ProjectWizard::onNewProjectPressed()
{
    _ui->listWidgetProjects->hide();
    _ui->pushButtonNew->hide();
    _ui->pushButtonOpen->hide();
    _ui->frameCreateProjectTip->hide();
    _ui->frameCreateProject->show();
    _ui->lineEditProjectName->setText(tr(PROJECT_NEW));
    _ui->lineEditProjectName->setSelection(0, _ui->lineEditProjectName->text().length());
    _ui->lineEditProjectName->setFocus();
    _ui->lineEditProjectDirectory->setText(QDir::homePath() + QString("/") + _ui->lineEditProjectName->text());
    _projectDirectoryRoot = QDir::homePath();
}

void ProjectWizard::onOpenProjectPressed()
{
    QList<QListWidgetItem*> selected = _ui->listWidgetProjects->selectedItems();
    if (selected.size() == 0)
    {
        QString projectFilePath = QFileDialog::getOpenFileName(this, tr("Open Project"), ".", tr("Project Files (*.project)"));
        if (!projectFilePath.isEmpty())
        {
            QDir projectPath = QFileInfo(projectFilePath).absoluteDir();
            _editor->show();
            emit projectOpened(projectPath.path());
            this->hide();
        }
        return;
    }

    QListWidgetItem* item = selected.at(0);
    if (!item)
        return;

    QVariant data = item->data(Qt::UserRole);
    QString path = data.toString();
    if (!QDir(path).exists())
    {
        QMessageBox::warning(this,
                             tr("Open Project"),
                             tr("Project directory no longer exists. Removing project from recents."),
                             QMessageBox::Ok);
        _ui->listWidgetProjects->removeItemWidget(item);
    }
    _editor->show();
    emit projectOpened(path);
    this->hide();
}

void ProjectWizard::onCancelProjectPressed()
{
    readProjects();
}

void ProjectWizard::onCreateProjectPressed()
{
    if (QDir(_ui->lineEditProjectDirectory->text()).exists())
    {
        QMessageBox::warning(this,
                             tr("Create Project"),
                             tr("Directory already exists.\nDelete the directory or pick a different folder name."),
                             QMessageBox::Ok);
        return;
    }

    if (QDir().mkdir(_ui->lineEditProjectDirectory->text()))
    {
        std::shared_ptr<Project> project = Project::create(_ui->lineEditProjectDirectory->text(),
                                                           _ui->lineEditProjectName->text());
        if (project)
        {
            // Save our project to recent system project
            _recentProjects.push_back(_ui->lineEditProjectDirectory->text());
            QSettings settings(PROJECT_SETTINGS_ORGANIZATION, PROJECT_SETTINGS_APPLICATION);
            settings.setValue(QString(QLatin1String(PROJECT_SETTINGS_RECENTS_PROECTS)), _recentProjects);

            // Show the editor and hide the wizard
            _editor->show();            
            emit projectOpened(_ui->lineEditProjectDirectory->text());
            this->hide();
            _ui->listWidgetProjects->show();
            _ui->pushButtonNew->show();
            _ui->pushButtonOpen->show();
        }
        else
        {
            QMessageBox::warning(this,
                                 tr("Create Project"),
                                 tr("Unable to create project contents."),
                                 QMessageBox::Ok);
            return;
        }
    }
    else
    {
        QMessageBox::warning(this,
                             tr("Create Project"),
                             tr("Unable to create project directory."),
                             QMessageBox::Ok);
        return;
    }
}

void ProjectWizard::onProjectNameTextChanged()
{
    _ui->lineEditProjectDirectory->setText(_projectDirectoryRoot +
                                           QString("/") +
                                           _ui->lineEditProjectName->text());
}

void ProjectWizard::onProjectDirectoryPressed()
{
    QString directory = QFileDialog::getExistingDirectory(this,
                                                          tr("Open Directory"),
                                                          _projectDirectoryRoot,
                                                          QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    if (!directory.isEmpty())
    {
        _projectDirectoryRoot = directory;
        _ui->lineEditProjectDirectory->setText(_projectDirectoryRoot +QString("/") + _ui->lineEditProjectName->text());
    }
}

void ProjectWizard::onProjectDirectoryTextChanged()
{
}
