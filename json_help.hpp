#ifndef _JSON_HELP_HPP_
#define _JSON_HELP_HPP_

#include <stdio.h>
#include <string>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <memory>

#include "json11.hpp"

class JsonHelp
{
public:
        JsonHelp(){};
        JsonHelp(const std::string &json_content)
        {
                LoadFromString(json_content);
        };
        JsonHelp(const json11::Json &json)
        {
                LoadFromJson(json);
        };
        JsonHelp(const json11::Json::array &arry)
        {
                LoadFromArray(arry);
        };
        ~JsonHelp(){};

        int LoadFromFile(const std::string &json_file)
        {
                std::string config;
                std::ifstream jfile(json_file, std::ios::in);
                if (!jfile.is_open()) {
                        return -1;
                }
                while (!jfile.eof()) {
                        std::string tmp;
                        jfile >> tmp;
                        config += tmp;
                }
                jfile.close();
                return LoadFromString(config);
        }
        int LoadFromString(const std::string &json_content)
        {
                last_err_.clear();
                json11::Json json = json11::Json::parse(json_content, last_err_);
                if (!last_err_.empty()) {
                        return -1;
                }
                arry_.push_back(json);
                valid_ = true;
                return 0;
        }
        int LoadFromJson(const json11::Json &json)
        {
                last_err_.clear();
                if (json.is_null()) {
                        last_err_ = "json is null";
                        return -1;
                }
                arry_.clear();
                arry_.push_back(json);
                valid_ = true;
                return 0;
        }
        int LoadFromArray(const json11::Json::array &arry)
        {
                last_err_.clear();
                arry_ = arry;
                valid_ = true;
                return 0;
        }

        bool IsString() const
        {
                bool res = false;
                last_err_.clear();
                if (!Valid()) {
                        last_err_ = "not valid";
                        return res;
                }
                if (!arry_[0].is_string()) {
                        last_err_ = "not string";
                        return res;
                }
                return true;
        }
        bool IsInt() const
        {
                bool res = false;
                last_err_.clear();
                if (!Valid()) {
                        last_err_ = "not valid";
                        return res;
                }
                if (!arry_[0].is_number()) {
                        last_err_ = "not int";
                        return res;
                }
                return true;
        }
        bool IsFloat() const
        {
                bool res = false;
                last_err_.clear();
                if (!Valid()) {
                        last_err_ = "not valid";
                        return res;
                }
                if (!arry_[0].is_number()) {
                        last_err_ = "not float";
                        return res;
                }
                return true;
        }
        bool IsBool() const
        {
                bool res = false;
                last_err_.clear();
                if (!Valid()) {
                        last_err_ = "not valid";
                        return res;
                }
                if (!arry_[0].is_bool()) {
                        last_err_ = "not bool";
                        return res;
                }
                return true;
        }

        std::string GetString() const
        {
                std::string res;
                last_err_.clear();
                if (!Valid()) {
                        last_err_ = "not valid";
                        return res;
                }
                if (!arry_[0].is_string()) {
                        last_err_ = "not string";
                        return res;
                }
                return arry_[0].string_value();
        }
        int GetInt() const
        {
                int res = 0;
                last_err_.clear();
                if (!Valid()) {
                        last_err_ = "not valid";
                        return res;
                }
                if (!arry_[0].is_number()) {
                        last_err_ = "not int";
                        return res;
                }
                return (int)arry_[0].number_value();
        }
        double GetFloat() const
        {
                double res = 0;
                last_err_.clear();
                if (!Valid()) {
                        last_err_ = "not valid";
                        return res;
                }
                if (!arry_[0].is_number()) {
                        last_err_ = "not float";
                        return res;
                }
                return arry_[0].number_value();
        }
        bool GetBool() const
        {
                bool res = false;
                last_err_.clear();
                if (!Valid()) {
                        last_err_ = "not valid";
                        return res;
                }
                if (!arry_[0].is_bool()) {
                        last_err_ = "not bool";
                        return res;
                }
                return arry_[0].bool_value();
        }
        json11::Json GetJson()
        {
                last_err_.clear();
                return arry_[0];
        }

        bool Valid() const
        {
                return valid_;
        }
        bool HasMember(const std::string &member) const
        {
                return (!arry_[0][member].is_null());
        }
        std::string LastErr() const
        {
                std::string err = last_err_;
                last_err_.clear();
                return err;
        }

        const JsonHelp operator[](size_t i) const
        {
                std::shared_ptr<JsonHelp> jh;
                JsonHelp *t = new JsonHelp(arry_[i]);
                jh = std::shared_ptr<JsonHelp>(t);
                return *jh;
        }
        const JsonHelp operator[](const std::string &key) const
        {
                std::shared_ptr<JsonHelp> jh;
                if (arry_[0][key].is_array()) {
                        json11::Json::array ary = arry_[0][key].array_items();
                        JsonHelp *t = new JsonHelp(ary);
                        jh = std::shared_ptr<JsonHelp>(t);
                } else {
                        json11::Json js = arry_[0][key];
                        JsonHelp *t = new JsonHelp(js);
                        jh = std::shared_ptr<JsonHelp>(t);
                }
                return *jh;
        }

private:
        mutable std::string last_err_;
        bool valid_ = false;
        json11::Json::array arry_;
};

#endif // _JSON_HELP_HPP_
