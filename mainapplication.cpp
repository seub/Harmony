#include "mainapplication.h"

#include "canvas.h"
#include "window.h"
#include "h2canvasdelegate.h"

MainApplication::MainApplication(int &argc, char **argv) : QApplication(argc, argv)
{
    errorCaught = false;
    srand (static_cast <unsigned> (time(0))); // Seed the random number generator

    try
    {
        createWindow();
    }
    catch(QString errorMessage)
    {
        qDebug() << "Error caught (by MainApplication::MainApplication): " << errorMessage;
        throw(QString("Error thrown in MainApplication::MainApplication"));
    }
}

void MainApplication::createWindow()
{
    window = new Window(&(this->handler));

    connect(window, SIGNAL(destroyed()), this, SLOT(quit()));
    window->setAttribute(Qt::WA_DeleteOnClose, true);

    handler.setWindow(window);
    handler.setContainer(&(this->container));

    window->show();
    window->resizeCanvases();
    window->setEnabled(true);
    window->setFocus();
}

MainApplication::~MainApplication()
{
}

bool MainApplication::notify(QObject * receiver, QEvent * event)
{
    try
    {
        return QApplication::notify(receiver, event);
    }
    catch(QString errorMessage)
    {
        if (errorCaught)
        {
            return QApplication::notify(receiver, event);
        }
        else
        {
            qDebug() << "In MainApplication::notify: errorMessage = " << errorMessage;
            // Do Something (see CirclePackings code)
        }
    }
    return false;
}
