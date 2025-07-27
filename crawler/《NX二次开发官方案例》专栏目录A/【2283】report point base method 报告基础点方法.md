### 【2283】report point base method 报告基础点方法

#### 代码

```cpp
    static void report_point_base_method(int method)  
    {  
        WRITE("method = ");  
        switch (method)  
        {  
            case UF_UI_POINT_INFERRED:  
                WRITE_D(UF_UI_POINT_INFERRED);  
                break;  
            case UF_UI_POINT_CURSOR_POS:  
                WRITE_D(UF_UI_POINT_CURSOR_POS);  
                break;  
            case UF_UI_POINT_EXISTING_PT:  
                WRITE_D(UF_UI_POINT_EXISTING_PT);  
                break;  
            case UF_UI_POINT_END_PT:  
                WRITE_D(UF_UI_POINT_END_PT);  
                break;  
            case UF_UI_POINT_CONTROL_PT:  
                WRITE_D(UF_UI_POINT_CONTROL_PT);  
                break;  
            case UF_UI_POINT_INTERSECT_PT:  
                WRITE_D(UF_UI_POINT_INTERSECT_PT);  
                break;  
            case UF_UI_POINT_CENTER_PT:  
                WRITE_D(UF_UI_POINT_CENTER_PT);  
                break;  
            case UF_UI_POINT_ANGLE_PT:  
                WRITE_D(UF_UI_POINT_ANGLE_PT);  
                break;  
            case UF_UI_POINT_QUADRANT_PT:  
                WRITE_D(UF_UI_POINT_QUADRANT_PT);  
                break;  
            case UF_UI_POINT_ON_CURVE_PT:  
                WRITE_D(UF_UI_POINT_ON_CURVE_PT);  
                break;  
            case UF_UI_POINT_ON_SURFACE_PT:  
                WRITE_D(UF_UI_POINT_ON_SURFACE_PT);  
                break;  
            case UF_UI_POINT_OFFSET_CSYS_PT:  
                WRITE_D(UF_UI_POINT_OFFSET_CSYS_PT);  
                break;  
            case UF_UI_POINT_DIALOG:  
                WRITE_D(UF_UI_POINT_DIALOG);  
                break;  
            case UF_UI_POINT_NO_METHOD:  
                WRITE_D(UF_UI_POINT_NO_METHOD);  
                break;  
            case UF_UI_POINT_APPLICATION_METHOD:  
                WRITE_D(UF_UI_POINT_APPLICATION_METHOD);  
                break;  
            default:  
                WRITE("Unknown UF_UI_POINT_base_method_t - ");  
                WRITE_D(method);  
                break;  
        }  
    }

```

#### 代码解析

> 这段代码是用于报告点基方法（point base method）的函数，其功能包括：
>
> 1. 输出方法名称。
> 2. 根据不同的方法值，使用switch语句输出不同的点基方法名称。
> 3. 对于未知的方法值，输出"Unknown UF_UI_POINT_base_method_t"。
> 4. 输出方法值。
> 5. 使用WRITE和WRITE_D宏来输出信息，这些宏可能用于日志或调试。
>
> 总体来说，这段代码的主要功能是根据传入的方法值，输出对应的点基方法名称，用于调试和日志记录。
>
