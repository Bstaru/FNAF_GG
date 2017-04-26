#include <windows.h>
#include"gdipload.h"


const int ALTO_VENTANA = 600;
const int BPP = 4;
const int TICK = 100;
const unsigned int BLUE = 0xFF0000FF;
const unsigned int GREEN = 0xFF00FF00;
const unsigned int RED = 0xFFFF0000;

struct POSITION {
	int X;
	int Y;
};

struct DIMENSION {
	int ANCHO;
	int ALTO;
};

#pragma region LENS_CODE
/* Pinta el fondo de la ventana de acuerdo al color especificado.
	@param *ptrBuffer.	Puntero al area de memoria reservada para el proceso de dibujado.
	@param color.		Color expresado en formato hexadecimal.
	@param area.		Area de la ventana.
	*/
void LimpiarFondo(int *ptrBuffer, unsigned int color, int area) 
{
	__asm {
			mov edi, ptrBuffer
			mov ecx, area
			mov eax, color

			//draw: 	
			//	mov [edi], eax
			//	add edi, BPP
			//loop draw

			rep stosd
	}
}

/* Funcion que pinta una figura rectangular en pantalla.
	@param *ptrBuffer.	Puntero al area de memoria reservada para el dibujado.
	@param color.		Color de la figura expresado en formato hexadecimal.
	@param anchoWnd.	Ancho total de la ventana.
	@param altoWnd.		Alto total de la ventana.
	@param dmnFigura.	Especifica las dimensiones de la figura en relacion con la ventana.
	@param posFigura.	Posiciona la figura en la ventana.
	*/
void DibujaFigura(int *ptrBuffer, unsigned int color, int anchoWnd, int altoWnd, DIMENSION dmnFigura, POSITION posFigura)
{
	int x = posFigura.X;
	int y = posFigura.Y;
	int ancho = dmnFigura.ANCHO;
	int alto = dmnFigura.ALTO;
	__asm {
			mov edi, ptrBuffer

			mov eax, y
			mul BPP
			mul anchoWnd
			add edi, eax

			mov eax, x
			mul BPP
			add edi, eax

			mov eax, color

			mov ecx, alto
			lazo_alto:
				push ecx
				mov ecx, ancho
				lazo_ancho:
					mov [edi], eax
					add edi, BPP
				//stosd
				loop lazo_ancho
				push eax
				mov eax, anchoWnd
				sub eax, ancho
				mul BPP
				add edi, eax
				pop eax
				pop ecx
			loop lazo_alto
	}
}

	void imageblt(int* punteroDestino, int* punteroOrigen, int inicioXDestino, int inicioYDestino, int inicioXOrigen, int inicioYOrigen, int ancho, int alto, int anchodefondo, int anchodeorigen){
		//blt = block transfer, transferencia de bloque de imagen
		int bytesporlineafondo = anchodefondo*4;
		int bytesporlineaorigen = anchodeorigen*4;
		int bytesporlineaimagen = ancho*4;
		
		__asm{
			
			mov edi, punteroDestino //movemos la direccion del bitmap a edi, para poder escribir en el
			//Conseguimos el pixel inicial donde empezaremos a dibujar
			mov eax, inicioYDestino 
			mul bytesporlineafondo //inicioY * bytesporlineafondo, asi iniciamos en la linea donde queremos 
			mov ebx,eax //ebx contendra el resultado anterior
			mov eax, 4 
			mul inicioXDestino //inicioX*4, para asi encontrar la columnda donde queremos empezar a dibujar
			add eax, ebx //posicion de columna + posicion de linea
			add edi,eax //Sumamos el desplazamiento anterior al inicio de nuestra imagen para empezar a trabajar en la posicion deseada

			mov esi, punteroOrigen //movemos la direccion de la imagen a dibujar a esi, para poder escribir de ella
			//Conseguimos el pixel inicial DEL CUAL empezaremos a dibujar
			mov eax, inicioYOrigen 
			mul bytesporlineaorigen //inicioY * bytesporlineaorigen, asi iniciamos en la linea donde queremos 
			mov ebx,eax //ebx contendra el resultado anterior
			mov eax, 4 
			mul inicioXOrigen //inicioX*4, para asi encontrar la columnda de donde queremos empezar a leer
			add eax, ebx //posicion de columna + posicion de linea
			add esi,eax //Sumamos el desplazamiento anterior al inicio de nuestra imagen para empezar a trabajar en la posicion deseada

			mov ecx, alto //movemos a ecx la cantidad de lineas que dibujaremos
lazollenarY:
			push ecx //guardamos el valor anterior de ecx, porque lo reemplazaremos en un nuevo ciclo
			mov ecx, ancho //la cantidad de columnas que dibujaremos
	lazollenarX:
				movsd //mueve un pixel de la direccion apuntada por esi a la apuntada por edi, e incrementa esi y edi por 4

				loop lazollenarX

				add edi, bytesporlineafondo //le sumamos la cantidad de bytes de la linea del fondo para pasar a la siguiente linea
				sub edi, bytesporlineaimagen //y retrocedemos una cantidad igual al a su ancho para dibujar desde la posicion X inicial y que no quede escalonado

				//Lo mismo para esi
				add esi, bytesporlineaorigen
				sub esi, bytesporlineaimagen

				pop ecx //recuperamos el valor del contador del ciclo exterior
			loop lazollenarY
		}

	}


