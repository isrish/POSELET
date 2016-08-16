#include <util.h>



cv::Mat getOpenCVImageView(image_t input_image)
{
  const boost::gil::bgr8_view_t input_view = boost::gil::const_view(input_image);
  const boost::gil::opencv::ipl_image_wrapper input_ipl = boost::gil::opencv::create_ipl_image(input_view);
  cv::Mat img(input_ipl.get());
  return img.clone();
}


image_t getGILImageView(cv::Mat input_image)
{
  const view_t the_view = boost::gil::interleaved_view(input_image.cols, input_image.rows,
                                                       reinterpret_cast<boost::gil::bgr8_pixel_t*>(input_image.data),
                                                       static_cast<size_t>(input_image.step));
  image_t ret;
  ret.recreate(the_view.dimensions());
  boost::gil::copy_pixels(the_view, boost::gil::view(ret));
  return ret;
}



bool file_exists(const char *filename)
{
  struct stat buffer;
  return (stat (filename, &buffer) == 0);
  // std::ifstream ifile(filename);
  // return !!ifile;
}

void getdir_filename(std::string input, std::string &dir, std::string &filename, std::string &ext)
{
  size_t nch = input.find_last_of("/\\");
  std::string base_filename = input.substr(nch + 1);
  std::string::size_type const p(base_filename.find_last_of('.'));
  filename = base_filename.substr(0, p);
  ext = base_filename.substr(p+1);
  if (nch == std::string::npos)
    {
      dir = ".";
      nch = -1;
    }
  else
    {
      dir = input.substr(0, nch);
    }
}

cv::Scalar get_color(int id)
{
  cv::Scalar color = cv::Scalar(((id * 120) % 256), ((id * 60) % 256), ((id * 30) % 256));
  return color;
}

cv::Mat drawObjectbb(cv::Mat img, std::vector<poselet_api::ObjectHit> object_hits,double scorethr)
{
  cv::Mat ret = img.clone();
  int thickline = floor(img.cols/250.);
  double alpha = 0.3;
  cv::Rect imgsize(0,0,img.cols, img.rows);
  for(int i=0; i<object_hits.size();++i)
    {
      if(object_hits[i].score >= scorethr)
        {
          cv::Rect bbs(object_hits[i].x0, object_hits[i].y0,object_hits[i].width,object_hits[i].height);
          //std::cout<<bbs<<"--->";
          if(bbs.x +  bbs.width >= img.cols)
            bbs.width = img.cols- bbs.x - 15.0;
          if(bbs.x<=0)
            bbs.x = 15.0;
          if(bbs.y +  bbs.height >= img.rows)
            bbs.height = img.rows- bbs.y- 15.0;
          if(bbs.y<=0)
            bbs.y = 15.0;
          if(IsInside(bbs,imgsize))
            {
              //std::cout<<bbs<<std::endl;
              cv::Mat roi = ret(bbs);
              cv::Mat color(roi.size(), CV_8UC3,get_color(i));
              cv::addWeighted(color, alpha, roi, 1.0 - alpha , 0.0, roi);
              std::ostringstream text;
              text << "["<<object_hits[i].score << "]";
              cv::rectangle(ret,bbs,get_color(i),thickline);
              setLabel(ret,text.str(), bbs.tl());
            }
        }

    }
  return ret;

}

cv::Mat drawObjectbb(cv::Mat img, std::vector<cv::Rect> object_hits, std::vector<double> score_hit)
{
  cv::Mat ret = img.clone();
  cv::Rect imgsize(0,0,img.cols,img.rows);
  int thickline = floor(img.cols/250.);
  double alpha = 0.3;
  for(int i=0; i<object_hits.size();++i)
    {
      cv::Rect bbs = object_hits[i];
      if(bbs.x +  bbs.width >= img.cols)
        bbs.width = img.cols- bbs.x - 15.0;
      if(bbs.x<=0)
        bbs.x = 15.0;
      if(bbs.y +  bbs.height >= img.rows)
        bbs.height = img.rows- bbs.y- 15.0;
      if(bbs.y<=0)
        bbs.y = 15.0;
      if(IsInside(bbs,imgsize))
        {
          cv::Mat roi = ret(bbs);
          cv::Mat color(roi.size(), CV_8UC3,get_color(i));
          cv::addWeighted(color, alpha, roi, 1.0 - alpha , 0.0, roi);
          std::ostringstream text;
          text << "["<<score_hit[i]<< "]";
          cv::rectangle(ret,bbs,get_color(i),thickline);
          setLabel(ret,text.str(), bbs.tl());
        }
    }
  return ret;
}


void getBBfromObjectHits(std::vector<poselet_api::ObjectHit> object_hits,double scorethr,cv::Rect imgsize,std::vector<cv::Rect>&bb_out,std::vector<double>&scores_out)
{
  bb_out.clear();
  scores_out.clear();
  for(int i=0; i<object_hits.size();++i)
    {
      if(object_hits[i].score >= scorethr)
        {
          cv::Rect bbs(object_hits[i].x0, object_hits[i].y0,object_hits[i].width,object_hits[i].height);
          if(bbs.x +  bbs.width >= imgsize.width)
            bbs.width = imgsize.width- bbs.x - 15.0;
          if(bbs.x<=0)
            bbs.x = 15.0;
          if(bbs.y +  bbs.height >= imgsize.width)
            bbs.height = imgsize.height- bbs.y- 15.0;
          if(bbs.y<=0)
            bbs.y = 15.0;
          if(IsInside(bbs,imgsize))
            {
              bb_out.push_back(bbs);
              scores_out.push_back(object_hits[i].score);
            }
        }
    }
}

