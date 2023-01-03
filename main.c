#include "main.h"
// Noi khai bao hang so
#define     LED     PORTD
#define     ON      1
#define     OFF     0

#define     INIT_SYSTEM        255
#define     ENTER_PASSWORD     1
#define     CHECK_PASSWORD     2
#define     UNLOCK_DOOR        3
#define     WRONG_PASSWORD     4

#define PORTD_OUT   PORTD
#define TRISD_OUT   TRISD

#define PORT_SEVEN_SEG  PORTA
#define TRIS_SEVEN_SEG  TRISA

void init_output(void);
// Noi khai bao bien toan cuc
unsigned char arrayMapOfOutput [8] = {0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};
unsigned char statusOutput[8] = {0,0,0,0,0,0,0,0};
// Khai bao cac ham co ban IO
void init_system(void);
void delay_ms(int value);
void init_seven_seg();
void display_seven_seg();
void OpenOutput(int index);
void CloseOutput(int index);
void TestOutput(void);
void ReverseOutput(int index);
void lcdRun(void);
void menuRun(void);
void test1(void);
void displayMenu();
unsigned char isButton1Pressed();
unsigned char isButton2Pressed();
unsigned char isButton3Pressed();
unsigned char isButtonMotorOn();
unsigned char isButtonMotorOff();
void MotorOn();
void MotorOff();
void BaiTap_Motor();
void Test_KeyMatrix();
//Chuong trinh Password Door
unsigned char arrayMapOfNumber [16] = {1,2,3,'A',4,5,6,'B',
                                       7,8,9,'C','*',0,'E','D'};
unsigned char arrayMapOfPassword [5][4]= {
  {1,2,3,4},
  {2,7,8,9},
  {3,3,3,3},
  {4,8,6,8},
  {5,'A','B','C'},
};
unsigned char arrayPassword[4];
unsigned char statusPassword = INIT_SYSTEM;
unsigned char indexOfNumber = 0;
unsigned char timeDelay = 0;
int select = 1;
void App_PasswordDoor();
unsigned char CheckPassword();
unsigned char isButtonNumber();
unsigned char numberValue;
unsigned char isButtonEnter();
void UnlockDoor();
void LockDoor();
// Den giao thong
//void Phase1_GreenOn();
//void Phase1_GreenOff();
//void Phase1_YellowOn();
//void Phase1_YellowOff();
//void Phase1_RedOn();
//void Phase1_RedOff();
//
//void Phase2_GreenOn();
//void Phase2_GreenOff();
//void Phase2_YellowOn();
//void Phase2_YellowOff();
//void Phase2_RedOn();
//void Phase2_RedOff();

#define     INIT_SYSTEM         255
#define     PHASE1_GREEN        0
#define     PHASE1_YELLOW       1
#define     PHASE2_GREEN        2
#define     PHASE2_YELLOW       3
#define     WAIT                4
#define     NUM_OPTIONS         4
//Auto run mode
#define     AUTO_INIT           10
#define     RG                  11
#define     RY                  12
#define     YR                  13
#define     GR                  14
//Set value mode
#define     SET_YELLOW          20
#define     SET_YELLOW_2        22
#define     SET_GREEN           21
#define     SET_GREEN_2         23
    //We don't need to set red value because RED_TIME = YELLOW_TIME + GREEN_TIME
//Manual mode
#define    MANUAL_RED           30
#define    MANUAL_GREEN         31
#define    MANUAL_YELLOW_1      32
#define    MANUAL_YELLOW_2      33
//Caution mode
#define    CAUTION              40
unsigned char status = INIT_SYSTEM;
/*
unsigned char timeOfGreenPhase1 = 25;
unsigned char timeOfYellowPhase1 = 3;
unsigned char timeOfGreenPhase2 = 20;
unsigned char timeOfYellowPhase2 = 3;
unsigned char timeOfLight = 0;
unsigned char cntOfLight = 0;
void UartDataReceiveProcess();
void UartDataReceiveProcess_ElectronicDeviceControl();
*/
//Initial values
int valueInput[2] = {0, 0};
int timeSEG = 0;
int timeR = 11, TIME_R = 11;
int timeG = 7, TIME_G = 7;
int timeY = 4, TIME_Y = 4;
int timeG_man = 3;
void AppTrafficLight();
char* menu[NUM_OPTIONS] = {
    "AUTO",  
    "SET TIME", 
    "MANUAL", 
    "CAUTION"
};
////////////////////////////////////////////////////////////////////
//Hien thuc cac chuong trinh con, ham, module, function duoi cho nay
////////////////////////////////////////////////////////////////////

