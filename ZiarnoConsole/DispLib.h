//---------------------------------------------------------------------------

#ifndef DispLibH
#define DispLibH

#include <opencv2/core/core.hpp>


using namespace cv;

//---------------------------------------------------------------------------
//const unsigned char outOfImageRed = 128;
//const unsigned char outOfImageGreen = 128;
//const unsigned char outOfImageBlue = 128;

const unsigned int RegColorsRGB[16] =
{
0x000000ff,
0x0000ff00,
0x00ff0000,
0x00ffff00,
0x00ff00ff,
0x0000ffff,
0x000080FF,
0x008000FF,
0x0000FF80,
0x0080FF00,
0x00FF0080,
0x00FF8000,
0x0000C4FF,
0x0000FFC4,
0x00FF00C4,
0x00C4FF00
};
const unsigned char RegColorsB[16]=
{

    0x00,
    0x00,
    0xff,
    0xff,
    0xff,
    0x00,
    0x00,
    0x80,
    0x00,
    0x80,
    0xFF,
    0xFF,
    0x00,
    0x00,
    0xFF,
    0xC4
};
const unsigned char RegColorsG[16]=
{
    0x00,
    0xff,
    0x00,
    0xff,
    0x00,
    0xff,
    0x80,
    0x00,
    0xFF,
    0xFF,
    0x00,
    0x80,
    0xC4,
    0xFF,
    0x00,
    0xFF
};
const unsigned char RegColorsR[16]=
{
    0xff,
    0x00,
    0x00,
    0x00,
    0xff,
    0xff,
    0xFF,
    0xFF,
    0x80,
    0x00,
    0x80,
    0x00,
    0xFF,
    0xC4,
    0xC4,
    0x00
};

const unsigned char colormapR[256] =
  {0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   3,   7,  11,  15,  19,  23,  27,  31,  35,  39,  43,  47,  51,  55,  59,  63,
  67,  71,  75,  79,  83,  87,  91,  95,  99, 103, 107, 111, 115, 119, 123, 127,
 131, 135, 139, 143, 147, 151, 155, 159, 163, 167, 171, 175, 179, 183, 187, 191,
 195, 199, 203, 207, 211, 215, 219, 223, 227, 231, 235, 239, 243, 247, 251, 255,
 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
 251, 247, 243, 239, 235, 231, 227, 223, 219, 215, 211, 207, 203, 199, 195, 191,
 187, 183, 179, 175, 171, 167, 163, 159, 155, 151, 147, 143, 139, 135, 131, 127};

 const unsigned char colormapG[256] =
{  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   3,   7,  11,  15,  19,  23,  27,  31,  35,  39,  43,  47,  51,  55,  59,  63,
  67,  71,  75,  79,  83,  87,  91,  95,  99, 103, 107, 111, 115, 119, 123, 127,
 131, 135, 139, 143, 147, 151, 155, 159, 163, 167, 171, 175, 179, 183, 187, 191,
 195, 199, 203, 207, 211, 215, 219, 223, 227, 231, 235, 239, 243, 247, 251, 255,
 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
 251, 247, 243, 239, 235, 231, 227, 223, 219, 215, 211, 207, 203, 199, 195, 191,
 187, 183, 179, 175, 171, 167, 163, 159, 155, 151, 147, 143, 139, 135, 131, 127,
 123, 119, 115, 111, 107, 103,  99,  95,  91,  87,  83,  79,  75,  71,  67,  63,
  59,  55,  51,  47,  43,  39,  35,  31,  27,  23,  19,  15,  11,   7,   3,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0};


const unsigned char colormapB[256] =
{131, 135, 139, 143, 147, 151, 155, 159, 163, 167, 171, 175, 179, 183, 187, 191,
 195, 199, 203, 207, 211, 215, 219, 223, 227, 231, 235, 239, 243, 247, 251, 255,
 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
 251, 247, 243, 239, 235, 231, 227, 223, 219, 215, 211, 207, 203, 199, 195, 191,
 187, 183, 179, 175, 171, 167, 163, 159, 155, 151, 147, 143, 139, 135, 131, 127,
 123, 119, 115, 111, 107, 103,  99,  95,  91,  87,  83,  79,  75,  71,  67,  63,
  59,  55,  51,  47,  43,  39,  35,  31,  27,  23,  19,  15,  11,   7,   3,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0};
//---------------------------------------------------------------------------
Mat ShowImage16PseudoColor(Mat Im16, float minVal, float maxVal);
Mat ShowImageF32PseudoColor(Mat ImF, float minVal, float maxVal);


Mat ShowRegion(Mat ImReg);

Mat ShowSolidRegionOnImage(Mat ImReg, Mat ImRGB);
Mat ShowSolidRegionOnImageInBlack(Mat ImReg, Mat ImRGB);

