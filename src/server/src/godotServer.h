//
// Created by totod on 18/02/2026.
//

#ifndef GODOTPP_GODOTSERVER_H
#define GODOTPP_GODOTSERVER_H

#include <snl.h>
#include <unordered_map>
#include <entt/entt.hpp>
#include "../../SharedDefinition.h"

struct ObjectContext {
    uint32_t network_id;
    entt::entity local_entity;
};

struct ClientSession {
    uint32_t client_id;
    std::string address;
};


class godotServer {
public:
    char ip_server[256]{};

    uint32_t next_network_id = 100;
    GameSocket* socket = nullptr;
    entt::registry registry;
    std::unordered_map<uint32_t, ObjectContext> network_to_local_map;
    std::unordered_map<uint32_t, ClientSession> sessions;


    uint8_t buffer[1024]{};
    char sender[256]{};
    int bytes_received = 0;


public:
    godotServer(const char* ip);

    void start();

    void broadcast(const uint8_t* data, const size_t size) const;

    void create_server_entity(uint32_t net_id, uint32_t type_id);

    void handle_helo();
};


#endif //GODOTPP_GODOTSERVER_H