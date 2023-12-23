// Дана функция ParseCitySubjson, обрабатывающая JSON-объект со списком городов конкретной страны:
#include<string>
#include<vector>
//создадим структуру так как все входные данные константные сслыки дабы не копировать строки,что затратно просто организуем их в структуру
struct CityParams{
         const std::string& country_name_;
         const std::string& country_iso_code_;
         const std::string& country_phone_code_;
         const std::string& country_time_zone_;
         const std::vector<Language>& languages_;
    };


void ParseCitySubjson(vector<City>& cities, const Json& json, CityParams params) {
    for (const auto& city_json : json.AsList()) {
        const auto& city_obj = city_json.AsObject();
        cities.push_back({ city_obj["name"s].AsString(), city_obj["iso_code"s].AsString(),
                          params.country_phone_code + city_obj["phone_code"s].AsString(), params.country_name, params.country_iso_code,
                          params.country_time_zone, params.languages });
    }
}

// ParseCitySubjson вызывается только из функции ParseCountryJson следующим образом:
void ParseCountryJson(vector<Country>& countries, vector<City>& cities, const Json& json) {
    for (const auto& country_json : json.AsList()) {
        const auto& country_obj = country_json.AsObject();
        countries.push_back({
            country_obj["name"s].AsString(),
            country_obj["iso_code"s].AsString(),
            country_obj["phone_code"s].AsString(),
            country_obj["time_zone"s].AsString(),
            });
        Country& country = countries.back();
        for (const auto& lang_obj : country_obj["languages"s].AsList()) {
            country.languages.push_back(FromString<Language>(lang_obj.AsString()));
        }
        CityParams params{
            country.name,
            country.iso_code, 
            country.phone_code,
            country.time_zone, 
            country.languages
        }
        ParseCitySubjson(cities, country_obj["cities"s],params);
    }
}