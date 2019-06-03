#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#define WIDTH 800
#define HEIGHT 600
#define POINTS 8
typedef struct 
{
    int id;
    struct sockaddr_in adres;
} uzytkownik;
int coords[9]={60,110,110,110,110,130,60,130};
int degrees=360;
int id=0;
uzytkownik tablicaUzytkownikow[999];
int sd, clen,i;
void wyslijPozycjeSamochodu(){
while(1){
    usleep(200000);
    for(i=0;i<id;i++){
        sendto(sd,coords, sizeof(coords), 0, (struct sockaddr *) &tablicaUzytkownikow[i].adres, clen);
    }
}
}
bool checkCollision(int x,int y){
    if(x>20 && y>0 && x<WIDTH && y<HEIGHT){
        return true;
    }
    return false;
}
void move(bool goForward){
    int i;
    if(goForward){
        if(checkCollision(coords[0]+5*cosf(coords[POINTS]*M_PI/180),coords[1]+5*sinf(coords[POINTS]*M_PI/180)) && checkCollision(coords[6]+5*cosf(coords[POINTS]*M_PI/180),coords[7]+5*sinf(coords[POINTS]*M_PI/180))){
    for (i=0; i<POINTS; i++) {
if(i%2==0){
coords[i]+=5*cosf(coords[POINTS]*M_PI/180);
}
else{
coords[i]+=5*sinf(coords[POINTS]*M_PI/180);
}
    }
}
}
else{
    if(checkCollision(coords[2]-5*cosf(coords[POINTS]*M_PI/180),coords[3]-5*sinf(coords[POINTS]*M_PI/180)) && checkCollision(coords[4]-5*cosf(coords[POINTS]*M_PI/180),coords[5]-5*sinf(coords[POINTS]*M_PI/180))){
    for (i=0; i<POINTS; i++) {
if(i%2==0){
coords[i]-=5*cosf(coords[POINTS]*M_PI/180);
}
else{
coords[i]-=5*sinf(coords[POINTS]*M_PI/180);
}   }
}
}
}
int main(){
    pthread_t tid;
    struct sockaddr_in sad, cad;
    int buff = 0;
    signal(SIGHUP, SIG_IGN);
    sd=socket(AF_INET, SOCK_DGRAM, 0);
    bzero((char *) &sad, sizeof(sad));
    sad.sin_family = AF_INET;
    sad.sin_addr.s_addr = htonl(INADDR_ANY);
    sad.sin_port = htons((ushort) 5000);
    bind(sd, (struct sockaddr *) &sad, sizeof(sad));
    clen = sizeof(cad);
    coords[POINTS]=degrees;
    pthread_create(&tid,NULL,wyslijPozycjeSamochodu,NULL);
    while (1)
    {
        recvfrom(sd, (char *) &buff, sizeof(int), 0, (struct sockaddr *) &cad, &clen);
        buff=htonl(ntohl(buff));
        switch(buff){
            case 1:{
                move(false);
                break;
            }
            case 2:{
                move(true);
                break;
            }
            case 4:{
                if(coords[POINTS]==360){
                    coords[POINTS]=15;
                }
                else{
                coords[POINTS]+=15;
            }
                break;
            }
            case 3:{
                if(coords[POINTS]==0){
                    coords[POINTS]=345;
                }
                else{
                coords[POINTS]-=15;
            }
                break;
            }
            case 5:{
                tablicaUzytkownikow[id].adres=cad;
                id++;
                if(id==999){
                    id=0;
                }
                break;
            }
            case 6:{
                break;
            }
        }
}
}