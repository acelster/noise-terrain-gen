#ifndef NMGUI_MODULETYPE_QHPP
#define NMGUI_MODULETYPE_QHPP

#include <QObject>
#include <QtQml/QQmlListProperty>

#include <nmlib/util/signals.hpp>

namespace nm {
class ModuleType;
}

namespace nmgui {

class ModuleInputQ;
class ModuleOutputQ;

class ModuleTypeQ : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name NOTIFY nameChanged)
    Q_PROPERTY(QString description READ description NOTIFY descriptionChanged)
    Q_PROPERTY(QQmlListProperty<nmgui::ModuleInputQ> inputs READ inputs NOTIFY inputsChanged)
    Q_PROPERTY(QQmlListProperty<nmgui::ModuleOutputQ> outputs READ outputs NOTIFY outputsChanged)
public:
    //should have been protected, but not allowed by qt
    explicit ModuleTypeQ(nm::ModuleType *moduleType = nullptr, QObject *parent = 0);
    static ModuleTypeQ *fromModuleType(nm::ModuleType &moduleType);
    static ModuleTypeQ *fromModuleType(const nm::ModuleType &moduleType);

    QString name() const;
    QString description() const;
    QQmlListProperty<ModuleInputQ> inputs();
    QQmlListProperty<ModuleOutputQ> outputs();
    nm::ModuleType* moduleType() const {return m_moduleType;}

signals:
    void inputsChanged();
    void outputsChanged();
    void nameChanged();
    void descriptionChanged();

private:

    nm::ModuleType *m_moduleType;

    static ModuleInputQ* inputAt(QQmlListProperty<ModuleInputQ> *list, int index);
    static int inputsCount(QQmlListProperty<ModuleInputQ> *list);

    static ModuleOutputQ* outputAt(QQmlListProperty<ModuleOutputQ> *list, int index);
    static int outputsCount(QQmlListProperty<ModuleOutputQ> *list);

    boost::signals2::scoped_connection moduleTypeDestroyedConnection;
    boost::signals2::scoped_connection inputAddedCon, outputAddedCon;
    boost::signals2::scoped_connection inputRemovedCon, outputRemovedCon;
    boost::signals2::scoped_connection nameChangedCon, descriptionChangedCon;
};

} // namespace nmgui

#endif // NMGUI_MODULETYPEQ_HPP
