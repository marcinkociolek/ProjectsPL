#include <opencv2/core/core.hpp>
#include "heightfromsideimage.h"

//--------------------------------------------------------------------------------------------------
int FindGrainHeighOnBRG(cv::Mat ImBGR, float threshold)
{
    int maxX = ImBGR.cols;
    int maxY = ImBGR.rows;
    if(maxX == 0 || maxY ==0)
        return 0;

    if(!ImBGR.isContinuous())
        return 0;

    unsigned char *wImBGR = (unsigned char*)ImBGR.data;
    int *BlacLineLengths = new int[maxY];
    int firstLine = 0;
    int firstBlackPixelOnEdgePos = 0;
    bool firstLineFound = false;

    for (int y = 0; y < maxY; y++)
    {
        int sumLineBlackPixels = 0;
        for (int x = 0; x < maxX; x++)
        {
            float brightness ;
            brightness = (float)*wImBGR * 0.114;
            wImBGR++;
            brightness += (float)*wImBGR * 0.587;
            wImBGR++;
            brightness += (float)*wImBGR * 0.299;
            wImBGR++;

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
        if(firstBlackPixelOnEdgePos)
        {
            break;
        }

    }
    int start = firstBlackPixelOnEdgePos - (firstBlackPixelOnEdgePos - firstLine) *2 / 3;

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
