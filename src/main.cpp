#include <QtGui/QGuiApplication>
#include "DataProvider.hpp"
#include "Renderer.hpp"
#include "View.hpp"

int main(int argc, char **argv)
{
    QGuiApplication app(argc, argv);

    viz::CDataProvider data_provider;

    viz::CRenderer renderer{data_provider};

    viz::CView view{renderer};
    view.show();

    return app.exec();
}
