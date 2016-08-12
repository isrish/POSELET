#Poselets: Body Part Detectors Trained Using 3D Human Pose Annotations

This repository contains c++ code from the following paper.
> Bourdev, Lubomir, and Jitendra Malik. "Poselets: Body part detectors trained using 3D human pose annotations." In 2009 IEEE 12th International Conference on Computer Vision, pp. 1365-1372. IEEE, 2009.

I'm using it to detect person in an image. It can be used to detect other object (check the authors <a href="https://www2.eecs.berkeley.edu/Research/Projects/CS/vision/shape/poselets/">web</a> page for more detection models). 

## Add-ons
<ul>
 <li>Automatically process different type of input streams. Uses OpenCV to read either an image and a video file. This is to say any image or video formate OpenCV readable can be passed as in input.</li>
 <li>Option to visulaize detection results </li>
 <li>Option to dumped detection results to a text (csv) file. This will be helpfull if you want to get the detection bounding boxes in csv file so that you can later process it.</li>
</ul>

## Usage
TODO
First compile the code. It requires boost c++ libraries.
```shell
 $ mkdir build
 $ cd build
 $ cmake ..
 $ make 
```
 Excutable will be in the bin folder.
 ```shell
 $ ./poseletdetection -h # displays the help message
 $ ./poseletdetection ../model/person.xml path_to_yourimage.or.video -v 1 -s 1 -p 2 -t 10
 ```

