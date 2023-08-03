//тестируем

#include <stdio.h>
#include <stdbool.h>  
#include <stdint.h>  
#include <stddef.h>  
/*
Вот твой протокол Дима
1. 4 байта преамбула 
2. 1 байт N байт сообщения 
3. 1 байт адрес источника
4. 1 байт адрес приемника
5. Данные Н байт                                -- максимум 255
6. CRC - XOR всех байт сообщения включая преамбулу

Максимальное время сообщения 1 сек.

Написать прототипы функций для обработки протокола
Предоставить реализацию на С алгоритма приема сообщений протокола

Будь внимателен, восприми эту секунду как таймаут
*/



/*
//TODO: бегать по буферу ++ стремно. сделай фиксированные номера элементов
    проверить логику работы таймера

*/
int MY_ADDREES=0x01;        //мой адресс

bool timer_start_ms(int ms){
///считаем вниз
return true;                                   ///для тестирования по дефолту
///устанавливаем счетчик таймер
///Перегрузку устанавливаем = 0
///стартуем счетчик таймер

///возвращаем 1 если все в штатном порядке
}

bool timer_stop(){
//останавливаем таймер

///возвращаем 1 если все в штатном порядке
}

bool timer_is_over(){
    //иначе 
    return false;                           ///для тестирования
}



bool bytes_available(){   //проверим доступность нового байта
//Читаем бит прерывания приемника например
return true;                               ///для тестирования
};


int get_byte(){   //возвращает новый свежепринятый байт
//достаем из приемника пришедший байт
int a;



printf("Input bite\n");
scanf("%i", &a);
//printf("%x\n", a);  
return a; 
};






bool get_packet(int reсive_byte, int packet_buffer[]){ // смотрим на пришедший байт, принимаем решения

static enum part // арифметическая операция
{
    PREABULA1,        
    PREABULA2,   
    PREABULA3,    
    PREABULA4,
    MESSAGE_SIZE,
    ADRESS_TX,
    ADRESS_RX,
    DATA,
    CRC   
} wait_packet_part;
static bool reset_flag;
static int buf_cnt;
static int message_size;
static int message_cnt;
static int crc_xor;


switch (wait_packet_part)
{
case PREABULA1:
    if(reсive_byte==0x1){
        buf_cnt=0;
        packet_buffer[buf_cnt]=reсive_byte;
        crc_xor=crc_xor^reсive_byte;
        buf_cnt=1;
        printf("PREABULA1 RECIVED\n");
        wait_packet_part=PREABULA2;
    } else reset_flag=1;
    break;
case PREABULA2:
    if(reсive_byte==0x2){
        packet_buffer[buf_cnt]=reсive_byte;
        crc_xor=crc_xor^reсive_byte;
        buf_cnt=2;
        printf("PREABULA2 RECIVED\n");
        wait_packet_part=PREABULA3;
    } else reset_flag=1;
    break;
case PREABULA3:
    if(reсive_byte==0x3){
        packet_buffer[buf_cnt]=reсive_byte;
        crc_xor=crc_xor^reсive_byte;
        buf_cnt=3;
        wait_packet_part=PREABULA4;
        printf("PREABULA3 RECIVED\n");
    }else reset_flag=1;
    break;
case PREABULA4:
    if(reсive_byte==0x4){
        packet_buffer[buf_cnt]=reсive_byte;
        crc_xor=crc_xor^reсive_byte;
        buf_cnt=4;
        printf("PREABULA4 RECIVED\n");
        wait_packet_part=MESSAGE_SIZE;
    }else reset_flag=1;
    break;
case MESSAGE_SIZE:
        packet_buffer[buf_cnt]=reсive_byte;
        message_size=reсive_byte;
        crc_xor=crc_xor^reсive_byte;
        buf_cnt=5;
        wait_packet_part=ADRESS_TX;
        printf("MESSAGE_SIZE RECIVED\n");
    break;
case ADRESS_TX:
        packet_buffer[buf_cnt]=reсive_byte;
        crc_xor=crc_xor^reсive_byte;
        buf_cnt=6;
        printf("ADRESS_TX RECIVED\n");
        wait_packet_part=ADRESS_RX;
        
    break;
case ADRESS_RX:
        packet_buffer[buf_cnt]=reсive_byte;
        crc_xor=crc_xor^reсive_byte;
        buf_cnt=7;
        printf("ADRESS_RX RECIVED\n");
        wait_packet_part=DATA; 
    break;    
case DATA:
        packet_buffer[buf_cnt]=reсive_byte;
        crc_xor=crc_xor^reсive_byte;
        buf_cnt++;
        message_cnt++;
        printf("DATA BYTE RECIVED\n");
        if(message_size==message_cnt){
                printf("DATA RECIVED\n");
                printf("NEXT BYTE MUST BE");
                printf(" %x",crc_xor);
                printf(" TO SUCCSUSFULL CHECK CRC\n");
                wait_packet_part=CRC;     
            }
    break;   
case CRC:
        if(reсive_byte==crc_xor){
        packet_buffer[buf_cnt]=reсive_byte;    
        printf("PACKET IS OK\n");
        reset_flag=1;
        return true;
        } else {printf("PACKET IS BAD\n");
                reset_flag=1;
        }  
    break;   
default:
    break;
}

if(reset_flag){ 
    printf("reseting variables\n");
reset_flag=0;
wait_packet_part=PREABULA1;
buf_cnt=0;
message_size=0;
message_cnt=0;
crc_xor=0;
return false;
};

}

bool packet_reaction(int *packet_buffer){  //когда пакет принят его нужно бы засунуть в структуру
    printf("PACKET IS REСIVED!!!\n");

    printf("size: ");
    printf("%x\n", packet_buffer[4]); 

    printf("tx_address: ");
    printf("%x\n", packet_buffer[5]); 

    printf("rx_address: ");
    printf("%x\n", packet_buffer[6]); 

    printf("DATA: ");
    for(int i=7; i<(7+packet_buffer[4]); i++){
        printf("%x\n", packet_buffer[i]);
        
    };

    printf("CRC: ");
    printf("%x\n\n\n", (packet_buffer[7+packet_buffer[4]]));  
};



int main()
{ 
 int packet_buffer[263]; //буфер 4+1+1+1+255+1

while (1)
{
    if(bytes_available()){                                //если доступен новый байт
        if(get_packet(get_byte(), packet_buffer))         //достаем его из приемника и формируем пакет
           packet_reaction(packet_buffer);               //потом формируем его структуру
    }
}
};
