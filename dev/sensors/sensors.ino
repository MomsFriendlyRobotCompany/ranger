#include <cstdint>

// error messages
constexpr uint8_t MSG_NO_ERROR = 0;
constexpr uint8_t MSG_ERROR = 1;

// message types
constexpr uint8_t MSG_RAW_IMU     = 0x41;
constexpr uint8_t MSG_IMU         = 0x42;
constexpr uint8_t MSG_TWIST       = 0x43;
constexpr uint8_t MSG_WRENCH      = 0x44;
constexpr uint8_t MSG_POSE        = 0x45;
constexpr uint8_t MSG_ATMOSPHERIC = 0x46;


typedef struct {
    uint8_t type;   // 1
    uint8_t error = MSG_NO_ERROR;  // 1
    uint64_t timestamp = micros(); // 8

    // inline void now() { timestamp = micros(); }
} header_t;

typedef struct {
    float x,y,z; // 4*3 = 12
} vec_t;

typedef struct {
    float w,x,y,z; // 4*4 = 16
} quaternion_t;

struct imu_raw_t: header_t {
  // this->type = MSG_RAW_IMU;
    vec_t linear_acceration;  // 12
    vec_t angular_velocity;   // 12
    vec_t magnetic_field;     // 12
    float temperature;        // 4
};

template<class T>
T build(const int type,const int err=MSG_NO_ERROR) {
  T v;
  v.type = type;
  v.error = err;
  v.timestamp = micros();
  return v;
}


void setup() {
  Serial.begin(9600);
    delay(100);
}

void loop() {
  if (Serial.available() > 0) {
    if (Serial.read() == 'g') {
      // imu_raw_t c; 
      // imu_raw_t c{MSG_RAW_IMU};
      // imu_raw_t c{MSG_RAW_IMU,MSG_NO_ERROR,(long)micros(),{1,2,3},{4,5,6},{7,8,9},10};
      // imu_raw_t c{MSG_RAW_IMU,MSG_NO_ERROR,(long)micros(),.linear_acceration{1,2,3},.angular_velocity{4,5,6},.magnetic_field{7,8,9},10};
      imu_raw_t c = build<imu_raw_t>(MSG_RAW_IMU);
      // c.now();
      Serial.write(reinterpret_cast<char*>(&c), sizeof(c));
    }
  }
}
