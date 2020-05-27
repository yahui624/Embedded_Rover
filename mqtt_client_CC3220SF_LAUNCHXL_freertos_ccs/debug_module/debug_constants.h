#ifndef DEBUG_CONSTANTS_H_
#define DEBUG_CONSTANTS_H_

#define BUFFER_MAX_LENGTH 25

// Defining DEBUG codes
#define MAX_DEBUG_LOCATIONS 127

// Test Thread Locations

#define TRUE  1
#define FALSE 0
#define LOW   0
#define HIGH  1
#define ZERO  0
#define JUNK  0
#define ONE   1
#define TWO   2

#define CLIENT_CALLBACK_CONNACK              1
#define CLIENT_CALLBACK_CONNECTION_SUCCESS   1
#define CLIENT_CALLBACK_CONNECTION_FAILURE   1
#define CLIENT_CALLBACK_RECEIVE_BEGIN        1
#define CLIENT_CALLBACK_BRIDGE_DISCONNECTION 1
#define CLIENT_IF_CONNECT_START_FAILURE      1
#define CLIENT_IF_CONNECT_AP_FAILURE         1
#define CLIENT_START_MSG_Q_CREATE_FAILURE    1
#define CLIENT_START_THREAD_CREATE_FAILURE   1
#define CLIENT_START_SUBSCRIBED              1
#define CLIENT_START_SUBSCRIPTION_FAILURE    1
#define CLIENT_CLIENT_STOP_UNSUBSCRIBE       1
#define CLIENT_SEND_MSG_QUEUE_FULL           1
#define CLIENT_PUBLISH_MSG                   1
#define CLIENT_PUBLISH_TOPIC                 1
#define CLIENT_PUBLISH_DATA                  1
#define CLIENT_MAIN_TASK_CREATE_FAILURE      1
#define CLIENT_MAIN_SL_START_FAILURE         1
#define CLIENT_MAIN_SL_START_FAILURE         1
#define CLIENT_MAIN_NO_DEVICE_INFO           1
#define CLIENT_MAIN_SL_STOP_FAILURE          1
#define CLIENT_PUBLISH_READQ_BEGIN           1
#define CLIENT_PUBLISH_READQ_END             1

#define MOTOR_THREAD_START               0
#define MOTOR_QUEUE_READ_FAILED          1
#define MOTOR_QUEUE_CREATE_FAILED        1
#define MOTOR_QUEUE_SEND_FAILED          1
#define MOTOR_THREAD_READ_QUEUE_BEGIN    1
#define MOTOR_THREAD_READ_QUEUE_DONE     1
#define MOTOR_THREAD_WHILE               1
#define MOTOR_UART_OPEN_FAILED           1
#define MOTOR_QUEUE_SEND_SUBSCRIBE_BEGIN 1
#define MOTOR_QUEUE_SEND_SUBSCRIBE_END   1

#define CLIENT_CLIENT_ONBOARD_DISCONNECT 1
#define CLIENT_QUEUE_FULL_SEND_NEW       1
#define CLIENT_CLIENT_COMPLETED          1

#define TIMER_CALLBACK_END            1
#define TIMER_CALLBACK_START          1
#define TIMER_OPEN_FAILED             1
#define TIMER_START_FAILED            1
#define TIMER_ENCODER_START_FAILED    1
#define TIMER_ENCODER_CALLBACK_START  1
#define TIMER_IR_CALLBACK_START       1
#define TIMER_MOVEMENT_CALLBACK_START 1
#define TIMER_ENCODER_CALLBACK_END    1
#define TIMER_IR_CALLBACK_END         1
#define TIMER_MOVEMENT_CALLBACK_END   1

#define SPI_CALLBACK_BEGIN 1
#define SPI_CALLBACK_END   1
#define SPI_OPEN_FAILED    1

#define GRIPPER_THREAD_START           1
#define GRIPPER_THREAD_WHILE_LOOP      1
#define GRIPPER_THREAD_READQ_BEGIN     1
#define GRIPPER_THREAD_READQ_END       1
#define GRIPPER_THREAD_PWM_OPEN_FAILED 1
#define GRIPPER_QUEUE_READ_FAILED      1
#define GRIPPER_QUEUE_CREATE_FAILED    1

#define PUBLISH_QUEUE_READ_FAILED   1
#define PUBLISH_QUEUE_CREATE_FAILED 1
#define PUBLISH_THREAD_START        1
#define PUBLISH_THREAD_WHILE_LOOP   1
#define PUBLISH_THREAD_READQ_BEGIN  1
#define PUBLISH_THREAD_READQ_END    1

#define STATISTICS_QUEUE_READ_FAILED   1
#define STATISTICS_QUEUE_CREATE_FAILED 1
#define STATISTICS_THREAD_START        1
#define STATISTICS_THREAD_WHILE_LOOP   1
#define STATISTICS_THREAD_READQ_BEGIN  1
#define STATISTICS_THREAD_READQ_END    1

