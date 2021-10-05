/**************************
 * File:   main.c                                                           *
 * Author: Esteban Lemos                                                    *
 * Created on 5 de febrero de 2014, 20:49                                   *
 * En caso de necesitar el proyecto completo ingresar en                    *
 * https://github.com/EEST1/Plantilla_6to                                   *
 **************************/


// PRORAMACION ACTUAL SOLO EJERCICIOS 1 2 3 4 Y 5 PRINCIPAL!!!!                         PLANTILLA TP //
#include <xc.h>
#include "confbits.h"       //no se debe de modificar este archivo para
//garantizar el funcionamiento del BOOTLOADER

#include "Tecnica1.h"       //segun el shield habilita distintas librerias

#include "ConfShield2_1.h" //cambiar para el teclado // esta viene x defecto
//el teclado utiliza los mismos pines que el entrenador pero la
//incializaci´on esta disponible en el Shield1.3 no en el 2.1.
#include "ConfShield1_3.h"
#include "lcd.h"
#include "teclado.h"
#include "EncoderIncremental.h"

#define MOTOR_ANTI  LATDbits.LD1 // e cambio el nombre para que sea mas facil trabajar con los leds
#define MOTOR_HORA  LATDbits.LD0
#define AGUA LATCbits.LC0
#define JABON LATCbits.LC1
#define LAVANDINA LATCbits.LC2
#define DESAGOTE LATAbits.LA2
#define CALEFACTOR LATAbits.LA5

long int cont_inicio = 7; //seg
long int flag_inicio = 0; // un falg que activa el contador de inicio etapaq 1
long int flag_etapa2 = 0; // un flag que activa la etapa 2
long int guarda_ADC;
long int kilos;
long int dec, uni;
long int multi=0;
long int bajo=5, medio=10, alto=20, flag2=0, flag3=0, flag4=0;//seg
long int flag5=0, flag6=0, flag7=0;


long int a=5, b=5, c=15, flag=0, fb=0, fa=0; // ETAPA 4 flags y contadores que uso para la etapa 4
long int cuatro=0; // ETAPA 4

long int flagdes=0, contdes_a=10; // ETAPA 5 flag y contador para la etapa 5
long int flag_siete=0, cont_siete=10; // ETAPA 7 flag y contador para la etapa 7


long int bajo3=5, medio3=10, alto3=20, t1=0, t2=0, t3=0; //etapa 6 que al final ni lo use pq no me anda el simulide con esto

long int cent=3,y1=0; //same que lo de arriba


long int flagbajo=0, seisbajo=5,flagmedio=0, seismedio=10, flagalto=0, seisalto=20, dec, uni; //ETAPA 6 flags y cont etapa 6
void pesobajoetapa6(void);//ETAPA 6 funciones que uso
void pesomedioetapa6(void);//ETAPA 6
void pesoaltoetapa6(void); //ETAPA 6

void casos (void); // etapa 4

void centrifugado(void); //etapa 8 al final creo que ni la uso pq el multi no hago que al final llegue a 8

void pesobajo(void);//usar normalmente definido para la etapa 3
void pesomedio(void);
void pesoalto(void);

void pesobajo2(void);// si telca lavandina fue presionada antes al final ni la use.
void pesomedio2(void);
void pesoalto2(void);

void pesobajo3(void);//para etapa 6 solo agua!! pero al final ni la use .
void pesomedio3(void);
void pesoalto3(void);

void principal(void);//protipo switch principal
void etapa5(void); //prototipo de etapa5 que contiene una maquina de estados al igual que la etapa4 al final no la use
void estapa1(void); //prototipo funcion etapa 1 sal final no la use
void descomponer(void); //prototipado de la funcion. para descomponer unos numeros y mostrarlos en el lcd que al final ni use
void etapa8(void);// creo que no lo use

void etapa4(void); //prototipos etapa 4 no lo use
void anti(void); //no lo use
void enjuague(void); //no lo use
 void desagotee(void);// no lo use

long int readADC(void);//funcion que devuelve el valor en kilos del adc.

