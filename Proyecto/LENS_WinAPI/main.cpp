#include <windows.h>
#include"gdipload.h"
#include "variables.h"

int WINAPI wWinMain(HINSTANCE hInstance, 
					 HINSTANCE hPrevInstance, 
					 PWSTR pCmdLine, 
					 int nCmdShow)
{
	WNDCLASSEX wc;									// Windows Class Structure
	HWND hWnd;
	/*PlaySound(L"Mb8.wav", NULL, SND_ASYNC | SND_FILENAME | SND_LOOP);*/
	MSG msg;

	TCHAR szAppName[] = TEXT("MyWinAPIApp");		
	TCHAR szAppTitle[] = TEXT("LENS App");

	hInstance = GetModuleHandle(NULL);				// Grab An Instance For Our Window

	wc.cbSize			= sizeof(WNDCLASSEX);
	wc.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;	// Redraw On Size, And Own DC For Window.
	wc.lpfnWndProc		= (WNDPROC) WndProc;					// WndProc Handles Messages
	wc.cbClsExtra		= 0;									// No Extra Window Data
	wc.cbWndExtra		= 0;									// No Extra Window Data
	wc.hInstance		= hInstance;							// Set The Instance	
	wc.hIcon			= LoadIcon(NULL, IDI_WINLOGO);			// Load The Default Icon
	wc.hCursor			= LoadCursor(NULL, IDC_ARROW);			// Load The Arrow Pointer
	wc.hbrBackground	= (HBRUSH) (COLOR_WINDOW + 1);			// No Background Required For GL
	wc.lpszMenuName		= NULL;									// We Don't Want A Menu	
	wc.lpszClassName	= szAppName;							// Set The Class Name
	wc.hIconSm			= LoadIcon(NULL, IDI_APPLICATION);
	
	if (!RegisterClassEx(&wc))									// Attempt To Register The Window Class
	{
		MessageBox(NULL,
			L"Fallo al registrar clase (Failed To Register The Window Class).",
			L"ERROR",
			MB_OK|MB_ICONEXCLAMATION);
		return 0;
	}

	hWnd = CreateWindowEx(	
		WS_EX_CLIENTEDGE | WS_EX_APPWINDOW | WS_EX_WINDOWEDGE,	// Extended Style For The Window
		szAppName,							// Class Name
		szAppTitle,							// Window Title
		WS_OVERLAPPEDWINDOW |				// Defined Window Style
		WS_CLIPSIBLINGS |					// Required Window Style
		WS_CLIPCHILDREN,					// Required Window Style
		0, 0,								// Window Position
		ANCHO_VENTANA,						// Calculate Window Width
		ALTO_VENTANA,						// Calculate Window Height
		NULL,								// No Parent Window
		NULL,								// No Menu
		hInstance,							// Instance
		NULL);								// Pass this class To WM_CREATE								

	if(hWnd == NULL) {
		MessageBox(NULL, 
			L"Error al crear ventana (Window Creation Error).", 
			L"ERROR", 
			MB_OK|MB_ICONEXCLAMATION);
		return 0;
	}
		
	SetForegroundWindow(hWnd);						// Slightly Higher Priority
	Init();
	ShowWindow(hWnd, nCmdShow);
	SetFocus(hWnd);

	SetTimer(hWnd, TICK, TICK, NULL);
	ZeroMemory(&msg, sizeof(MSG));

	while(GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	
	return(int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{

	switch(uMsg)									// Check For Windows Messages
	{
		case WM_CREATE:
		break;
		case WM_TIMER:
			if(wParam == TICK)
			{
				MainRender(hWnd);
			}
			break;
		case WM_PAINT:
			{
				HDC hdc; 
				PAINTSTRUCT ps;
				hdc = BeginPaint(hWnd, &ps);
				// TODO: agregar código de dibujo aquí...

				//BITMAP bm;
				HBITMAP h_CMC = CreateBitmap(ANCHO_VENTANA, ALTO_VENTANA, 1, 32, ptrBuffer);
				HDC hdcMem = CreateCompatibleDC(hdc);
				HBITMAP hbmOld = (HBITMAP) SelectObject(hdcMem, h_CMC);
				//GetObject(h_CMC, sizeof(bm), &bm);

				BitBlt(hdc, 0, 0, ANCHO_VENTANA, ALTO_VENTANA, hdcMem, 0, 0, SRCCOPY);

				DeleteObject(h_CMC);
				SelectObject(hdcMem, hbmOld);
				DeleteDC(hdcMem);
				//DeleteObject(hbmOld);
			}
			break;		
		case WM_KEYDOWN:							
			{
				switch(wParam){
			case input.Q:
				tonoazul++;
				break;
			case input.A:
				tonoazul--;
				break;
			}

				KEYS[ wParam ] = true;
				if(KEYS[input.A] || KEYS[input.Left]){
					movimiento = 1;
				}

				if(KEYS[input.D] || KEYS[input.Right]){
					movimiento = 2;
				}

				if(KEYS[input.W] || KEYS[input.Up]){
					movimiento  = 3;
				}

				if(KEYS[input.S] || KEYS[input.Down]){
					movimiento  =  4;
				}

			}
			break;
		case WM_KEYUP:
			{
				KEYS[ wParam ] = false;
				movimiento = 0;
			}
			break;
		case WM_CLOSE: 
			{
				DestroyWindow(hWnd);
			}
			break;
		case WM_DESTROY: //Send A Quit Message
			{
				KillTimer(hWnd, TICK);
				PostQuitMessage(0);
			}
			break;
		default:
			return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}

	return 0;
}

/* Inicializacion de variables y reserva de memoria.*/
void Init() 
{
	posFigura.X = 100;
	posFigura.Y = 100;
	dmnFigura.ANCHO = 50;
	dmnFigura.ALTO = 50;

	for(int i = 0; i < 256; i++)
	{
		KEYS[i] = false;
	}
	ptrBuffer = new int[ANCHO_VENTANA * ALTO_VENTANA];
	
	mbmp = gdipLoad(L"spriteuvaf.bmp");
	chica= gdipLoad(L"spritechic.bmp");
	fnaf1= gdipLoad(L"1.bmp"); fnaf[0] = fnaf1;
	fnaf2= gdipLoad(L"2.bmp");fnaf[1] = fnaf2;
	fnaf3= gdipLoad(L"3.bmp");fnaf[2] = fnaf3;
	fnaf4= gdipLoad(L"4.bmp");fnaf[3] = fnaf4;
	cup = gdipLoad(L"cupck.bmp");
	inicio=gdipLoad (L"inicio.bmp");
	fin = gdipLoad (L"glitscr.bmp");
	fin2= gdipLoad (L"fins.bmp");
	wea = gdipLoad (L"spritesbon.bmp");

	pcuadros = new cuadro[2];
	pcuadros2 = new cuadro[2];
	pcuadros3 = new cuadro[2];

	//frente-abajo
	pcuadros2[0].x = 100; pcuadros2[0].y = 0;
	pcuadros2[0].ancho = 100; pcuadros2[0].alto = 114;

	pcuadros2[1].x = 200; pcuadros2[1].y = 0;
	pcuadros2[1].ancho = 100; pcuadros2[1].alto = 114;

	//para arriba 
	pcuadros3[0].x = 100; pcuadros3[0].y = 115;
	pcuadros3[0].ancho = 100; pcuadros3[0].alto = 114;

	pcuadros3[1].x = 200; pcuadros3[1].y = 115;
	pcuadros3[1].ancho = 100; pcuadros3[1].alto = 114;

	//hacia un lado
	pcuadros[0].x = 100; pcuadros[0].y = 230;
	pcuadros[0].ancho = 100; pcuadros[0].alto = 114;

	pcuadros[1].x = 200; pcuadros[1].y = 230;
	pcuadros[1].ancho = 100; pcuadros[1].alto = 114;

	frameactual = 0;

	//chica
	pcuadros4 = new cuadro[2];

	pcuadros4[0].x = 100; pcuadros4[0].y = 0;
	pcuadros4[0].ancho = 100; pcuadros4[0].alto = 99;

	pcuadros4[1].x = 200; pcuadros4[1].y = 0;
	pcuadros4[1].ancho = 100; pcuadros4[1].alto = 99;

	//el nigga

	pcuadros5 = new cuadro [9];
	//f1
	pcuadros5[0].x = 95; pcuadros5[0].y = 0;
	pcuadros5[0].ancho = 95; pcuadros5[0].alto = 135;
	//f2
	pcuadros5[1].x = 190; pcuadros5[1].y = 0;
	pcuadros5[1].ancho = 95; pcuadros5[1].alto = 135;
	//f3
	pcuadros5[2].x = 285; pcuadros5[2].y = 0;
	pcuadros5[2].ancho = 95; pcuadros5[2].alto = 135;
	//f4
	pcuadros5[3].x = 380; pcuadros5[3].y = 0;
	pcuadros5[3].ancho = 95; pcuadros5[3].alto = 135;
	//f5
	pcuadros5[4].x = 475; pcuadros5[4].y = 0;
	pcuadros5[4].ancho = 95; pcuadros5[4].alto = 135;
	//f6
	pcuadros5[5].x = 570; pcuadros5[5].y = 0;
	pcuadros5[5].ancho = 95; pcuadros5[5].alto = 135;
	//f7
	pcuadros5[6].x = 665; pcuadros5[6].y = 0;
	pcuadros5[6].ancho = 95; pcuadros5[6].alto = 135;

	pcuadros5[7].x = 760; pcuadros5[7].y = 0;
	pcuadros5[7].ancho = 95; pcuadros5[7].alto = 135;

	pcuadros5[8].x = 855; pcuadros5[8].y = 0;
	pcuadros5[8].ancho = 95; pcuadros5[8].alto = 135;

	frameactual2=0;
}

/* Funcion principal. Encargada de hacer el redibujado en pantalla cada intervalo (o "tick") del timer que se haya creado.
@param hWnd. Manejador de la ventana. */

void MainRender(HWND hWnd) {	
	KeysEvents();
	
	switch(escenario){
	case 1:
		{
			imageblt(ptrBuffer,(fnaf[0].pixeles),0,0,0,0,fnaf[0].ancho,fnaf[0].alto,800,fnaf[0].ancho);
			if(cup1==true){
			scaleblt(ptrBuffer,(cup.pixeles), 550, 50, 0, 0, cup.ancho,cup.alto, 800,cup.ancho,2,1);
			}
			colichon();
		}break;
	case 2:
		{
			imageblt(ptrBuffer,(fnaf[1].pixeles),0,0,0,0,fnaf[1].ancho,fnaf[1].alto,800,fnaf[1].ancho);
				if (der==true)
			{
				transparentblt(ptrBuffer,(wea.pixeles),
					xb,100,
					pcuadros5[frameactual2].x,pcuadros5[frameactual2].y,
					pcuadros5[frameactual2].ancho,pcuadros5[frameactual2].alto,
					800,wea.ancho,0xFF00FF00);
				
				if(xb > 570){
					der=false;
				}
			}
			else {
					mirrorxblt(ptrBuffer,(wea.pixeles),
					xb,100,
					pcuadros5[frameactual2].x,pcuadros5[frameactual2].y,
					pcuadros5[frameactual2].ancho,pcuadros5[frameactual2].alto,
					800,wea.ancho);
					if(xb < 150){
						der=true;
					}
			}
			if(der==true){
				xb+=5;
			}
			else if(der==false){
				xb-=5;
			}


			if(cup2==true){
			scaleblt(ptrBuffer,(cup.pixeles), 350, 250, 0, 0, cup.ancho,cup.alto, 800,cup.ancho,2,1);
			}
			colichon();

		}break;
	case 3:
		{
			imageblt(ptrBuffer,(fnaf[2].pixeles),0,0,0,0,fnaf[2].ancho,fnaf[2].alto,800,fnaf[2].ancho);

			if (der==true)
			{
				transparentblt(ptrBuffer,(wea.pixeles),
					xb,yb,
					pcuadros5[frameactual2].x,pcuadros5[frameactual2].y,
					pcuadros5[frameactual2].ancho,pcuadros5[frameactual2].alto,
					800,wea.ancho,0xFF00FF00);
				
				if(xb > 470){
					der=false;
				}
			}
			else {
					mirrorxblt(ptrBuffer,(wea.pixeles),
					xb,yb,
					pcuadros5[frameactual2].x,pcuadros5[frameactual2].y,
					pcuadros5[frameactual2].ancho,pcuadros5[frameactual2].alto,
					800,wea.ancho);
					if(xb < 150){
						der=true;
					}
			}
			if(der==true){
				xb+=5;
			}
			else if(der==false){
				xb-=5;
			}

			if(cup3==true){
			scaleblt(ptrBuffer,(cup.pixeles), 550, 200, 0, 0, cup.ancho,cup.alto, 800,cup.ancho,2,1);
			}
			colichon();
		}break;
	case 4:
		{
		imageblt(ptrBuffer,(fnaf[3].pixeles),0,0,0,0,fnaf[3].ancho,fnaf[3].alto,800,fnaf[3].ancho);
		if(cup4==true){
		scaleblt(ptrBuffer,(cup.pixeles), 550, 350, 0, 0, cup.ancho,cup.alto, 800,cup.ancho,2,1);
		}

		if(pato==true){
		mirrorxblt(ptrBuffer,(chica.pixeles),
					100,200,
					pcuadros4[frameactual].x,pcuadros4[frameactual].y,
					pcuadros4[frameactual].ancho,pcuadros4[frameactual].alto,
					800,chica.ancho);
		}

		colichon();
		}break;
	case 5: {
			imageblt(ptrBuffer,(fin2.pixeles),0,0,0,0,fin2.ancho,fin2.alto,800,fin2.ancho);
	
			}break;

	default:
		{ if (escenario == 0){
			imageblt(ptrBuffer,(inicio.pixeles),0,0,0,0,inicio.ancho,inicio.alto,800,inicio.ancho);
		}

		}break;
	}
if (escenario != 5){
if (cup1==false){
			scaleblt(ptrBuffer,(cup.pixeles), 20, 460, 0, 0, cup.ancho,cup.alto, 800,cup.ancho,2,1);
			}
if (cup2==false){
			scaleblt(ptrBuffer,(cup.pixeles), 80, 460, 0, 0, cup.ancho,cup.alto, 800,cup.ancho,2,1);
}
if (cup3==false){
			scaleblt(ptrBuffer,(cup.pixeles), 140, 460, 0, 0, cup.ancho,cup.alto, 800,cup.ancho,2,1);
			}
if (cup4==false){
			scaleblt(ptrBuffer,(cup.pixeles), 200, 460, 0, 0, cup.ancho,cup.alto, 800,cup.ancho,2,1);
			}
}

 if (escenario > 0 &&  escenario < 5) {

	switch(movimiento){
	//izq
	case 1:
		{
			transparentblt(ptrBuffer,(mbmp.pixeles),
					xx,yy,
					pcuadros[frameactual].x,pcuadros[frameactual].y,
					pcuadros[frameactual].ancho,pcuadros[frameactual].alto,
					800,mbmp.ancho,0xFF00FF00);
		
		}break;
	//der
	case 2:
		{
			mirrorxblt(ptrBuffer,(mbmp.pixeles),
					xx,yy,
					pcuadros[frameactual].x,pcuadros[frameactual].y,
					pcuadros[frameactual].ancho,pcuadros[frameactual].alto,
					800,mbmp.ancho);
		
		}break;
	//arriba
	case 3:
		{
			transparentblt(ptrBuffer,(mbmp.pixeles),
					xx,yy,
					pcuadros3[frameactual].x,pcuadros3[frameactual].y,
					pcuadros3[frameactual].ancho,pcuadros3[frameactual].alto,
					800,mbmp.ancho,0xFF00FF00);
		
		}break;
	//abajo
	case 4:
		{
			mirrorxblt(ptrBuffer,(mbmp.pixeles),
					xx,yy,
					pcuadros2[frameactual].x,pcuadros2[frameactual].y,
					pcuadros2[frameactual].ancho,pcuadros2[frameactual].alto,
					800,mbmp.ancho);
		
		}break;

	default:
		{
			mirrorxblt(ptrBuffer,(mbmp.pixeles),
					xx,yy,
					pcuadros2[frameactual].x,pcuadros2[frameactual].y,
					pcuadros2[frameactual].ancho,pcuadros2[frameactual].alto,
					800,mbmp.ancho);
		}break;
	}

	frameactual++; 
	if(frameactual > 1 ) frameactual = 0;

	frameactual2++; 
	if(frameactual2 > 8 ) frameactual2 = 0;
	
 }
	if(tonoazul != 0)
	cambiarTonalidadAzul(ptrBuffer,ANCHO_VENTANA,ALTO_VENTANA,tonoazul);
	InvalidateRect(hWnd, NULL, FALSE);
	UpdateWindow(hWnd);
	
}

void KeysEvents() 
{
	if(KEYS[input.W] || KEYS[input.Up])
	{ 
		if(yy<40 && escenario != 4)
		{
			yy = 40;
		}
		else if(yy <40 && escenario == 4){
			yy = 420;
			escenario = 3;
		}
		yy-=25;

		movimiento = 3;
	}
	if(KEYS[input.D] || KEYS[input.Right])
	{
		if(xx>670 && escenario == 3)
		{
			xx=670;
		}
		else if(xx>670 && escenario == 1)
		{
			xx=60;
			escenario = 2;
		}

		else if(xx>670 && escenario == 2)
		{
			xx=60;
			escenario = 3;
		}
		else if(xx>670 && escenario == 4)
		{
			xx = 670;
		}
		xx+=25;
		
	}
	if(KEYS[input.S] || KEYS[input.Down])
	{
		
		if(yy>420 && escenario != 3)
		{
			yy = 420;
		}
		else if(yy>420 && escenario == 3){
			yy = 40;
			escenario = 4;
		}
		yy+=25;
		movimiento = 4;
	}

	if(KEYS[input.A] || KEYS[input.Left])
	{
		if(xx<20 && escenario == 1){
			xx= 20;
		}
		else if(xx<20 && escenario == 2)
		{
			xx=700;
			escenario = 1;
		}

		else if(xx<20 && escenario == 3)
		{
			xx=700;
			escenario = 2;
		}
		else if(xx<20 && escenario == 4){
			xx = 20;
		}
		xx-=25;
		
		movimiento = 1;
	}	

	 if (KEYS[input.Enter]){
			escenario = 1;
		}

	//music off-on
	 if (KEYS[input.M]){
		 PlaySound(L"Mb8.wav", NULL, SND_ASYNC | SND_FILENAME | SND_LOOP);
	 }
	 if (KEYS[input.N]){
		 PlaySound(L"sln.wav", NULL, NULL | NULL | NULL);
	 }

//	//colors
//
//	if (KEYS[input.C]){
//	 
//	if(tonoazul != 1)
//cambiarTonalidadAzul(ptrBuffer,ANCHO_VENTANA,ALTO_VENTANA,tonoazul);
//InvalidateRect(hWnd, NULL, FALSE);
//UpdateWindow(hWnd);
//	}
}

void colichon(){
	if (escenario == 1) {
		if (xx >= 525 && xx <= 580 ) {
		colx = true;
		}
		else colx = false; 

		if (yy >= 25 && yy <= 80 ) {
		coly = true;
		}
		else coly = false ;

		if (colx == true && coly == true) {
			//sonido
		if (cup1==true){
		PlaySound(L"Collect.wav", NULL, NULL | NULL | NULL);
		}
		 PlaySound(L"Mb8.wav", NULL, SND_ASYNC | SND_FILENAME | SND_LOOP);
			cup1=false;
		}
	}
	if (escenario == 2) {
		if (xx >= 325 && xx <= 380 ) {
		colx2 = true;
		}
		else colx2 = false; 

		if (yy >= 225 && yy <= 280 ) {
		coly2 = true;
		}
		else coly2 = false ;

		if (colx2 == true && coly2 == true) {
			//sonido
		if (cup2==true){
		PlaySound(L"Collect.wav", NULL, NULL | NULL | NULL);
		}
		 PlaySound(L"Mb8.wav", NULL, SND_ASYNC | SND_FILENAME | SND_LOOP);
			cup2=false;
		}
	}
	if (escenario == 3) {
		if (xx >= 525 && xx <= 580 ) {
		colx3 = true;
		}
		else colx3 = false; 

		if (yy >= 175 && yy <= 230 ) {
		coly3 = true;
		}
		else coly3 = false ;

		if (colx3 == true && coly3 == true) {
			//sonido
		if (cup3==true){
		PlaySound(L"Collect.wav", NULL, NULL | NULL | NULL);
		}
		 PlaySound(L"Mb8.wav", NULL, SND_ASYNC | SND_FILENAME | SND_LOOP);
			cup3=false;
		}
	}
	if (escenario == 4) {
		if (xx >= 525 && xx <= 580 ) {
		colx4 = true;
		}
		else colx4 = false; 

		if (yy >= 325 && yy <= 380 ) {
		coly4 = true;
		}
		else coly4 = false ;

		if (colx4 == true && coly4 == true) {
			//sonido
		if (cup4==true){
		PlaySound(L"Collect.wav", NULL, NULL | NULL | NULL);
		}
		 PlaySound(L"Mb8.wav", NULL, SND_ASYNC | SND_FILENAME | SND_LOOP);
			cup4=false;
		}
	}
if (escenario == 4) {
		//RAYAS
		if (xx >= 100 && xx <= 170 ) {
			//intento1
			transparentblt(ptrBuffer,(fin.pixeles),0,0,0,0,fin.ancho,fin.alto,800,fin.ancho,0xFF00FF00);
		}

		if (xx >= 100 && xx <= 150 ) {
			
		colx5 = true;
		}
		else colx5 = false; 

		if (yy >= 200 && yy <= 340 ) {
			
		coly5 = true;
		}
		else coly5 = false ;

		if (colx5 == true && coly5 == true) {
			//sonido
		if (pato==true){
		
		PlaySound(L"glich.wav", NULL, NULL | NULL | NULL);
		pato=false;
		escenario=5;
		}	
		}
	}
if(escenario==5){
	PlaySound(L"Mb2.wav", NULL, SND_ASYNC | SND_FILENAME | NULL );
	}
}

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

void cambiarTonalidadAzul(int* punteroImagen, int ancho, int alto, unsigned char color){

		int totalpixels = ancho * alto;

		__asm{

			mov edi,punteroImagen
			mov ecx, totalpixels
tonoloop:
			mov eax,0xFF00FFFF
			and [edi],eax ;Limpia el componente azul de la imagen
			;Mascaras para los otros colores:
			;Rojo: 0xFF00FFFF
			;Verde: 0xFFFF00FF
			;Azul: 0xFFFFFF00
			xor eax,eax
			mov al,color
			;shl eax,16 ;Ponemos el valor del color en eax, luego lo movemos a la izquierda para que quede en el lugar del componente rojo
			;Cantidad para el rojo: 16
			;Cantidad para el color verde: 8
			;Cantidad para azul: 0
			add [edi],eax

			add edi, 4
			loop tonoloop

		}

	}

#pragma endregion