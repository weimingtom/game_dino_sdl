#include <unistd.h>
#include <SDL/SDL.h> //sudo apt install libsdl1.2-dev
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include "sdl_api.h"

#define BUFSIZE 256

//#define WINDOW_BGCOLOR 0x000000 
//#define WINDOW_BGCOLOR 0xCCCCCC 

#define SSS_APPNAME "SimpleScriptSystem"
#define SSS_VERSION "0.0.1"

#define WIN_BPP 32
#define NUM_COLORS	256
#define SSS_TITLE SSS_APPNAME " SDL " SSS_VERSION

//extern void CanvasLock(void);
//extern void CanvasUnlock(void);
extern void MainFrameRefresh(void);
extern void CanvasSetClipRect(int x1, int y1, int x2, int y2);
extern void CanvasBitblt(int dx, int dy, int dw, int dh, int sx, int sy, int sw, int sh, const unsigned char *ptr, int w, int h, char hasPink);

//------------------------------------

//------------------------------------

extern int MainFrameGetMsg(void);
extern void MainFrameRefresh(void);
extern void MainFrameResize(int w, int h);
extern void MainFrameSetTitle(const char *str);

//------------------------------------

extern void MiscTrace(const char *fmt, ... );
extern void MiscAppInit(void);
extern void MiscAppExit(void);

//------------------------------------

extern void KeyboardInit(void);
extern void KeyboardRelease(void);
extern void KeyboardChar(int Key);
//extern int KeyboardGetKeyboardStatus(void);


//------------------------------------

extern void MouseInit(void);
extern void MouseRelease(void);
extern void MouseMove(int X, int Y);
extern void MouseLButtonDown(int X, int Y);
extern void MouseLButtonUp(int X, int Y);
extern int MouseGetMouseStatus(void);

//------------------------------------
extern SDL_Surface* CanvasInit(void);
extern void CanvasRelease(void);

//lock canvas
extern void CanvasLock(void);
extern void CanvasUnlock(void);

//color 
extern void CanvasSetColor(int color);
extern int CanvasRGB(int r, int g, int b);

//draw pixel
//extern void CanvasSetPixel(int x, int y, int color);
extern int CanvasGetPixel(int x, int y);

//draw line
extern void CanvasMoveTo(int x, int y);
extern void CanvasLineTo(int x, int y);
extern void CanvasDrawLine(int x1, int y1, int x2, int y2, int color, int alpha);
extern void CanvasFillRect(int x1, int y1, int x2, int y2, int color);

extern void CanvasBitblt(int dx, int dy, int dw, int dh, int sx, int sy, int sw, int sh, const unsigned char *ptr, int w, int h, char hasPink);
extern void CanvasDrawPixel(int x1, int y1, int color, int alpha);

//-------------------------------------


static SDL_Surface *s_screen;
static SDL_Rect s_screenArea;
static int s_done;

#ifdef __MINGW32__
#undef main
#endif
int main(int argc, char *argv[])
{
	MiscAppInit();
	if (SDL_Init(SDL_INIT_VIDEO) < 0) 
	{
		MiscTrace("Couldn't initialize SDL: %s\n",SDL_GetError());
		return 1;
	}
	MainFrameSetTitle(SSS_TITLE);
	s_screen = CanvasInit();
	MouseInit();
	KeyboardInit();
	s_screenArea.x = 0;
	s_screenArea.y = 0;
	s_screenArea.w = WINDOW_WIDTH;
	s_screenArea.h = WINDOW_HEIGHT;
	s_done = 0;
#if 0
	while (1) 
	{
		if(MainFrameGetMsg())
		{
			break;
		}
		{
			int k = 0;
			int width = WINDOW_WIDTH;
			int height = WINDOW_HEIGHT;
			CanvasLock();
			CanvasSetColor(CanvasRGB(0xff, 0, 0));
			CanvasMoveTo(0, 0);
			CanvasLineTo(width * 2, height * 2);
			for (k = height / 4; k < height / 2; k++)
			{
				CanvasSetPixel(width / 2, k, 
					CanvasRGB(0xff, 0, 0xff));
			}
			CanvasDrawLine(-width, height * 2, width * 2, -height, 
				CanvasRGB(0, 0, 0xff));
			CanvasUnlock();
			MainFrameRefresh();
		}
	}
#else
	//ScriptRun();
extern int main_();
	main_();
#endif
	KeyboardRelease();
	MouseRelease();
	CanvasRelease();
	SDL_Quit();
	return 0;
}

