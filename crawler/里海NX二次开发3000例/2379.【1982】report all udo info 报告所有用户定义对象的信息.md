### 【1982】report all udo info 报告所有用户定义对象的信息

#### 代码

```cpp
    /*HEAD REPORT_ALL_UDO_INFO CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_obj.h>  
    #include <uf_object_types.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，UF_print_syslog在V18中是新增的。

在V18版本中，新增了一个名为UF_print_syslog的函数。 */  
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
    static tag_t ask_next_udo(tag_t part, tag_t last)  
    {  
        UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_user_defined_object_type,  
            &last));  
        return (last);  
    }  
    static void report_udo_info(tag_t udo)  
    {  
        int  
            ii,  
            status,  
            subtype,  
            type;  
        UF_UDOBJ_all_data_t  
            data;  
        char  
            out[UF_UI_MAX_STRING_LEN];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(out,"\nUDO tag %d:\n", udo);  
        UF_CALL(UF_UI_write_listing_window(out));  
        UF_CALL(UF_UDOBJ_ask_udo_data(udo, &data));  
        sprintf(out, "\tUDO update status = %d\n", data.udo_status);  
        UF_CALL(UF_UI_write_listing_window(out));  
        sprintf(out, "\tUDO has %d integers\n", data.num_ints);  
        UF_CALL(UF_UI_write_listing_window(out));  
        for (ii = 0; ii < data.num_ints; ii++)  
        {  
            sprintf(out, "\t\t%d. %d\n", ii+1, data.ints[ii]);  
            UF_CALL(UF_UI_write_listing_window(out));  
        }  
        sprintf(out, "\tUDO has %d doubles\n", data.num_doubles);  
        UF_CALL(UF_UI_write_listing_window(out));  
        for (ii = 0; ii < data.num_doubles; ii++)  
        {  
            sprintf(out, "\t\t%d. %f\n", ii+1, data.doubles[ii]);  
            UF_CALL(UF_UI_write_listing_window(out));  
        }  
        sprintf(out, "\tUDO has %d strings\n", data.num_strings);  
        UF_CALL(UF_UI_write_listing_window(out));  
        for (ii = 0; ii < data.num_strings; ii++)  
        {  
            sprintf(out, "\t\t%d. %s\n", ii+1, data.strings[ii]);  
            UF_CALL(UF_UI_write_listing_window(out));  
        }  
        sprintf(out, "\tUDO has %d links\n", data.num_links);  
        UF_CALL(UF_UI_write_listing_window(out));  
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
        sprintf(out, "\tUDO has %d lengths\n", data.num_lengths);  
        UF_CALL(UF_UI_write_listing_window(out));  
        for (ii = 0; ii < data.num_lengths; ii++)  
        {  
            sprintf(out, "\t\t%d. %f\n", ii+1, data.lengths[ii]);  
            UF_CALL(UF_UI_write_listing_window(out));  
        }  
        sprintf(out, "\tUDO has %d areas\n", data.num_areas);  
        UF_CALL(UF_UI_write_listing_window(out));  
        for (ii = 0; ii < data.num_areas; ii++)  
        {  
            sprintf(out, "\t\t%d. %f\n", ii+1, data.areas[ii]);  
            UF_CALL(UF_UI_write_listing_window(out));  
        }  
        sprintf(out, "\tUDO has %d volumes\n", data.num_volumes);  
        UF_CALL(UF_UI_write_listing_window(out));  
        for (ii = 0; ii < data.num_volumes; ii++)  
        {  
            sprintf(out, "\t\t%d. %f\n", ii+1, data.volumes[ii]);  
            UF_CALL(UF_UI_write_listing_window(out));  
        }  
        UF_CALL(UF_UDOBJ_free_udo_data(&data));  
    }  
    static void do_it(void)  
    {  
        tag_t  
            part = UF_PART_ask_display_part(),  
            udo = NULL_TAG;  
        while ((udo = ask_next_udo(part, udo)) != NULL_TAG)  
            report_udo_info(udo);  
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

> 这段NX二次开发代码的主要功能是遍历当前活动部件中的所有用户定义对象(UDO)，并输出每个UDO的详细信息，包括其更新状态、整数、浮点数、字符串、链接、长度、面积和体积等属性。代码的主要流程如下：
>
> 1. 定义了一个宏UF_CALL，用于在调用NX API函数时捕获错误，并输出错误信息。
> 2. 定义了一个函数report_error，用于输出错误信息。
> 3. 定义了一个函数ask_next_udo，用于遍历部件中的下一个UDO，并返回其tag。
> 4. 定义了一个函数report_udo_info，用于输出指定UDO的详细信息。
> 5. 定义了一个函数do_it，用于遍历当前活动部件中的所有UDO，并调用report_udo_info函数输出每个UDO的信息。
> 6. 定义了ufusr函数，这是二次开发程序的入口函数。在这个函数中，首先初始化NX环境，然后调用do_it函数，最后关闭NX环境。
> 7. 定义了ufusr_ask_unload函数，用于在二次开发程序卸载时立即卸载。
>
> 综上所述，这段代码实现了对当前活动部件中所有UDO的遍历和详细信息输出，为用户提供了UDO的概览。
>
