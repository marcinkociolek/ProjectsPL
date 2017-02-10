#include "gradient.h"

#include <opencv2/core/core.hpp>

using namespace cv;
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
Mat GradientDown(Mat ImIn)
{
    Mat ImF, ImOut;
    ImIn.convertTo(ImF,CV_32F);



    int maxX = ImIn.cols;
    int maxY = ImIn.rows;
    int maxXY = maxX * maxY;
    int maxXa = maxX - 1;
    int maxYa = maxY - 1;

    ImOut = Mat::zeros(maxY,maxX,CV_32F);

    float *wImOut = (float*)ImOut.data;
    float *wImIn[9];

    wImIn[0] = (float*)ImF.data;
    wImIn[1] = wImIn[0] - maxX;
    wImIn[2] = wImIn[0] - maxX + 1;
    wImIn[3] = wImIn[0] + 1;
    wImIn[4] = wImIn[0] + maxX + 1;
    wImIn[5] = wImIn[0] + maxX;
    wImIn[6] = wImIn[0] + maxX - 1;
    wImIn[7] = wImIn[0] - 1;
    wImIn[8] = wImIn[0] - maxX - 1;

    float Grad[8];
    for(int i = 0; i < maxXY; i++)
    {
        int x = i % maxX;
        int y = i / maxX;

        Grad[0] = 0;

        if( y > 0 )
            Grad[0] = *wImIn[0] - *wImIn[1];
        else
            Grad[0] = 0;

        if( x < maxXa && y > 0 )
            Grad[1] = (*wImIn[0] - *wImIn[2])/1.414;
        else
            Grad[1] = 0;

        if( x < maxXa )
            Grad[2] = *wImIn[0] - *wImIn[3];
        else
            Grad[2] = 0;

        if( x < maxXa && y < maxYa )
            Grad[3] = (*wImIn[0] - *wImIn[4])/1.414;
        else
            Grad[3] = 0;

        if(y < maxYa )
            Grad[4] = *wImIn[0] - *wImIn[5];
        else
            Grad[4] = 0;

        if( x > 0 && y < maxYa )
            Grad[5] = (*wImIn[0] - *wImIn[6])/1.414;
        else
            Grad[5] = 0;

        if( x > 0 )
            Grad[6] = *wImIn[0] - *wImIn[7];
        else
            Grad[6] = 0;

        if( x > 0 && y > 0 )
            Grad[7] = (*wImIn[0] - *wImIn[8])/1.414;
        else
            Grad[7] = 0;

        float maxGrad = 0;
        for(int k = 0; k < 8; k++)
        {
            if(maxGrad < Grad[k])
                maxGrad = Grad[k];
        }
        *wImOut = maxGrad;

        for(int k = 0; k < 9; k++)
        {
            wImIn[k]++;
        }
        wImOut++;
    }
    return ImOut;
}
//---------------------------------------------------------------------------
Mat HorizontalGradientDown(Mat ImIn)
{
    Mat ImF, ImOut;
    ImIn.convertTo(ImF,CV_32F);

    int maxX = ImIn.cols;
    int maxY = ImIn.rows;
    int maxXY = maxX * maxY;
    int maxXa = maxX - 1;
    int maxYa = maxY - 1;

    ImOut = Mat::zeros(maxY,maxX,CV_32F);

    float *wImOut = (float*)ImOut.data;
    float *wImIn;
    float *wImInL;
    float *wImInR;

    wImIn = (float*)ImF.data;
    wImInL = wImIn - 1;
    wImInR = wImIn + 1;

    float GradL;
    float GradR;
    for(int i = 0; i < maxXY; i++)
    {
        int x = i % maxX;
        int y = i / maxX;

        if( x > 0 )
            GradL = *wImIn - *wImInL;
        else
            GradL = 0;

        if( x < maxXa)
            GradR = *wImIn - *wImInR;
        else
            GradR = 0;

        if(GradR < GradL)
            *wImOut = GradL;
        else
            *wImOut = GradR;

        wImIn++;
        wImInL++;
        wImInR++;

        wImOut++;
    }
    return ImOut;
}
//---------------------------------------------------------------------------
Mat HorizontalAbsGradient(Mat ImIn)
{
    Mat ImF, ImOut;
    ImIn.convertTo(ImF,CV_32F);

    int maxX = ImIn.cols;
    int maxY = ImIn.rows;
    int maxXY = maxX * maxY;
    int maxXa = maxX - 1;
    int maxYa = maxY - 1;

    ImOut = Mat::zeros(maxY,maxX,CV_32F);

    float *wImOut = (float*)ImOut.data;
    float *wImIn;
    float *wImInL;
    float *wImInR;

    wImIn = (float*)ImF.data;
    wImInL = wImIn - 1;
    wImInR = wImIn + 1;

    float GradL;
    float GradR;
    for(int i = 0; i < maxXY; i++)
    {
        int x = i % maxX;
        int y = i / maxX;

        if( x > 0 & x < maxXa )
            *wImOut = sqrt((*wImInL - *wImInL)*(*wImInL - *wImInL));
        else
            *wImOut = 0;

        //wImIn++;
        wImInL++;
        wImInR++;

        wImOut++;
    }
    return ImOut;
}
//-----------------------------------------------------------------------------------------------------------------------
Mat GradientThresh(Mat ImIn,float threshold)
{
    Mat ImOut;
//    ImIn.convertTo(ImF,CV_32F);

    int maxX = ImIn.cols;
    int maxY = ImIn.rows;
    int maxXY = maxX * maxY;

    ImOut = Mat::zeros(maxY,maxX,CV_16U);

    unsigned short *wImOut = (unsigned short*)ImOut.data;
    float *wImIn = (float*)ImIn.data;

    for(int i = 0; i < maxXY; i++)
    {
        if( *wImIn > threshold )
            *wImOut = 1;
        else
            *wImOut = 0;

        wImIn++;
        wImOut++;
    }
    return ImOut;
}
//------------------------------------------------------------------------------
Mat RemovingTinyReg9(Mat ImR)
{
// renewed version sweeps whole image
    int maxX = ImR.cols;
    int maxY = ImR.rows;
    int maxXY = maxX * maxY;
    int maxXa = maxX - 1;
    int maxYa = maxY - 1;

    Mat ImRTemp = Mat::zeros(maxY,maxX,CV_16U);

    unsigned short *wImRTemp = (unsigned short*)ImRTemp.data;

    unsigned short *wImR0 = (unsigned short*)ImR.data;
    unsigned short *wImR1 = wImR0 - maxX - 1;
    unsigned short *wImR2 = wImR0 - maxX;
    unsigned short *wImR3 = wImR0 - maxX + 1;
    unsigned short *wImR4 = wImR0 - 1;
    unsigned short *wImR5 = wImR0 + 1;
    unsigned short *wImR6 = wImR0 + maxX - 1;
    unsigned short *wImR7 = wImR0 + maxX;
    unsigned short *wImR8 = wImR0 + maxX + 1;

    for (int i = 0; i < maxXY; i++)
    {
        int x = i % maxX;
        int y = i / maxX;

        unsigned short sum = 0;
        if (y > 0 && x > 0)
        {
            sum += *wImR1;
        }
        if (y > 0)
        {
            sum += *wImR2;
        }
        if (y > 0 && x < maxXa)
        {
            sum += *wImR3;
        }
        if (x > 0)
        {
            sum += *wImR4;
        }
        if (x < maxXa)
        {
            sum += *wImR5;
        }
        if (y < maxYa && x > 0)
        {
            sum += *wImR6;
        }
        if (y < maxYa)
        {
            sum += *wImR7;
        }
        if (y < maxYa && x < maxXa)
        {
            sum += *wImR8;
        }

        if(sum)
            *wImRTemp = *wImR0;
        else
            *wImRTemp = 0;

        wImRTemp++;

        wImR0++;
        wImR1++;
        wImR2++;
        wImR3++;
        wImR4++;
        wImR5++;
        wImR6++;
        wImR7++;
        wImR8++;
    }
    return ImRTemp;
}
//---------------------------------------------------------------------------
void FillBorderWithValue(Mat ImReg, float value)
{
    int maxX = ImReg.cols;
    int maxY = ImReg.rows;
    int maxXY = maxX * maxY;
    unsigned short *wImReg; // moving pointer
    // first row
    wImReg = (unsigned short*)ImReg.data;
    for(int i = 0; i < maxX; i++)
    {
        if(!*wImReg)
            *wImReg = value;
        wImReg++;
    }
    // last row
    wImReg = (unsigned short*)ImReg.data + maxXY - maxX;
    for(int i = 0; i < maxX; i++)
    {
        if(!*wImReg)
            *wImReg = value;
        wImReg++;
    }
    // first column
    wImReg = (unsigned short*)ImReg.data;
    for(int i = 0; i < maxY; i++)
    {
        if(!*wImReg)
            *wImReg = value;
        wImReg += maxX;
    }
    // last row
    wImReg = (unsigned short*)ImReg.data + maxX-1;
    for(int i = 0; i < maxY; i++)
    {
        if(!*wImReg)
            *wImReg = value;
        wImReg += maxX;
    }
}
//------------------------------------------------------------------------------
void OneRegionFill5Fast1(Mat ImR,  unsigned short fillReg)
{
    int maxX = ImR.cols;
    int maxY = ImR.rows;
    int maxXY = maxX * maxY;

    int maxXa = maxX - 1;
    int maxYa = maxY - 1;

    bool wasAction = 1;

    while(1)
    {
        wasAction = 0;
        unsigned short *wImR0 = (unsigned short*)ImR.data;
        unsigned short *wImR1 = wImR0 - maxX;
        unsigned short *wImR2 = wImR0 - 1;
        unsigned short *wImR3 = wImR0 + 1;
        unsigned short *wImR4 = wImR0 + maxX;


        for (int i = 0; i < maxXY; i++)
        {
            int x = i % maxX;
            int y = i / maxX;

            if (x > 0 && x < maxXa && y > 0 && y < maxYa)
            {
                if(*wImR0 == 0 &&
                   (*wImR1 == fillReg || *wImR2 == fillReg ||
                    *wImR3 == fillReg || *wImR4 == fillReg ))
                {
                    *wImR0 = fillReg;
                    wasAction = 1;
                }
            }
            wImR0++;
            wImR1++;
            wImR2++;
            wImR3++;
            wImR4++;
        }
        if(!wasAction)
            return;

        wasAction = 0;
        wImR0 = (unsigned short*)ImR.data + maxXY;
        wImR1 = wImR0 - maxX;
        wImR2 = wImR0 - 1;
        wImR3 = wImR0 + 1;
        wImR4 = wImR0 + maxX;

        for (int i = maxXY; i > 0; i--)
        {
            int x = i % maxX;
            int y = i / maxX;

            if (x > 0 && x < maxXa && y > 0 && y < maxYa)
            {
                if(*wImR0 == 0 &&
                   (*wImR1 == fillReg || *wImR2 == fillReg ||
                    *wImR3 == fillReg || *wImR4 == fillReg ))
                {
                    *wImR0 = fillReg;
                    wasAction = 1;
                }
            }
            wImR0--;
            wImR1--;
            wImR2--;
            wImR3--;
            wImR4--;
        }
        if(!wasAction)
            return;
    }
}
//------------------------------------------------------------------------------
void FillHoles(Mat ImR)
{
    int maxX = ImR.cols;
    int maxY = ImR.rows;
    int maxXY = maxX * maxY;
    unsigned short *wImR = (unsigned short*)ImR.data;
    for (int i = 0; i < maxXY; i++)
    {
        if(*wImR == 0)
            *wImR = 1;
        wImR++;
    }
}
//---------------------------------------------------------------------------
void DeleteRegionFromImage(Mat ImR, unsigned short regToRemove)
{
    if(!regToRemove)
        return;
    int maxX = ImR.cols;
    int maxY = ImR.rows;
    int maxXY = maxX * maxY;
    unsigned short *wImR = (unsigned short*)ImR.data;
    for(int i = 0; i < maxXY; i++)
    {
        if(*wImR == regToRemove)
            *wImR = 0;
        wImR++;
    }
}
//---------------------------------------------------------------------------
unsigned short DivideSeparateRegions(Mat InReg, unsigned int minRegSize)
// fonction returns number of new segments
{
    int maxX = InReg.cols;
    int maxY = InReg.rows;
    int maxXY = maxX * maxY;
    int maxXa = maxX - 1;
    unsigned short *OutReg = new unsigned short[maxXY];


    // moving pointers
    unsigned short *wInReg = (unsigned short*)InReg.data;

    unsigned short *wInRegN1 = wInReg - 1; 		//y  ,x-1
    unsigned short *wInRegN2 = wInReg - maxX - 1; 	//y-1,x+1
    unsigned short *wInRegN3 = wInReg - maxX; 	//y-1,x
    unsigned short *wInRegN4 = wInReg - maxX + 1; 	//y-1,x-1

    unsigned short *wOutReg = OutReg;

    unsigned short *wOutRegN1 = OutReg - 1; 	//y  ,x-1
    unsigned short *wOutRegN2 = OutReg - maxX - 1; 	//y-1,x+1
    unsigned short *wOutRegN3 = OutReg - maxX; 	//y-1,x
    unsigned short *wOutRegN4 = OutReg - maxX + 1; 	//y-1,x-1

    unsigned short newReg = 1;

    int maxRegNr = 0xFFFC;
    unsigned short *Exchange = new unsigned short[maxRegNr];
    unsigned int *RegionSizes = new unsigned int[maxRegNr];

    unsigned short *wExchange = Exchange;
    unsigned int *wRegionSizes = RegionSizes;

    for (int i = 0; i < maxRegNr; i++)
    {
        *wExchange = i;
        *wRegionSizes = 0;
        wExchange++;
        wRegionSizes++;
    }

    unsigned short SameRegion[4];
    int iSameRegion;

    int licznik = 0;

    for (int i = 0; i < maxXY; i++)
    {
        int x = i % maxX;
        int y = i / maxX;
        if(*wInReg > 0 && *wInReg < 0xFFFD)
        {
            iSameRegion = -1;

            SameRegion[0] = 0xFFFF;
            SameRegion[1] = 0xFFFF;
            SameRegion[2] = 0xFFFF;
            SameRegion[3] = 0xFFFF;

            if (x > 0 && *wInReg == *wInRegN1)
            {
                iSameRegion++;
                SameRegion[iSameRegion] = *wOutRegN1;
            }
            if (y > 0 && x > 0 && *wInReg == *wInRegN2)
            {
                iSameRegion++;
                SameRegion[iSameRegion] = *wOutRegN2;
            }
            if (y > 0 && *wInReg == *wInRegN3)
            {
                iSameRegion++;
                SameRegion[iSameRegion] = *wOutRegN3;
            }
            if (y > 0 && x < maxXa &&*wInReg == *wInRegN4)
            {
                iSameRegion++;
                SameRegion[iSameRegion] = *wOutRegN4;
            }
            // there is no same neughbours. Create new region
            if (iSameRegion == -1)
            {
               *wOutReg = newReg;
               newReg++;
            }
            else
            {
                // looking for the smalest region
                unsigned short minSameRegion = SameRegion[0];
                for (int k = 1; k <= iSameRegion; k++)
                    if (minSameRegion > SameRegion[k])
                        minSameRegion = SameRegion[k];

                *wOutReg = minSameRegion;

                //Updating conversion table
                for (int k = 0; k <= iSameRegion; k++)
                    if(SameRegion[k] != minSameRegion)
                    {
                        RegionExchange(OutReg, i - 1,SameRegion[k], minSameRegion);
                        licznik++;
                    }
            }
        }
        else
            *wOutReg = *wInReg;

        //updating pointers
        wInReg++;

        wInRegN1++;
        wInRegN2++;
        wInRegN3++;
        wInRegN4++;

        wOutReg++;

        wOutRegN1++;
        wOutRegN2++;
        wOutRegN3++;
        wOutRegN4++;
    }

    wInReg = wInReg = (unsigned short*)InReg.data;
    wOutReg = OutReg;
    for (int i = 0; i < maxXY; i++)
    {

        *wInReg = *wOutReg;
        RegionSizes[*wInReg]++;
        wInReg++;
        wOutReg++;
    }
    //--------------------------------------------------------------------------
    // usuwanie ma?ych obszarów zmiana numeracji
    unsigned short newRegNR = 1;
    for(unsigned short k = 1; k <= newReg; k++)
    {
        if (RegionSizes[k] <=   minRegSize)
        {
            Exchange[k] = 0;
        }
        else
        {
            Exchange[k] = newRegNR;
            newRegNR++;
        }
    }
    wInReg = wInReg = (unsigned short*)InReg.data;
    //unsigned short oldRegNr;
    for (int i = 0; i < maxXY; i++)
    {
        if(*wInReg < 0xFFFD)
            *wInReg = Exchange[*wInReg];
        wInReg++;
    }
    delete[] Exchange;
    delete[] RegionSizes;
    delete[] OutReg;

    return (newRegNR - 1);
}
//------------------------------------------------------------------------------
void RegionExchange(unsigned short *ImReg, int stop,
                    unsigned short rI, unsigned  short rO)
{
    unsigned short *wImReg = ImReg;
    for (int i = 0; i <= stop; i++)
    {
        if(*wImReg == rI)
        {
            *wImReg = rO;
        }
        wImReg++;
    }
}
//------------------------------------------------------------------------------
//---------------------------------------------------------------------------
void DeleteRegTouchingBorder(Mat ImReg)
{
    int maxX = ImReg.cols;
    int maxY = ImReg.rows;
    int maxXY = maxX * maxY;

//    int maxXa = maxX - 1;
//    unsigned short *OutReg = new unsigned short[maxXY];

    bool RegToRemove[0xFFFF];
    bool *wRegToRemove = RegToRemove;
    for(int i = 0;i <= 0xFFFF; i++)
    {
        *wRegToRemove = 0;
        wRegToRemove++ ;
    }
    // first row
    unsigned short *wImReg;
    wImReg = (unsigned short*)ImReg.data;
    for(int i = 0;i < maxX; i++)
    {
        if(*wImReg)
            RegToRemove[*wImReg] = 1;
        wImReg++;
    }
    //last row
    wImReg = (unsigned short*)ImReg.data + maxX * (maxY - 1);
    for(int i = 0;i < maxX; i++)
    {
        if(*wImReg)
            RegToRemove[*wImReg] = 1;
        wImReg++;
    }

    wImReg = (unsigned short*)ImReg.data;
    for(int i = 0;i < maxY; i++)
    {
        if(*wImReg)
            RegToRemove[*wImReg] = 1;
        wImReg += maxX;
    }
    wImReg = (unsigned short*)ImReg.data + maxX - 1;
    for(int i = 0;i < maxY-1; i++)
    {
        if(*wImReg)
            RegToRemove[*wImReg] = 1;
        wImReg += maxX;
    }

    unsigned short RegToRemoveList[65535];
    int iRegToRemoveList = -1;
    wRegToRemove = RegToRemove + 1;
    for(int i = 1;i <= 0xFFFF; i++)
    {
        if(*wRegToRemove)
        {
            iRegToRemoveList++;
            RegToRemoveList[iRegToRemoveList] = i;
        }
        wRegToRemove++;
    }
    if(iRegToRemoveList == -1)
        return;
    for(int i = 0; i <= iRegToRemoveList; i++)
    {
        DeleteRegionFromImage(ImReg, RegToRemoveList[i]);
    }
    unsigned short regCount = DivideSeparateRegions(ImReg, 0);
    return;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//------------------------------------------------------------------------------
void RegionDilation5(Mat ImR)
{
// renewed version dilates whole imaga
    int maxX = ImR.cols;
    int maxY = ImR.rows;
    int maxXY = maxX * maxY;
    int maxXa = maxX - 1;
    int maxYa = maxY - 1;

    unsigned short *ImRTemp = new unsigned short[maxXY];

    unsigned short *wImRTemp = ImRTemp;

    unsigned short *wImR0 = (unsigned short*)ImR.data;
    unsigned short *wImR1 = wImR0 - maxX;
    unsigned short *wImR2 = wImR0 - 1;
    unsigned short *wImR3 = wImR0 + 1;
    unsigned short *wImR4 = wImR0 + maxX;

    for (int i = 0; i < maxXY; i++)
    {
        int x = i % maxX;
        int y = i / maxX;

        unsigned int sum = (unsigned int)*wImR0;
        if (y > 0)
        {
            sum += (unsigned int)*wImR1;
        }
        if (x > 0)
        {
            sum += (unsigned int)*wImR2;
        }
        if (x < maxXa)
        {
            sum += (unsigned int)*wImR3;
        }
        if (y < maxYa)
        {
            sum += (unsigned int)*wImR4;
        }
        if(sum)
            *wImRTemp = 1;
        else
            *wImRTemp = 0;

        wImRTemp++;

        wImR0++;
        wImR1++;
        wImR2++;
        wImR3++;
        wImR4++;
    }
    wImRTemp = ImRTemp;
    wImR0 = (unsigned short*)ImR.data;
    for (int i = 0; i < maxXY; i++)
    {
        *wImR0 = *wImRTemp;
        wImRTemp++;
        wImR0++;
    }
    delete[] ImRTemp;
}
//------------------------------------------------------------------------------
void RegionDilation9(Mat ImR)
{
// renewed version dilates whole image
    int maxX = ImR.cols;
    int maxY = ImR.rows;
    int maxXY = maxX * maxY;
    int maxXa = maxX - 1;
    int maxYa = maxY - 1;

    unsigned short *ImRTemp = new unsigned short[maxXY];

    unsigned short *wImRTemp = ImRTemp;

    unsigned short *wImR0 = (unsigned short*)ImR.data;
    unsigned short *wImR1 = wImR0 - maxX - 1;
    unsigned short *wImR2 = wImR0 - maxX;
    unsigned short *wImR3 = wImR0 - maxX + 1;
    unsigned short *wImR4 = wImR0 - 1;
    unsigned short *wImR5 = wImR0 + 1;
    unsigned short *wImR6 = wImR0 + maxX - 1;
    unsigned short *wImR7 = wImR0 + maxX;
    unsigned short *wImR8 = wImR0 + maxX + 1;

    for (int i = 0; i < maxXY; i++)
    {
        int x = i % maxX;
        int y = i / maxX;

        unsigned int sum = (unsigned int)*wImR0;
        if (y > 0 && x > 0)
        {
            sum += (unsigned int)*wImR1;
        }
        if (y > 0)
        {
            sum += (unsigned int)*wImR2;
        }
        if (y > 0 && x < maxXa)
        {
            sum += (unsigned int)*wImR3;
        }
        if (x > 0)
        {
            sum += (unsigned int)*wImR4;
        }
        if (x < maxXa)
        {
            sum += (unsigned int)*wImR5;
        }
        if (y < maxYa && x > 0)
        {
            sum += (unsigned int)*wImR6;
        }
        if (y < maxYa)
        {
            sum += (unsigned int)*wImR7;
        }
        if (y < maxYa && x < maxXa)
        {
            sum += (unsigned int)*wImR8;
        }

        if(sum)
            *wImRTemp = 1;
        else
            *wImRTemp = 0;


        wImRTemp++;

        wImR0++;
        wImR1++;
        wImR2++;
        wImR3++;
        wImR4++;
        wImR5++;
        wImR6++;
        wImR7++;
        wImR8++;
    }
    wImRTemp = ImRTemp;
    wImR0 = (unsigned short*)ImR.data;
    for (int i = 0; i < maxXY; i++)
    {
        *wImR0 = *wImRTemp;
        wImRTemp++;
        wImR0++;
    }
    delete[] ImRTemp;
}
//------------------------------------------------------------------------------
void RegionDilation13(Mat ImR)
{
// renewed version dilates whole imaga
    int maxX = ImR.cols;
    int maxY = ImR.rows;
    int maxXY = maxX * maxY;
    int maxXa = maxX - 1;
    int maxXb = maxX - 2;

    int maxYa = maxY - 1;
    int maxYb = maxY - 2;

    unsigned short *ImRTemp = new unsigned short[maxXY];

    unsigned short *wImRTemp = ImRTemp;

    unsigned short *wImR0  = (unsigned short*)ImR.data;
    unsigned short *wImR1  = wImR0 - (2 * maxX);
    unsigned short *wImR2  = wImR0 - maxX - 1;
    unsigned short *wImR3  = wImR0 - maxX;
    unsigned short *wImR4  = wImR0 - maxX + 1;
    unsigned short *wImR5  = wImR0 - 2;
    unsigned short *wImR6  = wImR0 - 1;
    unsigned short *wImR7  = wImR0 + 1;
    unsigned short *wImR8  = wImR0 + 2;
    unsigned short *wImR9  = wImR0 + maxX - 1;
    unsigned short *wImR10 = wImR0 + maxX;
    unsigned short *wImR11 = wImR0 + maxX + 1;
    unsigned short *wImR12 = wImR0 + (2 * maxX);


    for (int i = 0; i < maxXY; i++)
    {
        int x = i % maxX;
        int y = i / maxX;

        unsigned int sum = (unsigned int)*wImR0;

        if (y > 1)
        {
            sum += (unsigned int)*wImR1;
        }
        if (y > 0 && x > 0)
        {
            sum += (unsigned int)*wImR2;
        }
        if (y > 0)
        {
            sum += (unsigned int)*wImR3;
        }
        if (y > 0 && x < maxXa)
        {
            sum += (unsigned int)*wImR4;
        }
        if (x > 1)
        {
            sum += (unsigned int)*wImR5;
        }
        if (x > 0)
        {
            sum += (unsigned int)*wImR6;
        }
        if (x < maxXa)
        {
            sum += (unsigned int)*wImR7;
        }
        if (x < maxXb)
        {
            sum += (unsigned int)*wImR8;
        }

        if (y < maxYa && x > 0)
        {
            sum += (unsigned int)*wImR9;
        }
        if (y < maxYa)
        {
            sum += (unsigned int)*wImR10;
        }
        if (y < maxYa && x < maxXa)
        {
            sum += (unsigned int)*wImR11;
        }
        if (y < maxYb)
        {
            sum += (unsigned int)*wImR12;
        }

        if(sum)
            *wImRTemp = 1;
        else
            *wImRTemp = 0;


        wImRTemp++;

        wImR0++;
        wImR1++;
        wImR2++;
        wImR3++;
        wImR4++;
        wImR5++;
        wImR6++;
        wImR7++;
        wImR8++;
        wImR9++;
        wImR10++;
        wImR11++;
        wImR12++;
    }
    wImRTemp = ImRTemp;
    wImR0 = (unsigned short*)ImR.data;
    for (int i = 0; i < maxXY; i++)
    {
        *wImR0 = *wImRTemp;
        wImRTemp++;
        wImR0++;
    }
    delete[] ImRTemp;
}
//------------------------------------------------------------------------------
void RegionErosion5(Mat ImR)
{
// renewed version erodes whole imaga
    int maxX = ImR.cols;
    int maxY = ImR.rows;
    int maxXY = maxX * maxY;
    int maxXa = maxX - 1;
    int maxYa = maxY - 1;

    unsigned short *ImRTemp = new unsigned short[maxXY];

    unsigned short *wImRTemp = ImRTemp;

    unsigned short *wImR0 = (unsigned short*)ImR.data;;
    unsigned short *wImR1 = wImR0 - maxX;
    unsigned short *wImR2 = wImR0 - 1;
    unsigned short *wImR3 = wImR0 + 1;
    unsigned short *wImR4 = wImR0 + maxX;

    for (int i = 0; i < maxXY; i++)
    {
        int x = i % maxX;
        int y = i / maxX;

        unsigned int product = (unsigned int)*wImR0;
        if (y > 0)
        {
            product *= (unsigned int)*wImR1;
        }
        if (x > 0)
        {
            product *= (unsigned int)*wImR2;
        }
        if (x < maxXa)
        {
            product *= (unsigned int)*wImR3;
        }
        if (y < maxYa)
        {
            product *= (unsigned int)*wImR4;
        }
        if(product)
            *wImRTemp = 1;
        else
            *wImRTemp = 0;

        wImRTemp++;

        wImR0++;
        wImR1++;
        wImR2++;
        wImR3++;
        wImR4++;
    }
    wImRTemp = ImRTemp;
    wImR0 = (unsigned short*)ImR.data;;
    for (int i = 0; i < maxXY; i++)
    {
        *wImR0 = *wImRTemp;
        wImRTemp++;
        wImR0++;
    }
    delete[] ImRTemp;
}
//------------------------------------------------------------------------------
void RegionErosion9(Mat ImR)
{
// renewed version erodes whole imaga
    int maxX = ImR.cols;
    int maxY = ImR.rows;
    int maxXY = maxX * maxY;
    int maxXa = maxX - 1;
    int maxYa = maxY - 1;

    unsigned short *ImRTemp = new unsigned short[maxXY];

    unsigned short *wImRTemp = ImRTemp;

    unsigned short *wImR0 = (unsigned short*)ImR.data;;
    unsigned short *wImR1 = wImR0 - maxX - 1;
    unsigned short *wImR2 = wImR0 - maxX;
    unsigned short *wImR3 = wImR0 - maxX + 1;
    unsigned short *wImR4 = wImR0 - 1;
    unsigned short *wImR5 = wImR0 + 1;
    unsigned short *wImR6 = wImR0 + maxX - 1;
    unsigned short *wImR7 = wImR0 + maxX;
    unsigned short *wImR8 = wImR0 + maxX + 1;

    for (int i = 0; i < maxXY; i++)
    {
        int x = i % maxX;
        int y = i / maxX;

        unsigned int product = (unsigned int)*wImR0;
        if (y > 0 && x > 0)
        {
            product *= (unsigned int)*wImR1;
        }
        if (y > 0)
        {
            product *= (unsigned int)*wImR2;
        }
        if (y > 0 && x < maxXa)
        {
            product *= (unsigned int)*wImR3;
        }
        if (x > 0)
        {
            product *= (unsigned int)*wImR4;
        }
        if (x < maxXa)
        {
            product *= (unsigned int)*wImR5;
        }
        if (y < maxYa && x > 0)
        {
            product *= (unsigned int)*wImR6;
        }
        if (y < maxYa)
        {
            product *= (unsigned int)*wImR7;
        }
        if (y < maxYa && x < maxXa)
        {
            product *= (unsigned int)*wImR8;
        }

        if(product)
            *wImRTemp = 1;
        else
            *wImRTemp = 0;


        wImRTemp++;

        wImR0++;
        wImR1++;
        wImR2++;
        wImR3++;
        wImR4++;
        wImR5++;
        wImR6++;
        wImR7++;
        wImR8++;
    }
    wImRTemp = ImRTemp;
    wImR0 = (unsigned short*)ImR.data;;
    for (int i = 0; i < maxXY; i++)
    {
        *wImR0 = *wImRTemp;
        wImRTemp++;
        wImR0++;
    }
    delete[] ImRTemp;
}
//------------------------------------------------------------------------------
void RegionErosion13(Mat ImR)
{
// renewed version erodes whole imaga
    int maxX = ImR.cols;
    int maxY = ImR.rows;
    int maxXY = maxX * maxY;
    int maxXa = maxX - 1;
    int maxXb = maxX - 2;

    int maxYa = maxY - 1;
    int maxYb = maxY - 2;

    unsigned short *ImRTemp = new unsigned short[maxXY];

    unsigned short *wImRTemp = ImRTemp;

    unsigned short *wImR0  = (unsigned short*)ImR.data;
    unsigned short *wImR1  = wImR0 - (2 * maxX);
    unsigned short *wImR2  = wImR0 - maxX - 1;
    unsigned short *wImR3  = wImR0 - maxX;
    unsigned short *wImR4  = wImR0 - maxX + 1;
    unsigned short *wImR5  = wImR0 - 2;
    unsigned short *wImR6  = wImR0 - 1;
    unsigned short *wImR7  = wImR0 + 1;
    unsigned short *wImR8  = wImR0 + 2;
    unsigned short *wImR9  = wImR0 + maxX - 1;
    unsigned short *wImR10 = wImR0 + maxX;
    unsigned short *wImR11 = wImR0 + maxX + 1;
    unsigned short *wImR12 = wImR0 + (2 * maxX);


    for (int i = 0; i < maxXY; i++)
    {
        int x = i % maxX;
        int y = i / maxX;

        unsigned int product = (unsigned int)*wImR0;

        if (y > 1)
        {
            product *= (unsigned int)*wImR1;
        }
        if (y > 0 && x > 0)
        {
            product *= (unsigned int)*wImR2;
        }
        if (y > 0)
        {
            product *= (unsigned int)*wImR3;
        }
        if (y > 0 && x < maxXa)
        {
            product *= (unsigned int)*wImR4;
        }
        if (x > 1)
        {
            product *= (unsigned int)*wImR5;
        }
        if (x > 0)
        {
            product *= (unsigned int)*wImR6;
        }
        if (x < maxXa)
        {
            product *= (unsigned int)*wImR7;
        }
        if (x < maxXb)
        {
            product *= (unsigned int)*wImR8;
        }

        if (y < maxYa && x > 0)
        {
            product *= (unsigned int)*wImR9;
        }
        if (y < maxYa)
        {
            product *= (unsigned int)*wImR10;
        }
        if (y < maxYa && x < maxXa)
        {
            product *= (unsigned int)*wImR11;
        }
        if (y < maxYb)
        {
            product *= (unsigned int)*wImR12;
        }

        if(product)
            *wImRTemp = 1;
        else
            *wImRTemp = 0;


        wImRTemp++;

        wImR0++;
        wImR1++;
        wImR2++;
        wImR3++;
        wImR4++;
        wImR5++;
        wImR6++;
        wImR7++;
        wImR8++;
        wImR9++;
        wImR10++;
        wImR11++;
        wImR12++;
    }
    wImRTemp = ImRTemp;
    wImR0 = (unsigned short*)ImR.data;
    for (int i = 0; i < maxXY; i++)
    {
        *wImR0 = *wImRTemp;
        wImRTemp++;
        wImR0++;
    }
    delete[] ImRTemp;
}
//------------------------------------------------------------------------------//---------------------------------------------------------------------------
void RegionErosionHorisontal13(Mat ImR)
{
// renewed version erodes whole imaga
    int maxX = ImR.cols;
    int maxY = ImR.rows;
    int maxXY = maxX * maxY;
    int maxXa = maxX - 1;
    int maxXb = maxX - 2;

    int maxYa = maxY - 1;
    int maxYb = maxY - 2;

    unsigned short *ImRTemp = new unsigned short[maxXY];

    unsigned short *wImRTemp = ImRTemp;

    unsigned short *wImR0  = (unsigned short*)ImR.data;
    unsigned short *wImR1  = wImR0 - (2 * maxX);
    unsigned short *wImR2  = wImR0 - maxX - 1;
    unsigned short *wImR3  = wImR0 - maxX;
    unsigned short *wImR4  = wImR0 - maxX + 1;
    unsigned short *wImR5  = wImR0 - 2;
    unsigned short *wImR6  = wImR0 - 1;
    unsigned short *wImR7  = wImR0 + 1;
    unsigned short *wImR8  = wImR0 + 2;
    unsigned short *wImR9  = wImR0 + maxX - 1;
    unsigned short *wImR10 = wImR0 + maxX;
    unsigned short *wImR11 = wImR0 + maxX + 1;
    unsigned short *wImR12 = wImR0 + (2 * maxX);


    for (int i = 0; i < maxXY; i++)
    {
        int x = i % maxX;
        int y = i / maxX;

        unsigned int product = (unsigned int)*wImR0;

        if (y > 1)
        {
            product *= (unsigned int)*wImR1;
        }
        if (y > 0 && x > 0)
        {
            product *= (unsigned int)*wImR2;
        }
        if (y > 0)
        {
            product *= (unsigned int)*wImR3;
        }
        if (y > 0 && x < maxXa)
        {
            product *= (unsigned int)*wImR4;
        }
        if (x > 1)
        {
            product *= (unsigned int)*wImR5;
        }
        if (x > 0)
        {
            product *= (unsigned int)*wImR6;
        }
        if (x < maxXa)
        {
            product *= (unsigned int)*wImR7;
        }
        if (x < maxXb)
        {
            product *= (unsigned int)*wImR8;
        }

        if (y < maxYa && x > 0)
        {
            product *= (unsigned int)*wImR9;
        }
        if (y < maxYa)
        {
            product *= (unsigned int)*wImR10;
        }
        if (y < maxYa && x < maxXa)
        {
            product *= (unsigned int)*wImR11;
        }
        if (y < maxYb)
        {
            product *= (unsigned int)*wImR12;
        }

        if(product)
            *wImRTemp = 1;
        else
            *wImRTemp = 0;


        wImRTemp++;

        wImR0++;
        wImR1++;
        wImR2++;
        wImR3++;
        wImR4++;
        wImR5++;
        wImR6++;
        wImR7++;
        wImR8++;
        wImR9++;
        wImR10++;
        wImR11++;
        wImR12++;
    }
    wImRTemp = ImRTemp;
    wImR0 = (unsigned short*)ImR.data;
    for (int i = 0; i < maxXY; i++)
    {
        *wImR0 = *wImRTemp;
        wImRTemp++;
        wImR0++;
    }
    delete[] ImRTemp;
}
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
Mat GetContour5(Mat ImR)
{
    //modyfied version works on whole image
    int maxX = ImR.cols;
    int maxY = ImR.rows;
    int maxXY = maxX * maxY;

    int maxXa = maxX - 1;
    int maxYa = maxY - 1;

    Mat ImOut = Mat::zeros(maxY,maxX, CV_16U);

    unsigned short *wImOut = (unsigned short*)ImOut.data;



    unsigned short *wImR0 = (unsigned short*)ImR.data;
    unsigned short *wImR1 = wImR0 - maxX;
    unsigned short *wImR2 = wImR0 - 1;
    unsigned short *wImR3 = wImR0 + 1;
    unsigned short *wImR4 = wImR0 + maxX;

    for (int i = 0; i < maxXY; i++)
    {
        int x = i % maxX;
        int y = i / maxX;

        unsigned int product = 1;
        if (y > 0 && *wImR0 != *wImR1)
        {
            product *= 0;
        }
        if (x > 0 && *wImR0 != *wImR2)
        {
            product *= 0;
        }
        if (x < maxXa && *wImR0 != *wImR3)
        {
            product *= 0;
        }
        if (y < maxYa && *wImR0 != *wImR4)
        {
            product *= 0;
        }
        if(!product && *wImR0)
            *wImOut = *wImR0;
        else
            *wImOut = 0;

        wImOut++;

        wImR0++;
        wImR1++;
        wImR2++;
        wImR3++;
        wImR4++;
    }
    return ImOut;
}
//------------------------------------------------------------------------------
void GetContour9(Mat ImR)
{
    int maxX = ImR.cols;
    int maxY = ImR.rows;
    int maxXY = maxX * maxY;

    int maxXa = maxX - 1;
    int maxYa = maxY - 1;

    unsigned short *ImRTemp = new unsigned short[maxXY];

    unsigned short *wImRTemp = ImRTemp;

    unsigned short *wImR0 = (unsigned short*)ImR.data;
    unsigned short *wImR1 = wImR0 - maxX - 1;
    unsigned short *wImR2 = wImR0 - maxX;
    unsigned short *wImR3 = wImR0 - maxX + 1;
    unsigned short *wImR4 = wImR0 - 1;
    unsigned short *wImR5 = wImR0 + 1;
    unsigned short *wImR6 = wImR0 + maxX - 1;
    unsigned short *wImR7 = wImR0 + maxX;
    unsigned short *wImR8 = wImR0 + maxX + 1;


    for (int i = 0; i < maxXY; i++)
    {
        int x = i % maxX;
        int y = i / maxX;
        if (x > 0 && x < maxXa && y > 0 && y < maxYa)
        {
            if(*wImR0 && !(*wImR1 *  *wImR2 * *wImR3  * *wImR4
                      * *wImR5 *  *wImR6 * *wImR7  * *wImR8))
                *wImRTemp = *wImR0;
            else
                *wImRTemp = 0;
        }
        else
            *wImRTemp = 0;

        wImRTemp++;

        wImR0++;
        wImR1++;
        wImR2++;
        wImR3++;
        wImR4++;
        wImR5++;
        wImR6++;
        wImR7++;
        wImR8++;
    }
    wImRTemp = ImRTemp;
    wImR0 = (unsigned short*)ImR.data;
    for (int i = 0; i < maxXY; i++)
    {
        *wImR0 = *wImRTemp;
        wImRTemp++;
        wImR0++;
    }
    delete[] ImRTemp;
}


