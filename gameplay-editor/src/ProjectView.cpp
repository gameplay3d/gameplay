#include "ProjectView.h"
#include "Project.h"
#include "ui_ProjectView.h"
#include <QtWidgets>
#include <QFileSystemModel>

ProjectView::ProjectView(QWidget* parent) : QWidget(parent), 
    _ui(new Ui::ProjectView),
    _project(nullptr),
    _sortFilter(nullptr)
{
    _ui->setupUi(this);

    _ui->lineEditSearch->addAction(QIcon(":/res/images/search.png"), QLineEdit::LeadingPosition);

    connect(_ui->lineEditSearch, SIGNAL(textChanged(QString)), this, SLOT(onSearchTextChanged(QString)));
    connect(_ui->treeView, SIGNAL(doubleClicked(QModelIndex)), _ui->treeView, SLOT(onItemDoubleClicked(QModelIndex)));
    connect(_ui->actionOpenFile, SIGNAL(triggered(bool)), _ui->treeView, SLOT(onOpenFile()));
}

ProjectView::~ProjectView()
{
    delete _ui;
    closeProject();
}

Ui::ProjectView* ProjectView::ui()
{
    return _ui;
}

void ProjectView::openProject(const QString& path)
{
    closeProject();
    _project = Project::open(path, _ui->treeView);
    if (_project)
    {
        _project->setRootPath(path);

        // Name filter for file extension the project is aware of
        QStringList nameFilters;
        nameFilters << "*.scene" << "*.asset" <<
                       "*.fbx" << "*.blend" <<
                       "*.ttf" << "*.png" << "*.psd" <<
                       "*.wav" << "*.ogg" << "*.mp4" <<
                       "*.lua" << "*.script";
        _project->setNameFilters(nameFilters);
        _project->setNameFilterDisables(false);
        
        // Sort and search filter
        _sortFilter = new ProjectSortFilterProxyModel();
        _sortFilter->setDynamicSortFilter(true);
        _sortFilter->setFilterKeyColumn(0);
        _sortFilter->setFilterCaseSensitivity(Qt::CaseInsensitive);
        _sortFilter->setSourceModel(_project.get());
        _ui->treeView->setSortingEnabled(true);
        _ui->treeView->setModel(_sortFilter);
        _ui->treeView->setRootIndex(_sortFilter->mapFromSource(_project->index(path)));

        // Header sizing
        _ui->treeView->hideColumn(2);
        _ui->treeView->setColumnWidth(0, 700);
        _ui->treeView->setColumnWidth(1, 120);
        _ui->treeView->setColumnWidth(2, 120);

        // Expand the '/assets' folder
        QString assetsFolderPath = path + QString("/") + QString(QLatin1String("assets"));
        _ui->treeView->expand(_sortFilter->mapFromSource(_project->index(assetsFolderPath)));

        // Open the last scene editor for the project.        
        openScene(path + QString("/") + _project->getScenePath());
    }
}

void ProjectView::closeProject()
{
}

std::shared_ptr<Project> ProjectView::getProject() const
{
    return _project;
}

ProjectSortFilterProxyModel* ProjectView::sortFilter() const
{
    return _sortFilter;
}

void ProjectView::openScene(const QString& path)
{
    emit sceneOpened(path);
}

void ProjectView::onSearchTextChanged(const QString& text)
{ 
    _sortFilter->setFilterRegExp(text);
}
