#include "AgentEnvironmentSrv.h"
#include <cxxopts.hpp>

int main(int argc, char **argv) {

    // set options
    cxxopts::Options options("SimMap gRPC server", "This server provides map data (OpenDRIVE) via gRPC.");
    options.add_options()
            ("n,name", "The host name of the server (default: 0.0.0.0)",
             cxxopts::value<std::string>()->default_value("0.0.0.0"))
            ("p,port", "The port to be used for the server (default: 50051)",
             cxxopts::value<std::string>()->default_value("50051"))
            ("f,file", "Map file name (and OpenDRIVE file), required", cxxopts::value<std::string>())
            ("h,help", "Show help");

    // parse result
    auto result = options.parse(argc, argv);

    // print help and end
    if (result.count("help")) {
        std::cout << options.help() << std::endl;
        exit(0);
    }

    // get file name
    std::string mapFile;
    if (result.count("file")) {

        // get map file
        mapFile = result["file"].as<std::string>();

    } else {

        // error and end
        std::cerr << "Error: Please provide file OpenDRIVE filename." << std::endl;
        return 1;

    }

    // concat server address
    auto server_address(result["name"].as<std::string>());
    server_address += ":";
    server_address += result["port"].as<std::string>();

    // create service (and load map)
    AgentEnvironmentSrv service(mapFile);

    // server builder
    ServerBuilder builder;
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    builder.RegisterService(&service);

    // create server
    std::unique_ptr<Server> server(builder.BuildAndStart());
    std::cout << "Server listening on " << server_address << std::endl;

    // start server
    server->Wait();

    return 0;

}