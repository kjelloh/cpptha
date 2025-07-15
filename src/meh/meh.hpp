#pragma once
#include <variant>
#include <vector>
#include <string>
#include <optional>

// The CPP library to build the meh shared binary (dll)
// This library makes src/drive/driver.cpp generated source code compile
namespace meh {
  using MaybeString = std::optional<std::string>;
  class struct_tha {
  public:
    struct_tha(std::optional<std::string> identifier,std::string body);
    struct_tha(struct_tha const&) = default;
    struct_tha(struct_tha&&) = default;
    std::string to_string() const;
  private:
    MaybeString m_identifier;
    std::string m_body;
  };

  using tha = std::variant<struct_tha>;

  class meta_tha {
  public:
    meta_tha();
    meta_tha& operator+=(tha const& x);
    std::string to_string() const;
  private:
    std::vector<tha> m_env;
  };
} // meh