unsigned char cmd;
void uart_isr()
{
    //uart_isr_simulate_machine();
    cmd = RCREG;
    if (cmd == 'A') {
        select = 1;
        cmd = 'Z';
    }
    else if (cmd == 'M') {
        select = 2;
        cmd = 'Z';
    }
    else if (cmd == 'S') {
        select = 3;
        cmd = 'Z';
    }
    else if (cmd == 'C') {
        select = 4;
        cmd = 'Z';
    }
}
void test_key();

void main(void)
{
    int counterSEG = 0;
	unsigned int k = 0;
	init_system();
        //delay_ms(1000);
        TRISD = 0x00;
        //TRISC_OUT = 0x00;
        //uart_isr_simulate_machine();
	while (1)
	{
        uart_isr();
        //DisplayLcdScreen();
        //test_key();
 //       scan_key_matrix();
        if (flag_timer3) {
            flag_timer3 = 0;
            scan_key_matrix(); // 8 button
            AppTrafficLight();
            //LcdPrintStringS(0, 0, "HELLO");

            //DisplayDataReceive();
            DisplayLcdScreen(); //Output process 14ms vs 10ms with no timer
            //UartSendString(" ABC \r\n");    
        }
//            display_seven_seg();
        if (flag_timer0) {
            flag_timer0 = 0;
            if (counterSEG == 1) {
                display_seven_seg();
                counterSEG = 0;
            } else {
                counterSEG++;
            }
            
        }
        
        DisplayLcdScreen();
    }    
}
void test_key() {
    int e;
    for (e = 0; e < 16; e++) {
        if (key_code[e] == 1) {
            LcdPrintNumS(1, 0, e);
        }
    }
}
void displayMenu() {
    LcdPrintStringS(0, 0, menu[0]);
    LcdPrintStringS(0, 7, menu[1]);
    LcdPrintStringS(1, 0, menu[2]);
    LcdPrintStringS(1, 7, menu[3]);
    switch(select) {
        case 1:
            LcdPrintStringS(0, 6, "<");
            break;
        case 2:
            LcdPrintStringS(1, 6, "<");
            break;
        case 3:
            LcdPrintStringS(0, 15, "<");
            break;
        case 4:
            LcdPrintStringS(1, 15, "<");
            break;
        default:
            break;
    }
}
// Hien thuc cac module co ban cua chuong trinh
void delay_ms(int value)
{
	int i,j;
	for(i=0;i<value;i++)
		for(j=0;j<238;j++);
}

void init_output(void)
{
   TRISD_OUT = 0x00;
    PORTD_OUT = 0x00;
}

void init_system(void)
{
    init_seven_seg();
    init_uart();
    init_output();
    lcd_init();
    init_key_matrix_with_uart_i2c();
    init_interrupt();
    lcd_clear();
    LcdClearS();
    delay_ms(500);
   init_timer0(4695);//dinh thoi 1ms sai so 1%
    init_timer3(46950);//dinh thoi 10ms
    SetTimer0_ms(2);
    SetTimer3_ms(50); //Chu ky thuc hien viec xu ly input,proccess,output
    //PORTAbits.RA0 = 1;
    //uart_send_str("HELLO\r\n");
    
}

unsigned char number[10] = {0b0000,0b0001,0b0010,0b0011,0b0100,0b0101,0b0110,0b0111,0b1000,0b1001};

void init_seven_seg() {
    TRIS_SEVEN_SEG = 0x00;
    PORT_SEVEN_SEG = 0x00;
    TRISE = 0x00;
}

