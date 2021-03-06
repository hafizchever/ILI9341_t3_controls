/*
  The MIT License (MIT)

  library writen by Kris Kasprzak
  
  Permission is hereby granted, free of charge, to any person obtaining a copy of
  this software and associated documentation files (the "Software"), to deal in
  the Software without restriction, including without limitation the rights to
  use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
  the Software, and to permit persons to whom the Software is furnished to do so,
  subject to the following conditions:
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
  FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
  COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
  IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
  CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

  On a personal note, if you develop an application or product using this library 
  and make millions of dollars, I'm happy for you!

	rev		date			author				change
	1.0		9/2019			kasprzak			initial code
	2.0		9/2020			kasprzak			added shapes and sizes for handles


*/


#include "ILI9341_t3_Controls.h"
#include <ILI9341_t3.h>     // fast display driver lib



/*//////////////////////////////////////////////////////////////////////////////////////////////////////////


horizontal bar chart


*///////////////////////////////////////////////////////////////////////////////////////////////////////////

BarChartH::BarChartH(ILI9341_t3 *disp, float GraphXLoc, float GraphYLoc, float GraphWidth, float GraphHeight, float ScaleLow, float ScaleHigh, float ScaleInc){

	d = disp;
	Low = ScaleLow;
	High = ScaleHigh;
	Inc = ScaleInc;
	gx = GraphXLoc;
	gy = GraphYLoc;
	gw = GraphWidth;
	gh = GraphHeight;
		
}


void BarChartH::init(const char *Title, uint16_t TextColor, uint16_t BarColor, uint16_t BarBColor, uint16_t BackColor, const ILI9341_t3_font_t &TitleFont , const ILI9341_t3_font_t &ScaleFont ){

	
	strncpy(ti, Title, 20);
		
	tf = TitleFont;
	sf = ScaleFont;
	tc = TextColor;
	rc = BarColor;
	ac = BarBColor;
	bc = BackColor;

	redraw = true;

}

void BarChartH::plot(float val){

  if (redraw == true) {
    redraw = false;
	   

    // step val basically scales the hival and low val to the height
    // deducting a small value to eliminate round off errors
    // this val may need to be adjusted
	if (ss){
		d->setFont(sf);
		stepval = MapFloat(Inc, Low, High, 0, gw);

		// paint over previous y scale
		tHi = sf.cap_height * 2 + 8;
		d->fillRect(gx-10, gy + gh + 1, gw+30, tHi, bc);
		d->setTextColor(tc, bc);

		for (i = 0; i <= gw; i += stepval) {

		d->drawFastVLine(i + gx, gy + gh+ 1,  5, tc);
		// draw lables
     		if (High < .1) {
			Dec = 3;
		}
		else  if (High <= 1) {
			Dec = 2;
		}
		else  if (High <= 10) {
			Dec = 1;
		}
		else   {
			Dec = 0;
		}
		data =  i * (Inc / stepval);

		dtostrf(data, 0, Dec,text);
		tLen = d->strPixelLen(text) * 1.2;
		tHi =sf.cap_height;
		d->setCursor(i + gx - (tLen / 2) , gy + gh + 10);

		d->print(text);
		}
	}

	if(st){
		d->setTextColor(tc, bc);
		d->setFont(tf);
		tHi = sf.cap_height * 2 + 8;
		d->setCursor(gx , gy -tHi );
		d->print(ti);
	}

  }
	// compute level of bar graph that is scaled to the  height and the hi and low vals

	if (val >= High) {
		val = High;
	}
	if (val <= Low) {
		val = Low;
	}

	level = MapFloat( val, Low, High, gx, gx+gw);
	
  // draw the bar graph
  // write a upper and lower bar to minimize flicker cause by blanking out bar and redraw on update
	d->fillRect(level, gy + 1, gx+gw - level, gh - 2,ac);
	d->fillRect(gx, gy + 1 , level - gx,  gh - 2, rc);

	d->drawRect(gx , gy, gw, gh, tc);

}

void BarChartH::setBarColor(uint16_t BarColor){
	
	rc = BarColor;
		
}


void BarChartH::setScale(float ScaleLow, float ScaleHigh, float ScaleInc){

	Low = ScaleLow;
	High = ScaleHigh;
	Inc = ScaleInc;
	redraw = true;

}

void BarChartH::showTitle(bool val){
	
	st = val;
		
}

void BarChartH::showScale(bool val){
	
	ss = val;
		
}




/*//////////////////////////////////////////////////////////////////////////////////////////////////////////


vertical bar chart


*///////////////////////////////////////////////////////////////////////////////////////////////////////////

BarChartV::BarChartV(ILI9341_t3 *disp, float GraphXLoc, float GraphYLoc, float GraphWidth, float GraphHeight, float ScaleLow, float ScaleHigh, float ScaleInc){

	d = disp;
	Low = ScaleLow;
	High = ScaleHigh;
	Inc = ScaleInc;
	gx = GraphXLoc;
	gy = GraphYLoc;
	gw = GraphWidth;
	gh = GraphHeight;
		
}


void BarChartV::init(const char *Title, uint16_t TextColor, uint16_t BarColor, uint16_t BarBColor,uint16_t BackColor, const ILI9341_t3_font_t &TitleFont , const ILI9341_t3_font_t &ScaleFont ){

	
	strncpy(ti, Title, 20);
		
	tf = TitleFont;
	sf = ScaleFont;
	tc = TextColor;
	rc = BarColor;
	ac = BarBColor;
	bc = BackColor;
	redraw = true;

	
}

