#include <cpprest/http_listener.h>
#include <cpprest/json.h>

using namespace web;
using namespace web::http;
using namespace web::http::experimental::listener;

#include <iostream>
#include <map>
#include <set>
#include <string>
#include <vector>
#include <utility>
#include <sstream>

#include "fparser.hh"

using namespace std;

#define TRACE(msg)            wcout << msg
#define TRACE_ACTION(a, k, v) wcout << a << L" (" << k << L", " << v << L")\n"

typedef std::wstring string_t;

struct REQUEST_PARAMS {
    string_t equation;
    string_t varsString;
    string_t vars;
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

void calc(REQUEST_PARAMS client_params) {

    double min = client_params.min;
    double max = client_params.max;
    double step = client_params.step;

    const string equation = utility::conversions::to_utf8string(client_params.equation);
    const string varsString = utility::conversions::to_utf8string(client_params.varsString);
    const string vars = utility::conversions::to_utf8string(client_params.vars);

    vector<double> vars_values;
    vector<double> y_values;
    vector<double> x_values;

    double x = min;


    // строку типа "12, 132, 98" в массив double {12, 132, 98}
    istringstream iss(vars);

    for (double s; iss >> s;) {
        vars_values.push_back(s);
    }

    int n = vars_values.size()+1;
    double* vals;
    vals = new double[n];

    for (int i = 0; i < n-1; i++)
    {
        vals[i] = vars_values[i];
    }

    FunctionParser fp;

    int ret = fp.Parse(equation, varsString);
    
    if (ret < 0) {
        while (x < max)
        {
            vals[n-1] = x;
            y_values.push_back(fp.Eval(vals));
            x_values.push_back(x);
            x += step;
        }
    }

    wcout << y_values[2] << endl;
    wcout << ret << endl;
}

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

            REQUEST_PARAMS pm;
            pm = pm.JSonToObject(jvalue.as_object());

            calc(pm);

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