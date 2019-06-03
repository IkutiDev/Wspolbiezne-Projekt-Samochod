#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include <X11/Xatom.h>
#include <X11/keysym.h>
#include <X11/keysymdef.h>
#include <math.h>
#include <locale.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <signal.h>
#define WIDTH 800
#define HEIGHT 600
#define EDGES 4


/*  XLIB Variables  */

Display *     display;
Window win,root_window;
int           screen_num;
static char * appname;
GC            gc;
XColor    red, cyan; 
unsigned int width, height;
unsigned int border_width;
char *       window_name = "Aplikacja wieloosobowego prowadzenia pojazdu";
char *       icon_name   = "Aplikacja wieloosobowego prowadzenia pojazdu";
int          x, y;
char *        display_name = NULL;
unsigned int  display_width, display_height;
unsigned long white_pixel,  black_pixel;    
Colormap  colorMap;
XSizeHints *  size_hints;
XWMHints   *  wm_hints;
XClassHint *  class_hints;
XTextProperty windowName, iconName;
XEvent        report;
XGCValues     values;
XPoint points[EDGES] = { { 60, 110 } , { 110, 110 } , { 110, 130 } , { 60, 130 } };
XPoint rpoints2[EDGES];
XPoint rpoints[EDGES];
int xRotation,yRotation;
int degrees=360;
int coords[9];
int value;
struct sockaddr_in recipient;
int socket_descriptor,size;
void init_display(){
    if ( !( size_hints  = XAllocSizeHints() ) || 
     !( wm_hints    = XAllocWMHints()   ) ||
     !( class_hints = XAllocClassHint() )    ) {
    fprintf(stderr, "%s: couldn't allocate memory.\n", appname);
    exit(EXIT_FAILURE);
    }
    if ( (display = XOpenDisplay(display_name)) == NULL ) {
    fprintf(stderr, "%s: couldn't connect to X server %s\n", appname, display_name);
    exit(EXIT_FAILURE);
    }
    screen_num     = DefaultScreen(display);
    display_width  = DisplayWidth(display, screen_num);
    display_height = DisplayHeight(display, screen_num);

    root_window    = RootWindow(display, screen_num); /* find the ID of the root window of the screen. */
    white_pixel    = WhitePixel(display, screen_num); /* find the value of a white pixel on this screen. */
    black_pixel    = BlackPixel(display, screen_num); /* find the value of a black pixel on this screen. */ 
    x = y = 100;
    border_width = 2; 

    win = XCreateSimpleWindow(display, root_window,
                  x, y, WIDTH, HEIGHT, border_width,
                  white_pixel,black_pixel);
    if ( XStringListToTextProperty(&window_name, 1, &windowName) == 0 ) {
    fprintf(stderr, "%s: structure allocation for windowName failed.\n", appname);
    exit(EXIT_FAILURE);
    }

    if ( XStringListToTextProperty(&icon_name, 1, &iconName) == 0 ) {
    fprintf(stderr, "%s: structure allocation for iconName failed.\n", appname);
    exit(EXIT_FAILURE);
    }
    size_hints->flags       = PPosition | PSize | PMinSize | PMaxSize;
    size_hints->min_width   = WIDTH;
    size_hints->min_height  = HEIGHT;
    size_hints->max_width = WIDTH;
    size_hints->max_height = HEIGHT;

    wm_hints->flags         = StateHint | InputHint;
    wm_hints->initial_state = NormalState;
    wm_hints->input         = True;
    class_hints->res_name   = appname;
    class_hints->res_class  = "Aplikacja wieloosobowego prowadzenia pojazdu";
    XSetWMProperties(display, win, &windowName, &iconName, NULL, 0,
             size_hints, wm_hints, class_hints);
    XMapWindow(display, win);
    XSelectInput(display, win, 
             ExposureMask | KeyPressMask);
    gc = XCreateGC(display, win, 0, &values);
    XSetBackground(display, gc, white_pixel);
    colorMap = DefaultColormap(display, screen_num);  
    if (! XAllocNamedColor(display, colorMap, "red", &red, &red)) {
        fprintf(stderr, "XAllocNamedColor - failed to allocated 'red' color.\n");
        exit(1);
    }
    if (! XAllocNamedColor(display, colorMap, "cyan", &cyan, &cyan)) {
        fprintf(stderr, "XAllocNamedColor - failed to allocated 'cyan' color.\n");
        exit(1);
    }
}
/* Change rotation of the vehicle */
void rotate(XPoint *points, int rx, int ry)
{
    int i;
    for (i=0; i<EDGES; i++) {
    double x, y;
    x = points[i].x - rx;
    y = points[i].y - ry;
    points[i].x =  rx + x * cosf(degrees*M_PI/180) - y * sinf(degrees*M_PI/180);
    points[i].y =  ry + x * sinf(degrees*M_PI/180) + y * cosf(degrees*M_PI/180);
    }
}
void rysuj_samochod(){
    int i;
        for (i=0;i<EDGES;i++) rpoints2[i] = points[i];
        for(i=0;i<EDGES;i++) rpoints[i]=points[i];
            rpoints[1].x=points[1].x-40;
            rpoints[2].x=points[2].x-40;
            xRotation=(points[0].x+points[1].x)/2;
        yRotation=(points[1].y+points[2].y)/2;
        rotate(rpoints2, xRotation, yRotation);
        rotate(rpoints, xRotation, yRotation);
        XClearWindow(display, win);
        XSetForeground(display, gc, red.pixel);
        XFillPolygon(display, win, gc, rpoints2, EDGES, Convex, CoordModeOrigin);
        XSetForeground(display, gc, cyan.pixel);
        XFillPolygon(display,win,gc,rpoints,EDGES, Convex,CoordModeOrigin);
        XFlush(display);
}
void *kontroluj_samochod(){
while (1) {
    XNextEvent(display, &report);

    switch(report.type) {
    case KeyPress:
    if(report.xkey.keycode==0x71)
    {
        value=3;
        sendto(socket_descriptor,(char*) &value,sizeof(int), 0, (struct sockaddr*) &recipient, size);
    }
    else if(report.xkey.keycode==0x72){
        value=4;
        sendto(socket_descriptor,(char*) &value,sizeof(int), 0, (struct sockaddr*) &recipient, size);
    }
        else if(report.xkey.keycode==0x6f){
        value=1;
        sendto(socket_descriptor,(char*) &value,sizeof(int), 0, (struct sockaddr*) &recipient, size);
    }
        else if(report.xkey.keycode == 0x74){
        value=2;
        sendto(socket_descriptor,(char*) &value,sizeof(int), 0, (struct sockaddr*) &recipient, size);
    }
        else if (report.xkey.keycode == 0x09){ 
    value=6;
    sendto(socket_descriptor,(char*) &value,sizeof(int), 0, (struct sockaddr*) &recipient, size);
            exit(0);
        }

    case Expose:
    rysuj_samochod();
        break;
    }
    }
}

