#include "mainapplication.h"
#include "topfactory.h"

MainApplication::MainApplication(int &argc, char **argv) : QApplication(argc, argv)
{
    errorCaught = false;
    srand (static_cast <unsigned> (time(0))); // Seed the random number generator

    try
    {
        window = new Window(&(this->handler));
        connect(window, SIGNAL(destroyed()), this, SLOT(quit()));
        window->setAttribute(Qt::WA_DeleteOnClose, true);
        handler.setWindow(window);
        handler.setContainer(&(this->mathsContainer));

        factory = std::unique_ptr<TopFactory>(new TopFactory(&(this->mathsContainer), &(this->handler)));

        handler.setFactory(factory.get());
        handler.resetDelegatePointers();

        window->show();
        window->setEnabled(true);
        window->setFocus();
    }
    catch(QString errorMessage)
    {
        qDebug() << "Error caught (by MainApplication::MainApplication): " << errorMessage;
        throw(QString("Error thrown in MainApplication::MainApplication"));
    }
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
