#include <iostream>

// Prototypes

std::string getenv(const std::string& name);


// Main code
int main(void)
{
    std::string read;
    while ( read != "q" ) {
        std::cout << "introduce an enviroment variable ( q to exit ): ";
        std::cin >> read;
        if ( read != "q") {
            std::cout << "value: " << getenv(read) << std::endl;
        }
    }
    return 0;
}

// Functions

std::string getenv(const std::string& name)
{
  char* value = getenv(name.c_str());
  if (value) {
    return std::string(value);
  } else {
    return std::string();
  }
}