void transparentblt(int* punteroDestino, int* punteroOrigen, int inicioXDestino, int inicioYDestino, int inicioXOrigen, int inicioYOrigen, int ancho, int alto, int anchodefondo, int anchodeorigen, int colortransparente){
		//blt = block transfer, transferencia de bloque de imagen
		int bytesporlineafondo = anchodefondo*4;
		int bytesporlineaorigen = anchodeorigen*4;
		int bytesporlineaimagen = ancho*4;
		
		__asm{
			
			mov edi, punteroDestino //movemos la direccion del bitmap a edi, para poder escribir en el
			//Conseguimos el pixel inicial donde empezaremos a dibujar
			mov eax, inicioYDestino 
			mul bytesporlineafondo //inicioY * bytesporlineafondo, asi iniciamos en la linea donde queremos 
			mov ebx,eax //ebx contendra el resultado anterior
			mov eax, 4 
			mul inicioXDestino //inicioX*4, para asi encontrar la columnda donde queremos empezar a dibujar
			add eax, ebx //posicion de columna + posicion de linea
			add edi,eax //Sumamos el desplazamiento anterior al inicio de nuestra imagen para empezar a trabajar en la posicion deseada

			mov esi, punteroOrigen //movemos la direccion de la imagen a dibujar a esi, para poder escribir de ella
			//Conseguimos el pixel inicial DEL CUAL empezaremos a dibujar
			mov eax, inicioYOrigen 
			mul bytesporlineaorigen //inicioY * bytesporlineaorigen, asi iniciamos en la linea donde queremos 
			mov ebx,eax //ebx contendra el resultado anterior
			mov eax, 4 
			mul inicioXOrigen //inicioX*4, para asi encontrar la columnda de donde queremos empezar a leer
			add eax, ebx //posicion de columna + posicion de linea
			add esi,eax //Sumamos el desplazamiento anterior al inicio de nuestra imagen para empezar a trabajar en la posicion deseada

			mov eax, colortransparente

			mov ecx, alto //movemos a ecx la cantidad de lineas que dibujaremos
lazollenarY:
			push ecx //guardamos el valor anterior de ecx, porque lo reemplazaremos en un nuevo ciclo
			mov ecx, ancho //la cantidad de columnas que dibujaremos

	lazollenarX:
				//movsd //mueve un pixel de la direccion apuntada por esi a la apuntada por edi, e incrementa esi y edi por 4

			cmp eax, [esi]
			je nodibujar

				mov edx, [esi]
				mov [edi],edx

nodibujar:
				add esi,4
				add edi,4

				loop lazollenarX

				add edi, bytesporlineafondo //le sumamos la cantidad de bytes de la linea del fondo para pasar a la siguiente linea
				sub edi, bytesporlineaimagen //y retrocedemos una cantidad igual al a su ancho para dibujar desde la posicion X inicial y que no quede escalonado

				//Lo mismo para esi
				add esi, bytesporlineaorigen
				sub esi, bytesporlineaimagen

				pop ecx //recuperamos el valor del contador del ciclo exterior
			loop lazollenarY
		}

	}

