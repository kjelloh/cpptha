#pragma once
#include <variant>
#include <vector>
#include <string>
#include <optional>

// The CPP library to build the meh shared binary (dll)
// This library makes src/drive/driver.cpp generated source code compile
namespace meh {

  // Singleton for default constructable T
  template <class T>
  class Singleton {
  public:
    using pointer = std::unique_ptr<T>;
    static pointer& instance() {
      if (!m_instance_ptr) {
        m_instance_ptr = std::make_unique<T>();
      }
      return m_instance_ptr;
    }
  private:
    static pointer m_instance_ptr;
  };

  template <class T>
  typename Singleton<T>::pointer Singleton<T>::m_instance_ptr = nullptr;

  using Identifier = std::string;

  struct thaMeta {
    thaMeta();
    ~thaMeta();
  };
  struct thaStruct {
    thaStruct(Identifier const& identifier);
    ~thaStruct();
    Identifier m_identifier;
  };

  using thaT = std::variant<thaMeta,thaStruct>;

  class cppthaEnvironment {
  public:
    using value_type = std::pair<Identifier,thaT>;
    cppthaEnvironment& push(value_type const& entry);
    cppthaEnvironment& pop();
    cppthaEnvironment& append(value_type const& entry);
    std::string const& to_defacto();
  private:
    std::vector<value_type> m_environment{};
    int m_current_level{};
  };

  inline Singleton<cppthaEnvironment> the_environment{};

  namespace test {
    inline std::string meh_test_code = R"({
      thaMeta tha_meta{};
      {
        thaStruct{"A"};
      }
      return the_environment.instance()->to_defacto().c_str();
    })";
  }

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