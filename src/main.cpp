#include "cpptha.h"
#include <vector>
#include <string>

int main() {
    cpptha();

    std::vector<std::string> vec;
    vec.push_back("test_package");

    cpptha_print_vector(vec);
}