void scaleblt(int* punteroDestino, int* punteroOrigen, int inicioXDestino, int inicioYDestino, int inicioXOrigen, int inicioYOrigen, int ancho, int alto, int anchodefondo, int anchodeorigen, int escalaX, int escalaY){
		//blt = block transfer, transferencia de bloque de imagen
		int bytesporlineafondo = anchodefondo*4;
		int bytesporlineaorigen = anchodeorigen*4;
		int bytesporlineaimagen = ancho*4;
		
		__asm{
			
			mov edi, punteroDestino //movemos la direccion del bitmap a edi, para poder escribir en el
			//Conseguimos el pixel inicial donde empezaremos a dibujar
			mov eax, inicioYDestino 
			mul bytesporlineafondo //inicioY * bytesporlineafondo, asi iniciamos en la linea donde queremos 
			mov ebx,eax //ebx contendra el resultado anterior
			mov eax, 4 
			mul inicioXDestino //inicioX*4, para asi encontrar la columnda donde queremos empezar a dibujar
			add eax, ebx //posicion de columna + posicion de linea
			add edi,eax //Sumamos el desplazamiento anterior al inicio de nuestra imagen para empezar a trabajar en la posicion deseada

			mov esi, punteroOrigen //movemos la direccion de la imagen a dibujar a esi, para poder escribir de ella
			//Conseguimos el pixel inicial DEL CUAL empezaremos a dibujar
			mov eax, inicioYOrigen 
			mul bytesporlineaorigen //inicioY * bytesporlineaorigen, asi iniciamos en la linea donde queremos 
			mov ebx,eax //ebx contendra el resultado anterior
			mov eax, 4 
			mul inicioXOrigen //inicioX*4, para asi encontrar la columnda de donde queremos empezar a leer
			add eax, ebx //posicion de columna + posicion de linea
			add esi,eax //Sumamos el desplazamiento anterior al inicio de nuestra imagen para empezar a trabajar en la posicion deseada

			
			mov eax, [esi]

			mov ecx, alto //movemos a ecx la cantidad de lineas que dibujaremos
lazollenarY:

			push ecx
			mov ecx, escalaY
			escaladoY:

			push ecx //guardamos el valor anterior de ecx, porque lo reemplazaremos en un nuevo ciclo
			mov ecx, ancho //la cantidad de columnas que dibujaremos

	lazollenarX:
				//movsd //mueve un pixel de la direccion apuntada por esi a la apuntada por edi, e incrementa esi y edi por 4
		push ecx
		mov ecx, escalaX

escaladoX:

			cmp eax, [esi]
			je nodibujar

				mov edx, [esi]
				mov [edi],edx

nodibujar:
				add edi,4
				loop escaladoX
				
				add esi,4

				pop ecx
				loop lazollenarX

				add edi, bytesporlineafondo //le sumamos la cantidad de bytes de la linea del fondo para pasar a la siguiente linea
				
				push eax
				mov eax, bytesporlineaimagen
				mul escalaX
				sub edi, eax //y retrocedemos una cantidad igual al a su ancho para dibujar desde la posicion X inicial y que no quede escalonado
				pop eax

				sub esi, bytesporlineaimagen

				pop ecx
				loop escaladoY

				//Lo mismo para esi
				
				add esi, bytesporlineaorigen

				pop ecx //recuperamos el valor del contador del ciclo exterior
			loop lazollenarY
		}

	}

