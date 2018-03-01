#pragma once

#include "EditorWindow.h"
#include "ProjectSortFilterProxyModel.h"
#include <QWidget>

namespace Ui
{
class ProjectView;
}
class Project;


/**
 * Defines a project view showing files visible in the project directory.
 */
class ProjectView : public QWidget
{
    Q_OBJECT
    friend class ProjectTreeView;

public:

    /**
     * Constructor.
     *
     * @param parent The parent widget.
     */
    explicit ProjectView(QWidget* parent = nullptr);

    /**
     * Destructor.
     */
    ~ProjectView();

    /**
     * Opens a project at the specified path.
     *
     * @param path The project path.
     */
    void openProject(const QString& path);

    /**
     * Closes the currently open project.
     */
    void closeProject();

    /**
     * Gets the project that is open.
     *
     * @return The open project.
     */
    std::shared_ptr<Project> getProject() const;

    /**
     * Gets the sort filter for the project model.
     *
     * @return The sort filter for the project model.
     */
    ProjectSortFilterProxyModel* sortFilter() const;

public slots:

    /**
     * Handler for when the search filter text changes.
     *
     * @param text The changed text.
     */
    void onSearchTextChanged(const QString& text);

signals:

    /**
     * Signal emmited with a new scene has been opened.
     *
     * @param path The path of the scene being opened.
     */
    void sceneOpened(const QString& path);

protected:

    /**
     * Gets the ui associated with this view.
     *
     * @return The view ui.
     */
    Ui::ProjectView* ui();

    /**
     * Opens a scene at the specified path.
     *
     * @param path The path of the scene to be opened.
     */
    void openScene(const QString& path);

private:

    Ui::ProjectView* _ui;
    std::shared_ptr<Project> _project;
    ProjectSortFilterProxyModel* _sortFilter;
};