int MainFrameGetMsg(void)
{
	SDL_Event event;
	if(!s_done) // && SDL_WaitEvent(&event);
	{
		if (SDL_PollEvent(&event)) 
		{
			switch (event.type) 
			{
			case SDL_MOUSEMOTION:
				MouseMove(event.button.x, event.button.y);
				break;
		
			case SDL_MOUSEBUTTONDOWN:
				MouseLButtonDown(event.button.x, event.button.y);
				break;
		
			case SDL_MOUSEBUTTONUP:
				MouseLButtonUp(event.button.x, event.button.y);
				break;

			case SDL_KEYDOWN:
				KeyboardChar(event.key.keysym.sym);
				break;
		
			case SDL_QUIT:
				s_done = 1;
				break;
		
			default:
				break;
			}
		}
	}
	else
	{
		return 1;
	}
	return 0;
}

void MainFrameRefresh(void)
{
	SDL_UpdateRects(s_screen, 1, &s_screenArea);
}

void MainFrameResize(int w, int h)
{
	/*
	 * not support
	 */
}

void MainFrameSetTitle(const char *str)
{
	SDL_WM_SetCaption(str, NULL);
}

//------------------------------------------------------------------

//static SDL_Surface *s_screen;
static int s_currentColor;
static int s_currentX;
static int s_currentY;

static void CanvasClearScreen(void)
{
	CanvasLock();
	CanvasUnlock();
	SDL_UpdateRect(s_screen, 0, 0, 0, 0);
}

#if 0
static void DrawBox(int x1, int y1, int x2, int y2, int color)
{
	CanvasDrawLine(x1, y1, x2, y1, color);
	CanvasDrawLine(x2, y1, x2, y2, color);
	CanvasDrawLine(x2, y2, x1, y2, color);
	CanvasDrawLine(x1, y2, x1, y1, color);
}
#endif

SDL_Surface* CanvasInit(void)
{
	s_screen = SDL_SetVideoMode(WINDOW_WIDTH, WINDOW_HEIGHT, WIN_BPP, SDL_SWSURFACE);
	if (s_screen == NULL) 
	{
		MiscTrace("Couldn't set display mode: %s\n", SDL_GetError());
		exit(4);
	}
	CanvasClearScreen();
	return s_screen;
}

void CanvasRelease(void)
{
	SDL_FreeSurface(s_screen);
	s_screen = NULL;
}

#if 0
static void clear()
{
	Uint8 *buffer;
    buffer = (Uint8 *)s_screen->pixels;
	memset(buffer, 0, 
		(s_screen->h) * (s_screen->w) * (s_screen->format->BytesPerPixel));
}
#endif

void CanvasLock(void)
{
	static SDL_Rect area = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};
	if(SDL_MUSTLOCK(s_screen))
	{
		if(SDL_LockSurface(s_screen) < 0)
		{
			MiscTrace("Couldn't lock display surface: %s\n", SDL_GetError());
			exit(3);
		}
	}
	SDL_FillRect(s_screen, &area, 
		SDL_MapRGB(s_screen->format, 
		(Uint8)WINDOW_BGCOLOR, 
		(Uint8)(WINDOW_BGCOLOR >> 8), 
		(Uint8)(WINDOW_BGCOLOR >> 16)));
}

void CanvasUnlock(void)
{
	if (SDL_MUSTLOCK(s_screen))
	{
		SDL_UnlockSurface(s_screen);
	}
}

void CanvasSetPixel(int x, int y, int color)
{
	Uint32 *bufp;
	if(x < 0 || x >= s_screen->w || y < 0 || y >= s_screen->h) 
		return;
	bufp = (Uint32 *)s_screen->pixels + y * s_screen->pitch / 4 + x;
	*bufp = color;
}

int CanvasGetPixel(int x, int y)
{
	Uint32 color = 0;
	Uint32 *bufp;
	bufp = (Uint32 *)s_screen->pixels + y * s_screen->pitch / 4 + x;
	color = *bufp;
	return color;
}

/*
 * Bresenham's line algorithm
 * e'=2*e*dx.
 */
