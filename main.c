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
uint8_t MY_ADDREES=0x01;        //мой адресс

bool timer_start_ms(int ms){
///считаем вниз
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
//если значение таймера 0 то 
 timer_stop();
    return 1;
    //иначе 
    return 0;
}



bool bytes_available(){   //проверим доступность нового байта
//Читаем бит прерывания приемника например
};


bool get_byte(){   //возвращает новый свежепринятый байт
//достаем из приемника пришедший байт
};


                                                                                                  


bool get_packet(int reсive_byte, int packet_buffer[]){ // смотрим на пришедший байт, принимаем решения

    static int reset_flag;
    static int buf_cnt;
    static int preabula_is_recived;
        static int preabula_byte;
        static const int preabula_size = 3;  //0123 это же 4??
    static int size_is_recived;    
    static int adr_tx_is_recived;
    static int adr_rx_is_recived;
    static int message_is_recived;
        static int message_size;
        static int message_cnt;
    static int crc_is_recived;

    static int crc_xor=0;


    if(reset_flag){ 
        buf_cnt=0;
        preabula_is_recived=0;
        preabula_byte=0;
        size_is_recived=0;   
        adr_tx_is_recived=0;
        adr_rx_is_recived=0;
        message_is_recived=0;
        message_size=0;
        message_cnt=0;
        crc_is_recived=0;
        reset_flag=0;

        crc_xor=0;
    };
    //1. 4 байта преамбула 
    if(preabula_is_recived){    //принимаем преамбулу если не принята то принимаем.
        if(reсive_byte==0x1){
            buf_cnt=0;     // инкрементируем счетчик буфера
            packet_buffer[buf_cnt]=reсive_byte;
            buf_cnt++;
        }    
        if((reсive_byte==0x2)&&(buf_cnt==1)){
            packet_buffer[buf_cnt]=reсive_byte;
            crc_xor=reсive_byte;
            buf_cnt++;
        }
        if((reсive_byte==0x3)&&(buf_cnt==2)){
            packet_buffer[buf_cnt]=reсive_byte;
            crc_xor=crc_xor^reсive_byte;
            buf_cnt++;
        }    
        if((reсive_byte==0x4)&&(buf_cnt==3)){
            packet_buffer[buf_cnt]=reсive_byte;
            crc_xor=crc_xor^reсive_byte;
            preabula_is_recived=1;                     //поднимаем флаг принятой преабулы
            timer_start_ms(1000);                 //запускаем таймер таймаута
            buf_cnt++;
        } 
        else  buf_cnt=0;                           //если ношибка в приеме то начинаем сначала
    }

    //2. 1 байт N байт сообщения
    if (timer_is_over) reset_flag=1;                 //если таймаут то сбрасываем флаги принятия пакета                              
    if (preabula_is_recived){                       //если принята преамбула принимаем адрес'
        packet_buffer[buf_cnt]=reсive_byte;
        message_size=reсive_byte;
        crc_xor=crc_xor^reсive_byte;
        buf_cnt++;
        size_is_recived=1;    //взводим флаг принятого адреса   
    };   

//3. 1 байт адрес источника 
    if (timer_is_over) reset_flag=1;                 //если таймаут то сбрасываем флаги принятия пакета                              
    if (size_is_recived){                       //если принята преамбула принимаем адрес'
        packet_buffer[buf_cnt]=reсive_byte;
        crc_xor=crc_xor^reсive_byte;
        buf_cnt++;
        adr_tx_is_recived=1;    //взводим флаг принятого адреса   
    };   

//4. 1 байт адрес приемника
    if (timer_is_over) reset_flag=1;                 //если таймаут то сбрасываем флаги принятия пакета                              
    if (adr_tx_is_recived){                       //если принята адрес источника принимаем адрес приемник
        packet_buffer[buf_cnt]=reсive_byte;
        crc_xor=crc_xor^reсive_byte;
        buf_cnt++;
        adr_rx_is_recived=1;    //взводим флаг принятого адреса 
        /////Можно сверить адрес получателя и свой////  
    };   
   
//5.N байт сообщения по 1 байт -- максимум 255
    if (timer_is_over) reset_flag=1;                 //если таймаут то сбрасываем флаги принятия пакета                              
    if (adr_rx_is_recived){                       //если принята адрес премника принимаем массив
        packet_buffer[buf_cnt]=reсive_byte;
        crc_xor=crc_xor^reсive_byte;
        message_cnt++;                              //получаем байт и инкрементируем принятый размер   
        buf_cnt++;
        if(message_cnt==message_size);
            {message_is_recived=1;}      //если размер принятого равен размеру данных то взводим флаг принятых данных  
    };   

//6.//если принята  данных принимаем CRC
//если таймаут то сбрасываем флаги принятия пакета

    if (timer_is_over) reset_flag=1;                 //если таймаут то сбрасываем флаги принятия пакета                              
    if (message_is_recived=1){                       //если принята адрес источника принимаем адрес приемник
        packet_buffer[buf_cnt]=reсive_byte;
        
        crc_is_recived=1;    //взводим флаг принятого адреса   
    };   
   
    if (crc_is_recived=1){                       //если принята адрес источника принимаем адрес приемник
        if(packet_buffer[buf_cnt]==crc_xor) return 1;   //взводим флаг принятого адреса   
    };  

}

bool packet_reaction(uint8_t *packet_buffer){  //когда пакет принят его нужно бы засунуть в структуру
       return true;  
};






int main()
{ 
 int packet_buffer[263]; //буфер 4+1+1+1+255+1

 if(bytes_available()){                                //если доступен новый байт
    if(get_packet(get_byte(), &packet_buffer))         //достаем его из приемника и формируем пакет
        packet_reaction(packet_buffer);               //потом формируем его структуру
 }
};
