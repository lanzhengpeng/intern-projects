### 【2755】set drf align site 设置DRF对齐位置

#### 代码

```cpp
    static UF_DRF_align_position_t set_drf_align_site(UF_DRF_align_position_t pos)  
    {  
        int  
            mpi[100];  
        double  
            mpr[70];  
        char  
            rad[27],  
            dia[27];  
        UF_DRF_align_position_t  
            old_site;  
        if (!UF_CALL(UF_DRF_ask_preferences(mpi, mpr, rad, dia)))  
        {  
            old_site = mpi[30];  
            mpi[30] = pos;  
            UF_CALL(UF_DRF_set_preferences(mpi, mpr, rad, dia));  
        }  
        return old_site;  
    }

```

#### 代码解析

> 这段代码是NX Open C++ API编写的，用于设置NX DRF(Design Review Format)的定位点。
>
> 主要功能包括：
>
> 1. 使用UF_DRF_ask_preferences函数获取当前的DRF参数设置。
> 2. 读取当前的定位点设置mpi[30]，并存入old_site变量中。
> 3. 将新的定位点pos赋值给mpi[30]。
> 4. 调用UF_DRF_set_preferences函数，将新的定位点设置应用到DRF中。
> 5. 返回旧的定位点设置old_site。
> 6. 使用了UF_CALL宏来处理UF函数的返回值，以进行错误检查。
>
> 总体来说，这段代码实现了在NX中设置DRF定位点的功能，同时保留了旧的设置，并进行了错误检查。代码结构清晰，注释充足，是一个良好的NX二次开发示例。
>
