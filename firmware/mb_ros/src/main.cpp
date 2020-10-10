#include "main.h"
#include "ROS/bot1_mb_core.h"

/******************************************************************************
******************************************************************************/
uint32_t t;
uint32_t prev_update_time = 0;

/*******************************************************************************
* ROS NodeHandle
*******************************************************************************/
ros::NodeHandle nh;
ros::Time current_time;
uint32_t current_offset;

/*******************************************************************************
* Subscriber
*******************************************************************************/
ros::Subscriber<utmrbc_msgs::LED> led_sub("led", LEDCallback);
ros::Subscriber<utmrbc_msgs::BDC> bdc_sub("bdc", BDCCallback);
ros::Subscriber<utmrbc_msgs::HSPM> hspm_sub("hspm", HSPMCallback);

/*******************************************************************************
* Publisher
*******************************************************************************/
char buffer[200];
std_msgs::String info_msg;
ros::Publisher info_pub("mb/info", &info_msg);

utmrbc_msgs::InputPin ip_msg;
ros::Publisher ip_pub("ip", &ip_msg);

/*******************************************************************************
* SoftwareTimer
*******************************************************************************/
static uint32_t tTime[10];

/*******************************************************************************
 * Main function
 *******************************************************************************/
int main(void)
{
	Initialize();
	ROS_Init();
	System_Start();

	while (1)
	{
		t = millis();
		if ((t - prev_update_time) >= 1000 / PUBLISH_FREQUENCY)
			ROS_Update();
		nh.spinOnce();

		System_Run();
	}
}

#ifdef __cplusplus
extern "C"
{
#endif

	void TIM6_DAC_IRQHandler(void)
	{
		if (TIM_GetITStatus(TIM6, TIM_IT_Update) % 200)
		{

			led1 = !led1;
			TIM_ClearITPendingBit(TIM6, TIM_IT_Update);
		}
		return;
	}

#ifdef __cplusplus
}
#endif

void ROS_Init()
{
	nh.initNode();
	nh.getHardware()->setBaud(115200);

	nh.subscribe(led_sub);
	nh.subscribe(bdc_sub);
	nh.subscribe(hspm_sub);

	nh.advertise(info_pub);
	nh.advertise(ip_pub);

	prev_update_time = millis();
}

void ROS_Update()
{
	updateTime();
	publishInputPin();
	prev_update_time = t;
}

/*******************************************************************************
* Callback functions
*******************************************************************************/
void LEDCallback(const utmrbc_msgs::LED &led_msg)
{
	int32_t led = led_msg.led;
	bool signal = led_msg.signal;

	switch (led)
	{
	case 1:
		led1 = signal ? 1 : 0;
		break;
	case 2:
		led2 = signal ? 1 : 0;
		break;
	case 3:
		led3 = signal ? 1 : 0;
		break;
	}

	sprintf(buffer, "led: %d, signal: %d", led, signal);
	info_msg.data = buffer;
	info_pub.publish(&info_msg);
}

void BDCCallback(const utmrbc_msgs::BDC &bdc_msg)
{
	int32_t bdc = bdc_msg.bdc;
	int32_t value = bdc_msg.value;

	switch (bdc)
	{
	case 1:
		WriteBDC(&BDC1, value);
		break;
	case 2:
		WriteBDC(&BDC2, value);
		break;
	case 3:
		WriteBDC(&BDC3, value);
		break;
	case 4:
		WriteBDC(&BDC4, value);
		break;
	case 5:
		WriteBDC(&BDC5, value);
		break;
	case 6:
		WriteBDC(&BDC6, value);
		break;
	case 7:
		WriteBDC(&BDC7, value);
		break;
	case 8:
		WriteBDC(&BDC8, value);
		break;
	}

	sprintf(buffer, "bdc: %d, value: %d", bdc, value);
	info_msg.data = buffer;
	info_pub.publish(&info_msg);
}

