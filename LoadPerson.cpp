#include <string_view>
#include <string>
#include <vector>
#include <sstream>
using namespace std;
enum class DBLogLevel {LOW,HIGH,URGENT}; //возможный пример
struct Person {};

struct PersonDetails {
    int min_age = 0;
    int max_age = 100;
    std::string_view name_filter;
};
struct DBSettings{
    std::string_view db_name;
    int db_connection_timeout = 30;
    bool db_allow_exceptions = false;//ставим false и убираем одну из проверок внутри метода,всё равно дальше рабоать не будет если оно false  
    DBLogLevel db_log_level = DBLogLevel::LOW; //типовой параметр логгеров
};

std::vector<Person> LoadPersons(DBSettings dbsettings, PersonDetails persondetails) {
    DBConnector connector(dbsettings.db_allow_exceptions, dbsettings.db_log_level);
    DBHandler db;
    if (db_name.starts_with("tmp."s)) {
        db = connector.ConnectTmp(dbsettings.db_name, dbsettings.db_connection_timeout);
    }
    else {
        db = connector.Connect(dbsettings.db_name, dbsettings.db_connection_timeout);
    }

    if (!dbsettings.db_allow_exceptions && !db.IsOK()) {
        return {};
    }

    std::ostringstream query_str;
    query_str << "from Persons "s
        << "select Name, Age "s
        << "where Age between "s << persondetails.min_age << " and "s << persondetails.max_age << " "s
        << "and Name like '%"s << db.Quote(persondetails.name_filter) << "%'"s;
    DBQuery query(query_str.str());

    std::vector<Person> persons;
    for (auto [name, age] : db.LoadRows<string, int>(query)) {
        persons.push_back({ move(name),age });
    }
    return persons;
}

