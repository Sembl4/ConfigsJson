#include <iostream>
#include <config.hpp>
using namespace config;

void usualPractice()
{
    std::cout << "usualUsing:" << std::endl;
    int i;
    uint ui;
    float f;
    double d;
    std::string s;
    bool b;
    std::vector<int> v;
    std::map<std::string,int> m;
    nlohmann::json j;
    Config c;
    
    Config cfg;
    cfg.open("../examples/configs/config.json");

    // Check common types

    auto ii = cfg.get<int>("int");
    if(ii.has_value())
        i = ii.value();
    std::cout << "i val = " << i << std::endl;

    auto uuii = cfg.get<uint>("uint");
    if(uuii.has_value())
        ui = uuii.value();
    std::cout << "ui val = " << ui << std::endl;

    auto ff = cfg.get<float>("float");
    if(ff.has_value())
        f = ff.value();
    std::cout << "f val = " << f << std::endl;

    auto dd = cfg.get<double>("double");
    if(dd.has_value())
        d = dd.value();
    std::cout << "d val = " << d << std::endl;

    auto ss = cfg.get<std::string>("string");
    if(ss.has_value())
        s = ss.value();
    std::cout << "s val = " << s << std::endl;

    auto bb = cfg.get<bool>("bool");
    if(bb.has_value())
        b = bb.value();
    std::cout << "b val = " << b << std::endl;

    // Check vector and map
    auto vv = cfg.get<std::vector<int>>("vector");
    if(vv.has_value())
        v = vv.value();
    std::cout << "v vals : " << std::endl;
    for(auto vvv : v)
    {
        std::cout << vvv << " ";
    }std::cout << std::endl;

    auto mm = cfg.get<std::map<std::string,int>>("map");
    if(mm.has_value())
        m = mm.value();
    std::cout << "m vals : " << std::endl;
    for(auto mmm : m)
    {
        std::cout << mmm.first << " : " << mmm.second << ";";
    }std::cout << std::endl;

    // Check nlohmann::json and Config
    auto jj = cfg.get<nlohmann::json>("json");
    if(jj.has_value())
        j = jj.value();
    std::cout << "j val:" << j.dump() << std::endl;
    
    auto cc = cfg.get<Config>("config_absolute_path");
    if(cc.has_value())
        c = cc.value();
    std::cout << "c val:" << c.dump()<< std::endl;

    cfg.write("usualPractice.json");
}

// Any structure
struct Example
{
    int64_t i;
    std::string s;
    bool operator==(const Example& e)
    {
        return (e.i == i && e.s == s);
    }
};
// Redefinition insert and get for this structure
template<>
bool Config::insert<Example>(const std::string& key, Example val);
template<>
std::optional<Example> Config::get<Example>(const std::string& key);

void bestPractice()
{
    std::cout << "bestPractice:" << std::endl;
    Example e;
    e.i = 1123;
    e.s = "I am string";
    Config cfg;
    cfg.insert("example",e);


    Example e1;
    auto te = cfg.get<Example>("example");
    if(te.has_value())
    {
        e1 = te.value();
    }

    std::cout << "e == e1 : " << std::boolalpha << (e==e1) << std::endl;

    cfg.write("bestPractice.json");
}

int main() 
{
    usualPractice();
    bestPractice();
    return 0;
}


template<>
bool Config::insert<Example>(const std::string& key, Example val)
{
    Config cfg;
    if(!cfg.insert("i",val.i) || !cfg.insert("s",val.s)) return false;
    return this->insert(key,cfg);
}

template<>
std::optional<Example> Config::get<Example>(const std::string& key)
{
    auto tc = get<Config>(key);
    if(tc.has_value())
    {
        Example e;
        Config cfg = tc.value();
        auto ti = cfg.get<int64_t>("i");
        auto ts = cfg.get<std::string>("s");
        if(ti.has_value() && ts.has_value())
        {
            e.i = ti.value();
            e.s = ts.value();
            return e;
        }else return std::nullopt;
    } 
    return std::nullopt;
}