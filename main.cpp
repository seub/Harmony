#include "mainapplication.h"

#include "tests.cpp"

int main(int argc, char *argv[])
{


/*QApplication a(argc, argv);
runTests();

return a.exec();*/

return MainApplication(argc, argv).exec();
}