void BarChartV::plot(float val){

  if (redraw == true) {
	redraw = false;


	// step val basically scales the hival and low val to the height
	// deducting a small value to eliminate round off errors
	// this val may need to be adjusted

	if (ss) {
		d->setFont(sf);
		stepval = MapFloat( Inc,Low, High,gh - gh, gh);
	
		// paint over previous y scale
		d->fillRect(gx + gw, gy - gh - 15, 70, gh + 30, bc);
		d->setTextColor(tc, bc);
		for (i = 0; i <= gh; i += stepval) {
			TempY =  gy - gh + i;
			d->drawFastHLine(gx + gw , TempY,  5, tc);
			// draw lables
     
			if (High < .1) {
				Dec = 3;
			}
			else  if (High <= 1) {
				Dec = 2;
			}
			else  if (High <= 10) {
				Dec = 1;
			}
			else   {
				Dec = 0;
			}
			data = High - ( i * (Inc / stepval));
			dtostrf(data, 0, Dec,text);
			tLen = d->strPixelLen(text) * 1.2;
			tHi =sf.cap_height;
			d->setCursor(gx + gw + 12, TempY - (tHi / 2) );
			d->print(text);
		}
	}
	if (st){
		d->setTextColor(tc, bc);
		d->setFont(tf);
		tHi =sf.cap_height * 2 + 5;
		d->setCursor(gx , gy - gh -tHi );
		d->print(ti);
	}
  }
  // compute level of bar graph that is scaled to the  height and the hi and low vals
  // this is needed to accompdate for +/- range
  	if (val >= High) {
		val = High;
	}
	if (val <= Low) {
		val = Low;
	}
  level = (gh * (((val - Low) / (High - Low))));

   
  // draw the bar graph
  // write a upper and lower bar to minimize flicker cause by blanking out bar and redraw on update

  d->fillRect(gx+1, gy - gh, gw - 2, gh - level, ac);
  d->fillRect(gx+1, gy - level , gw - 2,  level, rc);
  d->drawRect(gx , gy - gh - 1 , gw, gh+2, tc);

}

void BarChartV::setBarColor(uint16_t BarColor){
	
	rc = BarColor;
		
}

void BarChartV::showTitle(bool val){
	
	st = val;
		
}

void BarChartV::showScale(bool val){
	
	ss = val;
		
}


void BarChartV::setScale(float ScaleLow, float ScaleHigh, float ScaleInc){


		Low = ScaleLow;
		High = ScaleHigh;
		Inc = ScaleInc;
		redraw = true;
}

/*//////////////////////////////////////////////////////////////////////////////////////////////////////////


cartesian style graphing functions


*///////////////////////////////////////////////////////////////////////////////////////////////////////////

CGraph::CGraph(ILI9341_t3 *disp, float GraphXLoc, float GraphYLoc, float GraphWidth, float GraphHeight, float XAxisLow, float XAxisHigh, float XAxisInc, float YAxisLow, float YAxisHigh, float YAxisInc){

		d = disp;

		XLow = XAxisLow;
		XHigh = XAxisHigh;
		XInc = XAxisInc;

		YLow = YAxisLow;
		YHigh = YAxisHigh;
		YInc = YAxisInc;

		gx = GraphXLoc;
		gy = GraphYLoc;
		gw = GraphWidth;
		gh = GraphHeight;
		
}


void CGraph::init(const char *Title, const char *XAxis, const char *YAxis, uint16_t TextColor, uint16_t GridColor, uint16_t AxisColor, uint16_t BackColor, uint16_t PlotColor, const ILI9341_t3_font_t &TitleFont , const ILI9341_t3_font_t &AxisFont ){

	strncpy(t, Title, 20);
	strncpy(xa, XAxis, 20);
	strncpy(ya, YAxis, 20);
	
	tf = TitleFont;
	af = AxisFont;
	tc = TextColor;

	gc = GridColor;
	ac = AxisColor;
	bc = BackColor;
	pc = PlotColor;

	st = true;
	sl = true;
	sxs = true;
	sys = true;

	Delta = XHigh - XLow;

	RedrawGraph = true;

	TextHeight = tf.cap_height;

}

int CGraph::add(const char *name, uint16_t color){

	// max number of plots is 10
	if (ID >= 10){
		return -1;
	}

	strncpy(dl[ID], name, 20);
	dc[ID] = color;
	ID++;	
	return ID-1;

}

void CGraph::setX(float xpoint){

	x = xpoint;

}



void CGraph::plot(int cID, float y){
	
	if (RedrawGraph){
		RedrawGraph = false;
		drawGraph();
	}
	
	// plot the data
	XPoint = MapFloat(x, XLow, XHigh, gx, gx + gw);
	YPoint = MapFloat(y, YHigh, YLow, gy - gh, gy);
	  
	if ((YPoint > gy)) {
		YPoint = gy;
	}
	if (YPoint < (gy - gh)) {
		YPoint = gy - gh;
	}

	if ((XPoint > gx) && (XPoint < gx + gw)) {
		if (HaveFirstPoint){
			d->drawLine(oXPoint, oYPoint[cID], XPoint, YPoint, dc[cID]);
		}
	}

	HaveFirstPoint = true;
	oYPoint[cID] = YPoint;

	oXPoint = XPoint;

	// test to see if we need to redraw

	if (XPoint + 2 > gx + gw) {

		Delta = XHigh - (XLow);
		XLow = XHigh;
		XHigh = XHigh + Delta;
		RedrawGraph = true;
	
	}
 
}