void CanvasDrawLine(int x1, int y1, int x2, int y2, int color)
{
    int x, y, ax, ay, sx, sy;
    ax = ((x2 - x1 > 0) ? (x2 - x1) : (x1 - x2)) << 1;
    ay = ((y2 - y1 > 0) ? (y2 - y1) : (y1 - y2)) << 1;
    sx = (x2 - x1 >= 0) ? 1 : -1;
    sy = (y2 - y1 >= 0) ? 1 : -1;
    x = x1;
    y = y1;
    if (ax > ay)
    {
        int d = ay - (ax >> 1);
        while (x != x2)
        {
            CanvasSetPixel(x, y, color);
            if (d > 0 || (d == 0 && sx == 1))
            {
                y += sy;
                d -= ax;
            }
            x += sx;
            d += ay;
        }
    }
    else
    {
        int d = ax - (ay >> 1);
        while (y != y2)
        {
            CanvasSetPixel(x, y, color);
            if (d > 0 || (d == 0 && sy == 1))
            {
                x += sx;
                d -= ay;
            }
            y += sy;
            d += ax;
        }
    }
    CanvasSetPixel(x, y, color);
}

void CanvasSetColor(int color)
{
	s_currentColor = color;
}

void CanvasMoveTo(int x, int y)
{
	s_currentX = x;
	s_currentY = y;
}

void CanvasLineTo(int x, int y)
{
	CanvasDrawLine(s_currentX, s_currentY, x, y, s_currentColor);
	s_currentX = x;
	s_currentY = y;
}

int CanvasRGB(int r, int g, int b)
{
	return SDL_MapRGB(s_screen->format, (Uint8)r, (Uint8)g, (Uint8)b);
}

//------------------------------------------------------------------

#ifndef BOOL
#define BOOL int
#endif
#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif

#define PEN_DOWN 0xC0
#define PEN_MOVE 0x90
#define PEN_UP 0x20
#define PEN_LEAVE 0x8

struct mouse_buffer_struct {
	int status; //Is dragging
	int buffer[BUFSIZE];
	int head; //Reading position
	int tail; //Writing position
	BOOL reading; //Is reading
	BOOL writing; //Is writing
};
static struct mouse_buffer_struct s_MBuffer;

static int MouseGetBufNum(int head, int tail)
{
	if (tail > head)
	{
		return tail - head;
	}
	else if (tail < head)
	{
		return BUFSIZE - head + tail;
	} 
	else
	{
		return 0;
	}
}

void MouseInit(void)
{
    s_MBuffer.status = 0;
    s_MBuffer.writing = FALSE;
    s_MBuffer.reading = FALSE;
    s_MBuffer.head = 0;
    s_MBuffer.tail = 0;
	memset(s_MBuffer.buffer, 0, sizeof(s_MBuffer.buffer));
}

void MouseRelease(void)
{
    s_MBuffer.head = 0;
    s_MBuffer.tail = 1;
    s_MBuffer.buffer[0] = -1;
}

void MouseMove(int X, int Y)
{
	if (s_MBuffer.status == 1 && 
		MouseGetBufNum(s_MBuffer.head, s_MBuffer.tail) < BUFSIZE - 1)
	{
		s_MBuffer.writing = TRUE;
		s_MBuffer.buffer[s_MBuffer.tail] = 
			((X & 0xFFF) << 12) | 
			(Y & 0xFFF) | 
			(PEN_MOVE << 24);
		if (s_MBuffer.tail == BUFSIZE - 1)
		{
			s_MBuffer.tail = 0;
		}
		else
		{
			s_MBuffer.tail++;
		}
		s_MBuffer.writing = FALSE;
	}
}

void MouseLButtonDown(int X, int Y)
{
	if (MouseGetBufNum(s_MBuffer.head, s_MBuffer.tail) < BUFSIZE - 1)
	{
		s_MBuffer.status = 1;
		s_MBuffer.writing = TRUE;
		s_MBuffer.buffer[s_MBuffer.tail] = 
			((X & 0xFFF) << 12) | 
			(Y & 0xFFF) | 
			(PEN_DOWN << 24);
		if (s_MBuffer.tail == BUFSIZE - 1)
		{
			s_MBuffer.tail = 0;
		}
		else
		{
			s_MBuffer.tail++;
		}
		s_MBuffer.writing = FALSE;
	}
}

void MouseLButtonUp(int X, int Y)
{
	if (s_MBuffer.status == 1)
	{
		s_MBuffer.writing = TRUE;
		s_MBuffer.buffer[s_MBuffer.tail] = 
			((X & 0xFFF) << 12) | 
			(Y & 0xFFF) | 
			(PEN_UP << 24);
		if (s_MBuffer.tail == BUFSIZE - 1)
		{
			s_MBuffer.tail = 0;
		}
		else
		{
			s_MBuffer.tail++;
		}
		s_MBuffer.writing = FALSE;
		s_MBuffer.status = 0;
	}
}

