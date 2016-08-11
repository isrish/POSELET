#include <iostream>
#include <vector>
#include <boost/gil/gil_all.hpp>
#include <boost/gil/extension/io/jpeg_io.hpp>
#include "util.h"
#include <boost/bind.hpp>
#include <boost/timer.hpp>
#include <boost/format.hpp>
#include <boost/ref.hpp>

#include "poselet_api.h"
#include "gil_draw.h"

#include <sys/types.h>

#include <stdio.h>
#include <fstream>
#include <sstream>
#include <string>
using namespace std;
using namespace boost::gil;
using namespace poselet_api;


template <typename Value>
void append_hit(const Value& oh, vector<Value>& hits) {
    hits.push_back(oh);
}


bool file_exists(const char *filename)
{
    ifstream ifile(filename);
    return !!ifile;
}

int main( int argc, char* argv[] ) {
    if (argc<3) {
        cout << "Usage: "<<argv[0]
             <<" <detector.xml> <image.jpg>"<<endl;
        return 0;
    }
    if (!file_exists(argv[1])) {
        cout << "Cannot open model file "<<argv[1]<<endl;
        return 0;
    }
    if (!file_exists(argv[2])) {
        cout << "Cannot open image file "<<argv[2]<<endl;
        return 0;
    }

    bgr8_image_t img;
    jpeg_read_image(argv[2],img);

    string filename(argv[1]);
    size_t nch = filename.find_last_of("/\\");
    string dir, modelname;
    if (nch == string::npos) {
        dir = ".";
        nch = -1;
    } else {
        dir = filename.substr(0, nch);
    }
    modelname = filename.substr(nch+1,filename.length()-nch-5);
    cout << "Dir: "<<dir<<" model:"<<modelname<<endl;

    InitDetector(dir.c_str(), modelname.c_str(), true);

    boost::timer t;

    Image img_proxy(
                img.width(), img.height(), const_view(img).pixels().row_size(),
                Image::k8Bit, Image::kRGB, interleaved_view_get_raw_data(const_view(img)));

    vector<ObjectHit> object_hits;
    vector<PoseletHit> poselet_hits;

    RunDetector(img_proxy, boost::bind(append_hit<PoseletHit>, _1,
                                       boost::ref(poselet_hits)), boost::bind(append_hit<ObjectHit>, _1,
                                                                              boost::ref(object_hits)), true, 0, false);

    double scoresum=0;
    for (size_t i=0; i<object_hits.size(); ++i) {
        scoresum+=object_hits[i].score;
    }

    cout << "Found "<<object_hits.size()<<" objects with scoresum "<<scoresum
         << " in "<<t.elapsed()<<" sec."<<endl;
    return 0;
}
