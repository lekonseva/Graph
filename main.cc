/*
 * main.cc
 *
 *  Created on: 5 дек. 2020 г.
 *      Author: User
 */
#define _USE_MATH_DEFINES
#include <cmath>
#include <fstream>
#include <vector>
#include <cstdint>
double sinc(double x){
	if(x==0) return 1.;
	return sin(x)/x;
}
double ff(double x,double y){
	return sinc(sqrt(x*x+y*y));
}
constexpr int WIDTH = 1280, HEIGHT = 1024;
constexpr double RX=30.,RY=30.,RZ=200.,XMAX=30.,XMIN=-30.,YMAX=30.,YMIN=-30.,STEP1=0.2,STEP2=0.001;
#pragma pack(push,1)
struct TGA{
	uint8_t id_len;
	uint8_t pal_type;
	uint8_t img_type;
	uint8_t pal_desc[5];
	uint16_t xpos,ypos;
	uint16_t w,h;
	uint8_t bpp;
	uint8_t img_desc;
};
#pragma pack(pop)
void plot(std::vector<uint32_t>& pixels, int x, int y, uint32_t c,std::vector<int>& h)
{
	if (x < 0 || x >= WIDTH) return;
	if ( y > h[x]) return;
	h[x]=y;
	if(y<0)return;
	if ( (int)(y * WIDTH + x) <= (int)pixels.size())
		pixels[y*WIDTH + x] = c;
}
int main(){
	int sx=0,sy=0;
	double z;
	std::fstream f;
	TGA h {};
	std::vector<int> hor;
	hor.resize(WIDTH);
	for(auto && a:hor)a=HEIGHT;
	std::vector <uint32_t> picture;
	picture.resize(WIDTH*HEIGHT);
	for(auto && c:picture)
		c=0xFF008080;
	h.img_type=2;
	h.w=WIDTH;
	h.h=HEIGHT;
	h.bpp=32;
	h.img_desc=0x20;
	for(double i=XMAX;i>XMIN;i=i-STEP1){
			for(double j=YMAX;j>YMIN;j=j-STEP2){
				z=ff(i,j);
				sx=int(WIDTH/2-RX*i*cos(M_PI/6)+RY*j*cos(M_PI/6));
				sy=int(HEIGHT/2+RX*i*sin(M_PI/6)+RY*j*sin(M_PI/6)-RZ*z);

				plot(picture, sx, sy, 0xffffffff,hor);
			}
		}
	for(auto && a:hor)a=HEIGHT;
	for(double j=YMAX;j>YMIN;j=j-STEP1){
			for(double i=XMAX;i>XMIN;i=i-STEP2){
				z=ff(i,j);
				sx=int(WIDTH/2-RX*i*cos(M_PI/6)+RY*j*cos(M_PI/6));
				sy=int(HEIGHT/2+RX*i*sin(M_PI/6)+RY*j*sin(M_PI/6)-RZ*z);

				plot(picture, sx, sy, 0xffffffff,hor);
			}
		}
	f.open("1.tga",std::ios::out|std::ios::binary);
	f.write(reinterpret_cast<char*>(&h),sizeof(TGA));
	f.write(reinterpret_cast<char*>(&picture[0]),WIDTH*HEIGHT*4);
	f.close();
	return 0;



}



