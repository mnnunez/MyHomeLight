/*
 * HomeLightProyect.c
 *
 * Created: 1/24/2022 12:17:08 AM
 * Author : mnnun
 */ 

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include "DEF_ATMEGA328P.h"
#include "LCD.h"
//DEFINICIÓN DE LOS PINES PARA LAS COLUMNAS Y SU POSTERIOR MANEJO
#define COLUMNA1	PINCbits.PINC0_
#define COLUMNA2	PINCbits.PINC1_
#define COLUMNA3	PINCbits.PINC2_
#define COLUMNA4	PINCbits.PINC3_
//DEFINICIÓN DE LOS PINES PARA LAS FILAS Y SU POSTERIOR MANEJO
#define FILA1		PORTBbits.PB0_
#define FILA2		PORTBbits.PB1_
#define FILA3		PORTBbits.PB2_
#define FILA4		PORTBbits.PB3_

//CLAVES PREDEFINIDAS PARA EL ACCESO
uint8_t clave[]="0A8C"; //uint8_t ENTERO DE 8BITS
uint8_t clave1[]="A11B";
uint8_t codigo[4];

uint8_t contador;
unsigned char tabla[16]={
	'1', '2', '3', 'A',
	'4', '5', '6', 'B',
	'7', '8', '9', 'C',
'*', '0', '#', 'D' };

//FUNCION PARA INICIALIZAR EL TECLADO QUE INICIALIZAN LOS PINES QUE ESTARÁN CONECTADOS
void KEYPAD_init(void)
{
	//CONFIGURACIÓN DE LOS PINES DE LAS FILAS COMO SALIDAS
	DDRBbits.DDRB0 = 1;
	DDRBbits.DDRB1 = 1;
	DDRBbits.DDRB2 = 1;
	DDRBbits.DDRB3 = 1;
	//CONFIGURACIÓN DE LOS PINES DE LAS COLUMNAS COMO ENTRADAS
	DDRCbits.DDRC0 = 0;
	DDRCbits.DDRC1 = 0;
	DDRCbits.DDRC2 = 0;
	DDRCbits.DDRC3 = 0;
	//HABILITACIÓN DE LAS RESISTENCIAS INTERNAS DE PULL-UP DEL MICROCONTROLADOR
	//PULL-UP, SI NO SE PRESIONA UNA TECLA, ESTADO LÓGICO ALTO
	PORTCbits.PC0_ = 1;
	PORTCbits.PC1_ = 1;
	PORTCbits.PC2_ = 1;
	PORTCbits.PC3_ = 1;
}

uint8_t COLUMNA_scan(void)
{


	//BARRIDO DE LA COLUMNA 1 PARA SABER QUE TECLA ESTÁ PRESIONADA
	if(COLUMNA1 == 0)
	{
		//SE ESPERA A QUE SE DEJE DE PRESIONAR EL BOTÓN EN CASO QUE EL USUARIO LO MANTENGA PRESIONADO
		while(COLUMNA1 == 0 || COLUMNA2 == 0 || COLUMNA3 == 0 || COLUMNA4 == 0);
		return 1;
	}
	contador++;
	
	//BARRIDO DE LA COLUMNA 2 PARA SABER QUE TECLA ESTÁ PRESIONADA
	if(COLUMNA2 == 0)
	{
		while(COLUMNA1 == 0 || COLUMNA2 == 0 || COLUMNA3 == 0 || COLUMNA4 == 0);
		return 1;
	}
	contador++;
	
	//BARRIDO DE LA COLUMNA 3 PARA SABER QUE TECLA ESTÁ PRESIONADA
	if(COLUMNA3 == 0)
	{
		while(COLUMNA1 == 0 || COLUMNA2 == 0 || COLUMNA3 == 0 || COLUMNA4 == 0);
		return 1;
	}
	contador++;
	
	//BARRIDO DE LA COLUMNA 4 PARA SABER QUE TECLA ESTÁ PRESIONADA
	if(COLUMNA4 == 0)
	{
		while(COLUMNA1 == 0 || COLUMNA2 == 0 || COLUMNA3 == 0 || COLUMNA4 == 0);
		return 1;
	}
	contador++;
	
	return 0;
	
}


