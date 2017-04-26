//Variables constantes
const int ANCHO_VENTANA = 800;
const int ALTO_VENTANA = 600;
const int BPP = 4;
const int TICK = 100;
const unsigned int BLUE = 0xFF0000FF;
const unsigned int GREEN = 0xFF00FF00;
const unsigned int RED = 0xFFFF0000;

int xx=60, yy=200, movimiento=0;
int xb=150, yb = 150;

int escenario = 0;

struct POSITION {
	int X;
	int Y;
};

struct DIMENSION {
	int ANCHO;
	int ALTO;
};

struct Input
{
	enum Keys
	{
		Backspace = 0x08, Tab,
		Clear = 0x0C, Enter,
		Shift = 0x10, Control, Alt,
		Escape = 0x1B,
		Space = 0x20, PageUp, PageDown, End, Home, Left, Up, Right, Down,
		Zero = 0x30, One, Two, Three, Four, Five, Six, Seven, Eight, Nine,
		A = 0x41, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
		NumPad0 = 0x60, NumPad1, NumPad2, NumPad3, NumPad4, NumPad5, NumPad6, NumPad7, NumPad8, NumPad9, 
		F1 = 0x70, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12
	};
}input;

struct cuadro{
	int x,y;
	int ancho,alto;
};//Similar a la estructura rect de winapi

//Variables Globales
int *ptrBuffer;
unsigned int anchoBmp;
unsigned int altoBmp;
DIMENSION dmnFigura;
POSITION posFigura;
bool KEYS[256];

miBitmap mbmp, fnaf1, fnaf2, fnaf3, fnaf4, chica, cup, fnaf[4], inicio, fin,fin2,wea;

cuadro *pcuadros, *pcuadros2, *pcuadros3, *pcuadros4, *pcuadros5;
int frameactual, frameactual2;

int tonoazul;

bool colx= false;
bool coly= false;
bool colx2= false;
bool coly2= false;
bool colx3= false;
bool coly3= false;
bool colx4= false;
bool coly4= false;
bool colx5= false;
bool coly5= false;

bool cup1= true;
bool cup2=true;
bool cup3= true;	
bool cup4= true;
bool pato= true;

bool der = true;



//Definicion de funciones
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
void LimpiarFondo(int *ptrBuffer, unsigned int color, int area);
void DibujaFigura(int *ptrBuffer, unsigned int color, int anchoWnd, int altoWnd, DIMENSION dimension, POSITION position);
void imageblt(int *punteroDestino, int *punteroOrigen, int inicioXDestino, int inicioYDestino, int inicioXOrigen, int inicioYOrigen, int ancho, int alto, int anchodefondo, int anchodeorigen);
void transparentblt(int* punteroDestino, int* punteroOrigen, int inicioXDestino, int inicioYDestino, int inicioXOrigen, int inicioYOrigen, int ancho, int alto, int anchodefondo, int anchodeorigen, int colortransparente);
void scaleblt(int* punteroDestino, int* punteroOrigen, int inicioXDestino, int inicioYDestino, int inicioXOrigen, int inicioYOrigen, int ancho, int alto, int anchodefondo, int anchodeorigen, int escalaX, int escalaY);
void mirrorxblt(int* punteroDestino, int* punteroOrigen, int inicioXDestino, int inicioYDestino, int inicioXOrigen, int inicioYOrigen, int ancho, int alto, int anchodefondo, int anchodeorigen);
//void mirroryblt(int* punteroDestino, int* punteroOrigen, int inicioXDestino, int inicioYDestino, int inicioXOrigen, int inicioYOrigen, int ancho, int alto, int anchodefondo, int anchodeorigen);
void mirrorxyblt(int* punteroDestino, int* punteroOrigen, int inicioXDestino, int inicioYDestino, int inicioXOrigen, int inicioYOrigen, int ancho, int alto, int anchodefondo, int anchodeorigen);
void cambiarTonalidadAzul(int* punteroImagen, int ancho, int alto, unsigned char color);
void colichon();
void MainRender(HWND hWnd);
void Init();
void KeysEvents();