int indexSEG = 0;
void display_seven_seg() {
//    PORT_SEVEN_SEG = number[timeR % 10];
    switch (indexSEG) {
        case 0:          
            OpenOutput(6);
            CloseOutput(7);
            PORTE = PORTE & ~arrayMapOfOutput[0];
            PORTE = PORTE & ~arrayMapOfOutput[1];
            PORTE = 0x00;
            PORT_SEVEN_SEG = number[timeSEG / 1000];  
            if (status != RG && status != RY && status != GR && status != YR && status != MANUAL_YELLOW_1 && status != MANUAL_YELLOW_2) {
                PORT_SEVEN_SEG = number[0];
            }
            break;
        case 1:        
            CloseOutput(6);
            OpenOutput(7);
            PORTE = PORTE & ~arrayMapOfOutput[0];
            PORTE = PORTE & ~arrayMapOfOutput[1];
            PORTE = 0x00;
            PORT_SEVEN_SEG = number[(timeSEG % 1000) / 100];   
            if (status != RG && status != RY && status != GR && status != YR && status != MANUAL_YELLOW_1 && status != MANUAL_YELLOW_2) {
                PORT_SEVEN_SEG = number[0];
            }
            break;
        case 2:        
            CloseOutput(6);
            CloseOutput(7);
            PORTE = PORTE | arrayMapOfOutput[0];
            PORTE = PORTE & ~arrayMapOfOutput[1];
            PORTE = 0x01;
            PORT_SEVEN_SEG = number[(timeSEG % 100) / 10];   
            if (status != RG && status != RY && status != GR && status != YR && status != MANUAL_YELLOW_1 && status != MANUAL_YELLOW_2) {
                PORT_SEVEN_SEG = number[0];
            }
            break;
        case 3:        
            CloseOutput(6);
            CloseOutput(7);
            PORTE = PORTE & ~arrayMapOfOutput[0];
            PORTE = PORTE | arrayMapOfOutput[1];
            PORTE = 0x02;
            PORT_SEVEN_SEG = number[timeSEG % 10];  
            if (status != RG && status != RY && status != GR && status != YR && status != MANUAL_YELLOW_1 && status != MANUAL_YELLOW_2) {
                PORT_SEVEN_SEG = number[0];
            }
            break;
        default:
            break;
    }
    indexSEG = (indexSEG + 1) % 4;
}

void OpenOutput(int index)
{
	if (index >= 0 && index <= 7)
	{
		PORTD_OUT = PORTD_OUT | arrayMapOfOutput[index];
	}


}

void CloseOutput(int index)
{
	if (index >= 0 && index <= 7)
	{
		PORTD_OUT = PORTD_OUT & ~arrayMapOfOutput[index];
	}
}

void ReverseOutput(int index)
{
    if (statusOutput[index]  == ON)
    {
        CloseOutput(index);
        statusOutput[index] = OFF;
    }
    else
    {
        OpenOutput(index);
        statusOutput[index] = ON;
    }
}

void TestOutput(void)
{
	int k;
	for (k=0;k<14 ;k++ )
	{
		OpenOutput(k);
		delay_ms(500);
		CloseOutput(k);
		delay_ms(500);
	}
}

void Phase1_GreenOn()
{
    OpenOutput(0);
}
void Phase1_GreenOff()
{
    CloseOutput(0);
}

void Phase1_YellowOn()
{
    OpenOutput(4);
}
void Phase1_YellowOff()
{
    CloseOutput(4);
}

void Phase1_RedOn()
{
    OpenOutput(6);
}
void Phase1_RedOff()
{
    CloseOutput(6);
}

void Phase2_GreenOn()
{
    OpenOutput(1);
}
void Phase2_GreenOff()
{
    CloseOutput(1);
}

void Phase2_YellowOn()
{
    OpenOutput(5);
}
void Phase2_YellowOff()
{
    CloseOutput(5);
}

