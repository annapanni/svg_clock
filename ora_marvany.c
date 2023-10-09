/*
!uses xlink for curved text
!some older svg viewers don't support curved texts or filters
compile as:
  gcc ora.c -o ora -lm
show current time:
  date "+%H %M %S" | ./ora
References: mdn web docs, svgfilters.com
*/
#include <stdio.h>
#include <math.h>
#ifndef M_PI
  #define M_PI 3.14159265358979323846
#endif


FILE *img;
char beige[] = "#ede9da";
char black[] = "black";
char transparent[] = "transparent";
char shadow[] = "#2E2E2EB5";
double sh_offset = 1.7;
double width = 200;
double height = 200;
double rad1 = 90;
double rad2 = 85;
double rad3 = 78;
double rad4 = 55;
double rad5 = 20;

//all drawing functions use the center of the picture as origo
void drawCircle(double cx, double cy, double r, char* stroke, char* fill, double stroke_width, char* extras){
  fprintf(img, "<circle cx=\"%lf\" cy=\"%lf\" r=\"%lf\" stroke=\"%s\" fill=\"%s\" stroke-width=\"%lf\" %s/>\n",
    cx + width/2, cy + height/2, r, stroke, fill, stroke_width, extras);
}

void drawLine(double x1, double y1, double x2, double y2, char* stroke, double stroke_width, char* extras){
  fprintf(img, "<line x1=\"%lf\" y1=\"%lf\" x2=\"%lf\" y2=\"%lf\" stroke=\"%s\" stroke-width=\"%lf\" %s/>\n",
    x1 + width/2, y1 + height/2, x2 + width/2, y2 + height/2, stroke, stroke_width, extras);
}

void drawCurvedText(char* text, double fi, double dist){
  fprintf(img, "<path id=\"curve%lf\" d=\"M %lf %lf L %lf %lf\" stroke=\"transparent\" fill=\"transparent\"/> \n\
    <text font-size=\"14px\" font-family=\"DejaVu, serif\"><textPath xlink:href=\"#curve%lf\" startOffset=\"50%%\" text-anchor=\"middle\">%s</textPath></text>\n",
    fi, //acts as id
    cos(fi-M_PI/12)*dist+width/2, sin(fi-M_PI/12)*dist+height/2, cos(fi+M_PI/12)*dist+width/2, sin(fi+M_PI/12)*dist+height/2,
    fi, //acts as id
    text
  );
}

void drawHand(double fi, double len, double startRad, double rhomb_size, double ctrlP_l,
              //angle, length, offset of the hand, relative size of rhomboids, length of control weight,
              char* stroke, double stroke_width, char* stroke_style, char* extras, double offset_x, double offset_y){
             //stroke colour, stroke width, stroke style (for the shadow: filter and offset)
  double cf = cos(fi);
  double sf = sin(fi);
  double side = len*rhomb_size/sqrt(3);
  //default x and y relative coordinates of a contolpoint
  double ctrlPx = cf*ctrlP_l;
  double ctrlPy = sf*ctrlP_l;
  //corners of the rhomboid relative to each other
  double px[4] = {cos(fi-M_PI/6)*side, cos(fi+M_PI/6)*side, cos(fi-M_PI/6)*side*-1, cos(fi+M_PI/6)*side*-1};
  double py[4] = {sin(fi-M_PI/6)*side, sin(fi+M_PI/6)*side, sin(fi-M_PI/6)*side*-1, sin(fi+M_PI/6)*side*-1};
  fprintf(img, "<path d=\" \
    M %lf %lf \
    l %lf %lf \
    c %lf %lf  %lf %lf  %lf %lf \
    c %lf %lf  %lf %lf  %lf %lf \
    c %lf %lf  %lf %lf  %lf %lf \
    c %lf %lf  %lf %lf  %lf %lf \
    l %lf %lf\"\
    stroke=\"%s\" stroke-width=\"%lf\" fill=\"transparent\" style=\"stroke-linecap:round;stroke-linejoin:%s;fill:none\" %s/> \
    \n",
    cos(fi + M_PI)*(startRad)+width/2+offset_x, sin(fi + M_PI)*(startRad)+height/2+offset_y, //starting point
    cf*(len*(1-rhomb_size)), sf*(len*(1-rhomb_size)),                      //straight line
    ctrlPx, ctrlPy, ctrlPy*-1+px[0], ctrlPx+py[0], px[0], py[0],           //
    ctrlPy*-1, ctrlPx, ctrlPx*-1+px[1], ctrlPy*-1+py[1], px[1], py[1],     //drawing the
    ctrlPx*-1, ctrlPy*-1, ctrlPy+px[2], ctrlPx*-1+py[2], px[2], py[2],     //rhomboid
    ctrlPy, ctrlPx*-1, ctrlPx+px[3], ctrlPy+py[3], px[3], py[3],           //
    cf*(len*-(1-rhomb_size)), sf*(len*-(1-rhomb_size)),                    //another straigth line
    stroke, stroke_width, stroke_style, extras
  );

}

