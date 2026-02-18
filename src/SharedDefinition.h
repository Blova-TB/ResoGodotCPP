//
// Created by totod on 18/02/2026.
//

#ifndef GODOTPP_SHAREDDEFINITION_H
#define GODOTPP_SHAREDDEFINITION_H
#include <cstdint>

enum class MessageType : uint8_t {
    HELO = 0x01,
    HSK = 0x02,
    DATA = 0x03,
    PING = 0x04,
    PONG = 0x05,
    SPAWN = 0x06,
};

enum class EntityType : uint32_t {
    PLAYER = 0x01
};

// #pragma pack(push, 1)
//
// struct PacketHeader {
//     MessageType type;
// };
//
// struct PacketHelo {
//     PacketHeader header;
//     uint32_t client_id;
// };
//
// #pragma pack(pop)

#endif //GODOTPP_SHAREDDEFINITION_H