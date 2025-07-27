### 【2088】report face bsurf data 报告面B曲面数据

#### 代码

```cpp
    #define WRITE_D(X) ECHO("%s = %d\n", #X, X);  
    #define WRITE_L(X) if (X) ECHO("%s = TRUE\n", #X); \  
        else ECHO("%s = FALSE\n", #X);  
    #define WRITE_F(X) ECHO("%s = %f\n", #X, X)  
    #define WRITEnF(n,X)(write_double_array_to_listing_window(#X, X, 1, n))  
    #define WRITEnF4(n,X) (write_double_array_to_listing_window(#X, X, n, 4))  
    static void write_double_array_to_listing_window(char *name, double *array,  
            int rows, int cols)  
    {  
        int  
            ii,  
            jj,  
            kk = 0;  
        char  
            blanks[UF_UI_MAX_STRING_LEN+1];  
        memset(blanks, ' ', strlen(name) + 3);  
        blanks[strlen(name) + 3] = '\0';  
        for (ii = 0; ii < rows; ii++)  
        {  
            if (cols > 0)  
                if (ii == 0) ECHO("%s = ", name);  
                else ECHO("%s", blanks);  
            for (jj = 0; jj < cols; jj++)  
            {  
                ECHO("%f", array[kk++]);  
                if (jj < cols - 1) ECHO("%s", ", ");  
            }  
            if (cols > 0) ECHO("%s", "\n");  
        }  
    }  
    static void report_face_bsurf_data(tag_t face, logical terse)  
    {  
        logical  
            untrimmed;  
        UF_MODL_bsurface_t  
            bsurf;  
        if (UF_CALL(UF_MODL_ask_bsurf(face, &bsurf))) return;  
        WRITE_D(bsurf.num_poles_u);  
        WRITE_D(bsurf.num_poles_v);  
        WRITE_D(bsurf.order_u);  
        WRITE_D(bsurf.order_v);  
        WRITE_L(bsurf.is_rational);  
        if (!terse)  
        {  
            WRITEnF(bsurf.num_poles_u + bsurf.order_u, bsurf.knots_u);  
            WRITEnF(bsurf.num_poles_v + bsurf.order_v, bsurf.knots_v);  
            WRITEnF4(bsurf.num_poles_u * bsurf.num_poles_v, (double *)bsurf.poles);  
        }  
        UF_CALL(UF_MODL_free_bsurf_data(&bsurf));  
        untrimmed = uf5411(&face);  
        WRITE_L(untrimmed);  
    }

```

#### 代码解析

> 这段代码是NX的二次开发代码，其主要功能是报告NX中面的B曲面数据。
>
> 主要步骤包括：
>
> 1. 使用宏定义了几个用于打印变量值的宏，如WRITE_D打印整型，WRITE_L打印布尔型，WRITE_F打印浮点型等。
> 2. 定义了一个名为write_double_array_to_listing_window的函数，用于将double数组打印到Listing窗口，可以指定打印的行数和列数。
> 3. 定义了一个名为report_face_bsurf_data的函数，用于报告面的B曲面数据。
> 4. 在report_face_bsurf_data函数中，首先调用UF_MODL_ask_bsurf询问面的B曲面数据，并使用WRITE_D和WRITE_L宏打印B曲面的基本参数，如极点数、阶数、是否为有理B曲面等。
> 5. 如果不是简洁模式，还会打印B曲面的节点向量、极点坐标数组等详细信息。
> 6. 最后释放B曲面数据，并报告面是否为未修剪的面。
> 7. 这段代码通过UF_MODL模块提供的函数获取面的B曲面数据，并打印出来，用于调试或查看面的B曲面信息。
>
