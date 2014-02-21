#include <nmlib/model/compositemoduletype.hpp>

#include <nmlib/model/moduletype.hpp>
#include <nmlib/model/inputlink.hpp>
#include <nmlib/model/outputlink.hpp>
#include <nmlib/model/module.hpp>

#include <nmlib/util.hpp>

#include <algorithm>
#include <cassert>

namespace nm {

CompositeModuleType::CompositeModuleType(const std::string& name, const std::string& description):
    c_name(name),
    c_description(description),
    m_inputs(),
    m_outputs(),
    m_internalModules()
{
//    auto inputModule = make_unique<Module>(m_inputModuleType, "inputs");
//    m_internalModules.emplace_back(std::move(inputModule));
}

CompositeModuleType::~CompositeModuleType()
{
    assert(m_modulesOfThisType.empty());
}

const ModuleInput *CompositeModuleType::getInput(std::string name) const
{
    using namespace std;
    auto it = find_if(begin(m_inputs), end(m_inputs),
                      [&](const std::unique_ptr<ModuleInput> &entry){return entry->getName()==name;});
    if(it != end(m_inputs)){
        return it->get();
    } else {
        return nullptr;
    }
}

const ModuleOutput *CompositeModuleType::getOutput(std::string name) const
{
    using namespace std;
    auto it = find_if(begin(m_outputs), end(m_outputs),
                      [&](const std::pair<std::unique_ptr<ModuleOutput>, const OutputLink&> &entry){return entry.first->getName()==name;});
    if(it != end(m_outputs)){
        return it->first.get();
    } else {
        return nullptr;
    }
}

std::vector<ModuleOutput *> CompositeModuleType::outputs()
{
    std::vector<ModuleOutput*> ret{};
    ret.reserve(m_inputs.size());
    for(auto &i : m_outputs){
        ret.emplace_back(i.first.get());
    }
    return ret;
}

std::vector<ModuleInput *> CompositeModuleType::inputs()
{
    std::vector<ModuleInput*> ret{};
    ret.reserve(m_inputs.size());
    for(auto &i : m_inputs){
        ret.emplace_back(i.get());
    }
    return ret;
}

void CompositeModuleType::onDestroyingModule(Module *module)
{
    std::remove(std::begin(m_modulesOfThisType), std::end(m_modulesOfThisType), module);
}

bool CompositeModuleType::addInput(std::string /*name*/, SignalType /*signalType*/)
{
//    if(getInput(name) != nullptr){
//        return false;
//    }
//    m_inputs.emplace_back(new ModuleInput{name, signalType, *this});
//    m_inputModuleType.addInput(name, signalType);
    return true;
}

bool CompositeModuleType::exportOutput(const OutputLink &/*outputLink*/, std::string /*externalName*/)
{
//    if(getOutput(externalName) != nullptr){
//        return false;
//    }
//    //verify that OutputLink is actually a part of the composite
//    if(getModule(outputLink.getOwner().getName()) != &(outputLink.getOwner())){
//        return false;
//    }
//    m_outputs.emplace_back(
//                std::unique_ptr<ModuleOutput>(new ModuleOutput(externalName, outputLink.getModuleOutput().getSignalType(), *this)),
//                outputLink);
    return true;
}
Module *CompositeModuleType::getModule(const std::string &name)
{
    using namespace std;
    auto it = find_if(begin(m_internalModules), end(m_internalModules),
            [&](const std::unique_ptr<Module> &module){return module->getName() == name;});
    return it != end(m_internalModules) ? it->get() : nullptr;
}

const Module *CompositeModuleType::getModule(const std::string &name) const
{
    return (const_cast<CompositeModuleType*>(this)->getModule(name));
}

} // namespace nm