void CGraph::drawGraph() {


	HaveFirstPoint = false;

	float xDiv =  ((XHigh-XLow)/XInc);
	float yDiv = ((YHigh-YLow)/YInc);

	float ylen = gh /  yDiv;
	float xlen = gw / xDiv;

	 d->setTextColor(tc, bc);
	// draw title
	if (st){
		d->setFont(tf);
		
		d->setCursor(gx , gy - gh - TextHeight-10);
		d->print(t);
	}

	// draw grid lines
	// first blank out xscale for redrawing
	d->fillRect(gx-10, gy+2, gw+20,25, bc);

	d->setFont(af);

	d->fillRect(gx, gy - gh, gw, gh, pc);
	// draw vertical lines
	for (j = 0; j <= xDiv; j++) {
		d->drawFastVLine(gx + ((1+j) * xlen), gy - gh, gh, gc);
		if (xDiv < .1) {
			XDec = 2;
		}
		else if (xDiv < 1) {
			XDec = 1;
		}
		else {
			XDec = 0;
		}
		if (sxs){
			dtostrf(XLow+(XInc*j), 0, XDec,text);
			d->setCursor(gx + (j * xlen)-5, gy+5);
			d->print(text);
		}
	}

	d->fillRect(gx-30,  gy -gh-TextHeight+4, 27, gh+TextHeight, bc);
	// draw horizontal lines
	for (i = 0; i <= yDiv; i++) {

		d->drawFastHLine(gx, gy - (ylen * i), gw, gc);

		if (YInc < .1) {
			YDec = 2;
		}
		else if (YInc < 1) {
			YDec = 1;
		}
		else {
			YDec = 0;
		}
		if (sys){
			
			dtostrf(YLow+(YInc*i), 0, YDec,text);
			d->setCursor(gx-30, gy - (ylen * i)-8);
			d->print(text);
		}
	}
	// put the y axis at the zero point
	if ((YLow < 0) &&  (YHigh > 0)) {
		YPoint = MapFloat(0, YHigh, YLow, gy - gh, gy);
	}
	else {
		YPoint = MapFloat(YLow, YHigh, YLow, gy - gh, gy);
	}

	
	d->drawFastHLine(gx,  YPoint, gw, ac);
	d->drawFastHLine(gx,  YPoint-1, gw, ac);

	d->drawFastVLine(gx-1, gy - gh, gh+1, ac);
	d->drawFastVLine(gx-2, gy - gh, gh+1, ac);


	// draw x lable
	if (sxs){
		
		d->setTextColor(tc, bc);
		d->setCursor(gx,gy+ TextHeight+5);	
		d->print(xa);
	}

	// draw legend
	if (sl){
		StartPointX = d->getCursorX()+ 10;
		StartPointY = d->getCursorY() ;

		for (k = 0; k < ID; k++){
			d->setCursor(StartPointX, StartPointY);
			d->print(dl[k]);
			StartPointX = d->getCursorX();
			d->drawFastHLine(StartPointX+3, StartPointY+TextHeight/4 - 1 , 20, dc[k]);
			d->drawFastHLine(StartPointX+3, StartPointY+TextHeight/4 , 20, dc[k]);
			d->drawFastHLine(StartPointX+3, StartPointY+TextHeight/4 + 1, 20, dc[k]);
			StartPointX += 30;
		}
	}
	// draw y lable
	if (sys){
		oOrientation = d->getRotation();
		d->setTextColor(tc, bc);
		d->setRotation(oOrientation - 1);
		d->setCursor(d->width()-gy,gx-44);	
		d->print(ya);
		d->setRotation(oOrientation);
	}
	
}

void CGraph::setYAxis(float Ylow, float Yhigh, float Yinc){

	YLow = Ylow;
	YHigh = Yhigh;
	YInc = Yinc;
	RedrawGraph = true;
}

void CGraph::showTitle(bool val){
	st = val;
}

void CGraph::showLegend(bool val){
	sl = val;
}
void CGraph::showXScale(bool val){
	sxs = val;
}
void CGraph::showYScale(bool val){
	sys = val;
}



/*//////////////////////////////////////////////////////////////////////////////////////////////////////////


dial type control


*///////////////////////////////////////////////////////////////////////////////////////////////////////////

Dial::Dial(ILI9341_t3 *disp, int CenterX, int CenterY, int DialRadius, float LowVal , float HiVal , float ValInc, float SweepAngle){

	degtorad = .0174532778;

	d = disp;			
	
	cx = CenterX;
	cy = CenterY;
	dr = DialRadius;
	lv = LowVal;
	hv = HiVal;
	inc = ValInc;
	sa = SweepAngle;
	
	// store initial values
	// this looks silly but we don't have needle size calculated yet but to keep needle from first draw being weird, just make a dot at the center
	px = CenterX;
	py = CenterY;
	pix = CenterX;
	piy = CenterY;
	plx = CenterX;
	ply = CenterY;
	prx = CenterX;
	pry = CenterY;

	Redraw = true;

}

void Dial::init(uint16_t NeedleColor, uint16_t DialColor, uint16_t TextColor, uint16_t TickColor, const char *Title, const ILI9341_t3_font_t &TitleFont , const ILI9341_t3_font_t &DataFont ) {

	tf = TitleFont;
	df = DataFont;
	strncpy(t, Title, 10);
	nc = NeedleColor;
	dc = DialColor;
	tc = TextColor;
	ic = TickColor;
	
}

