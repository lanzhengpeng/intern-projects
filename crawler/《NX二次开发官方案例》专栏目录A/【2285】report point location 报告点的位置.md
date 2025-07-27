### 【2285】report point location 报告点的位置

#### 代码

```cpp
    #define WRITE(X)   UF_UI_open_listing_window(); UF_UI_write_listing_window(X)  
    #define WRITE3F(X)  (write_double_array_to_listing_window(#X, X, 1, 3))  
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
    static void report_point_location(char *name, tag_t point)  
    {  
        double  
            location[3];  
        UF_CALL(UF_CURVE_ask_point_data(point, location));  
        WRITE(name);  
        WRITE(" is at ");  
        WRITE3F(location);  
    }

```

#### 代码解析

> 这段代码是NX的二次开发代码，主要包含以下内容：
>
> 1. 定义了一个宏WRITE，用于打开NX的列表窗口并写入内容。
> 2. 定义了一个宏WRITE3F，用于将一个double数组写入NX的列表窗口，最多只写入前3个元素。
> 3. 定义了一个函数write_double_array_to_listing_window，用于将一个double数组写入NX的列表窗口，可以指定行数和列数。
> 4. 定义了一个函数report_point_location，用于查询一个点的坐标，并将其写入NX的列表窗口。
> 5. 在report_point_location函数中，首先通过UF_CURVE_ask_point_data函数查询点的坐标。
> 6. 然后通过WRITE宏写入点的名称。
> 7. 接着通过WRITE宏写入“is at”。
> 8. 最后通过WRITE3F宏写入点的坐标数组。
> 9. 通过调用report_point_location函数，可以打印出点的名称和坐标信息到NX的列表窗口中。
>
> 综上所述，这段代码的主要作用是提供一些宏和函数，以便在NX的二次开发中，能够方便地将信息打印到NX的列表窗口，以进行调试或显示。
>