#define MOVEMENT_QUEUE_READ_FAILED          1
#define MOVEMENT_QUEUE_CREATE_FAILED        1
#define MOVEMENT_THREAD_START               1
#define MOVEMENT_THREAD_WHILE_LOOP          1
#define MOVEMENT_THREAD_READQ_BEGIN         1
#define MOVEMENT_THREAD_READQ_END           1
#define MOVEMENT_THREAD_PUBLISH_SENDQ_BEGIN 1
#define MOVEMENT_THREAD_PUBLISH_SENDQ_END   1
#define MOVEMENT_THREAD_GRIPPER_SENDQ_BEGIN 1
#define MOVEMENT_THREAD_GRIPPER_SENDQ_END   1

#define MOVEMENT_STATE_UNKNOWN      1
#define TIMER_MOVEMENT_START_FAILED 0

#define JSON_PARSER_PARSE_FAIL        1
#define JSON_PARSER_OBJECT_EXPECTED   1
#define JSON_PARSER_PARSE_EMPTY_ARRAY 1
#define JSON_PARSER_PARSE_EXTRA_DATA  1

#define MQTT_CALLBACK_START 1

#define ENCODER_STATE_UNKNOWN     1
#define ENCODER_STATE_SENDQ_BEGIN 1
#define ENCODER_STATE_SENDQ_END   1

#define ENCODER_THREAD_START       1
#define ENCODER_THREAD_WHILE       1
#define ENCODER_THREAD_READQ_BEGIN 1
#define ENCODER_THREAD_READQ_END   1

#define ENCODER_CLEAR_CTR_FAIL      1
#define ENCODER_RESET_DTR_FAIL      1
#define ENCODER_SET_MODE_FAILED     1
#define ENCODER_READ_FAIL           1
#define ENCODER_QUEUE_READ_FAILED   1
#define ENCODER_QUEUE_CREATE_FAILED 1

#define PID_QUEUE_READ_FAILED   1
#define PID_QUEUE_CREATE_FAILED 1

#define PID_THREAD_START               1
#define PID_THREAD_PUBLISH_SENDQ_END   1
#define PID_THREAD_WHILE_LOOP          1
#define PID_THREAD_PUBLISH_SENDQ_BEGIN 1
#define PID_THREAD_PUBLISH_SENDQ_END   1
#define PID_THREAD_MOTOR_SENDQ_BEGIN   1
#define PID_THREAD_MOTOR_SENDQ_END     1
#define PID_THREAD_READQ_BEGIN         1
#define PID_THREAD_READQ_END           1

#define IR_THREAD_START                1
#define IR_THREAD_WHILE_LOOP           1
#define IR_THREAD_MOVEMENT_SENDQ_BEGIN 1
#define IR_THREAD_MOVEMENT_SENDQ_END   1
#define IR_THREAD_READQ_BEGIN          1
#define IR_THREAD_READQ_END            1
#define IR_THREAD_ADC_OPEN_FAILED      1

#define IR_QUEUE_READ_FAILED   1
#define IR_QUEUE_CREATE_FAILED 1

#define PATH_WIDTH  10
#define PATH_LENGTH 20
#define MOTORS      3
#define PATH_BUFFER 150
#define ENCODER1    0
#define ENCODER2    1
#define ENCODER3    2
#define MOTOR1      0
#define MOTOR2      1
#define MOTOR3      2
#define MAX_POWER   127
#define FORWARD     1
#define BACKWARD    0

typedef struct
{
    long    encoder_reading;
    int8_t  direction;
    long    desired_speed;
    long    output_speed;
    uint8_t output_power;
    uint8_t active;
} Motor_Debug;

typedef struct
{
    int         message_id;
    int         has_block;
    int         block_color;
    uint32_t    receive_expected;
    uint32_t    receive_actual;
    uint32_t    sequence;
    Motor_Debug motor1;
    Motor_Debug motor2;
    Motor_Debug motor3;
} Publish_Message;

typedef struct
{
    int      message_id;
    uint32_t sequence;
    uint8_t  board_id;
    int      timer_elapsed;
} Statistics_Message;

typedef struct
{
    int message_id;
    int grip_signal;
} Gripper_Message;

typedef struct
{
    int      message_id;
    uint16_t ir_distance;
    uint8_t  color;
    uint8_t  position;
    uint8_t  pathable;
} Movement_Message;

typedef struct
{
    uint8_t message_id;
    long    encoder_reading[MOTORS];
    long    desired_speed[MOTORS];
    uint8_t direction[MOTORS];
    uint8_t motor[MOTORS];
} PID_Message;

typedef struct
{
    int message_id;
    int timer_elapsed;
    int transfer_done;
} Encoder_Message;

typedef struct
{
    uint8_t power[MOTORS];
    uint8_t direction[MOTORS];
    uint8_t motor[MOTORS];
} Motor_Message;

#endif /* DEBUG_CONSTANTS_H_ */
