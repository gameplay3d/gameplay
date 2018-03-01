#pragma once

#include <QTreeView>

class ProjectView;

/**
 * Defines the project tree view which display the project 
 * specific files from the projects file system.
 */
class ProjectTreeView : public QTreeView
{
    Q_OBJECT
public:

    /**
     * Constructor.
     *
     * @param parent The parent widget.
     */
    explicit ProjectTreeView(QWidget* parent = nullptr);

public slots:

    /**
     * Event fired when a project file is opened.
     */
    void onOpenFile();

    /**
     * Event fired when an item is double clicked.
     *
     * @param index The mode index for the item double clicked.
     */
    void onItemDoubleClicked(const QModelIndex& index);

protected:

    /**
     * Handler fo context menu events.
     *
     * @param event The context menu event.
     */
    void contextMenuEvent(QContextMenuEvent* event);

private:

    void openSelectedItem();

    ProjectView* _projectView;
    
};