int MaskImageInPseudocolors(Mat ImIn, Mat Roi, unsigned char grayLevel);
/*

void MakeWhiteBitmap(Graphics::TBitmap *Bitmap);

void ShowImage16PseudoColor(unsigned short *Im16, Graphics::TBitmap *Bitmap,
							int maxX,int maxY, int minVal, int maxVal);
void ShowPartOfImage16PseudoColor(unsigned short *Im16, Graphics::TBitmap *Bitmap,
								  int maxX,int maxY, int minVal, int maxVal,
								  int centerX, int centerY, int windowX ,int windowY);
void ShowRegionOnImage16(unsigned short *ImReg, unsigned short *Im16,
						 Graphics::TBitmap *Bitmap, int maxX,int maxY,
						 int minVal, int maxVal);
void ShowRegionOnImage16PseudoColor(unsigned short *ImReg, unsigned short *Im16,
									Graphics::TBitmap *Bitmap,
									int maxX,int maxY, int minVal, int maxVal);

void ShowPartRegionOnImage16(unsigned short *ImReg, unsigned short *Im16,
						 Graphics::TBitmap *Bitmap,
						 int maxX,int maxY, int minVal, int maxVal,
						 int centerX, int centerY, int windowX ,int windowY);
void ShowPartRegionOnImage16PseudoColor(unsigned short *ImReg, unsigned short *Im16,
						 Graphics::TBitmap *Bitmap,
						 int maxX,int maxY, int minVal, int maxVal,
						 int centerX, int centerY, int windowX ,int windowY);
void ShowRegion(unsigned short *ImReg, Graphics::TBitmap *Bitmap,
						 int maxX,int maxY);
void ShowPartRegion(unsigned short *ImReg, Graphics::TBitmap *Bitmap,
						 int maxX,int maxY,
						 int centerX, int centerY, int windowX ,int windowY);
void ShowRegionSpecial(unsigned short *ImReg, Graphics::TBitmap *Bitmap,
						 int maxX,int maxY);
void ShowImageFloatPseudoColor(float *ImIn, Graphics::TBitmap *Bitmap,
							   int maxX,int maxY, float minVal, float maxVal);
void ShowPartImageFloatPseudoColor(float *ImIn, Graphics::TBitmap *Bitmap,
								   int maxX,int maxY, float minVal, float maxVal,
								   int centerX, int centerY, int windowX ,int windowY);
void ShowPseudoColorScale(TImage *Im);
void ShowPseudoColorScale64x256(TImage *Im,int min, int max);
//---------------------------------------------------------------------------
void ShowImage8(unsigned char *Im8, Graphics::TBitmap *Bitmap,
				 int maxX,int maxY, int minVal, int maxVal);
void ShowImage8PseudoColor(unsigned char *Im8, Graphics::TBitmap *Bitmap,
							int maxX,int maxY, int minVal, int maxVal);
void ShowRegionOnImage8(unsigned short *ImReg, unsigned char *Im8,
						 Graphics::TBitmap *Bitmap, int maxX,int maxY,
						 int minVal, int maxVal);
void ShowRegionOnImage8Specjal(unsigned short *ImReg, unsigned char *Im8,
						 Graphics::TBitmap *Bitmap, int maxX,int maxY,
						 int minVal, int maxVal);
//---------------------------------------------------------------------------
void ShowRegionOnImage(unsigned short *ImReg, unsigned short *Im16,
									Graphics::TBitmap *Bitmap,
									int maxX,int maxY, int minVal, int maxVal);
void ShowRegioOnZoomed3Image(unsigned short *ImReg, unsigned short *Im16,
									Graphics::TBitmap *Bitmap,
									int maxX,int maxY, int minVal, int maxVal);
void ShowRegioOnZoomed3ImageOnePixel(unsigned short *ImReg, unsigned short *Im16,
									Graphics::TBitmap *Bitmap,
									int maxX,int maxY, int minVal, int maxVal);
void ShowContourOnZoomed3Image(unsigned short *ImReg, unsigned short *Im16,
									Graphics::TBitmap *Bitmap,
									int maxX,int maxY, int minVal, int maxVal);
void ShowRegioOnZoomed5ImageOnePixel(unsigned short *ImReg, unsigned short *Im16,
									Graphics::TBitmap *Bitmap,
									int maxX,int maxY, int minVal, int maxVal);
void ShowRegioOnZoomed5Image4Pixels(unsigned short *ImReg, unsigned short *Im16,
									Graphics::TBitmap *Bitmap,
									int maxX,int maxY, int minVal, int maxVal);
void ShowRegioOnZoomed5Image5Pixels(unsigned short *ImReg, unsigned short *Im16,
									Graphics::TBitmap *Bitmap,
									int maxX,int maxY, int minVal, int maxVal);
void ShowRegioOnZoomed5Image9Pixels(unsigned short *ImReg, unsigned short *Im16,
									Graphics::TBitmap *Bitmap,
									int maxX,int maxY, int minVal, int maxVal);
void ShowRegioOnZoomed5ImageBorder(unsigned short *ImReg, unsigned short *Im16,
									Graphics::TBitmap *Bitmap,
									int maxX,int maxY, int minVal, int maxVal);
void ShowContourOnZoomed5Image(unsigned short *ImReg, unsigned short *Im16,
									Graphics::TBitmap *Bitmap,
									int maxX,int maxY, int minVal, int maxVal);
//---------------------------------------------------------------------------
void MakeZoomedImage(unsigned short *ImIn, unsigned short *ImOut,
					 int maxX, int maxY, int magFactor);

void ShowUnsignedChar(unsigned char *Im8 ,Graphics::TBitmap *BitmapToShow,
						int maxX, int maxY);
*/

#endif
