### 【0209】ask face normalized parms 询问面归一化参数

#### 代码

```cpp
    static void ask_face_normalized_parms(tag_t face, double *pos, double *fparms)  
    {  
        double  
            minmax[4],  
            parms[2];  
        UF_CALL(UF_MODL_ask_face_parm(face, pos, parms, pos));  
        UF_CALL(UF_MODL_ask_face_uv_minmax(face, minmax));  
        fparms[0] = (parms[0] - minmax[0]) / (minmax[1] - minmax[0]);  
        fparms[1] = (parms[1] - minmax[2]) / (minmax[3] - minmax[2]);  
    }

```

#### 代码解析

> 这段代码是用于NX Open C++ API的二次开发代码，其主要功能是获取NX模型中指定面的位置参数和UV参数，并对其进行归一化处理。
>
> 具体来说，代码的主要功能包括：
>
> 1. 获取指定面的位置参数，即该面在模型坐标系中的位置信息，并将其存储在pos数组中。
> 2. 获取指定面的UV参数，即该面在UV空间中的参数信息，并将其存储在parms数组中。
> 3. 获取指定面的UV参数的最小和最大值，并将其存储在minmax数组中。
> 4. 对获取的UV参数进行归一化处理，即将UV参数转换到[0,1]的范围内，以便进行后续处理或显示。
> 5. 将归一化后的UV参数存储在fparms数组中。
> 6. 使用NX Open C++ API中的UF_MODL_ask_face_parm和UF_MODL_ask_face_uv_minmax函数实现以上功能。
>
> 总的来说，这段代码通过调用NX Open C++ API，实现了对NX模型中指定面的位置和UV参数的查询和归一化处理，为后续的几何分析和处理提供了基础。
>