void Phase2_RedOn()
{
    OpenOutput(7);
}
void Phase2_RedOff()
{
    CloseOutput(7);
}
//void setTrafficLight(int state[6]) {
//    int i;
//    for(i = 0; i < 6; i++) {
//        if (state[i] == 1)  
//            OpenOutput(i);
//        else 
//            CloseOutput(i);
//    }
//}
unsigned char isButton1Pressed()
{
    return (key_code[4] == 1) ? 1:0;    //Press '4' 
}

unsigned char isButton2Pressed()
{
    return (key_code[5] == 1) ? 1:0;    //Press '5'
}

unsigned char isButton3Pressed()
{
    return (key_code[6] == 1) ? 1:0;    //Press '6' 
}

void clearLCD(int line) //line = 1 or 2 => clear line 1 or 2 on LCD
{                       //line = 3 => clear all lines
    switch (line) {
        case 1:
            LcdPrintStringS(0,0,"                "); 
            break;
        case 2:
            LcdPrintStringS(1,0,"                "); 
            break;   
        case 3:
            LcdPrintStringS(0,0,"                ");             
            LcdPrintStringS(1,0,"                "); 
            break;    
        default:
            break;
    }
}
unsigned char keyMatrixNumberIndex[10] = {13, 0, 1, 2, 4, 5, 6, 8, 9, 10};
unsigned char isNumberButtonPressed() {
    int i;
    for (i = 0; i < 10; i++) {
        if (key_code[keyMatrixNumberIndex[i]] == 1) {
            return 1;
        }
    }  
    return 0;
}


void setTimeValue(unsigned char index) {
    int i;
    for (i = 0; i < 10; i++) {
        if (key_code[keyMatrixNumberIndex[i]] == 1) {
            valueInput[index] = i;
        }
    }
}
void clearOldInput() {
    valueInput[0] = 0;
    valueInput[1] = 0;
}
int set_state = 0;
unsigned char flag_1s = 0, flag_5s = 0, flag_10s = 0;
int cnt_1s = 0, cnt_5s = 0, cnt_10s = 0;
int isPrint = 0;
int set_flag = 0;
int print_flag = 0;
int cnt_led = 0, flag_led = 0;

/*
 TODO: Neu khon set time => Lay time truoc do 
 */