void Dial::draw(float Val) {


	// draw the dial only one time--this will minimize flicker
	if ( Redraw == true) {
		Redraw = false;
		d->fillCircle(cx, cy, dr , dc);
		d->drawCircle(cx, cy, dr, ic);
		d->drawCircle(cx, cy, dr - 1, ic);
	}

	// draw the current value
	d->setFont(df);
	d->setTextColor(tc, dc);
	d->setCursor(cx - 25, cy + 20 );
	//disp.println(Format(curval, dig, dec));
	// center the scale about the vertical axis--and use this to offset the needle, and scale text
	offset = (270 +  (sa / 2)) * degtorad;
	// find hte scale step value based on the hival low val and the scale sweep angle
	// deducting a small value to eliminate round off errors
	// this val may need to be adjusted
	stepval = (inc) * (sa / (float (hv - lv)));
	// draw the scale and numbers
	// note draw this each time to repaint where the needle was
		
	// first draw the previous needle in dial color to hide it
	d->fillTriangle (pix, piy,plx, ply, prx, pry, dc);

	for (i = 0.0; i <= (sa + 0.0001); i += stepval) {

		angle = ( i  * degtorad);
		angle = offset - angle ;
		ox =  (float) (dr - 2) * cos(angle) + cx;
		oy =  (float) (dr - 2) * sin(angle) + cy;
		ix =  (float) (dr - 10) * cos(angle) + cx;
		iy =  (float) (dr - 10) * sin(angle) + cy;
		tx =  (float) (dr - 30) * cos(angle) + cx;
		ty =  (float) (dr - 30) * sin(angle) + cy;
		dx =  (float) (dr - 20) * cos(angle) + cx;
		dy =  (float) (dr - 20) * sin(angle) + cy;

		d->drawLine(ox, oy, ix, iy, ic);

	
		if (hv < .1) {
			dec = 3;
		}
		else  if (hv <= 1) {
			dec = 2;
		}
		else  if (hv <= 10) {
			dec = 1;
		}
		else   {
			dec = 0;
		}

		data = hv - ( i * (inc / stepval)) ;
		dtostrf(data, 0, dec,buf);
		tLen = d->strPixelLen(buf);
		tHi = df.cap_height;
		d->setCursor(dx - (tLen/2), dy - (tHi/2));
	
		d->print(buf);
	

	}

	// compute and draw the needle
	angle = (sa * (1.0 - (((Val - lv) / (hv - lv)))));
	angle = angle * degtorad;
	angle = offset - angle  ;

	// draw a triangle for the needle (compute and store 3 vertiticies)
	// 5.0 is width of needle at center
	ix =  (float)(dr - 10.0) * cos(angle) + cx;
	iy =  (float)(dr - 10.0) * sin(angle) + cy;
	lx =  6.0 * cos(angle - 90.0 * degtorad) + cx;
	ly =  6.0 * sin(angle - 90.0 * degtorad) + cy;
	rx =  6.0 * cos(angle + 90.0 * degtorad) + cx;
	ry =  6.0 * sin(angle + 90.0 * degtorad) + cy;
		
	// then draw the new needle in need color to display it
	d->fillTriangle (ix, iy, lx, ly, rx, ry, nc);

	// draw a cute little dial center
	d->fillCircle(cx, cy, 8, tc);

	// print the title
	d->setTextColor(tc, dc);
	d->setFont(tf);
	tLen = d->strPixelLen(t);
			
	d->setCursor(cx - tLen/2, cy + 10);
	d->println(t);
	
	//save all current to old so the previous dial can be hidden
	pix = ix;
	piy = iy;
	plx = lx;
	ply = ly;
	prx = rx;
	pry = ry;

}





/*

class for a vertical type slider, you pass in the coordinates for placement and colors, we'll pass in scale during initilization as scale may depend on some computed value

*/

SliderV::SliderV(ILI9341_t3 *disp, uint16_t left, uint16_t top, uint16_t high, uint16_t sliderColor, uint16_t backColor, uint16_t handleColor)

{
	// map arguements to class variables
	d = disp;
	l = left;
	t = top;
	h = high;
	sColor = sliderColor;
	bColor = backColor;
	hColor = handleColor;
	
}



/*

initializer for the vertical slider, here we pass in the scale values

note user can specify a scale (0 for none), and a snap capability (0 for none). You can draw a 0-100 scale with tick marks every 25 but snap to each integer (1,2,3...)

*/

void SliderV::init(float scaleLow, float scaleHi, float scale, float snap ) {
    
	// map arguements to class variables
	sl = scaleLow;
	sh = scaleHi;
	sc = 0.0;
	sn = 0.0;
	ch = true;
	oy = -1;
	colorscale = false;
	handlesize = 10;
	handleshape = HANDLE_CIRCLE;

	// compute scale increments and snap increments
	if (scale != 0) {
		sc = (sh - sl) / scale;
		ce = abs(sh / scale);
	}
	if (snap != 0) {
		sn = (sh - sl) / snap;
	}

}


void SliderV::setColors(uint16_t sliderColor, uint16_t backColor, uint16_t handleColor) {

	// map arguements to class variables
	sColor = sliderColor;
	bColor = backColor;
	hColor = handleColor;
	
	// update colors by redrawing
	draw(pos);
}

/*

method to set hust the handle color, this is useful for drawing handle in warning color

*/

