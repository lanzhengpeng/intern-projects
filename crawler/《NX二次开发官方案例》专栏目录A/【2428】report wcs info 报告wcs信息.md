### 【2428】report wcs info 报告wcs信息

#### 代码

```cpp
    #define WRITE3F(X)  (write_double_array_to_listing_window(#X, X, 1, 3))  
    #define WRITE9F(X)  (write_double_array_to_listing_window(#X, X, 3, 3))  
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
    static void report_wcs_info()  
    {  
        double  
            axes[9],  
            origin[3];  
        tag_t  
            wcs,  
            wcs_mx;  
        UF_CALL(UF_CSYS_ask_wcs(&wcs));  
        UF_CALL(UF_CSYS_ask_csys_info(wcs, &wcs_mx, origin));  
        UF_CALL(UF_CSYS_ask_matrix_values(wcs_mx, axes));  
        WRITE3F(origin);  
        WRITE9F(axes);  
    }

```

#### 代码解析

> 这段代码是NX Open C++ API编写的，用于在NX中获取和打印当前工作坐标系(WCS)的信息。
>
> 主要功能包括：
>
> 1. 获取当前工作坐标系(WCS)的tag。
> 2. 获取WCS的名称和原点坐标。
> 3. 获取WCS的变换矩阵。
> 4. 打印WCS的原点坐标。
> 5. 打印WCS的变换矩阵。
>
> 通过这段代码，可以方便地获取和打印当前WCS的信息，有助于调试和了解当前WCS的状态。
>
