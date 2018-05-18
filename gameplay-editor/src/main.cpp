#include "ProjectWizard.h"
#include <QApplication>
#include <QCoreApplication>
#include <QGuiApplication>
#include <QStyleFactory>
#include <QPixmap>
#include <QSplashScreen>
#include <QThread>

/**
 * Main application entry point.
 *
 * @param argc The number of command line arguments.
 * @param argv The array of command line arguments.
 */
int main(int argc, char* argv[])
{
    // Set the application meta data
    QCoreApplication::setOrganizationDomain("io");
    QCoreApplication::setOrganizationName("gameplay");
    QCoreApplication::setApplicationName("GamePlay");
    QCoreApplication::setApplicationVersion("4.0.0");

    QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);


    QApplication app(argc, argv);
    app.setStyle(QStyleFactory::create("Fusion"));

    // Create and show splash screen
    QPixmap splashPixmap(":/res/images/splash");
    QSplashScreen* splashScreen = new QSplashScreen(splashPixmap);
    splashScreen->show();
    QThread::sleep(2);

    // Process the application event
    app.processEvents();

    // Create the project wizard
    ProjectWizard wizard;

    if (wizard.initialize())
    {
        // Show the wizard
        wizard.show();

        // Finish the splash
        splashScreen->finish(&wizard);

        // Start executing the app
        return app.exec();
    }
    else
    {
        return EXIT_FAILURE;
    }
}
