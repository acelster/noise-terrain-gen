#ifndef NM_PARSER_HPP
#define NM_PARSER_HPP

#include <nmlib/model.hpp>
#include <nmlib/optional.hpp>

#include <map>
#include <memory>

namespace nm {

class Module;

class Parser
{
public:
    explicit Parser();
    optional<std::map<std::string, std::unique_ptr<ModuleType>>> parse(std::string input);
};

} // namespace nm

#endif // NM_PARSER_HPP