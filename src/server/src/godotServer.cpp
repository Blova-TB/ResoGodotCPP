//
// Created by totod on 18/02/2026.
//

#include "godotServer.h"

#include <cstring>
#include <iostream>

godotServer::godotServer(const char* ip) {
    std::cout << "(Serveur) Hello, World!" << std::endl;
    std::strncpy(ip_server, ip, sizeof(ip_server) - 1);
    socket = net_socket_create(ip_server);
    std::cout << "(Serveur) ready to start!" << std::endl;
}

void godotServer::start() {
    while (true) {
        bytes_received = net_socket_poll(socket, buffer, sizeof(buffer), sender, sizeof(sender));

        if (bytes_received > 0) {
            switch (static_cast<MessageType>(buffer[0])) {

                case MessageType::HELO:
                    std::cout << "(Serveur) Received HELO message" << std::endl;
                    handle_helo();
                    break;

                case MessageType::HSK:
                    std::cout << "(Serveur) Received HSK message" << std::endl;
                    break;

                case MessageType::DATA:
                    std::cout << "(Serveur) Received DATA message" << std::endl;
                    break;

                case MessageType::PING:
                    std::cout << "(Serveur) Received PING message" << std::endl;
                    break;

                case MessageType::PONG:
                    std::cout << "(Serveur) Received PONG message" << std::endl;
                    break;

                default:
                    std::cout << "(Serveur) Received unknown message type: " << static_cast<int>(buffer[0]) << std::endl;
                    break;
            }
        }
    }
}

void godotServer::broadcast(const uint8_t *data, const size_t size) const {
    for (const auto& client_addr : sessions) {
        net_socket_send(socket, client_addr.second.address.c_str(), data, size);
    }
}

void godotServer::create_server_entity(uint32_t net_id, uint32_t type_id) {
    entt::entity e = registry.create();

    // registry.emplace<Position>(e, 0.0f, 0.0f);

    network_to_local_map[net_id] = { net_id, e };

    std::cout << "(Serveur) Entity Created. NetID: " << net_id << " LocalID: " << (uint32_t)e << std::endl;
}

void godotServer::handle_helo() {

    for (const auto& session : sessions) {
        if (session.second.address == sender) {
            return;
        }
    }

    // on ajoute le client à la liste des sessions
    uint32_t client_id = sessions.size() + 1;
    sessions[client_id] = { client_id, sender };

    // HELO response
    uint8_t heloResp[5];
    heloResp[0] = {static_cast<uint8_t>(MessageType::HELO)};
    std::memcpy(&heloResp[1], &client_id, sizeof(client_id));
    net_socket_send(socket, sender, heloResp, sizeof(heloResp));

    // On crée un joueur.
    int32_t network_id = next_network_id ++;
    create_server_entity(network_id, static_cast<uint32_t>(EntityType::PLAYER));

    uint8_t repBuffer[9];
    repBuffer[0] = static_cast<uint8_t>(MessageType::SPAWN);
    std::memcpy(&repBuffer[1], &network_id, sizeof(network_id));
    uint32_t type_id = static_cast<uint32_t>(EntityType::PLAYER);
    std::memcpy(&repBuffer[5], &type_id, sizeof(type_id));

    broadcast(repBuffer, sizeof(repBuffer));
}

