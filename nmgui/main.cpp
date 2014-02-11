#include "moduleq.hpp"
#include "inputlinkq.hpp"
#include "outputlinkq.hpp"
#include "texturerenderer.hpp"

#include "beziercurve.hpp"

#include <QtGui/QGuiApplication>
#include "qtquick2applicationviewer.h"

#include <QQmlContext>
#include <QtQml>

#include <nmlib/model.hpp>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    //qt-licensed types
    qmlRegisterType<BezierCurve>("CustomGeometry", 1, 0, "QtBezierCurve");

    //register types
    qmlRegisterType<nmgui::TextureRenderer>("NoiseModeler", 1, 0, "TextureRenderer");
    qmlRegisterType<nmgui::ModuleQ>("NoiseModeler", 1, 0, "Module");
    qmlRegisterType<nmgui::InputLinkQ>("NoiseModeler", 1, 0, "InputLink");
    qmlRegisterType<nmgui::OutputLinkQ>("NoiseModeler", 1, 0, "OutputLink");

    //create mockup data
    nm::TypeManager typeManager;
    typeManager.initBuiltinTypes();
    auto fbmModuleType = typeManager.getType("fbm");
    auto addModuleType = typeManager.getType("add");
    auto fbmModule = nm::Module::create(*fbmModuleType, "myfbm");
    auto fbmModule2 = nm::Module::create(*fbmModuleType, "myfbm2");
    auto AddModule = nm::Module::create(*addModuleType, "myAdd");
    auto mockModule = nmgui::ModuleQ::fromModule(*fbmModule);
    auto mockModule2 = nmgui::ModuleQ::fromModule(*fbmModule2);
    auto mockModule3 = nmgui::ModuleQ::fromModule(*AddModule);
//    mockModule.setName("TestName");

    QtQuick2ApplicationViewer viewer;
    viewer.rootContext()->setContextProperty("mockModule", mockModule);
    viewer.rootContext()->setContextProperty("mockModule2", mockModule2);
    viewer.rootContext()->setContextProperty("mockModule3", mockModule3);
    viewer.setMainQmlFile(QStringLiteral("qml/noisemodeler/main.qml"));
    viewer.showExpanded();

    return app.exec();
}
