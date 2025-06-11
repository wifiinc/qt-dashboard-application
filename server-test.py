import socket
import struct

HOST = '0.0.0.0'
PORT = 6969 

# Enums from packets.h
class PacketType:
    DATA = 0
    HEARTBEAT = 1
    DASHBOARD_POST = 2
    DASHBOARD_GET = 3
    DASHBOARD_RESPONSE = 4

class SensorType:
    NOOP = 0
    BUTTON = 1
    TEMPERATURE = 2
    CO2 = 3
    HUMIDITY = 4
    PRESSURE = 5
    LIGHT = 6
    MOTION = 7
    RGB_LIGHT = 8

# Struct formats
HEADER_FMT = '<BB'  # length (uint8), ptype (uint8)
METADATA_FMT = '<BB'  # sensor_type (uint8), sensor_id (uint8)
TEMP_FMT = METADATA_FMT + 'f'  # temp/float
CO2_FMT = METADATA_FMT + 'H'   # co2/uint16
HUMIDITY_FMT = METADATA_FMT + 'f'  # humidity/float
LIGHT_FMT = METADATA_FMT + 'B'  # light/uint8
RGB_FMT = METADATA_FMT + 'BBB'  # rgb/uint8, uint8, uint8
GENERIC_FMT = METADATA_FMT

# Dummy state for lampjes/rgb
lamp_state = {}
rgb_state = {}

def parse_packet(packet):
    header_len = struct.calcsize(HEADER_FMT)
    if len(packet) < header_len:
        return None

    length, ptype = struct.unpack_from(HEADER_FMT, packet)
    payload = packet[header_len:header_len+length]

    print(f"[#] Header: length={length}, ptype={ptype}")

    if ptype == PacketType.DASHBOARD_GET and len(payload) == struct.calcsize(GENERIC_FMT):
        sensor_type, sensor_id = struct.unpack(GENERIC_FMT, payload)
        print(f"[REQ] DASHBOARD_GET: sensor_type={sensor_type}, id={sensor_id}")
        return ptype, sensor_type, sensor_id, payload

    elif ptype == PacketType.DASHBOARD_POST:
        sensor_type, sensor_id = struct.unpack_from(METADATA_FMT, payload)
        if sensor_type == SensorType.LIGHT:
            target_state, = struct.unpack_from('B', payload, offset=2)
            lamp_state[sensor_id] = target_state
            print(f"[POST] LIGHT SET: id={sensor_id}, state={target_state}")
            return ptype, sensor_type, sensor_id, payload
        elif sensor_type == SensorType.RGB_LIGHT:
            r, g, b = struct.unpack_from('BBB', payload, offset=2)
            rgb_state[sensor_id] = (r, g, b)
            print(f"[POST] RGB_LIGHT SET: id={sensor_id}, RGB=({r}, {g}, {b})")
            return ptype, sensor_type, sensor_id, payload
        else:
            print(f"[POST] Unsupported sensor_type={sensor_type}")
            return ptype, sensor_type, sensor_id, payload
    else:
        print(f"[WARN] Unsupported packet type: {ptype}")
        return None

def create_dashboard_response(sensor_type, sensor_id):
    if sensor_type == SensorType.TEMPERATURE:
        value = 21.5  # dummy
        payload = struct.pack(TEMP_FMT, sensor_type, sensor_id, value)
        header = struct.pack(HEADER_FMT, len(payload), PacketType.DASHBOARD_RESPONSE)
        return header + payload
    elif sensor_type == SensorType.CO2:
        value = 420  # dummy ppm
        payload = struct.pack(CO2_FMT, sensor_type, sensor_id, value)
        header = struct.pack(HEADER_FMT, len(payload), PacketType.DASHBOARD_RESPONSE)
        return header + payload
    elif sensor_type == SensorType.HUMIDITY:
        value = 69.0  # dummy %
        payload = struct.pack(HUMIDITY_FMT, sensor_type, sensor_id, value)
        header = struct.pack(HEADER_FMT, len(payload), PacketType.DASHBOARD_RESPONSE)
        return header + payload
    elif sensor_type == SensorType.LIGHT:
        state = lamp_state.get(sensor_id, 0)
        payload = struct.pack(LIGHT_FMT, sensor_type, sensor_id, state)
        header = struct.pack(HEADER_FMT, len(payload), PacketType.DASHBOARD_RESPONSE)
        return header + payload
    elif sensor_type == SensorType.RGB_LIGHT:
        r, g, b = rgb_state.get(sensor_id, (0, 0, 0))
        payload = struct.pack(RGB_FMT, sensor_type, sensor_id, r, g, b)
        header = struct.pack(HEADER_FMT, len(payload), PacketType.DASHBOARD_RESPONSE)
        return header + payload
    else:
        # generic response
        payload = struct.pack(GENERIC_FMT, sensor_type, sensor_id)
        header = struct.pack(HEADER_FMT, len(payload), PacketType.DASHBOARD_RESPONSE)
        return header + payload

def handle_client(conn, addr):
    print(f"[+] Client connected from {addr}")
    buffer = b''

    while True:
        try:
            data = conn.recv(1024)
            if not data:
                print("[-] Client disconnected.")
                break

            print(f"[+] Received {len(data)} bytes: {data.hex(' ')}")
            buffer += data
            while len(buffer) >= 2:
                length, _ = struct.unpack_from(HEADER_FMT, buffer)
                full_len = 2 + length
                if len(buffer) < full_len:
                    print(f"[*] Waiting for more data. Need {full_len} bytes, have {len(buffer)}")
                    break

                packet = buffer[:full_len]
                buffer = buffer[full_len:]

                result = parse_packet(packet)
                if result:
                    ptype, sensor_type, sensor_id, payload = result
                    if ptype == PacketType.DASHBOARD_GET:
                        response = create_dashboard_response(sensor_type, sensor_id)
                        print(f"[+] Sending response: {response.hex(' ')}")
                        conn.sendall(response)
                        print(f"[RES] Sent DASHBOARD_RESPONSE for type={sensor_type}, id={sensor_id}")
                    elif ptype == PacketType.DASHBOARD_POST:
                        # Optionally: send back new state as confirmation
                        if sensor_type in (SensorType.LIGHT, SensorType.RGB_LIGHT):
                            response = create_dashboard_response(sensor_type, sensor_id)
                            print(f"[+] Sending response: {response.hex(' ')}")
                            conn.sendall(response)
                            print(f"[RES] Sent DASHBOARD_RESPONSE (confirm) for type={sensor_type}, id={sensor_id}")
        except Exception as e:
            print(f"[!] Error: {e}")
            break

    conn.close()

def start_server():
    print(f"[*] Starting server on {HOST}:{PORT}")
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as server:
        server.bind((HOST, PORT))
        server.listen(1)
        print("[*] Waiting for a client...")
        conn, addr = server.accept()
        handle_client(conn, addr)

if __name__ == '__main__':
    start_server()

