// I2C 프로토콜을 활용하기 위해 Wire 라이브러리를 불러온다.
// Wire 라이브러리는 아두이노 내장 라이브러리이며, 
// ROS2에서 차후 활용시 관련 의존성 라이브러리 다운로드 또는 자체 I2C 라이브러리를 활용해야 될 것이다.
// 구조는 현재 코드와 비슷하게 구성하면 될 것으로 보인다.
// Serial 통신과 I2C의 큰 차이점으로는
// Serial 통신은 비동기, 그리고 1:1 통신이 대표적이다.(UART)
// I2C 프로토콜은 동기처리, 그리고 N:N 통신이 가능하다.(SDA, SCL)
// 따라서 본 코드에서는 아두이노(Master) | MPU-6050(Slave)로 정리 될 수 있다. 
// Wire 함수에 대해 추가적인 참고는 아래 링크를 통해 확인 할 수 있다.
// https://www.arduino.cc/reference/en/language/functions/communication/wire/
#include <Wire.h>

// 대부분의 MCU 및 모듈은 사용 전 초기화(Init) 과정을 거쳐야됨.
void setup()
{
    // [컴퓨터]와 [아두이노]간 Serial 통신을 하기 위해 설정.(Baudrate:115200 Recommended)
    // 단순히 이는 Serial Monitor를 통해 사용자가 실시간 데이터를 확인하기 위해 설정한 것.
    Serial.begin(115200);

    // 여기서 Wire 라이브러리를 초기화한다.
    // begin 함수를 통해 I2C 버스를 컨트롤러로 결합.
    // 즉, 위에서 설명되었듯 [Arduino]를 Controller로
    // 연결되는 [MPU-6050]을 Peripheral로 설정하는 것이다.
    // 관련 Doc을 살펴보면 Wire.begin(address); 이다.
    // Parameter가 없을 경우 MASTER로 지정,
    // 있을 경우에는 7비트로 SLAVE 주소를 입력한다.(그렇게 되면 [Arduino]가 주변장치가 된다.)
    Wire.begin();

    // 위에서 해당 장치에 대해 I2C 의 역할 중 Master로 설정되었다면,
    // 주변 장치로 전송을 시작하여야한다.(시작전송)
    // 그렇다면 주소가 필요하다. Wire.beginTransmission(address)
    // 전송할 장치의 7비트 주소를 넣으면 된다.
    // 여기에서는 0x68 로서 MPU-6050의 주소이다.(관련 Datasheet 확인)
    Wire.beginTransmission(0x68);

    // 위 beginTransmission과 endTransmission 함수 사이에 위치하는 함수.
    // Byte를 대기열에 넣는다.
    // Parameter는 Value(decimal, float 등), String, Data, length 이다.
    // 아래는 107 이라는 Integer Decimal을 매개변수로 삽입되었다.
    // MPU-6050 Datasheet 상의 Addr(Dec.) 107은 Hexa 값으로는 6B이며,
    // Register Name이 PWR_MGMT_1이다.
    // 여기에 다음의 '0'을 보냄으로서, 모듈을 활성화시키는 것이다.
    Wire.write(107);
    Wire.write(0);

    // 위 과정까지는 write함수를 통해 Parameter로 입력된 Byte를 대기열에 위치하는 것이고,
    // 해당 endTransmission을 통해 대기열의 바이트를 Transmission 하는 것이다.
    // 해당 함수는 특정 I2C 장치와의 호환성을 위해 동작 변경을 위한 Bool 인수를 허용한다.
    // 따라서 true인 경우 전송 후 중지 메시지를 보내고 I2C 버스를 해제한다.(1)
    // False 인 경우 endTransmission() 전송 후 다시 시작 메시지를 보낸다.(0)
    // 이는 다른 컨트롤러 장치가 메시지 간 전송하는 것을 방지한다.
    // 아래 '0'은 성공.
    Wire.endTransmission(0);
}

