### 【1551】list where used in session 列出当前会话中部件的使用情况

#### 代码

```cpp
    /*HEAD LIST_WHERE_USED_IN_SESSION CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_assem.h>  
    #include <uf_part.h>  
    #include <uf_modl.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据注释内容，UF_print_syslog 是 V18 新增的函数。 */  
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
    static void add_parents_to_list(tag_t component, uf_list_p_t parent_list)  
    {  
        int  
            ii,  
            n;  
        tag_t  
            *parents;  
        n = UF_ASSEM_where_is_part_used(component, &parents);  
        for (ii = 0; ii < n; ii++)  
        {  
            UF_CALL(UF_MODL_put_list_item(parent_list, parents[ii]));  
            add_parents_to_list(parents[ii], parent_list);  
        }  
        if (n) UF_free(parents);  
    }  
    static int ask_where_used_in_session(tag_t part, uf_list_p_t *parents)  
    {  
        int  
            n;  
        UF_CALL(UF_MODL_create_list(parents));  
        add_parents_to_list(part, *parents);  
        UF_CALL(UF_MODL_ask_list_count(*parents, &n));  
        return n;  
    }  
    #define WRITE(X)   UF_UI_open_listing_window(); UF_UI_write_listing_window(X)  
    static void do_it(void)  
    {  
        logical  
            tceng;  
        int  
            ii,  
            n;  
        tag_t  
            parent,  
            work = UF_ASSEM_ask_work_part();  
        uf_list_p_t  
            used;  
        char  
            name[MAX_FSPEC_SIZE + 1],  
            msg[UF_UI_MAX_STRING_LEN + 1];  
        UF_CALL(UF_is_ugmanager_active(&tceng));  
        n = ask_where_used_in_session(work, &used);  
        UF_PART_ask_part_name(work, name);  
        sprintf(msg, "%s is used in:\n", name);  
        WRITE(msg);  
        for (ii = 0; ii < n; ii++)  
        {  
            UF_CALL(UF_MODL_ask_list_item(used, ii, &parent));  
            UF_PART_ask_part_name(parent, name);  
            if (tceng) UF_PART_name_for_display(name, name);  
            sprintf(msg, "  %s\n", name);  
            WRITE(msg);  
        }  
        UF_CALL(UF_MODL_delete_list(&used));  
    }  
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

> 这段代码是一个NX Open的二次开发示例，主要功能是查询当前会话中指定部件的使用情况，并打印出所有父组件的名称。
>
> 关键点包括：
>
> 1. 定义了一个错误报告函数report_error，用于在函数调用失败时打印错误信息。
> 2. 定义了一个递归函数add_parents_to_list，用于递归查询指定组件的所有父组件，并将它们添加到一个列表中。
> 3. 定义了一个函数ask_where_used_in_session，用于创建一个列表，查询指定部件的父组件，并递归调用add_parents_to_list函数填充列表。
> 4. 在主函数do_it中，首先检查UG管理器是否激活。然后查询当前工作部件，创建一个父组件列表，并打印出列表中的所有父组件名称。
> 5. 定义了ufusr函数作为二次开发的入口点，在初始化NX Open环境后调用do_it函数，并在完成后终止NX Open环境。
> 6. 定义了ufusr_ask_unload函数，返回立即卸载标志，表示二次开发模块不需要在NX会话中保留。
>
> 总的来说，这段代码通过递归查询实现了查询指定部件在当前会话中的使用情况，并打印出了所有父组件的名称。
>
