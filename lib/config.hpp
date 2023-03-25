#pragma once
#include <string>
#include <map>
#include <vector>
#include <nlohmann/json.hpp>
#include <optional>

class Config
{
    nlohmann::json config;
    nlohmann::json read(std::string file_path);
    nlohmann::json getFullConfig(std::string path, nlohmann::json cfg);
public:
    void ReadFromFile(std::string file_path); 
    void WriteToFile(std::string file_path);
    
    bool insert(std::string key, Config value);
    bool insert(std::string key,nlohmann::basic_json<std::map,std::vector,std::string,bool,int64_t,uint64_t,double> value);
    void insert_or_replace(std::string key, Config value);
    void insert_or_replace(std::string key, nlohmann::basic_json<std::map,std::vector,std::string,bool,int64_t,uint64_t,double> value);

    void erase(const std::string& key);

    std::optional<Config> getConfig(const std::string& key) const
    {
        // Config cfg;
        if(!config.contains(key)) return std::nullopt;
        if(!config.is_object()) return std::nullopt;
        Config cfg;
        cfg.config = config[key];
        return cfg;
    }

    std::optional<double> getDouble(const std::string& key) const
    {
        if(!config.contains(key)) return std::nullopt;
        if(!config[key].is_number_float()) return std::nullopt;
        return config[key].get<double>();
    }

    std::optional<int64_t> getInt(const std::string& key) const
    {
        if(!config.contains(key)) return std::nullopt;
        if(!config[key].is_number_integer()) return std::nullopt;
        return config[key].get<int64_t>();
    }

    std::optional<uint64_t> getUInt(const std::string& key) const
    {
        if(!config.contains(key)) return std::nullopt;
        if(!config[key].is_number_unsigned()) return std::nullopt;
        return config[key].get<uint64_t>();
    }
    
    std::optional<std::string> getString(const std::string& key) const
    {
        if(!config.contains(key)) return std::nullopt;
        if(!config[key].is_string()) return std::nullopt;
        return config[key].get<std::string>();
    }
    std::optional<bool> getBool(const std::string& key) const
    {
        if(!config.contains(key)) return std::nullopt;
        if(!config[key].is_boolean()) return std::nullopt;
        return config[key].get<bool>();
    }
    std::optional<std::vector<double>> getArrDouble(const std::string& key)
    {
        if(!config.contains(key)) return std::nullopt;
        if(!config[key].is_boolean()) return std::nullopt;
    }


    // template<typename Type>
    // Type operator[](std::string key) const
    // {
    //     return config[key].get<Type>();
    // }

    
};