void AppTrafficLight()
{
    cnt_10s++; flag_10s = 0;
    cnt_1s++; flag_1s = 0;
    cnt_led++;
    if (cnt_1s == 17) {  
        cnt_1s = 0;
        flag_1s = 1;
    }
    if (cnt_10s == 170) {
        cnt_10s = 0;
        flag_10s = 1;
    }
//    if (cnt_led == 1) {
//        display_seven_seg();
//        cnt_led = 0;
//    }
    switch (status)
    {
        case INIT_SYSTEM:
            LcdClearS();
//            //turnOffAllLEDs();
//            status = RG;
            timeR = TIME_R+1;
            timeG = TIME_G+1;
            

            displayMenu();
            if (isButton1Pressed()) {
                select++;
                if (select > 4) select = 1;
            }
            if (isButton2Pressed()) {
                switch(select) {
                    case 1:
                        UartSendString("-----AUTOMATIC MODE-----\r\n");
                        status  = RG;
                        print_flag = 0;
                        break;
                    case 2:
                        status = MANUAL_RED;
                        print_flag = 0;
                        break;
                    case 3:
                        clearOldInput(); 
                        status = SET_YELLOW; 
                        UartSendString("-----SET TIME MODE-----\r\n");
                        print_flag = 0;
                        break;
                    case 4:
                        UartSendString("CAUTION MODE\r\n");
                        status = CAUTION;
                        print_flag = 0;
                        break;
                    default:
                        break;
                }
            }
            break;
        case RG:
            //LED = 0b00100001;
            OpenOutput(0);
            CloseOutput(1);
            CloseOutput(2);
            CloseOutput(3);
            CloseOutput(4);
            OpenOutput(5);
            LcdClearS();
            LcdPrintStringS(0,0,"RED   ");             
            LcdPrintStringS(1,0,"GREEN "); 
            if (timeG > 0) {
                LcdPrintNumS(0, 14, timeR);
                LcdPrintNumS(1, 14, timeG);
                timeSEG = timeR * 100 + timeG;
                if (flag_1s) {
                    UartSendString("[Road 1]RED: ");
                    UartSendNumToString(timeR);
                    UartSendString("\r\n");
                    UartSendString("[Road 2]GREEN: ");
                    UartSendNumToString(timeG);
                    UartSendString("\r\n");
                    timeR--;
                    timeG--;                    
                }
                //DisplayLcdScreen();
            }
            else {
                isPrint = 0;
                status = RY;
                timeG = TIME_G+1;
            }
            if (isButton3Pressed()) status = INIT_SYSTEM;
            break;
        case RY:
            //LED = 0b00100010;
            OpenOutput(0);
            CloseOutput(1);
            CloseOutput(2);
            CloseOutput(3);
            OpenOutput(4);
            CloseOutput(5);
            LcdClearS();
            LcdPrintStringS(0,0,"RED   ");             
            LcdPrintStringS(1,0,"YELLOW"); 
            if (timeR > 0) {

                LcdPrintNumS(0, 14, timeR);
                LcdPrintNumS(1, 14, timeY);
                timeSEG = timeR * 100 + timeY;
                if (flag_1s) {
                    UartSendString("[Road 1]RED: ");
                    UartSendNumToString(timeR);
                    UartSendString("\r\n");
                    UartSendString("[Road 2]YELLOW: ");
                    UartSendNumToString(timeY);
                    UartSendString("\r\n");
                    timeR--;
                    timeY--;
                }
                //DisplayLcdScreen();
            }
            else {
                isPrint = 0;
                status = GR;
                timeR = TIME_R+1;
                timeY = TIME_Y;
            }
 
            if (isButton3Pressed()) status = INIT_SYSTEM;
            break;
        case GR:
            //LED = 0b00001100;
            CloseOutput(0);
            CloseOutput(1);
            OpenOutput(2);
            OpenOutput(3);
            CloseOutput(4);
            CloseOutput(5);
            LcdClearS();
            LcdPrintStringS(0,0,"GREEN");             
            LcdPrintStringS(1,0,"RED"); 
            if (timeG > 0) {
                LcdPrintNumS(0, 14, timeG);
                LcdPrintNumS(1, 14, timeR);
                timeSEG = timeG * 100 + timeR;
                if (flag_1s) {
                    UartSendString("[Road 1]GREEN: ");
                    UartSendNumToString(timeG);
                    UartSendString("\r\n");
                    UartSendString("[Road 2]RED: ");
                    UartSendNumToString(timeR);
                    UartSendString("\r\n");
                    timeG--;
                    timeR--;
                }
                //DisplayLcdScreen();
            }
            else {
                isPrint = 0;
                status = YR;
                timeG = TIME_G+1;
            }
            if (isButton3Pressed()) status = INIT_SYSTEM;
            break;
        case YR:
            //LED = 0b00010100;
            CloseOutput(0);
            OpenOutput(1);
            CloseOutput(2);
            OpenOutput(3);
            CloseOutput(4);
            CloseOutput(5);
            LcdClearS();
            LcdPrintStringS(0,0,"YELLOW");             
            LcdPrintStringS(1,0,"RED"); 
            if (timeR > 0) {
                LcdPrintNumS(0, 14, timeY);
                LcdPrintNumS(1, 14, timeR);
                timeSEG = timeY * 100 + timeR;
                if (flag_1s) {
                    UartSendString("[Road 1]YELLOW: ");
                    UartSendNumToString(timeY);
                    UartSendString("\r\n");
                    UartSendString("[Road 2]RED: ");
                    UartSendNumToString(timeR);
                    UartSendString("\r\n");
                    timeY--;
                    timeR--;
                }
                //DisplayLcdScreen();
            }
            else {
                isPrint = 0;
                status = RG;
                timeR = TIME_R+1;
                timeY = TIME_Y;
            }
            //LED = 0b01010000;
            if (isButton3Pressed()) status = INIT_SYSTEM;
            break;
        case SET_YELLOW:
            LcdClearS();
            LcdPrintStringS(0, 0, "SET YELLOW TIME");
            LcdPrintStringS(1, 0, "(1-9)");
            if (!print_flag) {
                UartSendString("Use buttons in your board to change yellow time...\r\n");
                print_flag = 1;
            }
            if (flag_10s) {
                status = INIT_SYSTEM;
                print_flag = 0;
            }
            else {
                if (set_state == 0) {
                    LcdPrintStringS(1, 15, "-");
                    DisplayLcdScreen();
                }
                else {
                    LcdPrintNumS(1, 15, valueInput[1]);
                    DisplayLcdScreen();

                }
                if (isButton1Pressed() == 1) {
                    valueInput[1]++;
                    if (valueInput[1] > 9) valueInput[1] = 1;
                    set_state = 1;
                    cnt_10s = 0;
                }
                if (isButton3Pressed() == 1) {
                    clearOldInput();
                    status = SET_GREEN;
                    UartSendString("You escaped from set yellow mode without saving.\r\n");
                }
                else {
                    if (isButton2Pressed() == 1) {
                        TIME_Y = valueInput[1];
                        TIME_R = TIME_Y + TIME_G;
                        status = SET_GREEN;
                        clearOldInput();
                        set_state = 0;
                        UartSendString("Successfully changed yellow time to ");
                        UartSendNumToString(TIME_Y);
                        UartSendString(" seconds.\r\n");
                        cnt_10s = 0;
                    }
                }
            }
            break;
        case SET_GREEN:
            LcdClearS();
            LcdPrintStringS(0, 0, "SET GREEN TIME");
            LcdPrintStringS(1, 0, "(10-99)");
            if (!print_flag) {
                UartSendString("Use buttons in your board to change green time...\r\n");
                print_flag = 1;
            }
            if (flag_10s) {
                status = INIT_SYSTEM;
                print_flag = 0;
            }
            else {
                if (set_state == 0) {
                    LcdPrintStringS(1,14,"--");
                    DisplayLcdScreen();
                }
                else if (set_state == 1) {
                    LcdPrintNumS(1, 14, valueInput[0]);
                    LcdPrintStringS(1, 15, "-");
                    DisplayLcdScreen();
                }
                else if (set_state == 2) {
                    LcdPrintNumS(1, 14, valueInput[0]);
                    LcdPrintNumS(1, 15, valueInput[1]);
                    DisplayLcdScreen();
                }
                if (isButton1Pressed() == 1) {
                    if (set_state == 0) set_state = 1;
                    if (set_state == 1) {
                        valueInput[0]++;
                        if (valueInput[0] > 9) valueInput[0] = 1;
                    }
                    else if (set_state == 2) {
                        valueInput[1]++;
                        if (valueInput[1] > 9) valueInput[1] = 0;
                    }
                    cnt_10s = 0;
                }
                if (isButton3Pressed() == 1) {
                    clearOldInput();
                    status = INIT_SYSTEM;
                    UartSendString("You escaped from set green mode without saving.\r\n");
                }
                else {
                    if (isButton2Pressed()) {
                        if (set_state == 1) {
                            set_state = 2;
                        }
                        else if (set_state == 2) {
                                status = RG;
                                TIME_G = (valueInput[0]*10) + valueInput[1];
                                TIME_R = TIME_G + TIME_Y;
                                timeR = TIME_R;
                                timeG = TIME_G;
                                timeY = TIME_Y;
                                UartSendString("Successfully changed green time to ");
                                UartSendNumToString(TIME_G);
                                UartSendString(" seconds.\r\n");
                                UartSendString("Your red time is now ");
                                UartSendNumToString(TIME_R);
                                UartSendString(" seconds.\r\n");
                                print_flag = 0;
                        }
                        cnt_10s = 0;
                    }
                }
            }
            break;
        case MANUAL_RED:
            LcdClearS();
            LcdPrintStringS(0, 0, "RED");
            LcdPrintStringS(1, 0, "GREEN");
            OpenOutput(0);
            CloseOutput(1);
            CloseOutput(2);
            CloseOutput(3);
            CloseOutput(4);
            OpenOutput(5);
            if (!print_flag) {
                UartSendString("[MANUAL]RED\r\n");
                UartSendString("[MANUAL]GREEN\r\n");
                print_flag = 1;
            }

            if (isButton1Pressed() == 1) {
                status = MANUAL_YELLOW_1;
                print_flag = 0;
            }
            if (isButton3Pressed()) status = INIT_SYSTEM;
            break;
        case MANUAL_GREEN:
            LcdClearS();
            LcdPrintStringS(0, 0, "GREEN");
            LcdPrintStringS(1, 0, "RED");
            CloseOutput(0);
            CloseOutput(1);
            OpenOutput(2);
            OpenOutput(3);
            CloseOutput(4);
            CloseOutput(5);
            if (!print_flag) {
                UartSendString("[MANUAL]GREEN\r\n");
                UartSendString("[MANUAL]RED\r\n"); 
                print_flag = 1;
            }
            if (isButton1Pressed() == 1) {
                status = MANUAL_YELLOW_2;
                print_flag = 0;
            }
            if (isButton3Pressed()) status = INIT_SYSTEM;
            break;
        case MANUAL_YELLOW_1:
            LcdClearS();
            LcdPrintStringS(0, 0, "RED");
            LcdPrintStringS(1, 0, "YELLOW");
            OpenOutput(0);
            CloseOutput(1);
            CloseOutput(2);
            CloseOutput(3);
            OpenOutput(4);
            CloseOutput(5);
            if (timeG_man > 0) {
                LcdPrintNumS(1, 14, timeG_man);
                timeSEG = 0 * 100 + timeG_man;
                if (flag_1s) {
                    UartSendString("[MANUAL]RED\r\n");
                    UartSendString("[MANUAL]YELLOW: ");
                    UartSendNumToString(timeG_man);
                    UartSendString("\r\n");
                    timeG_man--;
                }
            }
            else {
                status = MANUAL_GREEN;
                timeG_man = 3;
            }

            if (isButton3Pressed()) status = INIT_SYSTEM;
            break;
        case MANUAL_YELLOW_2:
            LcdClearS();
            LcdPrintStringS(0, 0, "YELLOW");
            LcdPrintStringS(1, 0, "RED");
            CloseOutput(0);
            OpenOutput(1);
            CloseOutput(2);
            OpenOutput(3);
            CloseOutput(4);
            CloseOutput(5);
            if (timeG_man > 0) {
                LcdPrintNumS(0, 14, timeG_man);
                timeSEG = timeG_man * 100 + 0;
                if (flag_1s) {
                    UartSendString("[MANUAL]YELLOW: ");
                    UartSendNumToString(timeG_man);
                    UartSendString("\r\n");
                    UartSendString("[MANUAL]RED");
                    UartSendString("\r\n");
                    timeG_man--;
                }
            }
            else {
                status = MANUAL_RED;
                timeG_man = 3;
            }
            if (isButton3Pressed()) status = INIT_SYSTEM;
            if (isButton3Pressed() == 1) status = INIT_SYSTEM;
            break;
        case CAUTION:
            LcdClearS();
            LcdPrintStringS(1, 0, "YELLOW");
            LcdPrintStringS(0, 0, "YELLOW");
            if (cnt_1s >= 0 && cnt_1s <= 7) {
                CloseOutput(0);
                CloseOutput(1);
                CloseOutput(2);
                CloseOutput(3);
                CloseOutput(4);
                CloseOutput(5);
            }
            else {
                CloseOutput(0);
                OpenOutput(1);
                CloseOutput(2);
                CloseOutput(3);
                OpenOutput(4);
                CloseOutput(5);
            }
            if (isButton3Pressed() == 1) status = INIT_SYSTEM;
            break;
        default:
            break;
    }
}





