#pragma once

#include <stdint.h>
// #include <signal.h> // alarm
// #include <stdio.h> // printf
// #include <stdlib.h>    // rand
// #include <sys/types.h> // pid (type int)
// #include <time.h> // clock_gettime
// #include <unistd.h>    // sleep


constexpr uint16_t LOCAL_IMU_PORT = 9998;
constexpr uint16_t LOCAL_LIDAR_PORT = 9999;
// constexpr uint16_t MEMORY_SET_PORT = 9100;
// constexpr uint16_t MEMORY_GET_PORT = 9200;

// using command_t               = uint16_t;

// constexpr command_t SET_IMU   = 100;
// constexpr command_t SET_POSE  = 101;
// constexpr command_t SET_GPS   = 102;
// constexpr command_t SET_LIDAR = 103;

// constexpr command_t GET_IMU   = 200;
// constexpr command_t GET_POSE  = 201;
// constexpr command_t GET_GPS   = 202;


#define FRAME_BODY 1
#define FRAME_IMU 10
#define FRAME_CAMERA 20

// uint64_t timestamp_us() {
//   struct timespec ts;
    
//   // defined in time.h
//   // CLOCK_REALTIME  = wall-clock time (Unix timestamp)
//   // CLOCK_MONOTONIC = time since some unspecified point (not affected by clock changes)
//   // CLOCK_UPTIME_RAW = ??
//   if (clock_gettime(CLOCK_REALTIME, &ts) == -1) {
//     printf("clock_gettime\n");
//     return 0;
//   }
  
//   // return (uint64_t)ts.tv_sec * 1000000LL + (uint64_t)(ts.tv_nsec / 1000);
//   return (uint64_t)ts.tv_nsec;
// }

struct __attribute__((packed)) header_t {
  uint16_t frame;
  uint64_t timestamp_us;
};

struct __attribute__((packed)) vec_t {
  float x, y, z;
};

struct __attribute__((packed)) quat_t {
  float w, x, y, z;
};

// struct __attribute__((packed)) pose_t {
//   vec_t position;
//   vec_t velocity;
//   quat_t orientation;
// };

// struct __attribute__((packed)) lidar_t {
//   point_t points[100];
//   uint64_t timestamp_us;
// };

struct __attribute__((packed)) full_imu_t {
  header_t header;
  vec_t a, g, m;
  quat_t q;
  float temperature;
  float pressure;
};

// enum IPC_STATE: uint8_t {
//   IPC_MAGIC1,
//   IPC_MAGIC2,
//   IPC_ID1,
//   IPC_ID2,
//   IPC_SIZE1,
//   IPC_SIZE2,
//   IPC_CRC,
//   IPC_PAYLOAD
// };

// static
// uint8_t calc_crc(const uint8_t* buffer, const size_t msg_size) {
//     // XOR all bytes in struct, but NOT ipc_header_t
//     uint64_t cs = 0;
//     for (size_t i = 0; i < msg_size; ++i) cs ^= buffer[i];
//     return (uint8_t)(cs & 0x000000FF);
// }

// template<typename T>
// uint8_t calc_crc(T *msg) {
//   uint8_t *p = reinterpret_cast<uint8_t*>(msg);
//   uint64_t cs = 0;
//   size_t size = sizeof(T);
//   for (size_t i = 0; i < size; ++i) cs ^= p[i];
//   uint8_t crc = (uint8_t)(cs & 0x000000FF);
//   return crc;
// }

// static
// uint16_t get_id(ipc::message_t& buffer) {
//   return uint16_t(buffer[2] + (buffer[3] << 8));
// }

// static
// ipc::message_t get_msg(uint8_t* buffer, size_t buffer_size) {
//   uint8_t state = IPC_MAGIC1;
//   uint16_t size = 0;
//   uint8_t crc = 0;
//   size_t length = buffer_size;
//   uint16_t id{0};

//   for (size_t i=0; i<length; ++i) {
//     uint8_t b = buffer[i];
//     switch(state) {
//       case IPC_MAGIC1:
//         if (b == 0xff) state = IPC_MAGIC2;
//         // printf("state: %d   index: %d  byte: %d\n", (int)state, (int)i,
//         (int)b); break;
//       case IPC_MAGIC2:
//         if (b == 0xff) state = IPC_ID1;
//         else state = IPC_MAGIC1;
//         break;
//       case IPC_ID1:
//         id = b;
//         state = IPC_ID2;
//         break;
//       case IPC_ID2:
//         id += (b << 8);
//         // printf("id: %d\n", (int)id);
//         state = IPC_SIZE1;
//         break;
//       case IPC_SIZE1:
//         size = b;
//         state = IPC_SIZE2;
//         break;
//       case IPC_SIZE2:
//         size += (b << 8);
//         // printf("size: %d\n", (int)size);
//         state = IPC_CRC;
//         break;
//       case IPC_CRC:
//         crc = b;
//         // printf("crc: %d\n", (int)crc);
//         state = IPC_PAYLOAD;
//         break;
//       case IPC_PAYLOAD:
//         uint8_t crc2 = calc_crc(&buffer[i], size-IPC_HEADER_SIZE);
//         // printf("crc2: %d\n", (int)crc2);
//         if (crc == crc2) {
//           ipc::message_t ret(size);
//           memcpy(ret.data(), &buffer[i-IPC_HEADER_SIZE], size);