void drawClock(int hour, int minute, int second){
  img = fopen("./ora2.svg", "w");
  fprintf(img, "<svg width=\"%lf\" height=\"%lf\" xmlns=\"http://www.w3.org/2000/svg\" xmlns:xlink=\"http://www.w3.org/1999/xlink\" version=\"1.1\">\n", width, height);
  fprintf(img, "<defs><filter id=\"f1\" x=\"-0.1\" y=\"-0.1\" width=\"1.2\" heigth=\"1.2\"> \
      <feGaussianBlur in=\"SourceGraphic\" stdDeviation=\"1\" /> \
    </filter> \
    <filter id=\"noise\"> \
    <feTurbulence baseFrequency=\"0.12\" numOctaves=\"12\" type=\"fractalNoise\" result=\"zaj1\"></feTurbulence> \
    <feComposite in=\"SourceGraphic\" in2=\"zaj1\" operator=\"in\" result=\"9b1445e9-8b1e-4c4a-bd32-b1f908b0d5cd\"></feComposite> \
    </filter></defs>\n");
  //draw background
    // "style=\"fill:none\"" is necessary for inkscape to handle transparency correctly
  drawCircle(0, 0, rad1, black, transparent, 6, "style=\"fill:none\"");
  drawCircle(0, 0, rad1, black, beige, 0, "filter=\"url(#noise)\"");
  drawCircle(0, 0, rad1-(rad1-rad2)/2,  "#ebdeab", transparent, rad1-rad2, "style=\"fill:none\"");
  drawCircle(0, 0, rad2, black, transparent, 0.5, "style=\"fill:none\"");
  drawCircle(0, 0, rad3, black, transparent, 0.5, "style=\"fill:none\"");
  drawCircle(0, 0, rad4-1, "#ebdeab", transparent, 2, "style=\"fill:none\"");
  drawCircle(0, 0, rad4, black, transparent, 0.2, "style=\"fill:none\"");
  drawCircle(0, 0, rad4-2, black, transparent, 0.2, "style=\"fill:none\"");

  //draw marks
  for (int i=0; i<60; i++){
    double fi = (2*M_PI/60) * i;
    drawLine(cos(fi)*rad2, sin(fi)*rad2, cos(fi)*rad3, sin(fi)*rad3, black, 1, "");
  }
  //draw large marks and numbers
  char* roman[12] = {"I", "II", "III", "IV", "V", "VI", "VII", "VIII", "IX", "X", "XI", "XII"};
  for (int i=0; i<12; i++){
    double fi = (2*M_PI/12) * i;
    drawLine(cos(fi)*rad2, sin(fi)*rad2, cos(fi)*rad3, sin(fi)*rad3, black, 3, "");
    drawCurvedText(roman[i], fi-M_PI/3, rad4+8);
  }
  //draw hands
  int allseconds = hour*3600 + minute*60 + second;
  double fi = (2*M_PI/(3600*12))*allseconds - (M_PI/2);
  drawHand(fi, rad4+rad5-10, rad5, 0.25, 4, black, 2, "miter", "", 0,0);
  drawHand(fi, rad4+rad5-10, rad5, 0.25, 4, shadow, 2, "miter", "filter=\"url(#f1)\"", sh_offset, sh_offset);
  fi = (2*M_PI/3600)*(allseconds - hour*3600) - (M_PI/2);
  drawHand(fi, rad5+rad3-8, rad5, 0.2, 5, black, 1.2, "round" ,"", 0,0);
  drawHand(fi, rad5+rad3-8, rad5, 0.2, 5, shadow, 1.2, "round" ,"filter=\"url(#f1)\"", sh_offset,sh_offset);
  fi = (2*M_PI/60)*second - (M_PI/2);
  drawLine(cos(fi+M_PI)*rad5, sin(fi+M_PI)*rad5, cos(fi)*(rad3-2), sin(fi)*(rad3-2), black, 0.5, "");
  drawLine(cos(fi+M_PI)*rad5+sh_offset, sin(fi+M_PI)*rad5+sh_offset, cos(fi)*(rad3-2)+sh_offset, sin(fi)*(rad3-2)+sh_offset, shadow, 0.5, "filter=\"url(#f1)\"");

  drawCircle(0,0, 2.5, black, black, 0, "");
  drawCircle(0,0, 1, black, "#bfb58c", 0, "");

  fprintf(img, "</svg>\n");
  fclose(img);
}

int main(void){
  int hour, minute, second;
  scanf("%d %d %d", &hour, &minute, &second);

  drawClock(hour, minute, second);

  return 0;
}
