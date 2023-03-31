banker's algorithm for deadlock detection
============================

The two submodules are both excellent implementations but neither of them is perfect. Currently, I can't find a good c++ generic implementation of balanced kdTree supporting deletion and insertion.

### ikd-Tree


Initially, I want to use [ikd-Tree](https://github.com/hku-mars/ikd-Tree) for a balanced persistent Kd-tree.

https://arxiv.org/abs/2102.10808

ikd-Tree uses [libpcl](https://github.com/PointCloudLibrary/pcl).

There is also a kdtree implementation in libpcl: https://pointclouds.org/documentation/group__kdtree.html

However, this implementation only supports 3D. It is confusing that this implementation uses a template para `PointType` which seems to support points with any dimension without a careful look into the source code or paper.

### kdtree-cpp

https://github.com/cdalitz/kdtree-cpp

This implementation doesn't support adding or removing points. 

I made some modification to the src for unbalanced deletion.(only deletion is needed in my application and no need to keep it balanced)