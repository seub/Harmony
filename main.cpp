#include "mainapplication.h"

int main(int argc, char *argv[])
{
    std::cout << std::endl;

    MainApplication mainApplication(argc, argv);
    return mainApplication.exec();
}
