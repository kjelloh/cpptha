#include "meh.hpp"

namespace meh {

  // -------------------------- 
  // 'Latest' meh api

  template <class thaT>
  static cppthaEnvironment::value_type make_entry(Identifier identifier,thaT tt) {
    return {identifier,tt};
  }

  thaMeta::thaMeta() {
    the_environment.instance()->push(make_entry("meta",*this));
  }
  thaMeta::~thaMeta() {
    the_environment.instance()->pop();
  }
  thaStruct::thaStruct(Identifier const& identifier) 
    : m_identifier{identifier} {
    the_environment.instance()->push(make_entry(m_identifier,*this));
  }
  thaStruct::~thaStruct() {
    the_environment.instance()->pop();
  }

  cppthaEnvironment& cppthaEnvironment::push(value_type const& entry) {
    m_environment.push_back(entry);
    m_current_level = m_environment.size();
    return *this;
  }
  cppthaEnvironment& cppthaEnvironment::pop() {
    m_environment.pop_back();
    m_current_level = m_environment.size();
    return *this;
  }
  cppthaEnvironment& cppthaEnvironment::append(value_type const& entry) {
    // ??
    return *this;
  }

  std::string const& cppthaEnvironment::to_defacto() {
    static std::string result{}; // static = life time of the user environment (shared binary)
    result = "cppthaEnvironment::to_defacto() not yet implemented"; 
    return result;
  }

  // -------------------------- 
  // 'Older' meh api

  struct_tha::struct_tha(std::optional<std::string> identifier,std::string body)
    :  m_identifier{identifier}
      ,m_body{body} {}
  
  std::string struct_tha::to_string() const {
    return std::string{"struct"} + " " + m_identifier.value_or("") + "{ " + m_body + " }";
  }

  meta_tha::meta_tha() {}
  meta_tha& meta_tha::operator+=(tha const& x) {
    m_env.push_back(x);
    return *this;
  }

  struct ThaToCppVisitor {
    template <class Meh>
    std::string operator()(Meh const& x) {
      return x.to_string();
    }
  };

  std::string meta_tha::to_string() const {
    std::string result{};
    ThaToCppVisitor tha_to_cpp{};
    for (auto const& x : m_env) {
      result += std::visit(tha_to_cpp,x);
    }
    return result;
  }

} // meh