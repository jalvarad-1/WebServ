#include "server/MultiServer.hpp"

int main()
{
    std::map<int, std::string> default_error_pages;
    default_error_pages[403] = "null";

    //Location a
    std::list<std::string> allowed_methods_a;
    allowed_methods_a.push_back("GET");

    LocationRules location_default("default",default_error_pages, 
                                "./YoupiBanane", -1, allowed_methods_a,
                                true, "index.html", "");

    LocationRules location_a( "/put_test",default_error_pages, 
                                "./YoupiBanane/put_here", -1, allowed_methods_a,
                                true, "", "");
    
    //Location b
    std::list<std::string> allowed_methods_b;
    allowed_methods_b.push_back("POST");
    LocationRules location_b("/post_body", default_error_pages, 
                                "", 100, allowed_methods_b,
                                true, "", "");

    // Location c
    LocationRules location_c("/directory", default_error_pages, 
                                "YoupiBanane/", -1, allowed_methods_a,
                                true, "youpi.bad_extension", "");
    
    //Location d
    std::list<std::string> allowed_methods_d;
    allowed_methods_d.push_back("POST");
    allowed_methods_d.push_back("GET");
    LocationRules location_d("/directory/pepe.bla", default_error_pages, 
                                "", -1, allowed_methods_d,
                                true, "", "test_mac/macos_cgi_tester");
    
    //Location e
    LocationRules location_e("/pepe.bla" ,default_error_pages, 
                                "", -1, allowed_methods_b,
                                true, "", "test_mac/macos_cgi_tester");

    std::list<std::string> serverNames;
    serverNames.push_back("youpi");

    std::map<std::string, LocationRules *> locations;
    locations["default"] = &location_default;
    locations["/put_test"] = &location_a;
    locations["/post_body"] = &location_b;
    locations["/directory"] = &location_c;
    locations["/directory/pepe.bla"] = &location_d;
    locations["/pepe.bla"] = &location_e;

    ServerConfig testServer(0, 8000, serverNames, locations);
    std::vector<ServerConfig> confServers;

    confServers.push_back(testServer);
    MultiServer multi(confServers);
    multi.run();
    return 0;
}