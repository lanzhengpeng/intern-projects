### 【0341】ask points from file 从文件中获取点

#### 代码

```cpp
    static int allocate_memory(unsigned int nbytes, void **where)  
    {  
        int  
            resp;  
        *where = UF_allocate_memory(nbytes, &resp);  
        return resp;  
    }  
    static int ask_points_from_file(char *fspec, double (**points)[3])  
    {  
        int  
            ii = 0,  
            n_points = 0;  
        char  
            a_line[MAX_LINE_SIZE+1];  
        FILE  
            *the_file;  
        if ((the_file = fopen(fspec, "r")) != NULL)  
        {  
            while ((fgets(a_line, MAX_LINE_SIZE, the_file)) != NULL) n_points++;  
            rewind(the_file);  
            UF_CALL(allocate_memory(n_points * sizeof(double[3]), (void **)points));  
            for (ii = 0; ii < n_points; ii++)  
            {  
                fscanf(the_file, "%lf %lf %lf",  
                    &(*points)[ii][0], &(*points)[ii][1], &(*points)[ii][2]);  
            }  
            fclose(the_file);  
        }  
        return n_points;  
    }

```

#### 代码解析

> 这是NX Open C++的二次开发代码，主要实现了以下功能：
>
> 1. allocate_memory函数：用于动态分配指定大小的内存空间，并返回指向分配内存的指针。
> 2. ask_points_from_file函数：从指定文件中读取点坐标，并按行读取点坐标，然后动态分配内存以存储所有点的坐标，最后将点的坐标读入到分配的内存空间中。
> 3. 代码首先打开指定的文件，并按行读取文件，统计文件中的点数。
> 4. 然后调用allocate_memory函数，根据点数动态分配内存空间以存储所有点的坐标。
> 5. 最后，按行读取文件，将每行的三个浮点数坐标值读取到动态分配的内存空间中，从而完成点的坐标读取。
> 6. 读取完成后，关闭文件并返回点数。
>
> 这段代码实现了从文件中读取点坐标的功能，并提供了动态内存分配的支持，可以方便地读取大量点坐标。
>
