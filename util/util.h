#ifndef UTIL_H
#define UTIL_H

#include <sys/stat.h> // to check if file exist or not
#include <magic.h> // to check for file type
#include <boost/gil/gil_all.hpp>
#include <boost/gil/extension/numeric/channel255float.hpp>
#include <boost/gil/extension/opencv/ipl_image_wrapper.hpp>
#include <boost/program_options.hpp>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <numeric>
#include <opencv2/opencv.hpp>
#include "bounds.h"
#include "poselet_api.h"
#include "logging.h"

#ifdef min
#undef min
#endif
#ifdef max
#undef max
#endif

extern Log plog;

// Divides elements of the range by their sum. Assumes non-negative values.
template <typename It> // Models mutable forward iterator.
double l1normalize(It first, It last, double eps=0.00001f)
{
  double sum=std::max<double>(eps, std::accumulate(first, last, 0.0f));
  while (first!=last)
    {
      (*first) = (*first)/sum;
      ++first;
    }
  return sum;
}

//////////////////////////////////////////
// points and bounds
//////////////////////////////////////////

typedef boost::gil::point2<int> int_point;
typedef boost::gil::point2<float> float_point;
typedef boost::gil::point2<double> double_point;

template <typename DstPoint, typename SrcPoint>
DstPoint cast_point(const SrcPoint& p) {
  return DstPoint(typename DstPoint::value_type(p.x),
                  typename DstPoint::value_type(p.y));
}

inline boost::gil::point2<ptrdiff_t> ptrdiff_pt(const int_point& p)
{
  return boost::gil::point2<ptrdiff_t>(p.x,p.y);
}

template <typename DstBounds, typename SrcBounds>
DstBounds cast_bounds(const SrcBounds& b)
{
  return DstBounds(cast_point<typename DstBounds::point_type>(b._min),
                   cast_point<typename DstBounds::point_type>(b._max), false);
}

namespace boost
{
  namespace gil
  {
    template <typename T> GIL_FORCEINLINE
    point2<T> operator/(const point2<T>& p, int t)
    {
      return t==0 ? point2<T>(0,0):point2<T>(p.x/t,p.y/t);
    }

    template <typename T> GIL_FORCEINLINE
    point2<T> operator*(const point2<T>& p, float t)
    {
      return point2<T>(p.x*t,p.y*t);
    }

    template <typename Point1, typename Point2, typename Scalar>
    inline void interpolate_point(const Point2& p2, Scalar w2,
                                  Point1& accum, Scalar w1=1)
    {
      accum.x=accum.x*w1 + p2.x*w2;
      accum.y=accum.y*w1 + p2.y*w2;
    }

    template <typename Point, typename Scalar>
    inline Point get_interpolated_point(const Point& p1, Scalar w1,
                                        const Point& p2, Scalar w2)
    {
      return Point(p1.x*w1 + p2.x*w2, p1.y*w1 + p2.y*w2);
    }


    template <typename Point>
    inline double l2_norm(const Point& p)
    {
      return sqrt(p.x*p.x + p.y*p.y);
    }

    template <typename T> std::ostream& operator<<(std::ostream& os,
                                                   const point2<T>& p)
    {
      return os << p.x <<" "<<p.y<<" ";
    }

    template <typename T> std::istream& operator>>(std::istream& is, point2<T>& p)
    {
      return is >> p.x >> p.y;
    }

  }
}

inline float_bounds operator*(const float_bounds& b, float scale)
{
  return float_bounds(b._min*scale,b._max*scale,true);
}


typedef boost::gil::bgr8_image_t image_t;
typedef boost::gil::bgr8_view_t  view_t;

cv::Mat getOpenCVImageView(image_t input_image);

image_t getGILImageView(cv::Mat input_image);


cv::Scalar get_color(int id);

bool file_exists(const char *filename);
void getdir_filename(std::string input, std::string &dir, std::string &filename, std::string &ext);

bool IsInside(const cv::Rect a, const cv::Rect b);

void setLabel(cv::Mat &im, const std::string &name, const cv::Point2f &og);

cv::Mat drawObjectbb(cv::Mat img, std::vector<poselet_api::ObjectHit>hits,double scorethr=-INFINITY);
cv::Mat drawObjectbb(cv::Mat img, std::vector<cv::Rect> object_hits, std::vector<double> score_hit);
void getBBfromObjectHits(std::vector<poselet_api::ObjectHit> object_hits,double scorethr,cv::Rect imgsize, std::vector<cv::Rect>&bb_out, std::vector<double>&scores_out);


typedef enum
{
  UNKNOWN = 0,
  IMAGE,
  VIDEO,
  FILETYPE_NUM,
} FILETYPE;

const std::string filetypes[] = {"UNKNOWN", "IMAGE","VIDEO"};

FILETYPE checkInputType(std::string fn);


typedef struct params params;
struct params
{
  std::string fn_model;
  std::string fn_input;
  FILETYPE intype; // input type, Video or Image
  int isplot; // 0 no plot, 1 plot, 2:plot and save as jpg
  int verbose; //verbosity on or off
  double detection_threshold; // detection score less than the threshold are discard (false positives)
  std::string out_txt_filename; // file to save the detection in csv format
};


std::ostream& operator <<(std::ostream &o, const params &p);

params parse_argument(int argc, char **argv);


inline std::string separator()
{
#ifdef _WIN32
  return "\\";
#else
  return "/";
#endif
}
#endif
