#ifndef PACKETS_H
#define PACKETS_H

#include <cstdint>

enum class SensorType : uint8_t {
    NOOP = 0,
    BUTTON = 1,
    TEMPERATURE = 2,
    CO2 = 3,
    HUMIDITY = 4,
    PRESSURE = 5,
    LIGHT = 6,
    MOTION = 7,
};

enum class PacketType : uint8_t {
    DATA = 0,
    HEARTBEAT = 1,
    DASHBOARD_POST = 2,
    DASHBOARD_GET = 3,
    DASHBOARD_RESPONSE = 4
};

struct sensor_header {
    uint8_t length;
    PacketType ptype;
} __attribute__((packed));

struct sensor_metadata {
    SensorType sensor_type;
    uint8_t sensor_id;
} __attribute__((packed));

// Specific packet structures (ensure alignment/packing matches expected format)

struct sensor_heartbeat {
    struct sensor_metadata metadata;
} __attribute__((packed));

struct sensor_packet_generic {
    struct sensor_metadata metadata;
    // bool value;
} __attribute__((packed));

struct sensor_packet_temperature {
    struct sensor_metadata metadata;
    float value;
} __attribute__((packed));

struct sensor_packet_co2 {
    struct sensor_metadata metadata;
    uint16_t value;
} __attribute__((packed));

struct sensor_packet_humidity {
    struct sensor_metadata metadata;
    float value;
} __attribute__((packed));

struct sensor_packet_light {
    struct sensor_metadata metadata;
    uint8_t target_state;
} __attribute__((packed));

struct sensor_packet_rgb_light {
    struct sensor_metadata metadata;
    uint8_t red_state;
    uint8_t green_state;
    uint8_t blue_state;
} __attribute__((packed));

// --- End Structures ---

struct sensor_packet {
    struct sensor_header header;

    union sensor_data {
        struct sensor_heartbeat heartbeat;
        struct sensor_packet_generic generic;
        struct sensor_packet_temperature temperature;
        struct sensor_packet_co2 co2;
        struct sensor_packet_humidity humidity;
        struct sensor_packet_light light;
        struct sensor_packet_rgb_light rgb_light;
    } data;
} __attribute__((packed));

#endif  // PACKETS_H
