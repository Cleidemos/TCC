#define  ABRIR_SHUTTER          0
#define  DECREMENTA_PASSOS      1
#define  INCREMENTA_PASSOS      2
#define  FECHA_SHUTTER          3
#define  STOP                   4
#define  POSICIONA_SENSOR       5
#define  RETIRA_SENSOR          6
#define  FIM_DE_CURSO1          RD2_Bit
#define  FIM_DE_CURSO2          RD3_Bit
#define  Passo1d                0b01100010
#define  Passo2d                0b01000110
#define  Passo3d                0b01001100
#define  Passo4d                0b01101000

char uart_rd;
unsigned char output[10];
unsigned int passos,j;
unsigned char funcao,i,k;

void main() {

  TRISC = 0b10000000;           //RC6  CONFIG. COMUNICAÇÃO SERIAL TRANSMIT
  PORTC = 0b10000000;           //RC7  AND CONFIG. COMUNICAÇÃO SERIAL RECIEVE
                                //RC2, 3, RD0, 1 COMANDO BOBINAS SENSOR

  TRISD = 0b00001100;           //RD2 FDC1=OPERANDO
  PORTD = 0b00001100;           //RD3 FDC2=DESCANSO

  TRISB = 0b00000000;           //RB1, 2, 3 E 5 COMANDO BOBINAS DISCO
  PORTB = 0b00000000;           //RB6 LED_SHUTTER E RB7 LED_COMUNICACAO

  UART1_Init(9600);              // Initialize UART module at 9600 bps
  Delay_ms(100);                 // Wait for UART module to stabilize

  for (i=0; i<2; i++){
        RB7_bit = 1;
        Delay_ms(100);
        RB7_bit = 0;
        Delay_ms(100);
  }

  while (1) {                          // Endless loop

        if (UART1_Data_Ready()) {      // If data is received,
        output[0] = 0;                 // Clean the variable that recieves UART
        output[1] = 0;
        output[2] = 0;
        output[3] = 0;
        output[4] = 0;
        output[5] = 0;
        output[6] = 0;
        output[7] = 0;
        output[8] = 0;
        output[9] = 0;
        UART1_Read_Text(output, "OK", 7);     // reads text until 'OK' is found

              RB7_bit = 1;
              Delay_ms(200);
              RB7_bit = 0;
              Delay_ms(200);

          if (output[0] == 'A'){                  // função de abrir o shutter
               funcao = 0;
          }
          else if (output[0] == 'D'){             // decrementa x passos
               passos = (output[1]-'0')*1000+(output[2]-'0')*100+(output[3]-'0')*10+(output[4]-'0');
               funcao = 1;
          }
          else if (output[0] == 'I'){             // incrementa x passos
               passos = (output[1]-'0')*1000+(output[2]-'0')*100+(output[3]-'0')*10+(output[4]-'0');
               funcao = 2;
          }
          else if (output[0] == 'F'){             // fecha shutter
               funcao = 3;
          }
          else if (output[0] == 'S'){             // erro
               funcao = 4;
          }
          else if (output[0] == 'P'){             // posiciona sensor
               funcao = 5;
          }
          else if (output[0] == 'R'){             // retira sensor
               funcao = 6;
          }
          else{
            for (i=0; i<2; i++){        // pisca indicando erro para qualquer outra letra
              RB7_bit = 1;
              delay_ms(200);
              RB7_bit = 0;
              delay_ms(200);
              }
          }

          //aqui chama cada uma das funções
          
          if (funcao == ABRIR_SHUTTER){
             RB6_Bit = 1;                //aciona ledshutter e envia sinal shutter
             RB7_bit = 1;                //pisca ledcom 1seg 1x
             delay_ms(300);
             RB7_bit = 0;
             delay_ms(300);
          }

          else if (funcao == INCREMENTA_PASSOS){
               RB7_bit = 1;
               Delay_ms(200);
               RB7_bit = 0;
               Delay_ms(200);
               for (j=0;j<passos;j++){
                   if (UART1_Data_Ready()) {
                     UART1_Read_Text(output, "OK", 7);
                       if (output[0] == 'S'){             // erro
                       funcao = 4;
                       j = passos;
                       }
                     }
                   if (i==0){
                      i=1;
                      PORTB = Passo1d;
                   }
                   else if (i==1){
                      i=2;
                      PORTB = Passo2d;
                   }
                   else if (i==2){
                      i=3;
                      PORTB = Passo3d;
                   }
                   else {
                      i=0;
                      PORTB = Passo4d;
                   }
                   delay_ms(20);  // ATRASO ENTRE OS PASSOS
               }
               RB7_bit = 1;
               Delay_ms(200);
               RB7_bit = 0;
               Delay_ms(200);
          }

          else if (funcao == DECREMENTA_PASSOS){
               RB7_bit = 1;
               Delay_ms(200);
               RB7_bit = 0;
               Delay_ms(200);
               for (j=0;j<passos;j++){
                   if (UART1_Data_Ready()) {
                     UART1_Read_Text(output, "OK", 7);
                       if (output[0] == 'S'){             // erro
                       funcao = 4;
                       j = passos;
                       }
                     }
                   if (i==0){
                      i=1;
                      PORTB = Passo1d;
                   }
                   else if (i==1){
                      i=2;
                      PORTB = Passo4d;
                   }
                   else if (i==2){
                      i=3;
                      PORTB = Passo3d;
                   }
                   else {
                      i=0;
                      PORTB = Passo2d;
                   }
                   delay_ms(20);  // ATRASO ENTRE OS PASSOS
               }
               RB7_bit = 1;
               Delay_ms(200);
               RB7_bit = 0;
               Delay_ms(200);
          }

          else if (funcao == FECHA_SHUTTER){
                RB6_bit = 0;
                for (i=0; i<2; i++){
                  RB7_bit = 1;
                  Delay_ms(100);
                  RB7_bit = 0;
                  Delay_ms(100);
                }
          }

          else if (funcao == STOP){
                RB6_bit = 0;
                passos =0;
          }

          else if (funcao == POSICIONA_SENSOR){
               while (FIM_DE_CURSO1 == 1 && funcao == POSICIONA_SENSOR){
                     if (UART1_Data_Ready()) {
                     UART1_Read_Text(output, "OK", 7);
                       if (output[0] == 'S'){             // erro
                       funcao = 4;
                       break;
                       }
                     }
                      if (i==0){
                          i=1;
                          RD0_bit = 0;
                          RD1_bit = 1;
                          RC2_bit = 1;
                          RC3_bit = 0;
                       }
                       else if (i==1){
                          i=2;
                          RD0_bit = 1;
                          RD1_bit = 1;
                          RC2_bit = 0;
                          RC3_bit = 0;
                       }
                       else if (i==2){
                          i=3;
                          RD0_bit = 1;
                          RD1_bit = 0;
                          RC2_bit = 0;
                          RC3_bit = 1;
                       }
                       else {
                          i=0;
                          RD0_bit = 0;
                          RD1_bit = 0;
                          RC2_bit = 1;
                          RC3_bit = 1;

                       }
                       delay_ms(23);
               }

                 if (FIM_DE_CURSO1 == 0){
                 RB7_bit = 1;
                 Delay_ms(200);
                 RB7_bit = 0;
                 Delay_ms(200);
                 }
          }

          else if (funcao == RETIRA_SENSOR){
               while (FIM_DE_CURSO2 == 0 && funcao != 4){
                     if (UART1_Data_Ready()) {
                     UART1_Read_Text(output, "OK", 7);
                       if (output[0] == 'S'){             // erro
                       funcao = 4;
                       break;       // Clean the variable that recieves UART
                       }
                     }
                     
                   if (i==0){
                      i=1;
                      RC2_bit = 1;
                      RC3_bit = 0;
                      RD0_bit = 0;
                      RD1_bit = 1;
                   }
                   else if (i==1){
                      i=2;
                      RC2_bit = 1;
                      RC3_bit = 1;
                      RD0_bit = 0;
                      RD1_bit = 0;
                   }
                   else if (i==2){
                      i=3;
                      RC2_bit = 0;
                      RC3_bit = 1;
                      RD0_bit = 1;
                      RD1_bit = 0;

                   }
                   else {
                      i=0;
                      RC2_bit = 0;
                      RC3_bit = 0;
                      RD0_bit = 1;
                      RD1_bit = 1;

                   }
                   delay_ms(23);
                 }

                   if (FIM_DE_CURSO2 == 0){
                      RB7_bit = 1;
                      Delay_ms(200);
                      RB7_bit = 0;
                      Delay_ms(200);
                   }
              }
        }
    }
}