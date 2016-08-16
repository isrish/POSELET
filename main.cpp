#include <iostream>
#include <vector>
#include <boost/gil/gil_all.hpp>
#include <boost/bind.hpp>
#include <boost/timer.hpp>
#include <boost/format.hpp>
#include <boost/ref.hpp>
#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <sys/types.h>
#include <stdio.h>
#include <fstream>
#include <sstream>
#include <string>

#include "poselet_api.h"
#include "util.h"
#include "resultwriter.h"
#include "logging.h"


using namespace std;
using namespace boost::gil;
using namespace poselet_api;


Log plog("POSELET DETECTION");



template <typename Value>
void append_hit(const Value& oh, std::vector<Value>& hits);

int main( int argc, char* argv[] )
{
  params argins = parse_argument(argc,argv);
  if (!file_exists(argins.fn_model.c_str()))
    {
      plog.messege("Cannot open model file: " + argins.fn_model, 0, MessegeType::Critical);
      return 0;
    }
  if (!file_exists(argins.fn_input.c_str()))
    {
      plog.messege("Cannot open image/video file: " + argins.fn_input, 0, MessegeType::Critical);
      return 0;
    }

  if(argins.verbose > 0)
    std::cout<<argins<<std::endl;

  string modeldir, modelname, inputfiledir, inputfilename, ext;
  bgr8_image_t img;
  cv::Mat cvimg,result;
  boost::timer t;
  vector<ObjectHit> object_hits;
  vector<PoseletHit> poselet_hits;

  getdir_filename(argins.fn_model, modeldir, modelname, ext);
  getdir_filename(argins.fn_input, inputfiledir, inputfilename, ext);
  InitDetector(modeldir.c_str(), modelname.c_str(), true);

  std::string outfn;
  if(argins.isplot==2 || argins.isplot==3)
    {
      outfn = inputfiledir + separator();
      if(argins.intype==FILETYPE::VIDEO)
        {
          outfn  = outfn + "result" + separator();
          boost::filesystem::path dir(outfn);
          if(!(boost::filesystem::create_directories(dir)))
            {
              plog.messege("Can not write output directory! " + outfn, false, MessegeType::Warning);
            }
        }
      else if(argins.intype==FILETYPE::IMAGE)
        {
          outfn = outfn + inputfilename + "-detection.";
        }
    }

  // this write the detection results to a csv file
  std::string outfiledir = inputfiledir + separator();
  boxreadwriter  write_(outfiledir,argins.out_txt_filename,writer);
  int frame_cnt = 0;
  std::vector<cv::Rect> resultBBs;
  std::vector<double> scores;

  if(argins.intype==FILETYPE::VIDEO)
    {
      cv::VideoCapture vid;
      if(!(vid.open(argins.fn_input)))
        {
          plog.messege("Error:opening video file file!",false,MessegeType::Critical);
          return 0;
        }
      while(vid.read(cvimg))
        {
          plog.messege("Frame:" + std::to_string(frame_cnt),true,MessegeType::Others);
          img = getGILImageView(cvimg);
          // clear
          poselet_hits.clear();
          object_hits.clear();
          // do detection
          Image img_proxy(img.width(), img.height(), const_view(img).pixels().row_size(),Image::k8Bit, Image::kRGB, interleaved_view_get_raw_data(const_view(img)));
          RunDetector(img_proxy, boost::bind(append_hit<PoseletHit>, _1,boost::ref(poselet_hits)), boost::bind(append_hit<ObjectHit>, _1,boost::ref(object_hits)), true, 0, false);
          // get detection bouding boxes with threshold score and limit bounding boxes to be inside the image.
          getBBfromObjectHits(object_hits,argins.detection_threshold,cv::Rect(0,0,cvimg.cols,cvimg.rows),resultBBs,scores);
          if(argins.isplot>0)
            {
              result = drawObjectbb(cvimg,resultBBs,scores);
              if (argins.isplot==1 || argins.isplot==2 )
                {
                  cv::imshow("Detection-Result", result);
                  cv::moveWindow("Detection-Result", 10, 10);
                  if(cv::waitKey(1) == 27)
                    break;
                }
              if(argins.isplot==2 || argins.isplot==3)
                {
                  std::string tmp_fn;
                  try
                  {
                    char tbuf[20];
                    std::sprintf(tbuf, "Img-%06d.png", frame_cnt);
                    tmp_fn = outfn + string(tbuf);
                    cv::imwrite(tmp_fn, result);
                  }
                  catch(std::exception &e)
                  {
                    plog.messege("Error: " + std::string(e.what()) + tmp_fn,true, MessegeType::Error);
                  }
                }
            }
          if(!argins.out_txt_filename.empty())
            {
              write_.writebb(frame_cnt,resultBBs,scores);
            }
          frame_cnt++;
        }
    }
  else if(argins.intype==FILETYPE::IMAGE)
    {
      cvimg = cv::imread(argins.fn_input);
      img = getGILImageView(cvimg);

      // run detector
      Image img_proxy(img.width(), img.height(), const_view(img).pixels().row_size(),Image::k8Bit, Image::kRGB, interleaved_view_get_raw_data(const_view(img)));
      RunDetector(img_proxy, boost::bind(append_hit<PoseletHit>, _1,boost::ref(poselet_hits)), boost::bind(append_hit<ObjectHit>, _1,boost::ref(object_hits)), true, 0, false);
      // get bounding box
      getBBfromObjectHits(object_hits,argins.detection_threshold,cv::Rect(0,0,cvimg.cols,cvimg.rows),resultBBs,scores);
      // save/plot results
      if(argins.isplot>0)
        {
          result = drawObjectbb(cvimg,resultBBs,scores);
          if (argins.isplot==1 || argins.isplot==2 )
            {
              cv::imshow("Detection-Result", result);
              cv::moveWindow("Detection-Result", 10, 10);
              cv::waitKey(0);
            }
          if(argins.isplot==2 || argins.isplot==3)
            {
              cv::imwrite(outfn + ext, result);
            }
        }

      if(argins.verbose > 0)
        {
          double scoresum=0;
          for (size_t i=0; i<object_hits.size(); ++i)
            {
              scoresum+=object_hits[i].score;
            }
          std::string msg = "Found " + std::to_string(object_hits.size()) + " objects with scoresum "_RED + std::to_string(scoresum)+_RESET " in "_RED + std::to_string(t.elapsed())+ _RESET" sec.";
          plog.messege(msg,false, MessegeType::Info);
        }
      if(!argins.out_txt_filename.empty())
        {
          write_.writebb(frame_cnt,resultBBs,scores);
        }
    }
  return 0;
}

template <typename Value>
void append_hit(const Value& oh, std::vector<Value>& hits)
{
  hits.push_back(oh);
}
