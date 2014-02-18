#include "moduletypeq.hpp"

#include <nmgui/moduleinputq.hpp>
#include <nmgui/moduleoutputq.hpp>

#include <nmlib/model/moduletype.hpp>

namespace nmgui {

ModuleTypeQ::ModuleTypeQ(nm::ModuleType *moduleType, QObject *parent) :
    QObject(parent),
    m_moduleType(moduleType)
{
    Q_ASSERT(m_moduleType!=nullptr);
    Q_ASSERT(m_moduleType->getUserData()==nullptr);
    m_moduleType->setUserData(this);

//    moduleTypeDestroyedConnection = m_moduleType->destroying.connect([&](nm::Module&){
//        deleteLater();
//        m_moduleType->setUserData(nullptr);
//        m_moduleType = nullptr;
    //    });
}

ModuleTypeQ *ModuleTypeQ::fromModuleType(nm::ModuleType &moduleType)
{
    //TODO switch to some Qt smart pointer instead of rawpointer?
    auto userData = static_cast<ModuleTypeQ*>(moduleType.getUserData());
    return userData != nullptr ? userData : new ModuleTypeQ(&moduleType);
}

ModuleTypeQ *ModuleTypeQ::fromModuleType(const nm::ModuleType &moduleType)
{
    return fromModuleType(const_cast<nm::ModuleType&>(moduleType));
}

QString ModuleTypeQ::name() const
{
    auto ss = m_moduleType->getName();
    return QString::fromUtf8(ss.data(), ss.size());
}

QQmlListProperty<ModuleInputQ> ModuleTypeQ::inputs()
{
    return QQmlListProperty<ModuleInputQ>(this, 0, nullptr, &ModuleTypeQ::inputsCount, &ModuleTypeQ::inputAt, nullptr);
}

QQmlListProperty<ModuleOutputQ> ModuleTypeQ::outputs()
{
    return QQmlListProperty<ModuleOutputQ>(this, 0, nullptr, &ModuleTypeQ::outputsCount, &ModuleTypeQ::outputAt, nullptr);
}

ModuleInputQ *ModuleTypeQ::inputAt(QQmlListProperty<ModuleInputQ> *list, int index)
{
    ModuleTypeQ *moduleType = qobject_cast<ModuleTypeQ *>(list->object);
    if(moduleType){
        //TODO this is very inefficient and redundant, but it works for now.
        return ModuleInputQ::fromModuleInput(*moduleType->m_moduleType->inputs().at(index));
    } else {
        return nullptr;
    }
}

int ModuleTypeQ::inputsCount(QQmlListProperty<ModuleInputQ> *list)
{
    ModuleTypeQ *moduleType = qobject_cast<ModuleTypeQ *>(list->object);
    if(moduleType){
        //TODO this is very inefficient and redundant, but it works for now.
        return moduleType->m_moduleType->inputs().size();
    } else {
        return 0;
    }
}

ModuleOutputQ *ModuleTypeQ::outputAt(QQmlListProperty<ModuleOutputQ> *list, int index)
{
    ModuleTypeQ *moduleType = qobject_cast<ModuleTypeQ *>(list->object);
    if(moduleType){
        return ModuleOutputQ::fromModuleOutput(*moduleType->m_moduleType->outputs().at(index));
    } else {
        return nullptr;
    }
}

int ModuleTypeQ::outputsCount(QQmlListProperty<ModuleOutputQ> *list)
{
    ModuleTypeQ *moduleType = qobject_cast<ModuleTypeQ *>(list->object);
    if(moduleType){
        //TODO this is very inefficient and redundant, but it works for now.
        return moduleType->m_moduleType->outputs().size();
    } else {
        return 0;
    }
}


} // namespace nmgui