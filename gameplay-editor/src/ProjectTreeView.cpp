#include "ProjectTreeView.h"
#include "Project.h"
#include "ProjectView.h"
#include "ui_ProjectView.h"
#include <QtWidgets>


ProjectTreeView::ProjectTreeView(QWidget* parent) : QTreeView(parent), 
    _projectView(static_cast<ProjectView*>(parent))
{
    setContextMenuPolicy(Qt::ContextMenuPolicy::DefaultContextMenu);
    setSortingEnabled(true);
}

void ProjectTreeView::onItemDoubleClicked(const QModelIndex& index)
{
    openSelectedItem();
}

void ProjectTreeView::onOpenFileTriggered()
{
    openSelectedItem();
}

void ProjectTreeView::openSelectedItem()
{
    std::shared_ptr<Project> project = _projectView->getProject();
    ProjectSortFilterProxyModel* sortFilter = _projectView->sortFilter();
    const QModelIndex index = sortFilter->mapToSource(this->selectionModel()->currentIndex());

    QString sceneFilePath = project->filePath(index);
    if (sceneFilePath.endsWith(".scene"))
    {
        _projectView->openScene(sceneFilePath);
    }
}

void ProjectTreeView::contextMenuEvent(QContextMenuEvent* event)
{
    QMenu menu(this);
    Ui::ProjectView* ui = _projectView->ui();
    menu.addAction(ui->actionOpenFile);
    menu.addSeparator();
    menu.addAction(ui->actionNewFile);
    menu.addAction(ui->actionNewFolder);
    menu.addSeparator();
    menu.addAction(ui->actionImportFile);
    menu.exec(event->globalPos());
}


