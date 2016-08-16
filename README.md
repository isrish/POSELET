#Poselets: Body Part Detectors Trained Using 3D Human Pose Annotations

This repository contains c++ code from the following paper.
> Bourdev, Lubomir, and Jitendra Malik. "Lubomir Bourdev, Subhransu Maji, Thomas Brox, Jitendra Malik,Detecting People Using Mutually Consistent Poselet Activations, ECCV 2010"

I'm using it to detect persons in an image. It can be used to detect other object (check the authors <a href="https://www2.eecs.berkeley.edu/Research/Projects/CS/vision/shape/poselets/">web</a> page for more pert-rained detector models). 

## Add-ons
<ul>
 <li>Automatically process different type of input streams. Uses OpenCV to read either an image or a video file. That is to say any image or video format OpenCV readable can be passed as in input.</li>
 <li>Option to visualize detection results.</li>
 <li>Option to dumped detection results to a text (csv) file. This will be helpfull if you want to get the detection bounding boxes in csv file so that you can process it at later.</li>
</ul>

## Usage
First download the source. Here is how to compile. Boost and OpenCV libraries are required.
```shell
 $ mkdir build
 $ cd build
 $ cmake ..
 $ make  
```
 Excutable will be in the bin folder.
 ```shell
 $ ./poseletdetection --help # displays the help message
 $ ./poseletdetection -m person.xml --input path_to_yourimage_or_video.file --plot 1 --out out_csv_file.txt --thr 10.5 --ver 1
 ```