/*

method to draw the slider

blank out old handle scale (needed if user calls draw to refresh the slider)
draw the scale
draw optional tick marks
get the actual scaled value and map to the appropriate screen position
draw the handle in the scaled location
reset the position for redraw method
remember the postion for painting over the original


*/
void  SliderV::draw(float val) {
	
	if (!ch){
		// no need to draw anything
		return;
	}


	if (oy >= 0){
		if (handleshape == HANDLE_CIRCLE) {
			d->fillCircle(l, oy, handlesize/2, bColor);
		}
		else if (handleshape == HANDLE_SQUARE) {
			d->fillRect(l - (handlesize / 2), oy- (handlesize / 2), handlesize, handlesize, bColor);
		}
		else if (handleshape == HANDLE_TRIANGLE_1) {
			d->fillTriangle(l - handlesize, oy- (handlesize / 2), l - handlesize, oy+ (handlesize / 2),l , oy  , bColor);
		}
		else if (handleshape == HANDLE_TRIANGLE_2) {
			d->fillTriangle(l + handlesize, oy- (handlesize / 2), l + handlesize, oy+ (handlesize / 2),l , oy  , bColor);
		}
		else if (handleshape == HANDLE_TRIANGLE_3) {
			d->fillTriangle(l - handlesize, oy- (handlesize / 2), l - handlesize, oy+ (handlesize / 2),l , oy  , bColor);
			d->fillTriangle(l + handlesize, oy- (handlesize / 2), l + handlesize, oy+ (handlesize / 2),l , oy  , bColor);
		}
	}

	pos = MapFloat(val, sl, sh, (float) t + (float) h, (float) t );
	oy = pos;

	if (colorscale){
		d->fillRect(l - 1 , t, 3, oy - t, sColor); // draw new slider
		d->fillRect(l - 1 , oy, 3, h - oy + t, hColor); // draw new slider
	}
	else {
		d->fillRect(l - 1, t, 3, h, sColor);
	}
	
	if (sc != 0.0) {
		for (i = 0; i <= sc; i++){

			d->fillRect(l - 3,(i * (h / sc) ) + t, 7, 1, sColor);

			if ((i == ce) | (i == 0) | (i == sc)) {
				d->fillRect(l - 3, (i * (h / sc)) + t, 7, 4, sColor);
			}
		}
	}

	if (handleshape == HANDLE_CIRCLE) {
		d->fillCircle(l,pos,handlesize/2,hColor);
		d->drawCircle(l,pos,handlesize/2,sColor);
		d->drawCircle(l,pos,(handlesize/2)-1,sColor);
	}
	else if (handleshape == HANDLE_SQUARE) {
		d->fillRect(l - (handlesize / 2), pos- (handlesize / 2), handlesize, handlesize, hColor);
		d->drawRect(l - (handlesize / 2),pos- (handlesize / 2),handlesize, handlesize,sColor);
		d->drawRect(l - (handlesize / 2) +1,pos- (handlesize / 2)+1,handlesize-2,handlesize-2,sColor);
	}

	else if (handleshape == HANDLE_TRIANGLE_1) {
		d->fillTriangle(l - handlesize,	  pos- (handlesize / 2), l - handlesize,   pos+ (handlesize / 2),l ,  pos , hColor);
		d->drawTriangle(l - handlesize,   pos- (handlesize / 2), l - handlesize,   pos+ (handlesize / 2),l ,  pos , sColor);
	}
	else if (handleshape == HANDLE_TRIANGLE_2) {
		d->fillTriangle(l + handlesize,	  pos- (handlesize / 2), l + handlesize,   pos+ (handlesize / 2),l ,  pos , hColor);
		d->drawTriangle(l + handlesize,   pos- (handlesize / 2), l + handlesize,   pos+ (handlesize / 2),l ,  pos , sColor);
	}
	else if (handleshape == HANDLE_TRIANGLE_3) {
		d->fillTriangle(l - handlesize,	  pos- (handlesize / 2), l - handlesize,   pos+ (handlesize / 2),l ,  pos , hColor);
		d->fillTriangle(l + handlesize,	  pos- (handlesize / 2), l + handlesize,   pos+ (handlesize / 2),l ,  pos , hColor);
		d->drawTriangle(l - handlesize,   pos- (handlesize / 2), l - handlesize,   pos+ (handlesize / 2),l ,  pos , sColor);
		d->drawTriangle(l + handlesize,   pos- (handlesize / 2), l + handlesize,   pos+ (handlesize / 2),l ,  pos , sColor);
	}


	oy = pos;
	pos = val;
	
		   
}

/*

method to more the slider
optionally get the snap increment
see if screen press is in controls range
draw the scale
draw optional tick marks
get the actual scaled value and map to the appropriate screen position
draw the handle in the scaled location
reset the position for redraw method
remember the postion for painting over the original


*/

float  SliderV::slide(uint16_t x, uint16_t y){
	

	ch = false;
	if (sn != 0.0) {
		y = y - t;
		y = (int) (y /  (h / sn));
		y = (y *  (h / sn)) + t;
	}

	if (y != oy){

		if (abs(x - (l - handlesize)) < handlesize) {

			if ((y >= t) & (y <= (t + h))) {
				ch = true;
				// it's in rage of ball
				if (handleshape == HANDLE_CIRCLE) {
					d->fillCircle(l, oy, handlesize/2, bColor);
				}
				else if (handleshape == HANDLE_SQUARE) {
					d->fillRect(l - (handlesize / 2), oy- (handlesize / 2), handlesize, handlesize, bColor);
				}
				else if (handleshape == HANDLE_TRIANGLE_1) {
					// weird i know but need to draw the black out slightly larger due to round off errors
					d->fillTriangle(l - handlesize, oy- (handlesize / 2)-1, l - handlesize, oy+ (handlesize / 2)+1,l+1 , oy  , bColor);
				}
				else if (handleshape == HANDLE_TRIANGLE_2) {
					// weird i know but need to draw the black out slightly larger due to round off errors
					d->fillTriangle(l + handlesize, oy- (handlesize / 2)-1, l + handlesize, oy+ (handlesize / 2)+1,l-1 , oy  , bColor);
				}
				else if (handleshape == HANDLE_TRIANGLE_3) {
					// weird i know but need to draw the black out slightly larger due to round off errors
					d->fillTriangle(l - handlesize, oy- (handlesize / 2)-1, l - handlesize, oy+ (handlesize / 2)+1,l+1 , oy  , bColor);
					d->fillTriangle(l + handlesize, oy- (handlesize / 2)-1, l + handlesize, oy+ (handlesize / 2)+1,l-1 , oy  , bColor);
				}

				if (colorscale){
					d->fillRect(l - 1 , t, 3, oy - t, sColor); // draw new slider
					d->fillRect(l - 1 , oy, 3, h - oy + t, hColor); // draw new slider
				}
				else {
					d->fillRect(l - 1, t, 3, h, sColor);
				}
	
				if (sc != 0.0) {
					for (i = 0; i <= sc; i++){

						d->fillRect(l - 3,(i * (h / sc) ) + t, 7, 1, sColor);

						if ((i == ce) | (i == 0) | (i == sc)) {
							d->fillRect(l - 3, (i * (h / sc)) + t, 7, 4, sColor);
						}
					}
				}

				if (handleshape == HANDLE_CIRCLE) {
					d->fillCircle(l,y,handlesize/2,hColor);
					d->drawCircle(l,y,handlesize/2,sColor);
					d->drawCircle(l,y,(handlesize/2)-1,sColor);
				}
				else if (handleshape == HANDLE_SQUARE) {
					d->fillRect(l - (handlesize / 2), y- (handlesize / 2), handlesize, handlesize, hColor);
					d->drawRect(l - (handlesize / 2),y- (handlesize / 2),handlesize, handlesize,sColor);
					d->drawRect(l - (handlesize / 2) +1,y- (handlesize / 2)+1,handlesize-2,handlesize-2,sColor);
				}
				else if (handleshape == HANDLE_TRIANGLE_1) {
					d->fillTriangle(l - handlesize,	  y- (handlesize / 2), l - handlesize, y+ (handlesize / 2) ,l ,  y , hColor);
					d->drawTriangle(l - handlesize,   y- (handlesize / 2), l - handlesize, y+ (handlesize / 2) ,l ,  y , sColor);
				}
				else if (handleshape == HANDLE_TRIANGLE_2) {
					d->fillTriangle(l + handlesize,	  y- (handlesize / 2), l + handlesize, y+ (handlesize / 2) ,l ,  y , hColor);
					d->drawTriangle(l + handlesize,   y- (handlesize / 2), l + handlesize, y+ (handlesize / 2) ,l ,  y , sColor);
				}
				else if (handleshape == HANDLE_TRIANGLE_3) {
					d->fillTriangle(l - handlesize,	  y- (handlesize / 2), l - handlesize, y+ (handlesize / 2) ,l ,  y , hColor);
					d->fillTriangle(l + handlesize,	  y- (handlesize / 2), l + handlesize, y+ (handlesize / 2) ,l ,  y , hColor);
					d->drawTriangle(l - handlesize,   y- (handlesize / 2), l - handlesize, y+ (handlesize / 2) ,l ,  y , sColor);
					d->drawTriangle(l + handlesize,   y- (handlesize / 2), l + handlesize, y+ (handlesize / 2) ,l ,  y , sColor);
				}
				oy = y;

				// get scaled val and pass back
				pos = MapFloat(y, (float) t, (float) t + (float) h, sh, sl);
				
			}
		}
	}
	
	return pos;
}


