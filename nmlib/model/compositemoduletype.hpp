#ifndef NM_COMPOSITEMODULETYPE_HPP
#define NM_COMPOSITEMODULETYPE_HPP

#include <nmlib/model/moduletype.hpp>

#include <nmlib/model/moduleinput.hpp>
#include <nmlib/model/moduleoutput.hpp>
#include <nmlib/model/inputmoduletype.hpp>

#include <memory>
#include <vector>

namespace nm {

class InputLink;
class OutputLink;
class Module;

class CompositeModuleType : public ModuleType
{
public:
    explicit CompositeModuleType(const std::string &name, const std::string &description);
    virtual ~CompositeModuleType();

    virtual std::string getName() const override {return c_name;}
    virtual std::string getDescription() const override {return c_description;}
    virtual const ModuleInput *getInput(std::string name) const override;
    virtual const ModuleOutput *getOutput(std::string name) const override;
    virtual std::vector<ModuleOutput*> outputs() override;
    virtual std::vector<ModuleInput*> inputs() override;
    virtual void onCreatedModule(Module &module) override {m_modulesOfThisType.push_back(&module);} //TODO add to a list of modules
    virtual void onDestroyingModule(Module *module) override;

    bool addInput(std::string name, SignalType signalType);
    bool exportOutput(const OutputLink &outputLink, std::string externalName);
    bool addModule(std::unique_ptr<Module> module);
    void clearModules();
    Module* getModule(const std::string &name);
    const Module* getModule(const std::string &name) const;
private:
    const std::string c_name;
    const std::string c_description;
    InputModuleType m_inputModuleType;
    std::vector<std::unique_ptr<ModuleInput>> m_inputs;
    std::vector<std::pair<std::unique_ptr<ModuleOutput>, const OutputLink&>> m_outputs;
    std::vector<std::unique_ptr<Module>> m_internalModules;
    std::vector<Module*> m_modulesOfThisType;
};

} // namespace nm

#endif // NM_COMPOSITEMODULETYPE_HPP
