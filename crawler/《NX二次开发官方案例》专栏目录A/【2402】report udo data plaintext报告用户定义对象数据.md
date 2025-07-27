### 【2402】report udo data plaintext报告用户定义对象数据

#### 代码

```cpp
    #define WRITE_D(X) (write_integer_to_listing_window(#X, X))  
    static void write_integer_to_listing_window(char *title, int n)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %d\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    #define WRITEnD(n,X) (write_integer_array_to_listing_window(#X, X, 1, n))  
    static void write_integer_array_to_listing_window(char *name, int *array,  
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
                sprintf(msg, "%d", array[kk++]);  
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
    #define WRITE_S(X) (write_string_to_listing_window(#X, X))  
    void write_string_to_listing_window(char *title, char *string)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = \"%s\"\n", title, string);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    #define WRITEnF(n,X)(write_double_array_to_listing_window(#X, X, 1, n))  
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
    static void report_udo_data(tag_t udo)  
    {  
        int  
            ii,  
            status,  
            subtype,  
            type;  
        char  
            *class_name;  
        UF_UDOBJ_all_data_t  
            data;  
        char  
            out[MAX_LINE_SIZE+1];  
        WRITE_D(udo);  
        if (UF_CALL(UF_UDOBJ_ask_udo_class_name(udo, &class_name))) return;  
        WRITE_S(class_name);  
        UF_free(class_name);  
        UF_CALL(UF_UDOBJ_ask_udo_data(udo, &data));  
        WRITE_D(data.udo_status);  
        WRITE_D(data.num_ints);  
        WRITEnD(data.num_ints, data.ints);  
        WRITE_D(data.num_doubles);  
        WRITEnF(data.num_doubles, data.doubles);  
        WRITE_D(data.num_strings);  
        for (ii = 0; ii < data.num_strings; ii++)  
        {  
            WRITE_S(data.strings[ii]);  
        }  
        WRITE_D(data.num_links);  
        for (ii = 0; ii < data.num_links; ii++)  
        {  
            if (data.link_defs[ii].assoc_ug_tag != NULL_TAG)  
            {  
                UF_CALL(UF_OBJ_ask_type_and_subtype(  
                    data.link_defs[ii].assoc_ug_tag, &type, &subtype));  
                status = UF_OBJ_ask_status(data.link_defs[ii].assoc_ug_tag);  
                sprintf(out, "\t\t%d. type %d to %d (%d/%d %d) status %d\n", ii+1,  
                    data.link_defs[ii].link_type, data.link_defs[ii].assoc_ug_tag,  
                    type, subtype, status, data.link_defs[ii].object_status);  
                UF_CALL(UF_UI_write_listing_window(out));  
            }  
            else  
            {  
                sprintf(out, "\t\t%d. type %d to NULL_TAG\n", ii+1,  
                    data.link_defs[ii].link_type);  
                UF_CALL(UF_UI_write_listing_window(out));  
            }  
        }  
        WRITE_D(data.num_lengths);  
        WRITEnF(data.num_lengths, data.lengths);  
        WRITE_D(data.num_areas);  
        WRITEnF(data.num_areas, data.areas);  
        WRITE_D(data.num_volumes);  
        WRITEnF(data.num_volumes, data.volumes);  
        UF_CALL(UF_UDOBJ_free_udo_data(&data));  
    }

```

#### 代码解析

> 这段代码主要实现了对NX UDO（用户定义对象）数据的打印报告功能。
>
> 具体步骤如下：
>
> 1. 定义了几个宏，用于简化打印整数、字符串、整数数组、浮点数数组和浮点数数组的操作。
> 2. 定义了write_udo_data函数，用于打印给定UDO对象的数据。
> 3. 在write_udo_data函数中，首先打印UDO对象的tag，然后通过调用UF_UDOBJ_ask_udo_class_name获取UDO的类名并打印。
> 4. 调用UF_UDOBJ_ask_udo_data获取UDO的数据，并依次打印：UDO的状态UDO包含的整数数量及具体整数数组UDO包含的浮点数数量及具体浮点数数组UDO包含的字符串数量及具体字符串数组UDO包含的链接数量，及每个链接的类型、关联对象tag、关联对象类型和子类型、关联对象状态等UDO包含的长度数量及具体长度数组UDO包含的面积数量及具体面积数组UDO包含的体积数量及具体体积数组
> 5. UDO的状态
> 6. UDO包含的整数数量及具体整数数组
> 7. UDO包含的浮点数数量及具体浮点数数组
> 8. UDO包含的字符串数量及具体字符串数组
> 9. UDO包含的链接数量，及每个链接的类型、关联对象tag、关联对象类型和子类型、关联对象状态等
> 10. UDO包含的长度数量及具体长度数组
> 11. UDO包含的面积数量及具体面积数组
> 12. UDO包含的体积数量及具体体积数组
> 13. 最后调用UF_UDOBJ_free_udo_data释放获取的UDO数据。
>
> 综上所述，这段代码实现了对NX UDO对象数据的详细打印报告，帮助开发者调试和了解UDO对象的内容。
>
