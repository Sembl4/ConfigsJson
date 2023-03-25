#include <iostream>
#include <config.hpp>


int main(int argc,char *argv[])
{
    Config cfg;
    // cfg.ReadFromFile("../tests/config.json");
    
    cfg.insert("test","test");
    cfg.insert("test_cfg",cfg);
    cfg.WriteToFile("test.json");
    // std::cout << argc << std::endl;
    // // std::cout << argv << std::endl;
    // for(int i = 0; i < argc; i++)
    // {
    //     std::cout << argv[i] << std::endl;
    // }
    // std::cout << "TEST CONFIGS" << std::endl;
    // Config conf;
    // conf.ReadFromFile("config.json");
    // nlohmann::json test = conf.read("../tests/config.json"); 
    // std::cout << test["something"] << std::endl;

    // std::cout << path << "\n" << filename << std::endl;
    return 0;
}