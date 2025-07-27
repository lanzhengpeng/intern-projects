### 【1981】report all udo data 报告所有用户定义对象数据

#### 代码

```cpp
    /*HEAD REPORT_ALL_UDO_DATA CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_obj.h>  
    #include <uf_udobj.h>  
    #include <uf_part.h>  
    #define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))  
    static int report_error( char *file, int line, char *call, int irc)  
    {  
        if (irc)  
        {  
            char err[133],  
                 msg[133];  
            sprintf(msg, "*** ERROR code %d at line %d in %s:\n+++ ",  
                irc, line, file);  
            UF_get_fail_message(irc, err);  
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据原文，UF_print_syslog是V18版本中新增的函数。 */  
            UF_print_syslog(msg, FALSE);  
            UF_print_syslog(err, FALSE);  
            UF_print_syslog("\n", FALSE);  
            UF_print_syslog(call, FALSE);  
            UF_print_syslog(";\n", FALSE);  
            if (!UF_UI_open_listing_window())  
            {  
                UF_UI_write_listing_window(msg);  
                UF_UI_write_listing_window(err);  
                UF_UI_write_listing_window("\n");  
                UF_UI_write_listing_window(call);  
                UF_UI_write_listing_window(";\n");  
            }  
        }  
        return(irc);  
    }  
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
    static void do_it(void)  
    {  
        tag_t  
            part = UF_PART_ask_display_part(),  
            udo = NULL_TAG;  
        while ((!UF_CALL(UF_OBJ_cycle_objs_in_part(part,  
            UF_user_defined_object_type, &udo))) && (udo != NULL_TAG))  
                report_udo_data(udo);  
    }  
    /* qq3123197280 */  
    void ufusr(char *param, int *retcode, int paramLen)  
    {  
        if (UF_CALL(UF_initialize())) return;  
        do_it();  
        UF_terminate();  
    }  
    int ufusr_ask_unload(void)  
    {  
        return (UF_UNLOAD_IMMEDIATELY);  
    }

```

#### 代码解析

> 这段代码是一个NX二次开发示例，其主要功能是遍历当前零件中的所有用户定义对象(UDO)，并输出每个UDO的数据信息。以下是代码的主要组成部分和功能：
>
> 1. 头文件包含：包含了一些常用的NX头文件，如uf.h、uf_ui.h等，用于访问NX的API函数。
> 2. 错误报告宏：定义了UF_CALL宏，用于调用NX API函数，并在出错时报告错误信息。
> 3. 数据写入宏：定义了几个宏，用于将数据写入NX的列表窗口，方便查看输出结果。
> 4. 报告UDO数据函数：实现了report_udo_data函数，用于获取并输出一个UDO的所有数据，包括状态、整数、浮点数、字符串、链接、长度、面积、体积等。
> 5. 执行函数：实现了do_it函数，用于遍历当前零件中的所有UDO，并调用report_udo_data函数报告每个UDO的数据。
> 6. UFUSR入口函数：实现了ufusr函数，这是NX二次开发的入口函数。在这里调用do_it函数执行主逻辑。
> 7. 卸载函数：实现了ufusr_ask_unload函数，返回立即卸载标志，表示二次开发应用程序可以立即卸载。
>
> 综上所述，这段代码通过遍历零件中的所有UDO，并输出每个UDO的详细数据，展示了如何使用NX的UDO API进行二次开发。
>
