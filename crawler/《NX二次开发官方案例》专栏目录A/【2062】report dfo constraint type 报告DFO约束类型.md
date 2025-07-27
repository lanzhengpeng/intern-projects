### 【2062】report dfo constraint type 报告DFO约束类型

#### 代码

```cpp
    #define WRITE(X)   UF_UI_open_listing_window(); UF_UI_write_listing_window(X)  
    #define WRITE_D(X) (write_integer_to_listing_window(#X, X))  
    static void write_integer_to_listing_window(char *title, int n)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %d\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    static void report_dfo_constraint_type(char *name,  
        UF_MODL_dfo_constraint_type_t type)  
    {  
        WRITE(name);  
        WRITE(" = ");  
        switch (type)  
        {  
            case UF_distance_dim:  
                WRITE_D(UF_distance_dim);  
                break;  
            case UF_angle_dim:  
                WRITE_D(UF_angle_dim);  
                break;  
            case UF_coincident:  
                WRITE_D(UF_coincident);  
                break;  
            case UF_parallel_con:  
                WRITE_D(UF_parallel_con);  
                break;  
            case UF_perpen_con:  
                WRITE_D(UF_perpen_con);  
                break;  
            case UF_tangent_con:  
                WRITE_D(UF_tangent_con);  
                break;  
            default:  
                WRITE("Unknown ");  
                WRITE_D(type);  
        }  
    }

```

#### 代码解析

> 这段代码是NX Open API的一个示例，用于在NX的列表窗口中输出变量值和距离、角度等约束类型。
>
> 主要功能包括：
>
> 1. 定义了两个宏：WRITE和WRITE_D。其中，WRITE用于输出字符串到列表窗口；而WRITE_D则用于输出变量名和值。
> 2. 定义了一个函数write_integer_to_listing_window，用于在列表窗口输出变量名和整数值。
> 3. 定义了一个函数report_dfo_constraint_type，用于在列表窗口输出距离、角度等约束的类型名和值。
> 4. 使用switch语句判断约束类型，并输出对应的字符串和值。
> 5. 如果约束类型未知，则输出"Unknown "加上类型值。
> 6. 输出到列表窗口的字符串格式为"变量名 = 值\n"。
>
> 通过这些宏和函数，可以在NX的列表窗口中输出变量值和约束类型，方便调试和查看。
>