void FILA_init(uint8_t fila)
{
	FILA1 = 1;
	FILA2 = 1;
	FILA3 = 1;
	FILA4 = 1;
	
	switch(fila)
	{
		case 1: FILA1 = 0; break;
		case 2: FILA2 = 0; break;
		case 3: FILA3 = 0; break;
		case 4: FILA4 = 0; break;
	}
	asm("NOP");
}

uint8_t KEYPAD_read(void)
{
	_delay_ms(10);
	contador= 0;
	
	//Barrido de fila1
	FILA_init(1);
	if(COLUMNA_scan())
	return tabla[contador];

	//Barrido de fila2
	FILA_init(2);
	if(COLUMNA_scan())
	return tabla[contador];
	
	//Barrido de fila3
	FILA_init(3);
	
	if(COLUMNA_scan())
	return tabla[contador];

	//Barrido de fila4
	FILA_init(4);
	if(COLUMNA_scan())
	return tabla[contador];
	
	return 0;
	
}

int main(void)
{
	LCD_init(); //Inicializamos la LCD
	LCD_printf("   MY HOME LIGHT   "); //Enviamos mensaje hacia la LCD
	_delay_ms(300);
	LCD_cmd(0x01); //Limpiamos la pantalla LCD
	LCD_printf("INGRESE LA CLAVE:"); //Enviamos mensaje hacia la LCD
	LCD_cmd(0xC0); //Comando para que se escriba en la 2da línea de la LCD
	
	
	
	KEYPAD_init(); //Inicializando el teclado matricial
	int contadorClave=0;
	
	/* Replace with your application code */
	while (1)
	{
		uint8_t dato = KEYPAD_read();
		
		//VALIDANDO QUE SE INGRESE UNA CLAVE DE 4 DIGITOS
		//ANTIREBOTE CON IF, ESPERA QUE DEJE DE PRESIONAR PARA MOSTRAR LA TECLA PRESIONADA EN LA LCD
		if (dato != 0){
			codigo[contadorClave]=dato; //GUARDO LA TECLA PRESIONADA EN LA VARIABLE CODIGO
			LCD_dato(dato); //IMPRIMO EN LA LCD LAS TECLAS PRESIONADAS
			contadorClave++; //AUMENTO EL CONTADOR DE LA CLAVE
		}
		if(contadorClave==4){
			_delay_ms(150);
			LCD_cmd(0x01); //LIMPIO LOS CARACTERES DE LA LCD
			LCD_printf("HA INGRESADO:"); // MENSAJE EN LA 1ERA LINEA DE LA LCD
			LCD_cmd(0xC0); // MENSAJE EN LA 2DA LINEA DE LA LCD
			for(int m=0; m<4; m++){
				LCD_dato(codigo[m]); // MUESTRO LOS CARACTERES INGRESADOS POR TECLADO EN LA LCD
				_delay_ms(50);
			}
			
			_delay_ms(100);
			LCD_cmd(0x01); //Limpiando la pantalla LCD
			//VALIDACIÓN DE CONTRASEÑA CORRECTA O INCORRECTA
			if(codigo[0]==clave[0]&&codigo[1]==clave[1]&&codigo[2]==clave[2]&&codigo[3]==clave[3]){
				LCD_printf("CLAVE CORRECTA");
				LCD_cmd(0xC0); //Mensaje en la 2DA línea de la LCD
				_delay_ms(100);
				LCD_printf("POR FAVOR, INGRESE");
				_delay_ms(500);
				LCD_cmd(0x01); //SE LIMPIA CARACTERES DE LA LCD
				LCD_printf("   MY HOME LIGHT   ");
				_delay_ms(250);
				LCD_cmd(0x01); //SE LIMPIA CARACTERES DE LA LCD PARA UN NUEVO INGRESO
				LCD_printf("INGRESE LA CLAVE:");
				LCD_cmd(0xC0);
				}else{
				LCD_printf("CLAVE INCORRECTA"); // CONTRASEÑA INCORRECTA
				_delay_ms(200);
				LCD_cmd(0x01); //LIMPIEZA DE CARACTERES
				LCD_printf("INGRESE LA CLAVE:");
				LCD_cmd(0xC0);
			}
			contadorClave=0; //CONTADOR DE CLAVE EN CERO PARA UN NUEVO INGRESO DE LA CLAVE
			//break;
		}
	}	
}
