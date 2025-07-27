### 【2375】report sketch constraint type 报告草图约束类型

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
    static void report_sketch_constraint_type(tag_t constraint)  
    {  
        UF_SKET_con_type_t  
            con_type;  
        if (UF_CALL(UF_SKET_ask_constraint_type(constraint, &con_type))) return;  
        WRITE("con_type = ");  
        switch (con_type)  
        {  
            case UF_SKET_fixed:  
                WRITE_D(UF_SKET_fixed);  
                break;  
            case UF_SKET_horizontal:  
                WRITE_D(UF_SKET_horizontal);  
                break;  
            case UF_SKET_vertical:  
                WRITE_D(UF_SKET_vertical);  
                break;  
            case UF_SKET_parallel:  
                WRITE_D(UF_SKET_parallel);  
                break;  
            case UF_SKET_perpendicular:  
                WRITE_D(UF_SKET_perpendicular);  
                break;  
            case UF_SKET_collinear:  
                WRITE_D(UF_SKET_collinear);  
                break;  
            case UF_SKET_equal_length:  
                WRITE_D(UF_SKET_equal_length);  
                break;  
            case UF_SKET_equal_radius:  
                WRITE_D(UF_SKET_equal_length);  
                break;  
            case UF_SKET_constant_length:  
                WRITE_D(UF_SKET_constant_length);  
                break;  
            case UF_SKET_constant_angle:  
                WRITE_D(UF_SKET_constant_angle);  
                break;  
            case UF_SKET_coincident:  
                WRITE_D(UF_SKET_coincident);  
                break;  
            case UF_SKET_concentric:  
                WRITE_D(UF_SKET_concentric);  
                break;  
            case UF_SKET_mirror:  
                WRITE_D(UF_SKET_mirror);  
                break;  
            case UF_SKET_point_on_curve:  
                WRITE_D(UF_SKET_point_on_curve);  
                break;  
            case UF_SKET_midpoint:  
                WRITE_D(UF_SKET_midpoint);  
                break;  
            case UF_SKET_tangent:  
                WRITE_D(UF_SKET_tangent);  
                break;  
            case UF_SKET_radius_dim:  
                WRITE_D(UF_SKET_radius_dim);  
                break;  
            case UF_SKET_diameter_dim:  
                WRITE_D(UF_SKET_diameter_dim);  
                break;  
            case UF_SKET_horizontal_dim:  
                WRITE_D(UF_SKET_horizontal_dim);  
                break;  
            case UF_SKET_vertical_dim:  
                WRITE_D(UF_SKET_vertical_dim);  
                break;  
            case UF_SKET_parallel_dim:  
                WRITE_D(UF_SKET_parallel_dim);  
                break;  
            case UF_SKET_perpendicular_dim:  
                WRITE_D(UF_SKET_perpendicular_dim);  
                break;  
            case UF_SKET_angular_dim:  
                WRITE_D(UF_SKET_angular_dim);  
                break;  
            case UF_SKET_reserved_con_1:         
                WRITE_D(UF_SKET_reserved_con_1);  
                break;  
            case UF_SKET_reserved_con_2:         
                WRITE_D(UF_SKET_reserved_con_2);  
                break;  
            case UF_SKET_reserved_con_3:          
                WRITE_D(UF_SKET_reserved_con_3);   
                break;  
            case UF_SKET_reserved_con_4:         
                WRITE_D(UF_SKET_reserved_con_4);  
                break;  
            case UF_SKET_reserved_con_5:         
                WRITE_D(UF_SKET_reserved_con_5);  
                break;  
            case UF_SKET_reserved_con_6:         
                WRITE_D(UF_SKET_reserved_con_6);  
                break;  
            case UF_SKET_point_on_string:  
                WRITE_D(UF_SKET_point_on_string);  
                break;  
            case UF_SKET_slope:                   
                WRITE_D(UF_SKET_slope);                   
                break;  
            case UF_SKET_uniform_scaled:  
                WRITE_D(UF_SKET_uniform_scaled);  
                break;  
            case UF_SKET_non_uniform_scaled:  
                WRITE_D(UF_SKET_non_uniform_scaled);  
                break;  
            case UF_SKET_assoc_trim:  
                WRITE_D(UF_SKET_assoc_trim);  
                break;  
            case UF_SKET_assoc_offset:  
                WRITE_D(UF_SKET_assoc_offset);  
                break;  
            case UF_SKET_perimeter_dim:  
                WRITE_D(UF_SKET_perimeter_dim);  
                break;  
            default:  
                WRITE("Unknown ");  
                WRITE_D(con_type);  
                break;  
        }  
    }

```

#### 代码解析

> 这段代码定义了一个用于报告NX草图约束类型的函数，主要功能如下：
>
> 1. 使用宏WRITE和WRITE_D来简化打开和写入NX列表窗口的操作。
> 2. 定义了一个名为write_integer_to_listing_window的函数，用于将整数写入NX列表窗口，接受字符串标题和整数数值作为参数。
> 3. 定义了名为report_sketch_constraint_type的函数，用于报告给定草图约束的类型。该函数首先获取约束的类型，然后根据不同的类型使用WRITE_D宏输出相应的枚举值。
> 4. 在函数report_sketch_constraint_type中，通过switch语句根据不同的约束类型输出对应的枚举值，并处理了未知类型的情况。
> 5. 使用了NX提供的UF函数来获取约束类型，并将输出写入NX的列表窗口，以便于调试和查看。
>
> 总的来说，这段代码通过宏和函数封装简化了NX列表窗口的写入操作，实现了根据草图约束类型输出不同枚举值的逻辑，并进行了错误处理，是一个典型的NX二次开发中用于调试和查看约束信息的函数。
>
