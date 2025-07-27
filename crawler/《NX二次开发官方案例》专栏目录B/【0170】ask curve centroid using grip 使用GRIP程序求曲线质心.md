### 【0170】ask curve centroid using grip 使用GRIP程序求曲线质心

#### 代码

```cpp
    /*  
        The function UF_CURVE_ask_centroid does not work correctly.  See PR  
        6551040.  This function can be used as a work around.  
        The source for GRIP program ask_centroid.grs looks like this:  
            ENTITY/ curve, temp_b  
            NUMBER/ a_res(32), xbar, ybar  
            UFARGS/ curve, xbar, ybar  
            temp_b = BOUND/curve  
            ANLSIS/TWOD, temp_b, INCHES, a_res  
            xbar = a_res(3)  
            ybar = a_res(4)  
            DELETE/temp_b  
            HALT  
        Put the ask_centroid.grx in any application folder, e.g.  
        UGII_USER_DIR\application\ask_centroid.grx  
    */  
    static void ask_curve_centroid(tag_t curve, double twod_centroid[2])  
    {  
        UF_args_t  
            args[3];  
        char  
            *path = NULL;  
        args[0].type = UF_TYPE_TAG_T;  
        args[0].length = 0;  
        args[0].address = &curve;  
        args[1].type = UF_TYPE_DOUBLE;  
        args[1].length = 0;  
        args[1].address = &twod_centroid[0];  
        args[2].type = UF_TYPE_DOUBLE;  
        args[2].length = 0;  
        args[2].address = &twod_centroid[1];  
        UF_CALL(UF_find_file("application", "ask_centroid.grx", &path));  
        if (path)  
        {  
            UF_CALL(UF_call_grip(path, 3, args));  
            UF_free(path);  
        }  
        else  
        {  
            ECHO("Cannot find application\ask_centroid.grx\n");  
        }  
    }

```

#### 代码解析

> 这段代码定义了一个函数ask_curve_centroid，用于计算NX中曲线的质心坐标。代码的主要功能如下：
>
> 1. 函数接收一个曲线的tag和一个double数组作为参数，用于存储计算得到的质心坐标。
> 2. 定义了一个UF_args_t类型的数组args，长度为3，用于存储传递给GRIP程序的参数。
> 3. 使用UF_find_file函数在application文件夹下查找名为ask_centroid.grx的GRIP程序，并获取其路径。
> 4. 如果找到GRIP程序，则调用UF_call_grip函数，传入GRIP程序的路径和参数数组，执行GRIP程序计算曲线的质心坐标，并将结果存储在传入的double数组中。
> 5. 如果没有找到GRIP程序，则输出错误信息。
> 6. GRIP程序ask_centroid.grx的源码使用实体分析(ANLSIS)命令计算曲线的质心坐标，并返回结果。
> 7. 这段代码提供了一个工作区，用于解决NX内置的UF_CURVE_ask_centroid函数在某些情况下计算结果不正确的问题。
>
> 综上所述，这段代码通过调用GRIP程序来获取曲线的质心坐标，是一个有效的NX二次开发解决方案。
>