void main(void) {

    PicIni21(); // activar todo para usar el micro del LCD

    TRISDbits.RD1 = 0; // defino estos pines como salidas
    TRISDbits.RD0 = 0;
    TRISCbits.RC0 = 0;
    TRISCbits.RC1 = 0;
    TRISCbits.RC2 = 0;
    TRISAbits.RA2 = 0;
    TRISAbits.RA5 = 0;

    TIMER0_INI(); //llamo a la funcion tmimer 0
    TMR0ON = 1; //lo prendo
    TIMER1_INI();
    TMR1ON = 1;
    ei(); //habilito las interrupciones
    tecladoIni13(); //inicializo teclado y conexiones etc

    LCD_init(); //inicializo el lcd;   



    while (1) {
        
        principal(); // luego de la etapa 1 2 3 4 y 5. No me deja seguir mas por eso lo guarde en abi escuela<digitales<junio<nuevas pruebas<abi<main.c
   
    }
}

long int readADC() { //hago una funcion que devuelve el valor de kilos transferido por el potenciometro
    guarda_ADC = obtener_ADC10(); //guardo el valor del adc10 en una variable
    kilos = (guarda_ADC * 50) / 1023; //hago la cuenta. explicado en la otra carpeta...
    //// para esto hago la regla de 3simples.
    //si 10mv son 100gr. 5v son 50000gr, o sea, 50kilos.
    //entonces si 50kg son 1023bits (pq ahora el conversor está en 10 bits tengo 1023 posiciones))
    // (x*50kg)/1023= valor de bits 
    
    return kilos;//devuelvo el valor en la variable kilos
}


void __interrupt myISR(void) {
    
    //TIMMER 1 PARA SEGUNDOS
    if (TMR0IF == 1) { //termino termino de contar el timer?
        TMR0IF = 0; //bajamos el flag
        TMR0L = 0xD2; //el timer contará 47 fosc/4 * 256 = 12032 * 0,0833us
        TMR0H = 0x48; //en total aprox 1.0022ms  casi 1ms
          //    des =~ des;
        //casi todo o todo esta definido a partir de u if si flag es ifgual a 1 pq sino los contadores
        //van a empezar a contar a partir de que prenda la maquina destado. entonces con esto defino que
        //comiencen a contar cuando el flag esté en 1, y eso lo defino recien
        //cuando entro al caso y ahi lo activo, asi recien ahi empieza a contar
if (flag_inicio == 1) { //si la tecla 1 fue presionada, el falg se pone en 1 simulando que aun esta presionada
            if (cont_inicio != 0) { //s es diferente a 0 decrementa!!
                cont_inicio--;
            }
        }
        
        if(flag2==1){
            if(bajo!=0){
                bajo--;
            }
        }
        
        if(flag3==1){
            if(medio !=0){
                medio--;
            }
        }
        
        if(flag4==1){
            if(alto !=0){
                alto--;
            }
        }
        
//        //etapa 4---------------
         if(flag==1){
            if(c!=0){
                c--;
            }
        }
        
         if(fa==1){
            if(a!=0){
                a--;
            }
        }
        
         if(fb==1){
            if(b!=0){
                b--;
            }
        }
        
        //etapa 4-------------------//
        
        
     // ETAPA 5//
          //============ETAPA 5============//
        if(flagdes==1){
            if(contdes_a!=0){
                contdes_a--;
            }
        }
        
   //=============ETAPA 5===============//
        
//        
       //========etapa 6===========//
                if(flagbajo==1){
          if(seisbajo!=0){
                seisbajo--;
            }
          
        }
        if(flagmedio==1){
          if(seismedio!=0){
                seismedio--;
            }
          
        }
        if(flagalto==1){
          if(seisalto!=0){
                seisalto--;
            }
          
        }
        //==============================//
        //etapa 7=========//

        if(flag_siete==1){
            if(cont_siete !=0){
                cont_siete--;
            }
        }
        //=============ni los use=================//
        if(t1==1){
            if(bajo3 !=0){
                bajo3--;
            }
        }
        
        if(t2==1){
            if(medio3 !=0){
                medio3--;
            }
        }
        
        if(t3==1){
            if(alto3 !=0){
                alto3--;
            }
        }

        //etapa 8-----creo que ni los use
         if(y1==1){
            if(cent !=0 )cent--;
        }
        
    }

    if (TMR1IF == 1) { //TIMER 1 PARA ms
        TMR1IF = 0; //bajamos el flag
        TMR1L = 0x1B; //el timer contará 1200 fosc/4 * 1 = 12032 * 0,0833us
        TMR1H = 0xD1; //en total aprox 99.96us  casi 100us
        tic_teclado();
        tic_LCD();
        if (RBIF == 1) { //si RBIF es 1
            marca_tecla(); //llamo a la funcion marca_tecla
        }
        RBIF = 0;
        
        
    }

}



