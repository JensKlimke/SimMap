/*
 *
 * Copyright 2015 gRPC authors.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

//#include <chrono>
//#include <iostream>
//#include <memory>
//#include <random>
//#include <string>
//#include <thread>
//
//#include <grpc/grpc.h>
//#include <grpcpp/channel.h>
//#include <grpcpp/client_context.h>
//#include <grpcpp/create_channel.h>
//#include <grpcpp/security/credentials.h>
//#include <client/Models.grpc.pb.h>
//
//using grpc::Channel;
//using grpc::ClientContext;
//using grpc::ClientReader;
//using grpc::ClientReaderWriter;
//using grpc::ClientWriter;
//using grpc::Status;
//using simulation::models::VehicleDefinition;
//using simulation::models::VehicleInput;
//using simulation::models::VehicleState;
//
//
//class RemoteControllerClient {
//public:
//
//    RemoteControllerClient(std::shared_ptr<Channel> channel) : stub_(simulation::models::RemoteController::NewStub(channel)) {
//
//    }
//
//    bool Create(const simulation::models::VehicleDefinition& def, simulation::models::VehicleState* state) {
//
//        ClientContext context;
//        Status status = stub_->CreateUnit(&context, def, state);
//
//        if (!status.ok()) {
//            std::cout << "GetFeature rpc failed." << std::endl;
//            return false;
//        }
//
//        return true;
//    }
//
//    std::unique_ptr<simulation::models::RemoteController::Stub> stub_;
//
//};

int main(int argc, char **argv) {

//    // Expect only arg: --db_path=path/to/route_guide_db.json.
//    // std::string db = routeguide::GetDbFileContent(argc, argv);
//    RemoteControllerClient client(grpc::CreateChannel("localhost:50051", grpc::InsecureChannelCredentials()));
//
//    simulation::models::VehicleDefinition def;
//    simulation::models::VehicleState state;
//
//    def.set_id(1);
//    client.Create(def, &state);

    return 0;
}