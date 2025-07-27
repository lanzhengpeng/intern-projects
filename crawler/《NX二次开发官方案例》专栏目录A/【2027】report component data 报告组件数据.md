### 【2027】report component data 报告组件数据

#### 代码

```cpp
    #define WRITE_S(X) (write_string_to_listing_window(#X, X))  
    void write_string_to_listing_window(char *title, char *string)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        sprintf(msg, "%s = \"%s\"\n", title, string);  
        ECHO(msg);  
    }  
    #define WRITE2F(X)  (write_double_array_to_listing_window(#X, X, 1, 2))  
    #define WRITE3F(X)  (write_double_array_to_listing_window(#X, X, 1, 3))  
    #define WRITE9F(X)  (write_double_array_to_listing_window(#X, X, 3, 3))  
    #define WRITE16F(X) (write_double_array_to_listing_window(#X, X, 4, 4))  
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
        memset(blanks, ' ', strlen(name) + 3);  
        blanks[strlen(name) + 3] = '\0';  
        for (ii = 0; ii < rows; ii++)  
        {  
            if (ii == 0) sprintf(msg, "%s = ", name);  
            else sprintf(msg, "%s", blanks);  
            if (cols > 0) ECHO(msg);  
            for (jj = 0; jj < cols; jj++)  
            {  
                sprintf(msg, "%f", array[kk++]);  
                ECHO(msg);  
                if (jj < cols - 1)  
                {  
                    sprintf(msg, ", ");  
                    ECHO(msg);  
                }  
            }  
            if (cols > 0) ECHO("\n");  
        }  
    }  
    /* 里海 */  
    static void report_component_data(tag_t comp)  
    {  
        double  
            csys_matrix[9],  
            origin[3],  
            transform[4][4];  
        char  
            *comp_part_name,  
            instance_name[UF_CFI_MAX_FILE_NAME_SIZE],  
            part_name[UF_CFI_MAX_PATH_NAME_SIZE],  
            refset_name[MAX_ENTITY_NAME_SIZE+1];  
        if (!UF_CALL(UF_ASSEM_ask_component_data(comp, part_name, refset_name,  
            instance_name, origin, csys_matrix, transform)))  
        {  
            UF_CALL(UF_PART_file_name_for_display(part_name, &comp_part_name));  
            WRITE_S(comp_part_name);  
            UF_free(comp_part_name);  
            WRITE_S(refset_name);  
            WRITE_S(instance_name);  
            WRITE3F(origin);  
            WRITE9F(csys_matrix);  
            WRITE16F((double*)transform);  
            ECHO("\n");  
        }  
    }

```

#### 代码解析

> 这段代码是一个NX二次开发示例，其主要功能是报告组件数据。具体来说：
>
> 1. 定义了几个宏，用于将字符串或浮点数组写入列表窗口，以方便调试。
> 2. 定义了一个write_double_array_to_listing_window函数，用于将浮点数组写入列表窗口。
> 3. 定义了一个report_component_data函数，用于报告组件的名称、引用集名称、实例名称、原点、坐标系矩阵和变换矩阵。
> 4. 在report_component_data函数中，通过UF_ASSEM_ask_component_data函数获取组件的各种数据，然后使用之前定义的宏将数据写入列表窗口。
> 5. 使用了NX提供的UF_PART_file_name_for_display函数，用于获取组件的显示名称。
> 6. 使用了NX提供的UF_CALL宏，用于简化UF函数的调用。
> 7. 使用了NX提供的UF_free函数，用于释放内存。
> 8. 通过ECHO宏将信息输出到列表窗口。
>
> 总体来说，这段代码利用NX提供的API，实现了组件数据的查询和输出，通过宏简化了代码，提高了可读性和可维护性。
>
