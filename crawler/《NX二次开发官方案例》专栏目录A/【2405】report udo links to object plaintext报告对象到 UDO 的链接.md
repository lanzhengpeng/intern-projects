### 【2405】report udo links to object plaintext报告对象到 UDO 的链接

#### 代码

```cpp
    #define WRITE_D(X) (write_integer_to_listing_window(#X, X))  
    static void write_integer_to_listing_window(char *title, int n)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        sprintf(msg, "%s = %d\n", title, n);  
        ECHO(msg);  
    }  
    #define WRITE_S(X) (write_string_to_listing_window(#X, X))  
    void write_string_to_listing_window(char *title, char *string)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        sprintf(msg, "%s = \"%s\"\n", title, string);  
        ECHO(msg);  
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
            if (cols > 0) ECHO(msg);  
            for (jj = 0; jj < cols; jj++)  
            {  
                sprintf(msg, "%d", array[kk++]);  
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
    static void report_udo_links_to_object(tag_t object)  
    {  
        int  
            ii,  
            n_links;  
        UF_UDOBJ_link_t  
            *udo_links;  
        UF_CALL(UF_UDOBJ_ask_udo_links_to_obj(object, &n_links, &udo_links));  
        WRITE_D(n_links);  
        for (ii = 0; ii < n_links; ii++)  
            report_udo_data(udo_links[ii].assoc_ug_tag);  
        if (n_links > 0) UF_free(udo_links);  
    }

```

#### 代码解析

> 这段代码是NX Open的C++代码，用于获取并打印用户定义对象(UDO)的数据。主要功能包括：
>
> 1. 定义了一些宏，用于格式化打印变量和数组。
> 2. 定义了几个函数，用于打开列表窗口，并格式化打印整型数组和浮点型数组。
> 3. report_udo_data函数：用于获取并打印一个UDO的所有数据，包括状态、整数、浮点数、字符串、关联对象、长度、面积、体积等。
> 4. report_udo_links_to_object函数：用于获取与一个NX对象关联的所有UDO，并打印每个UDO的数据。
> 5. 整个代码通过递归调用report_udo_data来打印每个UDO的数据，通过递归调用report_udo_links_to_object来打印每个NX对象关联的所有UDO的数据。
> 6. 使用了NX Open的UF_UDOBJ类和UF_OBJ类提供的接口来获取UDO和NX对象的数据。
> 7. 代码结构清晰，通过递归实现了UDO数据的遍历打印，具有一定的参考价值。
>
