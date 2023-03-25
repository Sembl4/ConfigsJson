#include <config.hpp>
#include <fstream>
#include <sstream>
#include <algorithm>
using namespace nlohmann;

#include <iostream>

json Config::read(std::string file_path)
{
    json cfg;
    std::ifstream file(file_path);
    cfg = json::parse(file);
    file.close();
    return cfg;
}

json Config::getFullConfig(std::string path, json cfg)
{
    auto it = cfg.begin();
    auto end = cfg.end();
    while(it != end)
    {
        std::cout << it.key() << std::endl;
        if(it.value().is_string())
        {
            if(it.key() == "cfg_file" || it.key() == "cfg_file_ap")
            {
                std::string current_path;
                if(it.key() == "cfg_file")
                    current_path = path + it.value().get<std::string>();
                else
                    current_path = it.value().get<std::string>();
                
                json temp = read(current_path);
                cfg.insert(temp.begin(),temp.end());
                cfg.erase(it.key());
                it = cfg.begin();
                end = cfg.end();               
            }
        }
        if(it.value().is_object())
        {
            std::cout << "go to " << it.key() << std::endl;
            cfg[it.key()] = getFullConfig(path, it.value());
            std::cout << "out of " << it.key() << std::endl;
        }
        it++;
    }
    return cfg;
}

void Config::ReadFromFile(std::string file_path)
{
    std::stringstream ss(file_path);
    std::string temp;
    std::vector<std::string> list;
    while(std::getline(ss,temp,'/'))
        list.push_back(temp);
    std::string filename = list.back();
    list.pop_back();
    std::string path = "";
    for(auto p : list)
        path+=p+'/';
        
    config = getFullConfig(path, read(file_path));
}

void Config::WriteToFile(std::string file_path)
{
    std::ofstream out(file_path);
    out << config.dump();
    out.close();
}
bool Config::insert(std::string key, Config value)
{
    if(config.contains(key)) return false;
    config[key] = value.config;
    return true;
}
bool Config::insert(std::string key,nlohmann::basic_json<std::map,std::vector,std::string,bool,int64_t,uint64_t,double> value)
{
    if(config.contains(key)) return false;
    config[key] = value;
    return true;
}
void Config::insert_or_replace(std::string key, Config value)
{
    config[key] = value.config;
}
void Config::insert_or_replace(std::string key, nlohmann::basic_json<std::map,std::vector,std::string,bool,int64_t,uint64_t,double> value)
{
    config[key] = value;
}
void Config::erase(std::string key)
{
    config.erase(key);
}