void autotransparentblt(int* punteroDestino, int* punteroOrigen, int inicioXDestino, int inicioYDestino, int inicioXOrigen, int inicioYOrigen, int ancho, int alto, int anchodefondo, int anchodeorigen){
		//blt = block transfer, transferencia de bloque de imagen
		int bytesporlineafondo = anchodefondo*4;
		int bytesporlineaorigen = anchodeorigen*4;
		int bytesporlineaimagen = ancho*4;
		
		__asm{
			
			mov edi, punteroDestino //movemos la direccion del bitmap a edi, para poder escribir en el
			//Conseguimos el pixel inicial donde empezaremos a dibujar
			mov eax, inicioYDestino 
			mul bytesporlineafondo //inicioY * bytesporlineafondo, asi iniciamos en la linea donde queremos 
			mov ebx,eax //ebx contendra el resultado anterior
			mov eax, 4 
			mul inicioXDestino //inicioX*4, para asi encontrar la columnda donde queremos empezar a dibujar
			add eax, ebx //posicion de columna + posicion de linea
			add edi,eax //Sumamos el desplazamiento anterior al inicio de nuestra imagen para empezar a trabajar en la posicion deseada

			mov esi, punteroOrigen //movemos la direccion de la imagen a dibujar a esi, para poder escribir de ella
			//Conseguimos el pixel inicial DEL CUAL empezaremos a dibujar
			mov eax, inicioYOrigen 
			mul bytesporlineaorigen //inicioY * bytesporlineaorigen, asi iniciamos en la linea donde queremos 
			mov ebx,eax //ebx contendra el resultado anterior
			mov eax, 4 
			mul inicioXOrigen //inicioX*4, para asi encontrar la columnda de donde queremos empezar a leer
			add eax, ebx //posicion de columna + posicion de linea
			add esi,eax //Sumamos el desplazamiento anterior al inicio de nuestra imagen para empezar a trabajar en la posicion deseada

			
			mov eax, [esi]

			mov ecx, alto //movemos a ecx la cantidad de lineas que dibujaremos
lazollenarY:
			push ecx //guardamos el valor anterior de ecx, porque lo reemplazaremos en un nuevo ciclo
			mov ecx, ancho //la cantidad de columnas que dibujaremos

	lazollenarX:
				//movsd //mueve un pixel de la direccion apuntada por esi a la apuntada por edi, e incrementa esi y edi por 4

			cmp eax, [esi]
			je nodibujar

				mov edx, [esi]
				mov [edi],edx

nodibujar:
				add esi,4
				add edi,4

				loop lazollenarX

				add edi, bytesporlineafondo //le sumamos la cantidad de bytes de la linea del fondo para pasar a la siguiente linea
				sub edi, bytesporlineaimagen //y retrocedemos una cantidad igual al a su ancho para dibujar desde la posicion X inicial y que no quede escalonado

				//Lo mismo para esi
				add esi, bytesporlineaorigen
				sub esi, bytesporlineaimagen

				pop ecx //recuperamos el valor del contador del ciclo exterior
			loop lazollenarY
		}

	}


	void mirrorxblt(int* punteroDestino, int* punteroOrigen, int inicioXDestino, int inicioYDestino, int inicioXOrigen, int inicioYOrigen, int ancho, int alto, int anchodefondo, int anchodeorigen){
		//blt = block transfer, transferencia de bloque de imagen
		int bytesporlineafondo = anchodefondo*4;
		int bytesporlineaorigen = anchodeorigen*4;
		int bytesporlineaimagen = ancho*4;
		
		__asm{
			
			mov edi, punteroDestino //movemos la direccion del bitmap a edi, para poder escribir en el
			//Conseguimos el pixel inicial donde empezaremos a dibujar
			mov eax, inicioYDestino 
			mul bytesporlineafondo //inicioY * bytesporlineafondo, asi iniciamos en la linea donde queremos 
			mov ebx,eax //ebx contendra el resultado anterior
			mov eax, 4 
			mul inicioXDestino //inicioX*4, para asi encontrar la columnda donde queremos empezar a dibujar
			add eax, ebx //posicion de columna + posicion de linea
			add edi,eax //Sumamos el desplazamiento anterior al inicio de nuestra imagen para empezar a trabajar en la posicion deseada

			mov esi, punteroOrigen //movemos la direccion de la imagen a dibujar a esi, para poder escribir de ella
			//Conseguimos el pixel inicial DEL CUAL empezaremos a dibujar
			mov eax, inicioYOrigen 
			mul bytesporlineaorigen //inicioY * bytesporlineaorigen, asi iniciamos en la linea donde queremos 
			mov ebx,eax //ebx contendra el resultado anterior
			mov eax, 4 
			mul inicioXOrigen //inicioX*4, para asi encontrar la columnda de donde queremos empezar a leer
			add eax, ebx //posicion de columna + posicion de linea
			add esi,eax //Sumamos el desplazamiento anterior al inicio de nuestra imagen para empezar a trabajar en la posicion deseada

			
			mov eax, [esi]

			mov ecx, alto //movemos a ecx la cantidad de lineas que dibujaremos
lazollenarY:
			push ecx //guardamos el valor anterior de ecx, porque lo reemplazaremos en un nuevo ciclo
			mov ecx, ancho //la cantidad de columnas que dibujaremos

			add esi, bytesporlineaimagen
			sub esi, 4

	lazollenarX:
				//movsd //mueve un pixel de la direccion apuntada por esi a la apuntada por edi, e incrementa esi y edi por 4

			cmp eax, [esi]
			je nodibujar

				mov edx, [esi]
				mov [edi],edx

nodibujar:
				sub esi,4
				add edi,4

				loop lazollenarX

				add edi, bytesporlineafondo //le sumamos la cantidad de bytes de la linea del fondo para pasar a la siguiente linea
				sub edi, bytesporlineaimagen //y retrocedemos una cantidad igual al a su ancho para dibujar desde la posicion X inicial y que no quede escalonado

				//Lo mismo para esi
				add esi, bytesporlineaorigen
				add esi, 4
				//sub esi, bytesporlineaimagen

				pop ecx //recuperamos el valor del contador del ciclo exterior
			loop lazollenarY
		}

	}

