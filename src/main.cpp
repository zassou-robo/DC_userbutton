#include <mbed.h>

int16_t pwm1[4] = {0, 0, 0, 0};
DigitalIn user_button(BUTTON1, PullUp);

CAN J3(PA_11, PA_12, 1e6);
// CAN J4(PB_12, PB_13, 1e6);
CANMessage motoro_msg;
constexpr int CAN_ID = 1; 

int push_amount = 0;

int main(){
    while(true){
        auto now = HighResClock::now();
        static auto pre = now;
        if(push_amount == 0){
            pwm1[0] = 0;
            pwm1[1] = 0;
            pwm1[2] = 0;
            pwm1[3] = 0;
        }else if(push_amount == 1){
            pwm1[0] = 16000;
            pwm1[1] = -16000;
            pwm1[2] = 16000;
            pwm1[3] = -16000;
        }
        // }else if(push_amount == 2){
        //     pwm1[0] = -16000;
        //     pwm1[1] = 16000;
        //     pwm1[2] = -16000;
        //     pwm1[3] = 16000;
        // }
        static bool pre_user_button = 0;
        if(pre_user_button && user_button == 0){
            push_amount++;
            if(push_amount > 1){
                push_amount = 0;
            }
        }
        pre_user_button = user_button;
        if(now - pre > 10ms){
            CANMessage motor1(CAN_ID,(uint8_t *)pwm1, 8);
            J3.write(motor1);
            pre = now;
        }
    }
}