void HSPMCallback(const utmrbc_msgs::HSPM &hspm_msg)
{
	int32_t hspm = hspm_msg.hspm;
	bool signal = hspm_msg.signal;

	switch (hspm)
	{
	case 1:
		hspm1s1 = signal ? 1 : 0;
		break;
	case 2:
		hspm1s2 = signal ? 1 : 0;
		break;
	case 3:
		hspm2s1 = signal ? 1 : 0;
		break;
	case 4:
		hspm2s2 = signal ? 1 : 0;
		break;
	case 5:
		hspm3s1 = signal ? 1 : 0;
		break;
	case 6:
		hspm3s2 = signal ? 1 : 0;
		break;
	case 7:
		hspm4s1 = signal ? 1 : 0;
		break;
	case 8:
		hspm4s2 = signal ? 1 : 0;
		break;
	case 9:
		hspm5s1 = signal ? 1 : 0;
		break;
	case 10:
		hspm5s2 = signal ? 1 : 0;
		break;
	case 11:
		hspm6s1 = signal ? 1 : 0;
		break;
	case 12:
		hspm6s2 = signal ? 1 : 0;
		break;
	case 13:
		hspm7s1 = signal ? 1 : 0;
		break;
	case 14:
		hspm7s2 = signal ? 1 : 0;
		break;
	case 15:
		hspm8s1 = signal ? 1 : 0;
		break;
	case 16:
		hspm8s2 = signal ? 1 : 0;
		break;
	}

	//    switch(hspm){
	//    case 1: WriteSR(&SR, hspm, signal, Bit6);
	//    case 2: WriteSR(&SR, hspm, signal, Bit7);
	//    case 3: WriteSR(&SR, hspm, signal, Bit4);
	//    case 4: WriteSR(&SR, hspm, signal, Bit5);
	//    case 5: WriteSR(&SR, hspm, signal, Bit2);
	//    case 6: WriteSR(&SR, hspm, signal, Bit3);
	//    case 7: WriteSR(&SR, hspm, signal, Bit0);
	//    case 8: WriteSR(&SR, hspm, signal, Bit1);
	//    case 9: WriteSR(&SR, hspm, signal, Bit6);
	//    case 10: WriteSR(&SR, hspm, signal, Bit7);
	//    case 11: WriteSR(&SR, hspm, signal, Bit4);
	//    case 12: WriteSR(&SR, hspm, signal, Bit5);
	//    case 13: WriteSR(&SR, hspm, signal, Bit2);
	//    case 14: WriteSR(&SR, hspm, signal, Bit3);
	//    case 15: WriteSR(&SR, hspm, signal, Bit0);
	//    case 16: WriteSR(&SR, hspm, signal, Bit1);
	//    }

	sprintf(buffer, "hspm: %d, signal: %d", hspm, signal);
	info_msg.data = buffer;
	info_pub.publish(&info_msg);
}

/*******************************************************************************
* Publish functions
*******************************************************************************/
void publishInputPin()
{
	int pin;
	bool signal;

	if (!IP1)
	{
		pin = 1;
		signal = IP1;
	}
	if (!IP2)
	{
		pin = 2;
		signal = IP2;
	}
	if (!IP3)
	{
		pin = 3;
		signal = IP3;
	}
	if (!IP4)
	{
		pin = 4;
		signal = IP4;
	}
	if (!IP5)
	{
		pin = 5;
		signal = IP5;
	}
	if (!IP6)
	{
		pin = 6;
		signal = IP6;
	}
	if (!IP7)
	{
		pin = 7;
		signal = IP7;
	}
	if (!IP8)
	{
		pin = 8;
		signal = IP8;
	}
	if (!IP9)
	{
		pin = 9;
		signal = IP9;
	}
	if (!IP10)
	{
		pin = 10;
		signal = IP10;
	}
	if (!IP11)
	{
		pin = 11;
		signal = IP11;
	}
	if (!IP12)
	{
		pin = 12;
		signal = IP12;
	}
	if (!IP13)
	{
		pin = 13;
		signal = IP13;
	}
	if (!IP14)
	{
		pin = 14;
		signal = IP14;
	}
	if (!IP15)
	{
		pin = 15;
		signal = IP15;
	}

	ip_msg.pin = pin;
	ip_msg.signal = signal;
	ip_pub.publish(&ip_msg);
}

/*******************************************************************************
* Update the base time for interpolation
*******************************************************************************/
void updateTime()
{
	current_offset = millis();
	current_time = nh.now();
}

/*******************************************************************************
* ros::Time::now() implementation
*******************************************************************************/
ros::Time rosNow()
{
	return nh.now();
}