void setLabel(cv::Mat &im, const std::string &name, const cv::Point2f &og)
{
  int face[] = {cv::FONT_HERSHEY_SIMPLEX, cv::FONT_HERSHEY_PLAIN, cv::FONT_HERSHEY_DUPLEX, cv::FONT_HERSHEY_COMPLEX,
                cv::FONT_HERSHEY_TRIPLEX, cv::FONT_HERSHEY_COMPLEX_SMALL, cv::FONT_HERSHEY_SCRIPT_SIMPLEX,
                cv::FONT_HERSHEY_SCRIPT_COMPLEX, cv::FONT_ITALIC};
  int fontface = face[0];
  double scale = 0.3;
  int thickness = 1;
  int baseline = 0.1;

  cv::Size text = cv::getTextSize(name, fontface, scale, thickness, &baseline);
  cv::rectangle(im, og + cv::Point2f(0, baseline), og + cv::Point2f(text.width, -text.height), CV_RGB(0,0,0), CV_FILLED);
  cv::putText(im, name, og, fontface, scale, CV_RGB(255,255,0), thickness, CV_AA);

}

// check if a is inside B
bool IsInside(const cv::Rect a, const cv::Rect b)
{
  bool valid_size = (a.x>=0) && (a.y>=0) && (a.width>=1) && (a.height>=1);
  return valid_size && (a.x>=b.x) && (a.y>=b.y) && ((a.x + a.width)<=(b.x+b.width)) && ((a.y + a.height)<=(b.y + b.height));
}


namespace po = boost::program_options;

std::ostream& operator <<(std::ostream &o, const params &p)
{
  std::string dir, modelname,ext,ext1;
  getdir_filename(p.fn_model, dir, modelname, ext);
  switch (p.isplot)
    {
    case 1:
      ext ="Yes and no save";
      break;
    case 2:
      ext = "Yes and Save";
      break;
    case 3:
      ext = "Just save";
      break;
    case 0:
    default:
      ext = "No";
      break;
    }
  ext1 = "No";
  if(p.out_txt_filename.length()>0)
    {
      ext1 = "Yes";
    }
  o<< "------------------\nModel Directory: "<<dir<<"\nModel:"<<modelname<<"\n"<<"Input type: "<<filetypes[p.intype]<<"\n"
   <<"Plot result: "<<ext<<"\n"<<"Save csv file: "<<ext1<<"\n------------------\n";
  return o;
}


params parse_argument(int argc, char **argv)
{
  po::options_description desc("POSELET Detection allowed options");
  desc.add_options()
      ("help,h","Produce this help message.")
      ("model,m", po::value<std::string>()->default_value("../model/person.xml"), "Full file path to the model file")
      ("input,i", po::value<std::string>(), "File path to input image or video")
      ("thr,t", po::value<double>()->default_value(5.1), "Detection scores threshold. Detection lower than this value are discarded.")
      ("plot,p", po::value<int>()->default_value(0), "show detection results, [ 0: no show, 1: show, 2: show and save as jpg , 3: just save ]")
      ("out,o", po::value<std::string>()->default_value("detections.txt"), "save the detection bounding box results to a text (csv) file")
      ("ver,v",po::value<int>()->default_value(0),"verbosity");
  po::variables_map vm;
  po::store(po::parse_command_line(argc,argv,desc),vm);
  try
  {
    po::notify(vm);
  }
  catch(po::error& e)
  {
    plog.messege("Error: " + std::string(e.what()), 0, MessegeType::Critical);
    exit(1);
  }
  if(vm.count("help"))
    {
      std::ostringstream tmp;
      desc.print(tmp);
      plog.messege(tmp.str(),false,Info);
      std::string usg = "Usage: " + std::string(argv[0]) +" -m <detector.xml> -i <image.jpg> -p 1 -d 0";
      plog.messege(usg,false,Info);
      exit(1);
    }
  params ret;
  ret.fn_model = vm["model"].as<std::string>();
  if(!vm.count("input"))
    {
      plog.messege("Input file can not be empty! Pass an image or a video file.",false,MessegeType::Critical);
      std::string usg = "Usage: " + std::string(argv[0]) +" -m <detector.xml> -i <image.jpg> -p 1 -d 0";
      plog.messege(usg,false,Info);
      exit(1);
    }
  else
    {
      ret.fn_input = vm["input"].as<std::string>();
      ret.intype = checkInputType(ret.fn_input);
      if (ret.intype == FILETYPE::UNKNOWN)
        {
          plog.messege("Unknow Input file. File can not be read! Make sure " + ret.fn_input + " is a valid image or video file.",false,MessegeType::Critical);
          exit(1);
        }
    }
  ret.detection_threshold = vm["thr"].as<double>();
  ret.isplot = vm["plot"].as<int>();
  ret.out_txt_filename = vm["out"].as<std::string>();
  ret.verbose = vm["ver"].as<int>();
  return ret;
}



FILETYPE checkInputType(std::string fn)
{
  magic_t m = magic_open(MAGIC_CONTINUE|MAGIC_ERROR/*|MAGIC_DEBUG*/|MAGIC_MIME);
  magic_load(m,NULL);
  std::string response(magic_file(m,fn.c_str()));
  magic_close(m);
  if(response.find("image") != std::string::npos)
    {
      return FILETYPE::IMAGE;
    }
  else if(response.find("video") !=std::string::npos)
    {
      return FILETYPE::VIDEO;
    }
  return FILETYPE::UNKNOWN;
}