void principal() {//declaro la función multiplexado(quiero mostrar número 13)
    switch (multi) {  //switch respecto a multi
        case 0://primer caso(DISPLAY1)
        { //etapa 1 2 y 3
             if (tecla() == 2) { // si toco la tecla 2
            flag_inicio = 1; //me ayudo a simular como que queda presionada
            CALEFACTOR = 1; //enciendo el calefactor
            set_CURSOR(0x01); //escribo en el lcd
            msg2LCD("##CALENTANDO##");
        }
        if (tecla() == 1) { //si la tecla 1 fue presionada
            if (cont_inicio == 0) { // y si pasaron los 2 minutos. haciendo caso al punto 1 que dice que se puede activar el leido del sensor solo despus de que pasaron el tiempo en que prende y se apaga el calefactor
                flag_etapa2 = 1;//simulo como que la tecla 2 sigue presionada
                clear_LCD(); //limpio el lcd
            }
        }

        if (cont_inicio == 0 && flag_etapa2 == 0) { //pregunto de nuevo si el cont de la resistencia 2min llego a 0 y la tecla 1 fue presionada(SIMULO))
            CALEFACTOR = 0; //apago el calefactor
            flag_inicio = 0; //velvo el falg a 0
            set_CURSOR(0x01); // escribo en el lcd
            msg2LCD("##FINALIZADO##");
        }

if (flag_etapa2 == 1) { //si la tecla 1 esta presionada SIMULACION // ahi si entro a la etapa 2
            set_CURSOR(0x00);
            msg2LCD("Peso sensado:");
            dec = readADC() / 10; //descompongo el numero y lo muestro en el lcd
            uni = readADC() % 10;
            set_CURSOR(0x40);
            char2LCD(dec + 48);
            char2LCD(uni + 48);
            msg2LCD("Kg");
        }
             //------si presiono la tecla 3 antes---------//
            //             if(tecla()==3){
            //                if(kilos>=5 && kilos<10){
            //                 multi=2;
            //             }
            //             if(kilos>=10 && kilos<15){
            //                 multi=2;
            //             }
            //             if(kilos>=15 && kilos<20){
            //                 multi=2;
            //             } 
            //             }
             
             //-----------etapa 3, si kilos está entre estos valores paso a la siguiente etapa
             //paso al siguiente caso donde estás definidas las funciones--------------//
             if(kilos>=5 && kilos<10){ //esto es en caso de la etapa 3
                 multi=1; //paso al siguiente caso
             }
             if(kilos>=10 && kilos<15){ 
                 multi=1;
             }
             if(kilos>=15 && kilos<20){
                 multi=1;
             }
        }
            break;//si entro a este caso, significa que los demás caso no es
             //entonces "rompe", y no entra en los siguientes casos 
            // sino que los ignora a todos.
        case 1:// etapa 3!!
        {
            pesobajo(); // si kilos está entre 5 y 10kg entrá acá. al finalizar esta funcion pasa al siguiente caso
            pesomedio();///si kilos esta entre 10 y 15 lo mismo 
            pesoalto(); //same

        }
            break;

        case 2:
        {
            set_CURSOR(0x00); // al final ni se ve pq tiene que pasar al siguiente estado que es el de los casos o sea la etapa 4
            msg2LCD(" AGUA Y JABON  ");
            set_CURSOR(0x40);
            msg2LCD(" CARGADOS");
                    multi=3;

            }
          
            break;
            
            case 3:
    {
        casos();//etapa 4 dentro de esta funcion al terminar devuelvo al caso siguiente
        
    }
            break;
            
        case 4: //DESAGOTE  ETAPA 5
        {
             flagdes=1; // comienza a contar cuando llego a esta etapa
            if(contdes_a!=0){ // si no llego a 0
                DESAGOTE=1; // lo prendo
               set_CURSOR(0x00); // escribo en el lcd
        msg2LCD("DESAGOTANDO...");
            }
            if(contdes_a==0){ // si llegó a 0
                DESAGOTE=0; // lo apago
                clear_LCD(); //limpio pantalla
                                             multi=6; // paso al siguente caso , igual acà hay que dirigirse a la otra carpea donde tengo las definiciones de las otras etapas que siguen que aca nose pq no funcionan

            }

        }
        break;
        
        case 5:  //ETAPA 6 no se puede simular! ver archivo NUEVAS PRUEBAS 
        {
//             set_CURSOR(0x00);
//            msg2LCD("PESO TOTAL: ");
//            dec = readADC() / 10; //descompongo el numero y lo muestro en el lcd
//            uni = readADC() % 10;
////            set_CURSOR(0x40);
//            char2LCD(dec + 48);
//            char2LCD(uni + 48);
//            msg2LCD("Kg");
//       
//            if(kilos>=5 && kilos<10){      
//                pesobajoetapa6();
//            }
//            if(kilos>=10 && kilos<15){
//                pesomedioetapa6();
//            }
//            if(kilos>=15 && kilos<20){
//                pesoaltoetapa6();
//            }
//        
//           
            multi=6;
        }
        break;
        
        case 6:
        {
             flag_siete=1; // comienza a contar cuando llego a esta etapa
            if(cont_siete!=0){ // si no llego a 0
                DESAGOTE=1; // lo prendo
               set_CURSOR(0x00); // escribo en el lcd
        msg2LCD("DESAGOTANDO...");
            }
            if(cont_siete==0){ // si llegó a 0
                DESAGOTE=0; // lo apago
                clear_LCD(); //limpio pantalla
//                multi=7;
            }
        }
            
    }
    

}