/*

method to set hust the handle color, this is useful for drawing handle in warning color

*/
void  SliderV::setHandleColor(uint16_t handleColor) {

	hColor = handleColor;
	
	draw(pos);
}

/*

method to see of the user just turned the switch on or off

*/
bool SliderV::changed(){

	return ch;

}

void SliderV::change(){

	ch = true;

}

void SliderV::drawSliderColor(bool val){

	colorscale = val;

}


void SliderV::setHandleSize(int val){

	if (val < 4) {
		handlesize = 4;
	}
	else if (val > 40) {
		handlesize = 40;
	}
	else {
		handlesize = val;
	}

}

void SliderV::setHandleShape(byte val){

	if (val < 0) {
		handleshape = HANDLE_CIRCLE;
	}
	else if (val > 5) {
		handleshape = HANDLE_CIRCLE;
	}
	else {
		handleshape = val;
	}

}


/*

class for a horizontal type slider, you pass in the coordinates for placement and colors, we'll pass in scale during initilization as scale may depend on some computed value

*/
SliderH::SliderH(ILI9341_t3 *disp, uint16_t left, uint16_t top, uint16_t wide, uint16_t sliderColor, uint16_t backColor, uint16_t handleColor)

{
	// map arguements to class variables
	d = disp;				
	l = left;
	t = top;
	w = wide;
	sColor = sliderColor;
	bColor = backColor;
	hColor = handleColor;

}

/*

initializer for the horizontal slider, here we pass in the scale values

note user can specify a scale (0 for none), and a snap capability (0 for none). You can draw a 0-100 scale with tick marks every 25 but snap to each integer (1,2,3...)


*/

void SliderH::init(float scaleLow, float scaleHi, float scale, float snap ) {
	// map arguements to class variables
	sl = scaleLow;
	sh = scaleHi;
	sc = 0.0;
	sn = 0.0;
	ch = true;
	ox = -1;
	colorscale = false;
	handlesize = 10;
	handleshape = HANDLE_CIRCLE;

	if (scale != 0) {
		sc =  (sh - sl ) /  scale ;
		ce = abs(sl / scale);
	}
	if (snap != 0) {
		sn = (sh - sl) / snap;
	}
	
}

/*

method to set colors, this is useful for disabling a slider by setting it to a dimmed color and ignoring screen presses

*/

void SliderH::setColors(uint16_t sliderColor, uint16_t backColor, uint16_t handleColor) {

	// map arguements to class variables
	sColor = sliderColor;
	bColor = backColor;
	hColor = handleColor;

}

/*

method to set colors, this is useful for disabling a slider by setting it to a dimmed color and ignoring screen presses

*/


void  SliderH::setHandleColor(uint16_t handleColor) {

	hColor = handleColor;

}



