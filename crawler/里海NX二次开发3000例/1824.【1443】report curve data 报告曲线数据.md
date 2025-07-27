### 【1443】report curve data 报告曲线数据

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
    #define WRITE_F(X) (write_double_to_listing_window(#X, X))  
    static void write_double_to_listing_window(char *title, double n)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %f\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    #define WRITE3F(X)  (write_double_array_to_listing_window(#X, X, 1, 3))  
    #define WRITEnF(n,X)(write_double_array_to_listing_window(#X, X, 1, n))  
    #define WRITEnF4(X,n) (write_double_array_to_listing_window(#X, X, n, 4))  
    static void write_double_array_to_listing_window(char *name, double *array,  
            int rows, int cols)  
    {  
        int  
            ii,  
            jj,  
            kk = 0;  
        char  
            blanks[UF_UI_MAX_STRING_LEN+1],  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        memset(blanks, ' ', strlen(name) + 3);  
        blanks[strlen(name) + 3] = '\0';  
        for (ii = 0; ii < rows; ii++)  
        {  
            if (ii == 0) sprintf(msg, "%s = ", name);  
            else sprintf(msg, "%s", blanks);  
            if (cols > 0) UF_CALL(UF_UI_write_listing_window(msg));  
            for (jj = 0; jj < cols; jj++)  
            {  
                sprintf(msg, "%f", array[kk++]);  
                UF_CALL(UF_UI_write_listing_window(msg));  
                if (jj < cols - 1)  
                {  
                    sprintf(msg, ", ");  
                    UF_CALL(UF_UI_write_listing_window(msg));  
                }  
            }  
            if (cols > 0) UF_CALL(UF_UI_write_listing_window("\n"));  
        }  
    }  
    static void report_curve_data(tag_t curve)  
    {  
        int  
            type;  
        double  
            *curve_data;  
        UF_CURVE_struct_p_t  
            curve_struct;  
        if (UF_CALL(UF_CURVE_ask_curve_struct(curve, &curve_struct))) return;  
        UF_CALL(UF_CURVE_ask_curve_struct_data(curve_struct,&type,&curve_data));  
        switch (type)  
        {  
            case UF_point_type:  
                WRITE("Point:\n ");  
                WRITE(" Absolute coordinates:");  
                WRITE3F(curve_data);  
                break;  
            case UF_line_type:  
                WRITE("Line:\n");  
                WRITE(" Start coordinates:  ");  
                WRITE3F(&curve_data[3]);  
                WRITE(" End coordinates:  ");  
                WRITE3F(&curve_data[6]);  
                break;  
            case UF_circle_type:  
                WRITE("Arc:\n");  
                WRITE(" Start angle (radians):  ");  
                WRITE_F(curve_data[3]);  
                WRITE(" End angle (radians):  ");  
                WRITE_F(curve_data[4]);  
                WRITE(" Center coordinates:  ");  
                WRITE3F(&curve_data[6]);  
                WRITE(" X axis of construction plane:  ");  
                WRITE3F(&curve_data[9]);  
                WRITE(" Y axis of construction plane:  ");  
                WRITE3F(&curve_data[12]);  
                break;  
            case UF_conic_type:  
                WRITE("Conic:\n");  
                WRITE(" Center coordinates:  ");  
                WRITE3F(&curve_data[8]);  
                WRITE(" axis1:  ");  
                WRITE3F(&curve_data[11]);  
                WRITE(" axis2:  ");  
                WRITE3F(&curve_data[14]);  
                break;  
            case UF_spline_type:  
                WRITE("Spline:\n");  
                WRITE(" Number of poles:  ");  
                WRITE_F(curve_data[3]);  
                WRITE(" Order:  ");  
                WRITE_F(curve_data[4]);  
                WRITE(" Knots:  ");  
                WRITEnF((int)(curve_data[3]+curve_data[4]),&curve_data[5]);  
                WRITE(" Poles:  ");  
                WRITEnF4((&curve_data[5])+(int)(curve_data[3]+curve_data[4]),  
                    curve_data[3]);  
                break;  
            default:  
                WRITE("Unknown curve type ");  
                WRITE_D(type);  
                break;  
        }  
        UF_CALL(UF_CURVE_free_curve_struct(curve_struct));  
        UF_free(curve_data);  
    }

```

#### 代码解析

> 这段代码是用于NX的二次开发，主要实现了以下功能：
>
> 1. 定义了几个宏，用于在列表窗口中打印信息。这些宏包括WRITE、WRITE_D、WRITE_F、WRITE3F、WRITEnF和WRITEnF4，它们分别用于打印字符串、整数、浮点数、3个浮点数、n个浮点数以及4*n个浮点数。
> 2. 定义了write_integer_to_listing_window、write_double_to_listing_window和write_double_array_to_listing_window三个函数，用于实现上述宏的具体功能。
> 3. 定义了一个report_curve_data函数，用于输出NX中曲线的数据信息。这个函数首先获取曲线的结构和数据，然后根据曲线类型，调用不同的宏打印出曲线的详细信息，包括点、线、圆弧、二次曲线、样条等不同类型曲线的数据。
> 4. 使用了NX提供的UF_CURVE系列函数来获取曲线数据，以及UF_UI系列函数来打印信息。
> 5. 在输出曲线数据时，使用了switch-case结构，针对不同类型的曲线调用不同的宏进行打印。
> 6. 最后释放了获取的曲线结构和数据。
>
> 总的来说，这段代码通过封装打印函数和宏，实现了NX中曲线数据的查询和打印功能，为NX的二次开发提供了便利。
>