void pesobajo() {
    if (kilos >= 5 && kilos < 10 && flag2 == 0 && tecla()==5) { // le agrego la funcion de que esto pase cuando aprete la tecla  pq sino lo toma automaticamente y no me deja ver el valor de kg
        clear_LCD();
        flag2 = 1;
        AGUA = 1;
        JABON = 1;
        set_CURSOR(0x01);
        msg2LCD("## CARGANDO ##");
        set_CURSOR(0x40);
        msg2LCD("       ");
    }
    if (bajo == 0) {
        AGUA = 0;
        JABON = 0;
        flag2 = 0;
        multi=2;
    }
}

void pesomedio() {
    if (kilos >= 10 && kilos < 15 && flag3 == 0 && tecla()==5) {
        clear_LCD();
        flag3 = 1;
        AGUA = 1;
        JABON = 1;
        set_CURSOR(0x01);
        msg2LCD("## CARGANDO ##");
         set_CURSOR(0x40);
        msg2LCD("       ");
    }
    if (medio == 0) {
        AGUA = 0;
        JABON = 0;
        flag3 = 0;
        multi=2;
    }
}

void pesoalto() {

    if (kilos >= 15 && kilos < 20 && flag4 == 0 && tecla()==5) {

        flag4 = 1;
        AGUA = 1;
        JABON = 1;
        set_CURSOR(0x01);
        msg2LCD("## CARGANDO ##");
         set_CURSOR(0x40);
        msg2LCD("       ");
    }
    if (alto == 0) {
        AGUA = 0;
        JABON = 0;
        flag4 = 0;
        multi=2;
    }
}

