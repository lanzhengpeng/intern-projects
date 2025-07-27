### 【2403】report udo link data 报告中文名为“报告用户定义对象链接数据”

#### 代码

```cpp
    /*HEAD REPORT_UDO_LINK_DATA CCC UFUN */  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:翻译如下：

请注意，UF_print_syslog是在V18版本中新增的功能。请只回答翻译，不要添加任何无关的评论。 */  
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
    static int allocate_memory(unsigned int nbytes, void **where)  
    {  
        int  
            resp;  
        *where = UF_allocate_memory(nbytes, &resp);  
        return resp;  
    }  
    static int ask_udo_links(tag_t udo, UF_UDOBJ_link_t **links)  
    {  
        int  
            ii,  
            n;  
        UF_UDOBJ_all_data_t  
            data;  
        UF_CALL(UF_UDOBJ_ask_udo_data(udo, &data));  
        n = data.num_links;  
        if (!UF_CALL(allocate_memory(n * sizeof(UF_UDOBJ_link_t), (void **)links)))  
            for (ii = 0; ii < n; ii++)  
                (*links)[ii] = data.link_defs[ii];  
        UF_CALL(UF_UDOBJ_free_udo_data(&data));  
        return n;  
    }  
    static void report_udo_link_info(tag_t udo)  
    {  
        int  
            ii,  
            n,  
            status,  
            subtype,  
            type;  
        char  
            out[UF_UI_MAX_STRING_LEN];  
        UF_UDOBJ_link_t  
            *link_defs;  
        UF_CALL(UF_UI_open_listing_window());  
        n = ask_udo_links(udo, &link_defs);  
        sprintf(out, "UDO %d has %d links\n", udo, n);  
        UF_CALL(UF_UI_write_listing_window(out));  
        for (ii = 0; ii < n; ii++)  
        {  
            if (link_defs[ii].assoc_ug_tag != NULL_TAG)  
            {  
                UF_CALL(UF_OBJ_ask_type_and_subtype(  
                    link_defs[ii].assoc_ug_tag, &type, &subtype));  
                status = UF_OBJ_ask_status(link_defs[ii].assoc_ug_tag);  
                sprintf(out, "  %d. type %d to %d (%d/%d %d) status %d\n", ii+1,  
                    link_defs[ii].link_type, link_defs[ii].assoc_ug_tag,  
                    type, subtype, status, link_defs[ii].object_status);  
                UF_CALL(UF_UI_write_listing_window(out));  
            }  
            else  
            {  
                sprintf(out, "  %d. type %d to NULL_TAG\n", ii+1,  
                    link_defs[ii].link_type);  
                UF_CALL(UF_UI_write_listing_window(out));  
            }  
        }  
        if (n > 0) UF_free(link_defs);  
    }  
    static void do_it(void)  
    {  
        tag_t  
            part = UF_PART_ask_display_part(),  
            udo = NULL_TAG;  
        while ((udo = ask_next_udo(part, udo)) != NULL_TAG)  
            report_udo_link_info(udo);  
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

> 这是段用于NX Open C++ API的二次开发代码，主要用于报告用户定义对象(UDO)的链接信息。代码的主要功能包括：
>
> 1. 定义了一个错误报告函数report_error，用于在API调用失败时打印错误信息。
> 2. 定义了ask_next_udo函数，用于遍历部件中的所有UDO，并返回下一个UDO的tag。
> 3. 定义了allocate_memory函数，用于申请内存。
> 4. 定义了ask_udo_links函数，用于获取指定UDO的所有链接信息。
> 5. 定义了report_udo_link_info函数，用于报告指定UDO的所有链接信息。
> 6. 定义了do_it函数，用于遍历当前显示部件中的所有UDO，并报告每个UDO的链接信息。
> 7. 定义了ufusr函数，这是NX Open C++ API的入口函数。在这个函数中，初始化API，调用do_it函数，然后终止API。
> 8. 定义了ufusr_ask_unload函数，用于设置二次开发程序的卸载模式。
>
> 整体来看，这段代码通过遍历当前显示部件中的所有UDO，并获取每个UDO的链接信息，实现了对UDO链接信息的查询和报告功能。
>
