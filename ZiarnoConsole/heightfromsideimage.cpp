#include <opencv2/core/core.hpp>
#include "heightfromsideimage.h"

//--------------------------------------------------------------------------------------------------
int FindGrainHeighOnBRG(cv::Mat ImBGR, int threshold)
{
    int maxX = ImBGR.cols;
    int maxY = ImBGR.rows;
    if(maxX == 0 || maxY ==0)
        return -1;

    if(!ImBGR.isContinuous())
        return -2;

    unsigned char *wImBGR = (unsigned char*)ImBGR.data;
    int *BlacLineLengths = new int[maxY];
    int firstLine = -1;
    int firstBlackPixelOnEdgePos = -1;
    bool firstLineFound = false;

    for (int y = 0; y < maxY; y++)
    {
        int sumLineBlackPixels = 0;
        for (int x = 0; x < maxX; x++)
        {
            float brightness ;
            brightness = (int)*wImBGR * 114;
            wImBGR++;
            brightness += (int)*wImBGR * 587;
            wImBGR++;
            brightness += (int)*wImBGR * 299;
            wImBGR++;
            brightness /=1000;
            if(brightness <= threshold)
            {
                sumLineBlackPixels ++;
                if(x == 0 && firstBlackPixelOnEdgePos < y)
                    firstBlackPixelOnEdgePos = y;
            }
        }
        BlacLineLengths[y] = sumLineBlackPixels;
        if(sumLineBlackPixels && !firstLineFound)
        {
            firstLine = y;
            firstLineFound = true;
        }
        if(firstBlackPixelOnEdgePos>=0)
        {
            break;
        }

    }
    if(!firstLineFound)
        return -3;
    if(firstBlackPixelOnEdgePos == -1)
        return -4;

    int start = firstBlackPixelOnEdgePos - (firstBlackPixelOnEdgePos - firstLine) * 2 / 3;

    int minLineLength = maxX;
    int minLinePosition = 0;
    for(int y = start; y<firstBlackPixelOnEdgePos; y++)
    {
        if(minLineLength > BlacLineLengths[y])
        {
            minLineLength = BlacLineLengths[y];
            minLinePosition = y;
        }
    }
    delete[] BlacLineLengths;
    return minLinePosition - firstLine;
}
//--------------------------------------------------------------------------------------------------
int FindGrainHeighOnGray(cv::Mat ImGray, unsigned char threshold)
{
    int maxX = ImGray.cols;
    int maxY = ImGray.rows;
    if(maxX == 0 || maxY ==0)
        return -1;

    if(!ImGray.isContinuous())
        return -2;

    unsigned char *wImGray = (unsigned char*)ImGray.data;
    int *BlacLineLengths = new int[maxY];
    int firstLine = -1;
    int firstBlackPixelOnEdgePos = -1;
    bool firstLineFound = false;

    for (int y = 0; y < maxY; y++)
    {
        int sumLineBlackPixels = 0;
        for (int x = 0; x < maxX; x++)
        {
            if(*wImGray <= threshold)
            {
                sumLineBlackPixels ++;
                if(x == 0 && firstBlackPixelOnEdgePos < y)
                    firstBlackPixelOnEdgePos = y;
            }
            wImGray++;
        }
        BlacLineLengths[y] = sumLineBlackPixels;
        if(sumLineBlackPixels && !firstLineFound)
        {
            firstLine = y;
            firstLineFound = true;
        }
        if(firstBlackPixelOnEdgePos>=0)
        {
            break;
        }

    }
    if(!firstLineFound)
        return -3;
    if(firstBlackPixelOnEdgePos == -1)
        return -4;

    int start = firstBlackPixelOnEdgePos - (firstBlackPixelOnEdgePos - firstLine) * 2 / 3;

    int minLineLength = maxX;
    int minLinePosition = 0;
    for(int y = start; y<firstBlackPixelOnEdgePos; y++)
    {
        if(minLineLength > BlacLineLengths[y])
        {
            minLineLength = BlacLineLengths[y];
            minLinePosition = y;
        }
    }
    delete[] BlacLineLengths;
    return minLinePosition - firstLine;
}
//--------------------------------------------------------------------------------------------------
