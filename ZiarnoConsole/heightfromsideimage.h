#ifndef HEIGHTFROMSIDEIMAGE_H
#define HEIGHTFROMSIDEIMAGE_H


#include <opencv2/core/core.hpp>

int FindGrainHeighOnBRG(cv::Mat ImBGR, int threshold = 100);

#endif // HEIGHTFROMSIDEIMAGE_H
