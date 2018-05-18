#pragma once

#include "ProjectWizardItemDelegate.h"
#include <memory>
#include <QDialog>
#include <QStyledItemDelegate>

namespace Ui
{
class ProjectWizard;
}
class EditorWindow;
class Project;


/**
 * Defines a class used for selecting an existing
 * project/sample to open or a new project
 * to be created.
 */
class ProjectWizard : public QDialog
{
    Q_OBJECT
public:

    /**
     * Constructor.
     *
     * @param parent The parent widget.
     */
    explicit ProjectWizard(QWidget* parent = nullptr);

    /**
     * Destructor.
     */
    ~ProjectWizard();

    /**
     * Initialize the project wizard and ui.
     */
    bool initialize();

    /**
     * Reads all the projects.
     */
    void readProjects();

public slots:

    /**
     * Handles the new project pressed.
     */
    void onNewProjectPressed();

    /**
     * Handles the open project pressed.
     */
    void onOpenProjectPressed();

    /**
     * Handles the cancel project pressed.
     */
    void onCancelProjectPressed();

    /**
     * Handles the create project pressed.
     */
    void onCreateProjectPressed();

    /**
     * Handles the project name text being changed.
     */
    void onProjectNameTextChanged();

    /**
     * Handles the project directory being pressed.
     */
    void onProjectDirectoryPressed();

    /**
     * Handles the project directory text being changed.
     */
    void onProjectDirectoryTextChanged();

signals:

    /**
     * Signal emitted when a project is opened.
     *
     * @param path The project path.
     */
    void projectOpened(const QString& path);

private:
    void resetEditor();

    Ui::ProjectWizard* _ui;
    std::unique_ptr<EditorWindow> _editor;
    QStringList _recentProjects;
    QString _projectDirectoryRoot;
    QString _projectDirectory;
    ProjectWizardItemDelegate _itemDelegate;
};
