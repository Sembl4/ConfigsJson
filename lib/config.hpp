#pragma once
#include <string>
#include <map>
#include <vector>
#include <optional>
#include <nlohmann/json.hpp>
#include <iostream>

namespace config
{
class Config;

namespace cfg_commons
{
    // Config file name in current main config location
    constexpr const char* cfg_fl = "cfg_file";
    // Config file absolute path
    constexpr const char* cfg_fl_ap = "cfg_file_ap";
};

// To determine this is vector
template<typename T>
struct is_vector : std::false_type {};
template<typename... Ts>
struct is_vector<std::vector<Ts...>> : std::true_type {};
// To determine this is map
template<typename T>
struct is_map : std::false_type {};
template<typename... Ts>
struct is_map<std::map<Ts...>> : std::true_type {};
// To determine this is common type from T to common types
template<class T>
struct is_common : std::integral_constant<bool, std::is_convertible_v<T,int64_t>   || 
                                                std::is_convertible_v<T,uint64_t>  || 
                                                std::is_convertible_v<T,float>     ||
                                                std::is_convertible_v<T,double>    ||
                                                std::is_convertible_v<T,bool>      ||
                                                std::is_convertible_v<T,std::string>> {};
// To determine this is json
template<class T>
struct is_json : std::integral_constant<bool, std::is_same_v<T,nlohmann::json>>{};
// To determine this is config
template<class T>
struct is_config : std::integral_constant<bool, std::is_same_v<T,Config>>{};

class Config
{
    // Main config file
    nlohmann::json config;
    // Just read from file
    nlohmann::json read(std::string file_path);
    // Get full nesting configs
    nlohmann::json getFullConfig(std::string path, nlohmann::json cfg);

    std::string rmComm(std::ifstream& i);

    bool insertConfig(std::string key,Config value);

    template<typename T>
    bool insertVector(std::string key,std::vector<T> value)
    {
        if constexpr (is_common<T>::value)
        {
            config[key] = value;
            return true;
        }
        return false;
    }
    template<typename T>
    bool insertCommon(std::string key, T value)
    {
        config[key] = value;
        return true;
    }
    template<typename K,typename V>
    bool insertMap(std::string key,std::map<K,V> value)
    {
        if constexpr((is_common<K>::value && is_common<V>::value) && !is_json<K>::value)
        {
            config[key] = value;
            return true;
        }
        return false;
    }
    std::optional<Config> getConfig(const std::string& key)
    {
        if(config[key].is_object())
        {
            Config cfg;
            cfg.config = config[key].get<nlohmann::json>();
            return cfg;
        }
        return std::nullopt;
    }

public:
    Config(){}
    Config(std::string file_path)
    {
        open(file_path);
    }

    ~Config()
    {
        config.clear();
    }
    /*
    * Read config from file_path,
    * You can read one file, and if it has nesting cfg_files("_cfg_fl"/"_cfg_fl_ap"), it will be added to current config
    */
    void open(std::string file_path); 
    // Write this config to file_path. It write full config to file
    void write(std::string file_path);

    bool contains(const std::string& key)
    {
        return config.contains(key);
    }
    // Get full string of config
    std::string dump()
    {
        return config.dump(4);
    }
    // Clear config
    void clear();
    // Insert key, value to config
    template<typename T>
    bool insert(const std::string& key, T value)
    {
        if constexpr(is_vector<T>::value)
        {
            return insertVector(key,value);
        }else if constexpr(is_map<T>::value)
        {
            return insertMap(key,value);
        }
        else if constexpr(is_common<T>::value)
        {
            return insertCommon(key,value);
        }else if constexpr(is_config<T>::value)
        {
            return insertConfig(key,value);
        }else
        {
            return false;
        }
    }
    // Get some value with key
    template<typename T>
    std::optional<T> get(const std::string& key)
    {   
        if(!contains(key)) return std::nullopt;

        if constexpr(is_config<T>::value)
        {
            return getConfig(key);
        }else
        {
            try
            {
                return config[key].get<T>();
            }catch(nlohmann::detail::type_error& e)
            {
                std::cerr << e.what() << std::endl;
                return std::nullopt;
            }
        }
    }
};
}