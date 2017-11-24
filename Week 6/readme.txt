For this homework, I had a hard time when trying to figure out what was the code in main.c talking about, especially what was a sphereflake. After googling and asking my friends who are taking some graph-making courses, I had some basic understandings. 

At first, I knew we should use thread-create and thread-join functions to attain our goal, but I didn't know where to modify. Then I tried to write those codes in main, and I realize I should move some code chunks out of main, and make them a new function in order to pass it as an arguments in thread-create function. That's how I starded.

Then I went into some troubles of dealing with variables. I forgot to put the scene variable out of the main function as a global variable at the beginning, so my code didn't run and it reported a error. Being fixed, it all seemed smooth.

By examing the ouput which I put into make-log.txt, especially the actual time consuming, we can see a significant inprovement between single thread, which is nearly 50 secs, and 8 threads, which is 6.4 secs. This is mainly because when using multi threads, we can do tasks concurrently, which is the essential of parallelism. 