// =======etapa 6============//

  void pesobajoetapa6() {
    if (kilos >= 5 && kilos < 10 && flagbajo == 0) {
        clear_LCD();
        flagbajo = 1;
        AGUA = 1;
        JABON = 1;
//        set_CURSOR(0x01);
//        msg2LCD("## CARGANDO ##");
//        set_CURSOR(0x40);
//        msg2LCD("       ");
    }
    if (seisbajo == 0) {
        AGUA = 0;
        JABON = 0;
        flagbajo = 0;
        LAVANDINA=1;
        //multi=1;
        
    }
}
  
    void pesomedioetapa6() { // 
    if (kilos >= 10 && kilos < 15 && flagmedio == 0) {
    clear_LCD();
    flagmedio = 1;
    AGUA = 1;
    JABON = 1;
    //        set_CURSOR(0x01);
    //        msg2LCD("## CARGANDO ##");
    //        set_CURSOR(0x40);
    //        msg2LCD("       ");
    }
    if (seismedio == 0) {
    AGUA = 0;
    JABON = 0;
    flagmedio = 0;
   // multi=1;

    }
    }
                    
    void pesoaltoetapa6() {
    if (kilos >= 15 && kilos < 20 && flagalto == 0) { 
    clear_LCD();
    flagalto = 1;
    AGUA = 1;
    JABON = 1;
    //        set_CURSOR(0x01);
    //        msg2LCD("## CARGANDO ##");
    //        set_CURSOR(0x40);
    //        msg2LCD("       ");
    }
    if (seisalto == 0) {
    AGUA = 0;
    JABON = 0;
    flagalto = 0;
  //  multi=1;

    }
    }

//void pesoalto2() {
//        if (kilos >= 15 && kilos < 20 && flag4 == 0 && alto != 0) {
//
//            flag4 = 1;
//            alto--;
//            AGUA = 1;
//            JABON = 1;
//            LAVANDINA = 1;
//        }
//        if (alto == 0) {
//            AGUA = 0;
//            JABON = 0;
//            LAVANDINA = 0;
//            flag4 = 0;
//        }
//    
//}

//void etapa4() {
//
//    if(horario !=0 && f==0){
//        f=1;
//        MOTOR_HORA=1;
//    }
//    if (horario==0){
//        MOTOR_HORA=0;
//        horario=30;
//    }
//
//}
//
//
//
////dos=5 flag2=0
//void anti(){
//    
//    if(antihor !=0 && f2==0){
//        f2=1;
//        MOTOR_ANTI=1;
//    }
//    if (antihor==0){
//        MOTOR_ANTI=0;
//        antihor=30;
//    }
//}
//
//void enjuague (){
//    etapa4();
//        if(a==0){
//            anti();
//        }
//}

//void desagotee() {//declaro la función multiplexado(quiero mostrar número 13)
//    switch (cinco) {  //switch respecto a multi
//        case 0://primer caso(DISPLAY1)
//        {
//              if (desag != 0) { // si a no llegó a 0
//
//                            f3 = 1; // activo el flag para qu recien ahi comeince el temporizador de ese led
//                            DESAGOTE = 1; // activo el led
//                            set_CURSOR(0x01); //escribo en el lcd
//                            msg2LCD(" ## DESAGOTE ## ");
//                            set_CURSOR(0x41);
//                            msg2LCD("        ");
//                        }
//                        if (desag == 0) { // si llegó a 0
//                            f3 = 0; // pongo el flag en 0 para que deje de contar
//                            clear_LCD(); // limpio pantalla lcd
//                            cinco = 1; // paso al siguiente caso
//                        }
//
//        }
//            break;//si entro a este caso, significa que los demás caso no es
//             //entonces "rompe", y no entra en los siguientes casos 
//            // sino que los ignora a todos.
//        case 1://segundo caso(DISPLAY 2)
//        {
//              if (desag == 0) { // si el contador llego a 0
//                            DESAGOTE = 0; // apago el lec
//                            clear_LCD(); //limpio pantalla
//                            
//                            //paso al sguiente caso
//                        }
//        }
//        break;
//    }
//}


//
//void desagotee(){
//    
//    if(desag !=0 && f3==0){
//        f3=1;
//        DESAGOTE=1;
//    }
//    if (desag==0){
//        DESAGOTE=0;
//        desag=30;
//    }
//}

// estas funciones de abajo ni las use

