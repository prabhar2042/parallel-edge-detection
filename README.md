
# Parallelizing the Canny Edge Detection Algorithm using OpenMP and CUDA - Pratik Bharadwaj, Krish Sethi

URL: 

## Summary
We are going to parallelize the process of detecting edges in images using the Canny Edge Detector algorithm. 
This will involve two implementations. 
- An OpenMP version on CPU
- A Cuda version on NVIDIA GPUs 
We will then analyze the speedup provided by each method and provide a comprehensive analysis of the costs and rewards.

## Background

The Canny edge detection algorithm is a popular image processing technique used to detect edges in digital images. The algorithm involves several steps, including Gaussian smoothing, gradient calculation, non-maximum suppression, and thresholding.

First, the image is smoothed using a Gaussian filter to reduce noise and remove small variations in intensity. Then, the gradient magnitude and direction are calculated using a Sobel filter, which helps to identify regions of the image with high gradient values. Next, non-maximum suppression is performed to thin out the edges and produce a single-pixel-wide edge map. Finally, thresholding is applied to the edge map to produce a binary image of the edges.

The Canny edge detection algorithm can benefit from parallelism as many of its steps, such as the Gaussian smoothing and gradient calculation, involve convolutions and matrix operations that can be performed concurrently on multiple processors or threads. Parallel processing can significantly reduce the processing time required for edge detection in large, high-resolution images, making it a valuable technique for real-time computer vision applications such as object detection and tracking.

## The Challenge
The size and type of the input image may vary. Since different parts of the algorithm have different computation requirements, a static division between the compute nodes may lead to workload imbalance and poor performance 

Gaussian Filter and Gradient computation are essentially convolution operations in which the image is convolved with a filter to get the output.  These two steps require a large number of memory accesses. Hence careful analysis of the memory access pattern is required to reduce contention and false sharing. 

The Canny Edge Detection algorithm involves several stages that require synchronization between different processors or threads. Each stage in the Algorithm is dependent on the previous stage, which may require the use of Barriers. The hysteresis, or the edge tracking stage, has many internal dependencies. 	Work done on each pixel is dependent on its neighbors, neighbors of neighbors, and so on.  Additional Synchronization and communication required in this stage can introduce additional overhead and reduce performance and should be carefully managed.

## Resources

We will need a sequential implementation of the edge detection algorithm, as well as access to CUDA and OpenMP libraries in order to implement all parts of our project successfully. 

## Goals and Deliverables
75%
- Implement a sequential version, 
- parallel version on OpenMP 
- parallel version on  CUDA 


100% :  
- Implement a sequential version,
- A parallel version on OpenMP with tunning to get a high speedup (optimized Problem constrained scaling)
- A parallel version using CUDA 
- A comparison between OpenMP and CUDA implementations

125% :  
- Implement a sequential version,
- A parallel version on OpenMP with tunning to get a high speedup (optimized Problem constrained scaling)
- A parallel version on CUDA with tunning to get a high speedup (optimized Problem constrained scaling)
- A comparison between OpenMP and CUDA implementations

## Platform Choice
We believe the combination of the GHC clusters and the C++ language should be enough to meet our needs in this project. This is because C++ already has a lot of support in terms of libraries for OpenMP and CUDA, and thus will be a good choice to code an algorithm that is computationally heavy. 

## Schedule
| Week  | Task | Status |
| ------------- | ------------- | ------------ |
| Week 1  | Review the literature to understand the algorithm better.<br>Setup environment for CUDA and OpenMP  | Complete |
| Week 2  | Implement the sequential version of the algorithm. | Complete |
| Week 3  | Implement the parallel version in OpenMP and tune<br>implementation to achieve the maximum possible speedup. | Complete |
| Week 4  | Implement the parallel version in CUDA and tune<br>the implementation to achieve the maximum possible speedup.  | Complete |
| Week 5  | Benchmark and compare the two implementations  | Complete |
| Week 6  | Write project report and prepare for the poster session.  | Complete |


## Project Writeup

[Writeup](https://linktodocumentation)






