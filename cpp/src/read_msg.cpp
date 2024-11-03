#include <stdio.h>
#include <unistd.h>     // sleep, getpid()
#include <sys/types.h>  // pid (type int)

#include "ipc.hpp"
#include "signals.hpp"
#include "message.hpp"

#include "common.h"



constexpr uint16_t IPC_MSG_TEST = 5;

// struct ipc_header_t {
//   uint8_t magic[2]{0xff,0xff}; 2
//   uint16_t id{0};              2
//   uint16_t size{0};            2
//   uint8_t crc{0};              1
// };
struct __attribute__((packed)) test_t {
  int i;             // 4
  int d;             // 4
  uint8_t img[5]{0}; // 5 = 13
};



int main() {
  ipc::message_t buffer(256);
  // ipc::message_t buffer(UDP_BUFFER_SIZE);

  test_t t;
  t.i = 3456;
  t.d = -3345;
  memset(t.img, 25, 5);

  // printf("size: %d  crc: %d\n",(int)sizeof(t), (int)t.crc);
  ipc::message_t msg = ipc_pack<test_t>(IPC_MSG_TEST, t);

  memcpy(buffer.data()+6, msg.data(), msg.size());

  memset(t.img, 35, 5);
  msg = ipc_pack<test_t>(IPC_MSG_TEST, t);
  memcpy(buffer.data()+msg.size()+6+1, msg.data(), msg.size());

  for (int i=1; i<60;++i) {
    printf("%4d", (int)buffer[i-1]);
    if (i%10 == 0) printf("\n");
  }
  printf("\n");

  printf("buffer size: %d\n", (int)buffer.size());

  ipc::message_t m = get_msg(buffer);
  test_t tt;
  if (m.size() > 0) {
    // memcpy(&tt, m.data()+7, sizeof(test_t));
    tt = ipc_unpack<test_t>(m);
    printf("i: %d  d: %d\n", tt.i, tt.d);
    for (int i=0; i<5; ++i) printf("img[%d]: %d\n",i,(int)tt.img[i]);
  }
  printf("buffer size: %d\n", (int)buffer.size());


  for (int i=1; i<60;++i) {
    printf("%4d", (int)buffer[i-1]);
    if (i%10 == 0) printf("\n");
  }
  printf("\n");

  m = get_msg(buffer);
  if (m.size() > 0) {
    // memcpy(&tt, m.data(), sizeof(test_t));
    tt = ipc_unpack<test_t>(m);
    printf("i: %d  d: %d\n", tt.i, tt.d);
    for (int i=0; i<5; ++i) printf("img[%d]: %d\n",i,(int)tt.img[i]);
  }
  printf("buffer size: %d\n", (int)buffer.size());

  return 0;
}


// uint8_t* get_id(uint8_t* buffer, size_t length, uint16_t& id) {
//   uint8_t state = IPC_MAGIC1;
//   uint16_t size = 0;
//   uint8_t crc = 0;

//   for (size_t i=0; i<length; ++i) {
//     uint8_t b = buffer[i];
//     switch(state) {
//       case IPC_MAGIC1:
//         if (b == 0xff) state = IPC_MAGIC2;
//         printf("state: %d   index: %d\n", (int)state, (int)i);
//         break;
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
//         printf("id: %d\n", (int)id);
//         state = IPC_SIZE1;
//         break;
//       case IPC_SIZE1:
//         size = b;
//         state = IPC_SIZE2;
//         break;
//       case IPC_SIZE2:
//         size += (b << 8);
//         printf("size: %d\n", (int)size);
//         state = IPC_CRC;
//         break;
//       case IPC_CRC:
//         crc = b;
//         printf("crc: %d\n", (int)crc);
//         state = IPC_PAYLOAD;
//         break;
//       case IPC_PAYLOAD:
//         uint8_t crc2 = calc_crc(&buffer[i-7], size);
//         printf("crc2: %d\n", (int)crc2);
//         if (crc == crc2) return buffer+i-7; //return &buffer[i-7];
//         else state = IPC_MAGIC1; // crap, start over
//         break;
//     }
//   }
//   return 0;
// }