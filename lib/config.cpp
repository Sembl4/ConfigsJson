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
void Config::erase(const std::string& key)
{
    config.erase(key);
}
void Config::clear()
{
    config.clear();
}
std::optional<Config> Config::getConfig(const std::string& key) const
{
    if(!config.contains(key)) return std::nullopt;
    if(!config.is_object()) return std::nullopt;
    Config cfg;
    cfg.config = config[key].get<json>();
    return cfg;
}
std::optional<double> Config::getDouble(const std::string& key) const
{
    if(!config.contains(key)) return std::nullopt;
    if(!config[key].is_number_float()) return std::nullopt;
    return config[key].get<double>();
}
std::optional<int64_t> Config::getInt(const std::string& key) const
{
    if(!config.contains(key)) return std::nullopt;
    if(!config[key].is_number_integer()) return std::nullopt;
    return config[key].get<int64_t>();
}
std::optional<uint64_t> Config::getUInt(const std::string& key) const
{
    if(!config.contains(key)) return std::nullopt;
    if(!config[key].is_number_unsigned()) return std::nullopt;
    return config[key].get<uint64_t>();
}
std::optional<std::string> Config::getString(const std::string& key) const
{
    if(!config.contains(key)) return std::nullopt;
    if(!config[key].is_string()) return std::nullopt;
    return config[key].get<std::string>();
}
std::optional<bool> Config::getBool(const std::string& key) const
{
    if(!config.contains(key)) return std::nullopt;
    if(!config[key].is_boolean()) return std::nullopt;
    return config[key].get<bool>();
}
std::optional<std::vector<double>> Config::getArrDouble(const std::string& key) const
{
    if(!config.contains(key)) return std::nullopt;
    if(!config[key].is_array()) return std::nullopt;
    std::vector<double> vec;
    for(auto it : config[key])
    {
        if(!it.is_number_float()) return std::nullopt;
        vec.push_back(it.get<double>());
    }
    return vec;
}
std::optional<std::vector<int64_t>> Config::getArrInt(const std::string& key) const
{
    if(!config.contains(key)) return std::nullopt;
    if(!config[key].is_array()) return std::nullopt;
    std::vector<int64_t> vec;
    for(auto it : config[key])
    {
        if(!it.is_number_integer()) return std::nullopt;
        vec.push_back(it.get<int64_t>());
    }
    return vec;
}
std::optional<std::vector<uint64_t>> Config::getArrUInt(const std::string& key) const
{
    if(!config.contains(key)) return std::nullopt;
    if(!config[key].is_array()) return std::nullopt;
    std::vector<uint64_t> vec;
    for(auto it : config[key])
    {
        if(!it.is_number_unsigned()) return std::nullopt;
        vec.push_back(it.get<uint64_t>());
    }
    return vec;
}
std::optional<std::vector<std::string>> Config::getArrString(const std::string& key) const
{
    if(!config.contains(key)) return std::nullopt;
    if(!config[key].is_array()) return std::nullopt;
    std::vector<std::string> vec;
    for(auto it : config[key])
    {
        if(!it.is_string()) return std::nullopt;
        vec.push_back(it.get<std::string>());
    }
    return vec;
}
std::optional<std::vector<bool>> Config::getArrBool(const std::string& key) const
{
    if(!config.contains(key)) return std::nullopt;
    if(!config[key].is_array()) return std::nullopt;
    std::vector<bool> vec;
    for(auto it : config[key])
    {
        if(!it.is_boolean()) return std::nullopt;
        vec.push_back(it.get<bool>());
    }
    return vec;
}
std::optional<std::vector<Config>> Config::getArrConfig(const std::string& key) const
{
    if(!config.contains(key)) return std::nullopt;
    if(!config[key].is_array()) return std::nullopt;
    std::vector<Config> vec;
    for(auto it : config[key])
    {
        if(!it.is_object()) return std::nullopt;
        Config temp;
        temp.config = it.get<json>();
        vec.push_back(temp);
    }
    return vec;
}