/*

method to draw the horizontal slider

blank out old handle scale (needed if user calls draw to refresh the slider)
draw the scale
draw optional tick marks
get the actual scaled value and map to the appropriate screen position
draw the handle in the scaled location
reset the position for redraw method
remember the postion for painting over the original

*/
void  SliderH::draw(float val) {
	

	if (!ch){
		// no need to draw anything
		return;
	}

	if (ox >= 0){
		if (handleshape == HANDLE_CIRCLE) {
			d->fillCircle(ox, t, handlesize/2, bColor);
		}
		else if (handleshape == HANDLE_SQUARE) {
			d->fillRect(ox- (handlesize / 2), t - (handlesize / 2), handlesize, handlesize, bColor);
		}
		else if (handleshape == HANDLE_TRIANGLE_1) {
			d->fillTriangle(ox - (handlesize / 2), t - handlesize, ox + (handlesize / 2), t - handlesize, ox,t, bColor);
		}
		else if (handleshape == HANDLE_TRIANGLE_2) {
			d->fillTriangle(ox - (handlesize / 2), t + handlesize, ox + (handlesize / 2), t + handlesize, ox,t, bColor);
		}
		else if (handleshape == HANDLE_TRIANGLE_3) {
			d->fillTriangle(ox - (handlesize / 2), t - handlesize, ox + (handlesize / 2), t - handlesize, ox,t, bColor);
			d->fillTriangle(ox - (handlesize / 2), t + handlesize, ox + (handlesize / 2), t + handlesize, ox,t, bColor);
		}
	}

		pos = MapFloat(val, sl, sh, (float) l, (float) l + (float) w);
		ox = pos;
		
		if (colorscale){
			d->fillRect(l, t-1, ox-l, 3, hColor);
			d->fillRect(ox, t-1, w - ox + l, 3, sColor);

		}
		else{
			d->fillRect(l, t-1, w+1, 3, sColor);
		}

		if (sc != 0.0) {
			for (i = 0; i <= sc; i++){
				
				d->fillRect((i * (w / sc) ) + l, t-3, 1, 7, sColor);

				if ((i == ce) | (i == 0) | (i == sc)) {
					d->fillRect((i * (w / sc)) + l-1, t - 3, 3, 7, sColor);
				}
			}
		}
		ch = false;
		
		if (handleshape == HANDLE_CIRCLE) {
			d->fillCircle(ox, t, handlesize/2, hColor);
			d->drawCircle(ox, t, handlesize/2,sColor);
			d->drawCircle(ox, t, (handlesize/2) - 1,sColor);
		}
		else if (handleshape == HANDLE_SQUARE) {
			d->fillRect(ox- (handlesize / 2), t - (handlesize / 2), handlesize, handlesize, hColor);
			d->drawRect(ox- (handlesize / 2), t - (handlesize / 2), handlesize, handlesize, sColor);
			d->drawRect(ox- (handlesize / 2)+1, t - (handlesize / 2)+1, handlesize-2, handlesize-2, sColor);
		}
		else if (handleshape == HANDLE_TRIANGLE_1) {
			d->fillTriangle(ox - (handlesize / 2), t - handlesize, ox + (handlesize / 2), t - handlesize, ox,t, hColor);
			d->drawTriangle(ox - (handlesize / 2), t - handlesize, ox + (handlesize / 2), t - handlesize, ox,t, sColor);
		}
		else if (handleshape == HANDLE_TRIANGLE_2) {
			d->fillTriangle(ox - (handlesize / 2), t + handlesize, ox + (handlesize / 2), t + handlesize, ox,t, hColor);
			d->drawTriangle(ox - (handlesize / 2), t + handlesize, ox + (handlesize / 2), t + handlesize, ox,t, sColor);
		}
		else if (handleshape == HANDLE_TRIANGLE_3) {
			d->fillTriangle(ox - (handlesize / 2), t - handlesize, ox + (handlesize / 2), t - handlesize, ox,t, hColor);
			d->fillTriangle(ox - (handlesize / 2), t + handlesize, ox + (handlesize / 2), t + handlesize, ox,t, hColor);

			d->drawTriangle(ox - (handlesize / 2), t - handlesize, ox + (handlesize / 2), t - handlesize, ox,t, sColor);
			d->drawTriangle(ox - (handlesize / 2), t + handlesize, ox + (handlesize / 2), t + handlesize, ox,t, sColor);

		}






		ox = pos;
		pos = val;
	
}




/*

method to more the slider

optionally get the snap increment
see if the press location is in the controls range
draw the scale
draw optional tick marks
get the actual scaled value and map to the appropriate screen position
draw the handle in the scaled location
reset the position for redraw method
remember the postion for painting over the original


*/

float  SliderH::slide(float x,float y){
	
	ch = false;
	if (sn != 0.0 ) {
 		x = x + handlesize;  
		x = x - l;
		x =  (int) (x /  (w / sn));
		x = (x *  (w / sn)) + l;
	}
	// draw ball and scale
	if (x != ox){
		
		if ((x >= l) & (x <= (l + w))) {

			if ((abs(y - t)) <= handlesize) {

				ch = true;
				if (handleshape == HANDLE_CIRCLE) {
					d->fillCircle(ox, t, handlesize/2, bColor);
				}
				else if (handleshape == HANDLE_SQUARE) {
					d->fillRect(ox- (handlesize / 2), t - (handlesize / 2), handlesize, handlesize, bColor);
				}
				else if (handleshape == HANDLE_TRIANGLE_1) {
					d->fillTriangle(ox - (handlesize / 2), t - handlesize, ox + (handlesize / 2), t - handlesize, ox,t, bColor);
				}
				else if (handleshape == HANDLE_TRIANGLE_2) {
					d->fillTriangle(ox - (handlesize / 2), t + handlesize, ox + (handlesize / 2), t + handlesize, ox,t, bColor);
				}
				else if (handleshape == HANDLE_TRIANGLE_3) {
					d->fillTriangle(ox - (handlesize / 2), t - handlesize, ox + (handlesize / 2), t - handlesize, ox,t, bColor);
					d->fillTriangle(ox - (handlesize / 2), t + handlesize, ox + (handlesize / 2), t + handlesize, ox,t, bColor);
				}

				if (colorscale){
					d->fillRect(l, t-1, ox-l, 3, hColor);
					d->fillRect(ox, t-1, w - ox + l, 3, sColor);
				}
				else{
					d->fillRect(l, t-1, w+1, 3, sColor);
				}
			

				if (sc != 0.0) {
					for (i = 0; i <= sc; i++){
				
						d->fillRect((i * (w / sc) ) + l, t-3, 1, 7, sColor);

						if ((i == ce) | (i == 0) | (i == sc)) {
							d->fillRect((i * (w / sc)) + l-1, t - 3, 3, 7, sColor);
						}
					}
				}

			//	d->fillCircle(x, t, handlesize, hColor);
			//	d->drawCircle(x, t, handlesize, sColor);
			//	d->drawCircle(x, t, handlesize - 1, sColor);


				if (handleshape == HANDLE_CIRCLE) {
					d->fillCircle(x, t, handlesize/2, hColor);
					d->drawCircle(x, t, handlesize/2,sColor);
					d->drawCircle(x, t, (handlesize/2) - 1,sColor);
				}
				else if (handleshape == HANDLE_SQUARE) {
					d->fillRect(x- (handlesize / 2), t - (handlesize / 2), handlesize, handlesize, hColor);
					d->drawRect(x- (handlesize / 2), t - (handlesize / 2), handlesize, handlesize, sColor);
					d->drawRect(x- (handlesize / 2)+1, t - (handlesize / 2)+1, handlesize-2, handlesize-2, sColor);
				}
				else if (handleshape == HANDLE_TRIANGLE_1) {
					d->fillTriangle(x - (handlesize / 2), t - handlesize, x + (handlesize / 2), t - handlesize, x,t, hColor);
					d->drawTriangle(x - (handlesize / 2), t - handlesize, x + (handlesize / 2), t - handlesize, x,t, sColor);
				}
				else if (handleshape == HANDLE_TRIANGLE_2) {
					d->fillTriangle(x - (handlesize / 2), t + handlesize, x + (handlesize / 2), t + handlesize, x,t, hColor);
					d->drawTriangle(x - (handlesize / 2), t + handlesize, x + (handlesize / 2), t + handlesize, x,t, sColor);
				}
				else if (handleshape == HANDLE_TRIANGLE_3) {
					d->fillTriangle(x - (handlesize / 2), t - handlesize, x + (handlesize / 2), t - handlesize, x,t, hColor);
					d->fillTriangle(x - (handlesize / 2), t + handlesize, x + (handlesize / 2), t + handlesize, x,t, hColor);

					d->drawTriangle(x - (handlesize / 2), t - handlesize, x + (handlesize / 2), t - handlesize, x,t, sColor);
					d->drawTriangle(x - (handlesize / 2), t + handlesize, x + (handlesize / 2), t + handlesize, x,t, sColor);

				}
				
				ox = x;

				// get scaled val and pass back
				pos = MapFloat(x, (float) l, (float) l + (float) w, sl, sh);
				
			}
		}

	}
	
	return pos;
}

