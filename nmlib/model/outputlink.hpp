#ifndef NM_OUTPUTLINK_HPP
#define NM_OUTPUTLINK_HPP

#include <nmlib/util/userdataprovider.hpp>
#include <nmlib/util/signals.hpp>

#include <set>

namespace nm {

class Module;
class ModuleOutput;
class InputLink;

class OutputLink : public UserDataProvider
{
public:
    explicit OutputLink(const Module &owner, const ModuleOutput &type):
        c_owner(owner),
        c_moduleOutput(type),
        m_inputLinks()
    {}

    /**
     * @brief Add a connection from this OutputLink to the specified InputLink
     * @param input
     * @return Whether a new link was added.
     */
    bool addLink(InputLink &input);

    /**
     * @brief Remove the link an InputLink if it exists
     * @param input
     * @return Whether a link was removed
     */
    bool unlink(InputLink *input);

    /**
     * @brief Remove links to all inputs.
     *
     * This also removes the links from the inputs to the output
     */
    void unlinkAll();

    const Module &getOwner() const {return c_owner;}
    const ModuleOutput &getModuleOutput() const {return c_moduleOutput;}

    //signals
    signal<void (OutputLink&)> linksChanged;

private:
    const Module &c_owner;
    const ModuleOutput &c_moduleOutput;
    std::set<InputLink *> m_inputLinks;
};

} // namespace nm

#endif // NM_OUTPUTLINK_HPP