// 늘 그렇듯 MCU는 초기화 작업을 마친 뒤, 일정 프로세스를 계속적으로 반복한다.
void loop() {
  
  // unsigned 8비트 정수. 즉, 0에서 255까지 값을 저장할 수 있다.(+)
  // 1. RGB 색상 값, 문자값(ASCII), 작은 카운터에서 필요
  // 2. 메모리 효율성으로 작은 단위로 사용 uint8_t는 1Byte만 사용하기 때문.
  // 3. 특정 프로토콜의 데이터 필드로 데이터 포맷이 보통 '8bit'크기 요구하기 때문에 충족 가능
  uint8_t i;
  
  // signed 16비트 정수. 즉, -32768 에서 32767까지의 값을 저장할 수 있다.(-,+)
  // MPU-6050을 통한 가속도계 값 자이로값을 RAW_DATA로 Array에 변수 지정한다.
  /* MPU-6050 Accelerometer Registers
    * Addr (Hex) | Addr (Dec) | Register Name   | Serial I/F | Bit7 to Bit0
    * ---------------------------------------------------------------------
    * 0x3B       | 59         | ACCEL_XOUT_H    | Read       | ACCEL_XOUT[15:8]
    * 0x3C       | 60         | ACCEL_XOUT_L    | Read       | ACCEL_XOUT[7:0]
    * 0x3D       | 61         | ACCEL_YOUT_H    | Read       | ACCEL_YOUT[15:8]
    * 0x3E       | 62         | ACCEL_YOUT_L    | Read       | ACCEL_YOUT[7:0]
    * 0x3F       | 63         | ACCEL_ZOUT_H    | Read       | ACCEL_ZOUT[15:8]
    * 0x40       | 64         | ACCEL_ZOUT_L    | Read       | ACCEL_ZOUT[7:0]
    *
    * MPU-6050 Gyroscope Registers
    * Addr (Hex) | Addr (Dec) | Register Name | Serial I/F | Bit7 to Bit0
    * -------------------------------------------------------------------
    * 0x43       | 67         | GYRO_XOUT_H   | Read       | GYRO_XOUT[15:8]
    * 0x44       | 68         | GYRO_XOUT_L   | Read       | GYRO_XOUT[7:0]
    * 0x45       | 69         | GYRO_YOUT_H   | Read       | GYRO_YOUT[15:8]
    * 0x46       | 70         | GYRO_YOUT_L   | Read       | GYRO_YOUT[7:0]
    * 0x47       | 71         | GYRO_ZOUT_H   | Read       | GYRO_ZOUT[15:8]
    * 0x48       | 72         | GYRO_ZOUT_L   | Read       | GYRO_ZOUT[7:0]
  */
  int16_t acc_raw[3], gyro_raw[3];

  // Get Accel
  Wire.beginTransmission(0x68);
  Wire.write(59);
  Wire.endTransmission();
  Wire.requestFrom(0x68, 6);

  // for(i = 0; i < 3; i++)
  // 위 반복문은 총 [0, 1, 2] 3회 실행된다.
  // acc_raw[i] = (Wire.read << 8) | Wire.read()
  // 위 부분은 가속도계 데이터를 읽고 처리하는 핵심 코드이다.
  // Wire.read(): I2C 버스를 통해 데이터를 읽는다. 호출될 때마다 1Byte(8bit) 데이터를 반환한다.
  // (Wire.read() << 8): read 함수 호출로 읽은 데이터를 8비트 왼쪽으로 이동시킨다.
  // 00000001 > 0000000100000000 : 이렇게 되면 high byte가 생성됨.
  // | Wire.read 두번째로 read 호출로 읽은 데이터를 첫번재 데이터와 비트 단위 OR 연산을 한다.
  // 그리하여 이 연산으로 두 바이트를 하나의 16비트 정수로 결합한다.
  // 결과적으로 acc_raw[i]에는 16비트의 가속도계 데이터가 저장된다.
  // 이는 첫번째 Wire.read()로 얻은 상위 8비트와 두 번째 Wire.read()로 얻은 하위 8비트가 16비트로 결합된 것이다.
  // 이 코드는 가속도 계 센서에서 X,Y,Z 축에 대한 16비트 데이터를 순서대로 읽어,
  // 배열에 저장하는 과정을 간결하게 구현한다.
  // 각 축의 데이터는 2바이트(16bit)로 구성되어 있으며, 이 코드를 통해 센서에게 전송된 원시(RAW) 데이터를
  // 정확하게 처리할 수 있다.
  for(i = 0; i < 3; i++) acc_raw[i] = (Wire.read() << 8) | Wire.read();
  // Get Gyro
  Wire.beginTransmission(0x68);
  Wire.write(67);
  Wire.endTransmission();
  Wire.requestFrom(0x68, 6);
  for(i = 0; i < 3; i++) gyro_raw[i] = (Wire.read() << 8) | Wire.read();


  // Serial print
  char str[100] = "";
  // printf 함수를 통해 처리된 가속도계와 자이로스코프 값을 출력한다.
  // sprintf() 함수는 C 프로그래밍 언어의 StdIO 일부로,
  // extern int sprintf(char *__s, const char *__fmt, ...);
  // 사용자가 지정한 형식의 __fmt에 따라 데이터를 문자열 __s 로 변환하고 저장하는 기능을 제공한다.
  // printf() 함수와 유사하지만, 출력 결과를 표준 출력 대신 사용자가 지정한 문자열 버퍼에 저장한다느 점이 다르다.
  // sprintf()는 버퍼 오버플로우를 자체적으로 방지하지 않아 주의해야 한다.

  sprintf(str, "Acc X:%d Y:%d Z%d / Gyro X:%d Y:%d Z%d",
  acc_raw[0], acc_raw[1], acc_raw[2],
  gyro_raw[0], gyro_raw[1], gyro_raw[2]);
  Serial.println(str);

  // 해당 프로세스를 반복한다.
}