void pesobajo3() {
    if (kilos >= 5 && kilos < 10 && t1 == 0 && bajo3 != 0) {
        t1 = 1;
        bajo3--;
        AGUA = 1;
    }
    if (bajo3 == 0) {
        AGUA = 0;
        t1 = 0;
        multi=5;
    }
}


void pesomedio3() {
    if (kilos >= 5 && kilos < 10 && t2 == 0 && medio3 != 0) {
        t2 = 1;
        medio3--;
        AGUA = 1;
    }
    if (medio3 == 0) {
        AGUA = 0;
        t2 = 0;
        multi=5;
    }
}


void pesoalto3() {
    if (kilos >= 5 && kilos < 10 && t3== 0 && alto3 != 0) {
        t3 = 1;
        alto3--;
        AGUA = 1;
    }
    if (alto3 == 0) {
        AGUA = 0;
        t3 = 0;
        multi=5;
    }
}

//esto tmp ni lo use
void centrifugado(){
    
    if(cent !=0 && y1==0){
        y1=1;
        MOTOR_ANTI=1;
    }
    if (cent==0){
        MOTOR_ANTI=0;
        cent=3;
    }
}

//==================== ETAPA 4 =============//
void casos() {//etapa 4

    if (c != 0) { // si c no llegó a 0 o sea que si los 10 min o cualquier valor de tiempo todavia no terminaron
        flag = 1; // activo el flaf para que ahi comience el temporizador. recien ahora coienzo a rotar el encendido y apagado de los leds
        switch (cuatro) { //switch respecto a multi
            case 0://primer caso(enciendo el led 1)
            {
                if (a != 0) { // si a no llegó a 0. este contador esta cargado a 5seg

                    fa = 1; // activo el flag para qu recien ahi comeince el temporizador de ese led
                    MOTOR_HORA = 1; // activo el led
                    set_CURSOR(0x01); //escribo en el lcd
                    msg2LCD("MOTOR HORARIO");
                    set_CURSOR(0x41);
                    msg2LCD("LAVANDO...");
                } // si llego a 0 paso al otro caso donde lo apago. same en caso 2
                if (a == 0) { // si llegó a 0
                    fa = 0; // pongo el flag en 0 para que deje de contar
                    clear_LCD(); // limpio pantalla lcd
                    cuatro = 1; // paso al siguiente caso
                }

}
                break; //si entro a este caso, significa que los demás caso no es
                //entonces "rompe", y no entra en los siguientes casos 
                // sino que los ignora a todos.
            case 1://segundo caso
            {
                if (a == 0) { // si el contador llego a 0
                    MOTOR_HORA = 0; // apago el lec
                    clear_LCD(); //limpio pantalla
                    a = 5; // cargo el contador a 5 paqra la proxima vez
                    cuatro=2; //paso al sig caso
                    //paso al sguiente caso
                }
            }
                break;

            case 2:
            {
                if(b!=0){ // si el cont b no llego a 0
                    fb=1; // activo el cont de este led
                    MOTOR_ANTI=1; // enciendo el led
                    set_CURSOR(0x01); //escribo en el lcd
                    msg2LCD("MOTOR ANTI");
                    set_CURSOR(0x41);
                    msg2LCD("LAVANDO...");
                }
                if(b==0){ // si b llegó a 0
                    fb=0; //apago el flag
                    cuatro=3; //paso al sig caso
                    clear_LCD(); // limpio pantalla
                }
            }
            break;

            case 3:
            {
                if (b == 0) { // si llegó a 0
                    MOTOR_ANTI = 0; // apago el led
                    clear_LCD(); //limpio pantalla
                    b = 5; //cargo para la proxiima vez
                    cuatro=0; //vuelvo al primer caso

                }
            }
        }
    }
    if(c==0){ // pero x fuera del suich digo que si el contador principal de los 10min llegó a 0
        flag=0; //entonces apago el flag para que no entre a la maquina de estados
        MOTOR_HORA=0; // y apago los leds x las dudas
        MOTOR_ANTI=0;
        set_CURSOR(0x01); //escribo en el lcd
        msg2LCD("## LAVADO ##");
        set_CURSOR(0x42);
        msg2LCD("FINALIZADO");
        multi=4; // vuelvo a la maquina de estados principal.
    }
}