//           // pop off the bytes that have been checked and resize
//           // buffer IF there are still bytes to check
//           // size_t new_size = buffer.size() - (i-IPC_HEADER_SIZE+size);
//           // if (new_size > 0) {
//           //   uint8_t *tmp = new uint8_t[new_size];
//           //   memcpy(tmp, &buffer[i-IPC_HEADER_SIZE+size], new_size);
//           //   buffer.resize(new_size);
//           //   memcpy(buffer.data(), tmp, new_size);
//           //   delete[] tmp;
//           // }

//           return std::move(ret);
//         }
//         else state = IPC_MAGIC1; // crap, start over
//         break;
//     }
//   }

//   ipc::message_t ret;
//   return std::move(ret);
// }

// static
// ipc::message_t get_msg(ipc::message_t& buffer) {
//   uint8_t state = IPC_MAGIC1;
//   uint16_t size = 0;
//   uint8_t crc = 0;
//   size_t length = buffer.size();
//   uint16_t id{0};

//   for (size_t i=0; i<length; ++i) {
//     uint8_t b = buffer[i];
//     switch(state) {
//       case IPC_MAGIC1:
//         if (b == 0xff) state = IPC_MAGIC2;
//         // printf("state: %d   index: %d  byte: %d\n", (int)state, (int)i,
//         (int)b); break;
//       case IPC_MAGIC2:
//         if (b == 0xff) state = IPC_ID1;
//         else state = IPC_MAGIC1;
//         break;
//       case IPC_ID1:
//         id = b;
//         state = IPC_ID2;
//         break;
//       case IPC_ID2:
//         id += (b << 8);
//         // printf("id: %d\n", (int)id);
//         state = IPC_SIZE1;
//         break;
//       case IPC_SIZE1:
//         size = b;
//         state = IPC_SIZE2;
//         break;
//       case IPC_SIZE2:
//         size += (b << 8);
//         // printf("size: %d\n", (int)size);
//         state = IPC_CRC;
//         break;
//       case IPC_CRC:
//         crc = b;
//         // printf("crc: %d\n", (int)crc);
//         state = IPC_PAYLOAD;
//         break;
//       case IPC_PAYLOAD:
//         uint8_t crc2 = calc_crc(&buffer[i], size-IPC_HEADER_SIZE);
//         // printf("crc2: %d\n", (int)crc2);
//         if (crc == crc2) {
//           ipc::message_t ret(size);
//           memcpy(ret.data(), &buffer[i-IPC_HEADER_SIZE], size);

//           // pop off the bytes that have been checked and resize
//           // buffer IF there are still bytes to check
//           // size_t new_size = buffer.size() - (i-IPC_HEADER_SIZE+size);
//           // if (new_size > 0) {
//           //   uint8_t *tmp = new uint8_t[new_size];
//           //   memcpy(tmp, &buffer[i-IPC_HEADER_SIZE+size], new_size);
//           //   buffer.resize(new_size);
//           //   memcpy(buffer.data(), tmp, new_size);
//           //   delete[] tmp;
//           // }

//           return std::move(ret);
//         }
//         else state = IPC_MAGIC1; // crap, start over
//         break;
//     }
//   }
//   // no messages in buffer, clear
//   buffer.clear();

//   ipc::message_t ret;
//   return std::move(ret);
// }

// /*
// Given a message struct, this packs the message and a
// ipc_header_t into a message_t container to be sent via
// ipc.
// */
// template<typename T>
// ipc::message_t ipc_pack(const uint8_t msgid, T& m) {
//   uint16_t size = IPC_HEADER_SIZE + sizeof(T);

//   ipc_header_t hdr{.id=msgid,.size=size};
//   hdr.crc = calc_crc<T>(&m);

//   ipc::message_t msg(size);

//   memcpy(msg.data(), &hdr, IPC_HEADER_SIZE);
//   memcpy(msg.data()+7, &m, sizeof(T));

//   return std::move(msg);
// }

// /*
// Given a message_t that contains the ipc_header_t and another
// struct, only the message struct is returned. The
// ipc_header_t is ignored.
// */
// template<typename T>
// T ipc_unpack(const ipc::message_t& m) {
//   T ret;
//   memcpy(&ret, m.data() + IPC_HEADER_SIZE, sizeof(T));
//   return std::move(ret);
// }
