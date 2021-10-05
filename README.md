# Abigail Miranda - Lavaropa
# PIC18F4550 │ Lenguaje C │ Mplab & compilador XC8 
# Codigo en el archivo main.c │ simulacion del codigo con el archivo simulide

#Informacion sobre el codigo:
-Realizada en base a una maquina de estado.
Los valores de tiempo extendidos fueron cambiados por otros de mas corta duracion para el rapido chequeo de funcionamiento.
1- Si se recibe la orden de lavar con agua caliente (TECLA 2) se debe activar la resistencia que caliente el agua por 2 minutos desactivando la posibilidad de comenzar el lavado. 

2- Al recibir la orden de cominezo START se debera sensar el peso de la ropa para determinar la cantidad de agua y jabon que suiministrará.
Los mismos tienen 3 niveles:BAJO, MEDIO, ALTO (5-10KG,10KG-15KG, 15-20KG) respectivamente. Para el sensado se utilizó el conversor AD del PIC18F4550 configurado en 10 BITS
y con una tenison de referencia de entre 0 y 5v e implementarla en una funcion readADC() que devuelva un unsigned char con el peso en KG. El sensor entrega 10mv cada 100gr.

3- Activar los actuadores para cargar el agua y el jabon. Los 3 niveles se lograran con el tiempo de encendido de cada actuador:

  nivel     tiempo
  bajo      activar 5 seg cada actuador
  medio     activar 10 seg cada actuador
  alto      activar 20 seg cada actuador
  
4- Realizar el lavado. Esta accion se realiza activando el motor 30 segundos en sentido horario y 30 segundo en sentido antihorario por un lapso de 10 minutos.

5. activar la bomba de desagote durante 10 seg

6- Realizar el enjuague. Se realiza idem al punto 3 y 4 solo que no se deposita jabon.

7- Realizar el desagote. Idem punto 5.

8- Realizar el centrifugado: se realiza activando durante 5 min el motor en un unico sentido.

el circuito conta con un teclado matricial, pic, pantalla lcd para visualizar los datos, potenciometro para simular el peso de la ropa y leds para simular los actuadores.
