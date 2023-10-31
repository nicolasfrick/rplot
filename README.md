# rplot
ROS package providing basic plotting functionality via ROOT-framework. 

![plot]([/out/plot.png =100x100](https://github.com/nicolasfrick/rplot/blob/main/out/plot.png)) 
<!-- ![smiley](smiley.png){:height="36px" width="36px"} -->

## Install
Download the [CERN Root Framework](https://root.cern) [pre-compiled binary distribution](https://root.cern/install/#download-a-pre-compiled-binary-distribution) to your '$HOME' folder and install required dependencies like described.

Download the rplot package and build the workspace.

```bash
    $ cd catkin_ws/src
    $ git clone  git clone git@github.com:nicolasfrick/rplot.git
    $ cd .. && catkin build
```

##Use
Include the rplot package to your project, instantiate a MainFrame, add data and plot either to pdf file or to interactive window.

```
    #include "rplot.h"
    rplot::MainFrame mf;
    for (size_t idx = 0; idx < 100; idx++)
        mf.addData1(idx, sin(idx*0.1), 2*cos(idx*0.2), 0.5*cos(idx*3));
    mf.savePdf(true, true, true, false, "plot.pdf");
```

Tested for Ubuntu 20.04 and ROS-noetic.
