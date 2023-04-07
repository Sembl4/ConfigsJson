#include <config.hpp>
#include <fstream>
#include <sstream>
#include <algorithm>

using namespace nlohmann;
using namespace config;
using namespace cfg_commons;

std::string Config::rmComm(std::ifstream& i)
{
    std::string json_data = "";
    std::string temp;
    bool in_comm = false;
    while(std::getline(i,temp))
    {
        if(temp.find("//") == std::string::npos)
        {
            json_data += temp;
        }
    }
    return json_data;
}

json Config::read(std::string file_path)
{
    std::ifstream file(file_path);
    json cfg = json::parse(rmComm(file));
    file.close();
    return cfg;
}

json Config::getFullConfig(std::string path, json cfg)
{
    auto it = cfg.begin();
    auto end = cfg.end();
    while(it != end)
    {
        if(it.value().is_string())
        {
            if(it.key() == cfg_fl || it.key() == cfg_fl_ap)
            {
                std::string current_path;
                if(it.key() == cfg_fl)
                    current_path = path + it.value().get<std::string>();
                else
                    current_path = it.value().get<std::string>();
                
                json temp = read(current_path);
                cfg.insert(temp.begin(),temp.end());
                cfg.erase(it.key());
                it = cfg.begin();
                end = cfg.end();
                continue;               
            }
        }
        if(it.value().is_object())
        {
            cfg[it.key()] = getFullConfig(path, it.value());
        }
        it++;
    }
    return cfg;
}

void Config::open(std::string file_path)
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

void Config::write(std::string file_path)
{
    std::ofstream out(file_path);
    out << dump();
    out.close();
}

void Config::clear()
{
    config.clear();
}

bool Config::insertConfig(std::string key,Config value)
{
    config[key] = value.config;
    return true;
}