//	void mirroryblt(int* punteroDestino, int* punteroOrigen, int inicioXDestino, int inicioYDestino, int inicioXOrigen, int inicioYOrigen, int ancho, int alto, int anchodefondo, int anchodeorigen){
//		//blt = block transfer, transferencia de bloque de imagen
//		int bytesporlineafondo = anchodefondo*4;
//		int bytesporlineaorigen = anchodeorigen*4;
//		int bytesporlineaimagen = ancho*4;
//		
//		__asm{
//			
//			mov edi, punteroDestino //movemos la direccion del bitmap a edi, para poder escribir en el
//			//Conseguimos el pixel inicial donde empezaremos a dibujar
//			mov eax, inicioYDestino 
//			mul bytesporlineafondo //inicioY * bytesporlineafondo, asi iniciamos en la linea donde queremos 
//			mov ebx,eax //ebx contendra el resultado anterior
//			mov eax, 4 
//			mul inicioXDestino //inicioX*4, para asi encontrar la columnda donde queremos empezar a dibujar
//			add eax, ebx //posicion de columna + posicion de linea
//			add edi,eax //Sumamos el desplazamiento anterior al inicio de nuestra imagen para empezar a trabajar en la posicion deseada
//
//			mov esi, punteroOrigen //movemos la direccion de la imagen a dibujar a esi, para poder escribir de ella
//			//Conseguimos el pixel inicial DEL CUAL empezaremos a dibujar
//			mov eax, inicioYOrigen
//			add eax, alto
//			dec eax
//			mul bytesporlineaorigen //inicioY * bytesporlineaorigen, asi iniciamos en la linea donde queremos 
//			mov ebx,eax //ebx contendra el resultado anterior
//			mov eax, 4 
//			mul inicioXOrigen //inicioX*4, para asi encontrar la columnda de donde queremos empezar a leer
//			add eax, ebx //posicion de columna + posicion de linea
//			add esi,eax //Sumamos el desplazamiento anterior al inicio de nuestra imagen para empezar a trabajar en la posicion deseada
//
//			
//			mov eax, [esi]
//
//			mov ecx, alto //movemos a ecx la cantidad de lineas que dibujaremos
//lazollenarY:
//			push ecx //guardamos el valor anterior de ecx, porque lo reemplazaremos en un nuevo ciclo
//			mov ecx, ancho //la cantidad de columnas que dibujaremos
//
//	lazollenarX:
//				//movsd //mueve un pixel de la direccion apuntada por esi a la apuntada por edi, e incrementa esi y edi por 4
//
//			cmp eax, [esi]
//			je nodibujar
//
//				mov edx, [esi]
//				mov [edi],edx
//
//nodibujar:
//				add esi,4
//				add edi,4
//
//				loop lazollenarX
//
//				add edi, bytesporlineafondo //le sumamos la cantidad de bytes de la linea del fondo para pasar a la siguiente linea
//				sub edi, bytesporlineaimagen //y retrocedemos una cantidad igual al a su ancho para dibujar desde la posicion X inicial y que no quede escalonado
//
//				//Lo mismo para esi
//				sub esi, bytesporlineaorigen
//				sub esi, bytesporlineaimagen
//
//				pop ecx //recuperamos el valor del contador del ciclo exterior
//			loop lazollenarY
//		}
//
//	}
//
//		void mirrorxyblt(int* punteroDestino, int* punteroOrigen, int inicioXDestino, int inicioYDestino, int inicioXOrigen, int inicioYOrigen, int ancho, int alto, int anchodefondo, int anchodeorigen){
//		//blt = block transfer, transferencia de bloque de imagen
//		int bytesporlineafondo = anchodefondo*4;
//		int bytesporlineaorigen = anchodeorigen*4;
//		int bytesporlineaimagen = ancho*4;
//		
//		__asm{
//			
//			mov edi, punteroDestino //movemos la direccion del bitmap a edi, para poder escribir en el
//			//Conseguimos el pixel inicial donde empezaremos a dibujar
//			mov eax, inicioYDestino 
//			mul bytesporlineafondo //inicioY * bytesporlineafondo, asi iniciamos en la linea donde queremos 
//			mov ebx,eax //ebx contendra el resultado anterior
//			mov eax, 4 
//			mul inicioXDestino //inicioX*4, para asi encontrar la columnda donde queremos empezar a dibujar
//			add eax, ebx //posicion de columna + posicion de linea
//			add edi,eax //Sumamos el desplazamiento anterior al inicio de nuestra imagen para empezar a trabajar en la posicion deseada
//
//			mov esi, punteroOrigen //movemos la direccion de la imagen a dibujar a esi, para poder escribir de ella
//			//Conseguimos el pixel inicial DEL CUAL empezaremos a dibujar
//			mov eax, inicioYOrigen
//			add eax, alto
//			dec eax
//			mul bytesporlineaorigen //inicioY * bytesporlineaorigen, asi iniciamos en la linea donde queremos 
//			mov ebx,eax //ebx contendra el resultado anterior
//			mov eax, 4 
//			mul inicioXOrigen //inicioX*4, para asi encontrar la columnda de donde queremos empezar a leer
//			add eax, ebx //posicion de columna + posicion de linea
//			add esi,eax //Sumamos el desplazamiento anterior al inicio de nuestra imagen para empezar a trabajar en la posicion deseada
//
//			
//			mov eax, [esi]
//
//			mov ecx, alto //movemos a ecx la cantidad de lineas que dibujaremos
//lazollenarY:
//			add esi, bytesporlineaimagen
//			sub esi, 4
//
//			push ecx //guardamos el valor anterior de ecx, porque lo reemplazaremos en un nuevo ciclo
//			mov ecx, ancho //la cantidad de columnas que dibujaremos
//	lazollenarX:
//				//movsd //mueve un pixel de la direccion apuntada por esi a la apuntada por edi, e incrementa esi y edi por 4
//
//			cmp eax, [esi]
//			je nodibujar
//
//				mov edx, [esi]
//				mov [edi],edx
//
//nodibujar:
//				sub esi,4
//				add edi,4
//
//				loop lazollenarX
//
//				add edi, bytesporlineafondo //le sumamos la cantidad de bytes de la linea del fondo para pasar a la siguiente linea
//				sub edi, bytesporlineaimagen //y retrocedemos una cantidad igual al a su ancho para dibujar desde la posicion X inicial y que no quede escalonado
//
//				//Lo mismo para esi
//				sub esi, bytesporlineaorigen
//				add esi,4
//				//sub esi, bytesporlineaimagen
//
//				pop ecx //recuperamos el valor del contador del ciclo exterior
//			loop lazollenarY
//		}
//
//	}

#pragma endregion