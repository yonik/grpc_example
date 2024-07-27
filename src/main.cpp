#include <iostream>
#include <thread>
#include <grpcpp/grpcpp.h>

// Include the generated protobuf header
#include "proto/example.grpc.pb.h"

// Server-side handler for greeter
class GreeterServiceImpl final : public example::Greeter::Service {
  grpc::Status SayHello(grpc::ServerContext* context, const example::HelloRequest* request, example::HelloReply* reply) override {
    reply->set_message(std::string("Heya ") + request->name() + "!");
    return grpc::Status::OK;
  }
};

// the server is global so we can shut it down from the main thread.
std::unique_ptr<grpc::Server> server;

void sync_server() {
  std::string server_address("0.0.0.0:50051");
  GreeterServiceImpl greeterService;

  grpc::EnableDefaultHealthCheckService(true);
  grpc::ServerBuilder builder;
  // Listen on the given address without any authentication mechanism.
  builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
  builder.RegisterService(&greeterService);

  server = builder.BuildAndStart();
  std::cout << "Server listening on " << server_address << std::endl;

  // Wait for the server to shutdown. Note that some other thread must be
  // responsible for shutting down the server for this call to ever return.
  server->Wait();
}


void sync_client() {
  // A channel provides a connection to a gRPC server on a specified host and port.
  std::shared_ptr<grpc::Channel> channel = grpc::CreateChannel("localhost:50051", grpc::InsecureChannelCredentials());;

  // a client stub using the channel that provides the interface to talk to the server.
  std::unique_ptr<example::Greeter::Stub> greeterStub = example::Greeter::NewStub(channel);

  grpc::ClientContext context;

  // fill in the request data
  example::HelloRequest request;
  request.set_name("Yonik");

  example::HelloReply reply;

  // call the server and get the response
  grpc::Status status = greeterStub->SayHello(&context, request, &reply);

  if (status.ok()) {
    // normal response
    std::cout << "Client got back response: " << reply.message() << std::endl;
  } else {
    std::cout << "Client call failed: " << status.error_message() << std::endl;
  }
}

int main() {
  // run the server in a different thread
  std::thread server_thread(sync_server);

  // sleep for a bit to make sure the server is up
  std::this_thread::sleep_for(std::chrono::milliseconds(1000));

  // create a client and make a call to the server
  sync_client();

  server->Shutdown();
  server_thread.join();
  server.reset();

  return 0;
}
