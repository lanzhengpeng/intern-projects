### 【2089】report face data 报告面数据

#### 代码

```cpp
    #define WRITE_D(X) (write_integer_to_listing_window(#X, X))  
    #define WRITE_F(X) (write_double_to_listing_window(#X, X))  
    #define WRITE3F(X)  (write_double_array(#X, X, 1, 3))  
    #define WRITE6F(X)  (write_double_array(#X, X, 2, 3))  
    static void write_integer_to_listing_window(char *title, int n)  
    {  
        char  
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %d\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    static void write_double_to_listing_window(char *title, double n)  
    {  
        char  
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %f\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    static void write_double_array(char *name, double *array, int rows, int cols)  
    {  
        int  
            ii,  
            jj,  
            kk = 0;  
        char  
            blanks[MAX_LINE_SIZE+1],  
            msg[MAX_LINE_SIZE+1];  
        memset(blanks, ' ', strlen(name) + 3);  
        blanks[strlen(name) + 3] = '\0';  
        for (ii = 0; ii < rows; ii++)  
        {  
            if (ii == 0) sprintf(msg, "%s = ", name);  
            else sprintf(msg, "%s", blanks);  
            UF_CALL(UF_UI_write_listing_window(msg));  
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
            sprintf(msg, "\n");  
            UF_CALL(UF_UI_write_listing_window(msg));  
        }  
    }  
    static void report_face_data(tag_t face)  
    {  
        int  
            norm_dir,  
            type;  
        double  
            center[3],  
            dir[3],  
            box[6],  
            radius,  
            rad_data;  
        UF_CALL(UF_MODL_ask_face_data(face, &type, center, dir, box, &radius,  
            &rad_data, &norm_dir));  
        WRITE_D(face);  
        WRITE_D(type);  
        WRITE3F(center);  
        WRITE3F(dir);  
        WRITE6F(box);  
        WRITE_F(radius);  
        WRITE_F(rad_data);  
        WRITE_D(norm_dir);  
    }

```

#### 代码解析

> 这段代码是用于NX（Siemens NX）CAD软件的二次开发代码，其主要功能是获取和报告面的各种数据信息。
>
> 代码的关键组成部分包括：
>
> 1. 定义了几个宏，用于将变量输出到列表窗口，包括整数、浮点数和浮点数组。
> 2. 实现了三个辅助函数：write_integer_to_listing_window：输出整数到列表窗口write_double_to_listing_window：输出浮点数到列表窗口write_double_array：输出浮点数组到列表窗口
> 3. write_integer_to_listing_window：输出整数到列表窗口
> 4. write_double_to_listing_window：输出浮点数到列表窗口
> 5. write_double_array：输出浮点数组到列表窗口
> 6. 实现了report_face_data函数，该函数接受一个面的tag，并使用UF_MODL_ask_face_data函数获取该面的各种数据，包括类型、中心点、法向量、包围盒、半径等，然后使用定义的宏将这些数据输出到列表窗口。
> 7. report_face_data函数可以被调用，以获取和报告指定面的各种数据信息。
>
> 总的来说，这段代码利用NX提供的API函数，实现了获取面数据并输出到列表窗口的功能，用于二次开发中的数据处理和显示。
>
