#pragma once
#include <string>
#include <map>
#include <vector>
#include <nlohmann/json.hpp>
#include <optional>

namespace cfg_commons
{
    // Config file name in current main config location
    constexpr const char* cfg_fl = "_cfg_fl";
    // Config file absolute path
    constexpr const char* cfg_fl_ap = "_cfg_fl_ap";
};

class Config
{
    // Main config file
    nlohmann::json config;
    // Just read from file
    nlohmann::json read(std::string file_path);
    // Get full nesting configs
    nlohmann::json getFullConfig(std::string path, nlohmann::json cfg);
public:
    Config(){}
    Config(std::string file_path)
    {
        ReadFromFile(file_path);
    }
    ~Config()
    {
        config.clear();
    }
    /*
    * Read config from file_path,
    * You can read one file, and if it has nesting cfg_files("_cfg_fl"/"_cfg_fl_ap"), it will be added to current config
    */
    void ReadFromFile(std::string file_path); 
    // Write this config to file_path. It write full config to file
    void WriteToFile(std::string file_path);
    //----------------------------------------------------------------------------------------------------------------------------------
    // Inserts, replaces and erase
    /*
    * Simple config insert.
    * It will return true if there is no key in this config and false if it is not.
    */
    bool insert(std::string key, Config value);
    /*
    * Simple value insert. 
    * You can insert here only std::map/std::vector/std::string/bool/int64_t/uint64_t/double
    * It will return true if there is no key in this config and false if it is not.
    */
    bool insert(std::string key,nlohmann::basic_json<std::map,std::vector,std::string,bool,int64_t,uint64_t,double> value);
    /*
    * Force config insert.
    * It will insert, or replace this key if it contains in this config
    */
    void insert_or_replace(std::string key, Config value);
    /*
    * Force value insert.
    * You can insert here only std::map/std::vector/std::string/bool/int64_t/uint64_t/double
    * It will insert, or replace this key if it contains in this config
    */
    void insert_or_replace(std::string key, nlohmann::basic_json<std::map,std::vector,std::string,bool,int64_t,uint64_t,double> value);
    // Erase this key from config
    void erase(const std::string& key);
    // Clear config
    void clear();
    //----------------------------------------------------------------------------------------------------------------------------------
    //----------------------------------------------------------------------------------------------------------------------------------
    // Getters

    // It will return std::nullopt if config is not contains this key, or if value is not Config
    std::optional<Config> getConfig(const std::string& key) const;
    // It will return std::nullopt if config is not contains this key, or if value is not Bool
    std::optional<bool> getBool(const std::string& key) const;
    // It will return std::nullopt if config is not contains this key, or if value is not Int
    std::optional<int64_t> getInt(const std::string& key) const;
    // It will return std::nullopt if config is not contains this key, or if value is not Unsigned Int
    std::optional<uint64_t> getUInt(const std::string& key) const;
    // It will return std::nullopt if config is not contains this key, or if value is not Double
    std::optional<double> getDouble(const std::string& key) const;
    // It will return std::nullopt if config is not contains this key, or if value is not String
    std::optional<std::string> getString(const std::string& key) const;
    /*
    * It will return std::nullopt if config is not contains this key, 
    * or if value is not Array, or if Array values is not Config
    */
    std::optional<std::vector<Config>> getArrConfig(const std::string& key) const;
    /*
    * It will return std::nullopt if config is not contains this key, 
    * or if value is not Array, or if Array values is not Unsigned Bool
    */
    std::optional<std::vector<bool>> getArrBool(const std::string& key) const;
    /*
    * It will return std::nullopt if config is not contains this key, 
    * or if value is not Array, or if Array values is not Int
    */ 
    std::optional<std::vector<int64_t>> getArrInt(const std::string& key) const;
    /*
    * It will return std::nullopt if config is not contains this key, 
    * or if value is not Array, or if Array values is not Unsigned Int
    */ 
    std::optional<std::vector<uint64_t>> getArrUInt(const std::string& key) const;
    /*
    * It will return std::nullopt if config is not contains this key, 
    * or if value is not Array, or if Array values is not Double
    */ 
    std::optional<std::vector<double>> getArrDouble(const std::string& key) const;
    /*
    * It will return std::nullopt if config is not contains this key, 
    * or if value is not Array, or if Array values is not String
    */
    std::optional<std::vector<std::string>> getArrString(const std::string& key) const;
    //----------------------------------------------------------------------------------------------------------------------------------
};