int MouseGetMouseStatus(void)
{
    int status;
    if (MouseGetBufNum(s_MBuffer.head, s_MBuffer.tail) > 0)
    {
		/*
		MiscTrace("MouseGetBufNum: %d\n", 
			MouseGetBufNum(s_MBuffer.head, s_MBuffer.tail));
        */
		s_MBuffer.reading = TRUE;
        status = s_MBuffer.buffer[s_MBuffer.head];
		if (s_MBuffer.head == BUFSIZE - 1)
		{
			s_MBuffer.head = 0;
		}
		else
		{
			s_MBuffer.head++;
		}
		s_MBuffer.reading = FALSE;
        return status;
    }
    return 0;
}

//-------------------------------------------

struct keyboard_buffer_struct {
	int status;
	int buffer[BUFSIZE];
	int head;
	int tail;
	BOOL reading;
	BOOL writing;
};
static struct keyboard_buffer_struct s_KBuffer;

static int KeyboardGetBufNum(int head, int tail)
{
	if (tail > head)
	{
		return tail - head;
	}
	else if (tail < head)
	{
		return BUFSIZE - head + tail;
	} 
	else
	{
		return 0;
	}
}

void KeyboardInit(void)
{
    s_KBuffer.head = 0;
    s_KBuffer.tail = 0;
	memset(s_KBuffer.buffer, 0, sizeof(s_KBuffer.buffer));
}

void KeyboardRelease(void)
{
    s_KBuffer.head = 0;
    s_KBuffer.tail = 1;
    s_KBuffer.buffer[0] = -1;
}

void KeyboardChar(int Key)
{
	if (KeyboardGetBufNum(s_KBuffer.head, s_KBuffer.tail) < BUFSIZE - 1)
	{
		int s = 0;
		s_KBuffer.writing = TRUE;
		s_KBuffer.buffer[s_KBuffer.tail] = ((int)Key) | (s << 8);
		if (s_KBuffer.tail == BUFSIZE - 1)
		{
			s_KBuffer.tail = 0;
		}
		else
		{
			s_KBuffer.tail++;
		}
		s_KBuffer.writing = FALSE;
	}
}

int KeyboardGetKeyboardStatus(void)
{
    int status;
    if (KeyboardGetBufNum(s_KBuffer.head, s_KBuffer.tail) > 0)
    {
		/*
		MiscTrace("KeyboardGetBufNum: %d\n", 
		   KeyboardGetBufNum(s_KBuffer.head, s_KBuffer.tail));
		*/
        s_KBuffer.reading = TRUE;
        status = s_KBuffer.buffer[s_KBuffer.head];
		if (s_KBuffer.head == BUFSIZE - 1)
		{
			s_KBuffer.head = 0;
		}
		else
		{
			s_KBuffer.head++;
		}
		s_KBuffer.reading = FALSE;
        return status;
    }
    return 0;
}

//-------------------------------------------

void MiscTrace(const char *fmt, ...)
{
	char str[1000];
	va_list args;
	va_start(args, fmt);
	vsprintf(str, fmt, args);
#if USE_ANA
	__android_log_vprint(ANDROID_LOG_INFO, SSS_APPNAME, fmt, args);
#else
#ifdef WIN32
	OutputDebugStringA(str);
#else
	fprintf(stderr, "%s", str);
#endif
#endif
    va_end(args);
}

void MiscSleep(int n) {
#if defined(WIN32)
	Sleep((unsigned long)(1000L * n));
#else
	sleep((unsigned long)(n));
#endif
}

void MiscAppInit(void)
{
#if defined(WIN32) && defined(_DEBUG) && USE_WIN_LEAK
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#ifdef _CONSOLE
	_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
	_CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDOUT);
	_CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_FILE);
	_CrtSetReportFile(_CRT_ERROR, _CRTDBG_FILE_STDOUT);
	_CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_FILE);
	_CrtSetReportFile(_CRT_ASSERT, _CRTDBG_FILE_STDOUT);
#else
	_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG);
	_CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_DEBUG);
	_CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_DEBUG);
#endif
#endif
	/* Test Memory Leak */
	/*
	malloc(10);
	*/
}

void MiscAppExit(void)
{
#if defined(WIN32) && defined(_DEBUG) && USE_WIN_LEAK
	_CrtDumpMemoryLeaks();
#endif
}

//--------------------

//FIXME:
void CanvasDrawPixel(int x1, int y1, int color, int alpha)
{
	if (alpha != 255)
	{
		//FIXME:
		//__asm int 3;
	}
	else
	{
		CanvasSetPixel(x1, y1, color);
	}
}


