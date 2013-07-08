#include <QApplication>
#include <QDebug>
#include <QWidget>


#include "types.h"

#include "canvas.h"
#include "canvasdelegate.h"
#include "h2canvasdelegate.h"
#include "discretegroup.h"

int main(int argc, char *argv[])
{    
    std::cout << std::endl;
    QApplication a(argc, argv);

    std::vector<generatorName> generators;
    generators.push_back("a");
    generators.push_back("b");
    generators.push_back("c");
    word w;
    std::vector<word> relations;
    w.push_back(letter(0, 1));
    w.push_back(letter(1, -1));
    w.push_back(letter(2, 1));
    w.push_back(letter(0, 3));
    relations.push_back(w);
    DiscreteGroup Gamma1(generators, relations);
    std::cout << Gamma1 << std::endl;

    generators.clear();
    generators.push_back("c");
    generators.push_back("d");
    generators.push_back("e");
    w.clear();
    relations.clear();
    w.push_back(letter(0, 1));
    w.push_back(letter(1, 1));
    w.push_back(letter(2, 1));
    w.push_back(letter(0, -2));
    relations.push_back(w);
    DiscreteGroup Gamma2(generators, relations);
    std::cout << Gamma2 << std::endl;

    DiscreteGroup Gamma = amalgamateOverCommonGenerator(Gamma1, Gamma2);
    std::cout << Gamma << std::endl;

    /*Canvas * canvas = new Canvas;
    H2CanvasDelegate * delegate = new H2CanvasDelegate(canvas);
    canvas->setDelegate(delegate);
    delegate->drawExample();
    canvas->show();*/



    std::cout << std::endl;
    return a.exec();
}