int main(int argc, char **argv) {
    pthread_t tid;
      if(argc != 2)
  {
    printf("Niepoprawne wywolanie programu.\n");
    printf("./klient ADRES_IP_SERWERA\n");
    return -1;
  }
    size = sizeof(struct sockaddr_in);
    recipient.sin_family = AF_INET;
    recipient.sin_port = htons((ushort) 5000);
    inet_pton(AF_INET, argv[1], &recipient.sin_addr);

    socket_descriptor = socket(PF_INET, SOCK_DGRAM, 0);
bind(socket_descriptor, (struct sockaddr*) &recipient, size);
    init_display();
    value=5;
    sendto(socket_descriptor,(char*) &value,sizeof(int), 0, (struct sockaddr*) &recipient, size);
    recvfrom(socket_descriptor, coords, sizeof(coords), 0, (struct sockaddr*) &recipient, &size);
    int j=0;
    degrees=coords[8];
        for(int i=0;i<EDGES;i++){
        points[i].x=coords[j];
        points[i].y=coords[j+1];
        j+=2;
    }
    pthread_create(&tid,NULL,kontroluj_samochod,NULL);
    int result=0;
while(1){
    recvfrom(socket_descriptor, coords, sizeof(coords), 0, (struct sockaddr*) &recipient, &size);
    degrees=coords[8];
    j=0;
    for(int i=0;i<EDGES;i++){
        points[i].x=coords[j];
        points[i].y=coords[j+1];
        j+=2;
    }
    rysuj_samochod();
}
    return 0;   
}