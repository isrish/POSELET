#include "resultwriter.h"


bool  boxreadwriter::writebb(const int &frm, const std::vector<cv::Rect>&rt,const std::vector<int> &tid)
{
  bool ret = false;
  if(file_.is_open())
    {
      if(tid.empty())
        {
          for(size_t i =0; i<rt.size();i++)
            {
              file_<<frm <<", " <<-1<<", "<< rt[i].x <<", " << rt[i].y <<", " << rt[i].width <<", " << rt[i].height <<", "<<0.0<<", "<<-1 <<", "<<-1<<", "<<-1<<"\n";
            }
        }
      else
        {
          for(size_t i =0; i<rt.size();i++)
            {
              file_<<frm <<", " <<tid[i]<<", "<< rt[i].x <<", " << rt[i].y <<", " << rt[i].width <<", " << rt[i].height <<", "<<0.0<<", "<<-1 <<", "<<-1<<", "<<-1<<"\n";
            }
        }
      file_.flush();
      ret = true;
    }
  else
    {
      Log::messege("can not write",MessegeType::Error);
    }
  return ret;
}

bool  boxreadwriter::writebb(const int &frm, const std::vector<cv::Rect>&rt,const std::vector<double> &score)
{
  bool ret = false;
  if(file_.is_open())
    {
      for(size_t i =0; i<rt.size();i++)
        {
          file_<<frm <<", " <<-1<<", "<< rt[i].x <<", " << rt[i].y <<", " << rt[i].width <<", " << rt[i].height <<", "<<score[i]<<", "<<-1 <<", "<<-1<<", "<<-1<<"\n";
        }
      file_.flush();
      ret = true;
    }
  else
    {
      Log::messege("can not write",MessegeType::Error);
    }
  return ret;
}




