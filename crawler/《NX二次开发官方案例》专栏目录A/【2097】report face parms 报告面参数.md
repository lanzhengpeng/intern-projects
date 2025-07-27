### 【2097】report face parms 报告面参数

#### 代码

```cpp
    static void report_face_parms(tag_t face, double loc[3])  
    {  
        double  
            parms[2],  
            pnt[3];  
        UF_CALL(UF_MODL_ask_face_parm(face, loc, parms, pnt));  
        ECHOF(parms[0]);  
        ECHOF(parms[1]);  
        display_temporary_point(pnt);  
    }

```

#### 代码解析

> 这段代码是一个用于在NX中获取和显示面参数的函数，具体介绍如下：
>
> 1. 函数名为report_face_parms，接受两个参数：面标签face和点数组loc。
> 2. 定义了两个double类型的数组parms和pnt，用于存储面参数和参数点。
> 3. 使用UF_MODL_ask_face_parm函数，通过面标签face和点数组loc，获取该点的面参数和参数点。
> 4. 输出两个面参数到控制台。
> 5. 调用display_temporary_point函数，显示参数点。
> 6. UF_CALL宏用于检查UF函数的返回码，如果有错误会输出错误信息。
> 7. ECHOF宏用于输出double类型的参数。
> 8. display_temporary_point函数需要自行实现，用于在图形窗口中显示参数点。
>
> 总体来说，这个函数通过UF函数获取指定点的面参数和参数点，并输出参数信息以及显示参数点，实现了获取和显示面参数的功能。
>