bool SliderH::changed(){

	return ch;

}

void SliderH::change(){

	ch = true;

}
void SliderH::drawSliderColor(bool val){

	colorscale = val;

}

void SliderH::setHandleSize(int val){

	if (val < 4) {
		handlesize = 4;
	}
	else if (val > 40) {
		handlesize = 40;
	}
	else {
		handlesize = val;
	}

}

void SliderH::setHandleShape(byte val){

	if (val < 0) {
		handleshape = HANDLE_CIRCLE;
	}
	else if (val > 5) {
		handleshape = HANDLE_CIRCLE;
	}
	else {
		handleshape = val;
	}

}

/*

class for a simpel slider-type on off switch, you pass in the coordinates for placement and colors, we'll pass in scale during initilization as scale may depend on some computed value

*/
SliderOnOff::SliderOnOff(ILI9341_t3 *disp, uint16_t left, uint16_t top, uint16_t width, uint16_t height, uint16_t sliderColor, uint16_t backColor, uint16_t OnColor, uint16_t OffColor)

{
	// map arguements to class variables
	_d = disp;
	_l = left;
	_t = top;
	_w = width;
	_h = height;
	_sColor = sliderColor;
	_bColor = backColor;
	_onColor = OnColor;
	_offColor = OffColor;

}

/*

method to more the on/off slider

see if the screen press is in the controls range
optionally get the snap increment
draw the scale
draw optional tick marks
get the actual scaled value and map to the appropriate screen position
draw the handle in the scaled location
reset the position for redraw method
remember the postion for painting over the original


*/

bool SliderOnOff::slide(float x, float y){

	_changed = false;

	if ((x >= _l) && (x <= (_l + _w))) {


		if (abs(y - (_t + (_h / 2))) < _h) {

			_changed = true;

			// it's in range of slider ball
			if (x < (_l + (_w / 2))){
				// press in the off range
				if (_pos){

					// clear on button
					_d->fillCircle(_l + _w - (_h / 2), _t + (_h / 2), (_h / 2) - 2, _bColor);
					// draw off button
					_d->fillCircle(_l + (_h / 2), _t + (_h / 2), (_h / 2) - 2, _offColor);
					_pos = false;
				}
			}
			else {
				// it's in the on range
				if (!_pos){
					// clear on button
					_d->fillCircle(_l + (_h / 2), _t + (_h / 2), (_h / 2) - 2, _bColor);
					// draw off button
					_d->fillCircle(_l + _w - (_h / 2), _t + (_h / 2), (_h / 2) - 2, _onColor);

					_pos = true;
				}
			}

		}
	}

	return _pos;

}



void  SliderOnOff::draw(bool val) {

	_pos = val;

	_d->fillRoundRect(_l, _t, _w, _h, _h / 2, _bColor);
	_d->drawRoundRect(_l, _t, _w, _h, _h / 2, _sColor);
	_d->drawRoundRect(_l + 1, _t + 1, _w - 2, _h, _h / 2, _sColor);
	

	if (val) {
		// draw on button
		_d->fillCircle(_l + _w - (_h / 2), _t + (_h / 2), (_h / 2) - 2, _onColor);
	}

	else {
		// draw off button
		_d->fillCircle(_l + (_h / 2), _t + (_h / 2), (_h / 2) - 2, _offColor);

	}

}


/*

method to see of the user just turned the switch on or off

*/
bool SliderOnOff::changed(){

	return _changed;

}

bool SliderOnOff::getValue(){

	return _pos;

}






/*

the mapF for mapping float values

*/

float BarChartV::MapFloat(float x, float in_min, float in_max, float out_min, float out_max) {

  if (in_min < 0) {
    in_max = in_max + abs(in_min);
    in_min = 0.0;
  }

  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

float BarChartH::MapFloat(float x, float in_min, float in_max, float out_min, float out_max) {

  if (in_min < 0) {
    in_max = in_max + abs(in_min);
    in_min = 0.0;
  }

  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}


float CGraph::MapFloat(float x, float in_min, float in_max, float out_min, float out_max) {

  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

float SliderH::MapFloat(float val, float fromLow, float fromHigh, float toLow, float toHigh) {

  return (val - fromLow) * (toHigh - toLow) / (fromHigh - fromLow) + toLow;

}

float SliderV::MapFloat(float val, float fromLow, float fromHigh, float toLow, float toHigh) {
  return (val - fromLow) * (toHigh - toLow) / (fromHigh - fromLow) + toLow;

}



