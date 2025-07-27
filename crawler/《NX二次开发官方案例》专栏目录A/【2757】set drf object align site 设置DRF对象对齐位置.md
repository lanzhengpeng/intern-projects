### 【2757】set drf object align site 设置DRF对象对齐位置

#### 代码

```cpp
    static void set_drf_object_align_site(tag_t object, UF_DRF_align_position_t pos)  
    {  
        int  
            mpi[100];  
        double  
            mpr[70];  
        char  
            rad[27],  
            dia[27];  
        if (!UF_CALL(UF_DRF_ask_object_preferences(object, mpi, mpr, rad, dia)))  
        {  
            mpi[30] = pos;  
            UF_CALL(UF_DRF_set_object_preferences(object, mpi, mpr, rad, dia));  
        }  
    }

```

#### 代码解析

> 这段代码是NX Open C++ API实现的用于设置NX对象对齐位置的二次开发代码。
>
> 主要功能包括：
>
> 1. 定义了NX对象对齐位置枚举类型UF_DRF_align_position_t。
> 2. 定义了set_drf_object_align_site函数，用于设置指定NX对象的对齐位置。
> 3. 函数首先调用UF_DRF_ask_object_preferences查询当前NX对象的对齐位置，并将其存储在数组mpi中。
> 4. 如果查询成功，则修改mpi[30]为指定的对齐位置pos。
> 5. 最后，调用UF_DRF_set_object_preferences设置NX对象的对齐位置。
> 6. 代码使用了NX Open C++ API中的UF_CALL宏来简化错误处理。
> 7. 代码注释详细，变量命名清晰，便于理解和使用。
>
> 通过这段代码，我们可以快速设置NX对象的对齐位置，并提供了错误处理机制，提高了代码的健壮性。
>
