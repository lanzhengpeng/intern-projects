### 【2039】report csys info ```plaintext报告坐标系信息```

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
    static void report_csys_info(tag_t csys)  
    {  
        tag_t  
            matrix;  
        double  
            origin[3],  
            vectors[9];  
        if (UF_CALL(UF_CSYS_ask_csys_info(csys, &matrix, origin))) return;  
        WRITE3F(origin);  
        UF_CALL(UF_CSYS_ask_matrix_values(matrix, vectors));  
        WRITE9F(vectors);  
    }

```

#### 代码解析

> 这段代码是NX Open的二次开发代码，主要用于报告坐标系信息。代码中包含了几个关键部分：
>
> 1. 宏定义：定义了两个宏WRITE3F和WRITE9F，用于将3x1和3x3的浮点数数组输出到日志窗口。这些宏调用了write_double_array_to_listing_window函数，以方便打印数组。
> 2. write_double_array_to_listing_window函数：该函数用于将浮点数数组打印到日志窗口。它接受数组名称、数组指针、行数和列数作为参数，并按行和列格式化输出数组。
> 3. report_csys_info函数：该函数用于报告给定坐标系的详细信息。首先调用UF_CSYS_ask_csys_info函数获取坐标系信息，包括原点坐标和方向矩阵。然后分别使用WRITE3F和WRITE9F宏将原点坐标和方向矩阵打印到日志窗口。
>
> 总的来说，这段代码提供了报告坐标系信息的实用功能，通过宏定义简化了打印数组的代码，并实现了坐标系信息的输出。
>
