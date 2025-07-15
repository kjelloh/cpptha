#include "meh.hpp"

namespace meh {
  struct_tha::struct_tha(std::optional<std::string> identifier,std::string body)
    :  m_identifier{identifier}
      ,m_body{body} {}
  
  std::string struct_tha::to_string() const {
    return std::string{"struct"} + " " + m_identifier.value_or("") + "{ " + m_body + " }";
  }

  meta_tha::meta_tha() {}
  meta_tha& meta_tha::operator+=(tha const& x) {
    m_env.push_back(x);
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