#include <cpprest/http_listener.h>
#include <cpprest/json.h>

using namespace web;
using namespace web::http;
using namespace web::http::experimental::listener;

#include <iostream>
#include <map>
#include <set>
#include <string>
using namespace std;

#define TRACE(msg)            wcout << msg
#define TRACE_ACTION(a, k, v) wcout << a << L" (" << k << L", " << v << L")\n"


struct REQUEST_PARAMS {
    utility::string_t equation;
    utility::string_t varsString;
    utility::string_t vars;
    double min;
    double max;
    double step;
    
    /////////////////////////////////
    // Convert a JSON Object to a C++ Struct
    //
    static REQUEST_PARAMS JSonToObject(const web::json::object& object) {
        REQUEST_PARAMS result;
        result.equation = object.at(U("equation")).as_string();
        result.varsString = object.at(U("varsString")).as_string();
        result.vars = object.at(U("vars")).as_string();
        result.min = object.at(U("min")).as_double();
        result.max = object.at(U("max")).as_double();
        result.step = object.at(U("step")).as_double();
        return result;
    }

    /*web::json::value ObjectToJson() const {
        web::json::value result = web::json::value::object();
        result[U("equation")] = web::json::value::string(name);
        result[U("varsString")] = web::json::value::number(age);
        result[U("varsString")] = web::json::value::number(salary);
        return result;
    }*/
};

void display_json(
    json::value const& jvalue,
    utility::string_t const& prefix)
{
    wcout << prefix << jvalue.serialize() << endl;
}

void handle_request(
    http_request request,
    function<void(json::value const&, json::value&)> action)
{
    auto answer = json::value::object();

    request
        .extract_json()
        .then([&answer, &action](pplx::task<json::value> task) {
        try
        {
            auto const& jvalue = task.get();
            display_json(jvalue, L"R: ");
                       

            if (!jvalue.is_null())
            {
                action(jvalue, answer);
            }
        }
        catch (http_exception const& e)
        {
            wcout << e.what() << endl;
        }
            })
        .wait();


        display_json(answer, L"S: ");

        request.reply(status_codes::OK, answer);
}

void handle_post(http_request request)
{
    TRACE("\nhandle POST\n");

    handle_request(
        request,
        [](json::value const& jvalue, json::value& answer)
        {
            REQUEST_PARAMS pm;
            pm.JSonToObject(jvalue.as_object());
        });
}


int main()
{
    http_listener listener(L"http://localhost:3000");

    listener.support(methods::POST, handle_post);

    try
    {
        listener
            .open()
            .then([&listener]() {TRACE(L"\nstarting to listen\n"); })
            .wait();

        while (true);
    }
    catch (exception const& e)
    {
        wcout << e.what() << endl;
    }

    return 0;
}