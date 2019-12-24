Overall implementing parallelism with pthread_t was not that difficult with this program because it is embarrisngly parallel so aside from a few decisions and problems I had to solve it 
was mostly a plug and chug problem.  

One of the decisions I had to make was how to break up the picture using my parallel threads.  One solution I considered was breaking up the picture into squares but there are many
problems with this solution with the current implementation of the code.  One problem was that depending on the number of threads it could be difficult to break up the picture neatly into
squares without most of them quickly turning into strangley shaped rectangles with more threads doing a lot more work than others.

I ultimatley decided to break up the image into vertical strips.  I didn't do horizontal stips simply because the code iterated over pixels with the outermost loop being the x coordinate
and I thought keeping my changes on the edges of the code would be easier for me to work with.  each thread is allocated a certain range of x coordinates and has to process all coordinates
x,y in that range where y is from 0 to height.  If there are some extra pixels left that were not included because of rounding when allocating x coordinate ranges, the last thread
will do some extra work to polish those edges.  

Another problem I had was implementing the scaled_color part of the code with parallelism.  I had to redefine the float array as a multidimensional array which allowed me to make changes to 
individual pixels on each loop of every thread.

I also briefly had some problems with pointers where I was getting segmentation faults because I was incrementing incorrectly and would try to change pixels that didn't exist.  I got 
segmentation errors when I tried to pass an argument to my parallel function using pthread_create by casting an int to void* but that was fixed by creating an array of ints and referencing
the array instead.

I moved several variable definitions outside of main so that my parallel function could use them as well. 

Analyzing SRT performance.

time ./srt 1-test.ppm >1-test.ppm.tmp
real    0m41.288s
user    0m41.275s
sys     0m0.005s

time ./srt 2-test.ppm >2-test.ppm.tmp
real    0m21.362s
user    0m41.663s
sys     0m0.004s

time ./srt 4-test.ppm >4-test.ppm.tmp
real    0m14.717s
user    0m42.861s
sys     0m0.003s

time ./srt 8-test.ppm >8-test.ppm.tmp
real    0m9.263s
user    0m44.095s
sys     0m0.005s

An observation of the data above shows that SRT improved performance very quickly when adding more threads to help process the pixels.  Going from 1 to 2 threads, the time it took to
complete the program halved.  However, the performance gain seen by doubling the number of threads to 4 and then to 8 was not as significant.  

Although more threads does continue to improve the performance of the program, one would assume there is a cap to the the efficiency of the program with parallelism alone.  At that point, 
programmers looking to improve performance should try cleaning up their code and removing any redundant code or replacing it with something better.

There is also a cost benefit analysis to be done when deciding how many threads to allocated to a program.  More threads will increase the program's performance but additional threads
will also give slightly less of a performance boost than previous threads, so programmers should balance the number of threads a single program is using with the demands of other 
programs to maximize performance.  