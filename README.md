#Poselets: Body Part Detectors Trained Using 3D Human Pose Annotations

This repository contains c++ code from the following paper.
> Bourdev, Lubomir, and Jitendra Malik. "Poselets: Body part detectors trained using 3d human pose annotations." In 2009 IEEE 12th International Conference on Computer Vision, pp. 1365-1372. IEEE, 2009.

I'm using it to detect person in an image. It can be used to detect other object (check the authors <a href="https://www2.eecs.berkeley.edu/Research/Projects/CS/vision/shape/poselets/">web</a> page for more detection models). 

## Add-ons
<ul>
 <li>Interace with OpenCV to visulize ouput results. </li>
 <li>Results can be dumped to text (csv) file. </li>
</ul>

## Usage
TODO
First compile the code. It requires boost c++ libraries.
<code>
 $ mkdir build
 $ cd build
 $ cmake ..
 $ make 
</code>
 Excutable will be in the $SRC/bin folder.
 <code>
 $ ./poseletdetection ../model/person.xml path_to_yourimage.jpg -v 1 -s 1
 </code>

'''
