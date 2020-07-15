#include <algorithm>
#include <chrono>
#include <cmath>
#include <iostream>
#include <memory>
#include <string>

#include <grpc/grpc.h>
#include <grpcpp/server.h>
#include <grpcpp/server_builder.h>
#include <grpcpp/server_context.h>
#include <grpcpp/security/server_credentials.h>
#include <mapserver/simmap.grpc.pb.h>

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::ServerReader;
using grpc::ServerReaderWriter;
using grpc::ServerWriter;
using grpc::Status;
using simulation::models::VehicleDefinition;
using simulation::models::VehicleInput;
using simulation::models::VehicleState;
using std::chrono::system_clock;


class RemoteControllerImpl final : public simulation::models::RemoteController::Service {

public:

    explicit RemoteControllerImpl() {
        // simulation::models::Parse(db, &feature_list_);
    }

    Status CreateUnit(::grpc::ServerContext *context, const ::simulation::models::VehicleDefinition *request,
                      ::simulation::models::VehicleState *response) override {

        std::cout << "Create Unit" << std::endl;

        response->set_acceleration(0.0);
        response->set_velocity(0.0);
        response->set_distance(0.0);

        return Status::OK;

    }

    Status SendRequest(::grpc::ServerContext *context, const ::simulation::models::VehicleInput *request,
                       ::simulation::models::VehicleState *response) override {

        std::cout << "Send request" << std::endl;
        return Service::SendRequest(context, request, response);

    }


private:

    std::mutex mu_;

};

void RunServer() {

    std::string server_address("0.0.0.0:50051");
    RemoteControllerImpl service;

    ServerBuilder builder;
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    builder.RegisterService(&service);
    std::unique_ptr<Server> server(builder.BuildAndStart());
    std::cout << "Server listening on " << server_address << std::endl;

    server->Wait();

}

int main(int argc, char **argv) {

    // Expect only arg: --db_path=path/to/route_guide_db.json.
    // std::string db = routeguide::GetDbFileContent(argc, argv);
    RunServer();

    return 0;
}