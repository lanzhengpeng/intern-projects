### 【1056】demo select CAE nodes Demo选择CAE节点

#### 代码

```cpp
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_part.h>  
    #include <uf_undo.h>  
    #include <stdarg.h>  
    static void ECHO(char *format, ...)  
    {  
        char msg[UF_UI_MAX_STRING_LEN+1];  
        va_list args;  
        va_start(args, format);  
        vsprintf(msg, format, args);  
        va_end(args);  
        UF_UI_open_listing_window();  
        UF_UI_write_listing_window(msg);  
        UF_print_syslog(msg, FALSE);  
    }  
    #define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))  
    static int report_error( char *file, int line, char *call, int irc)  
    {  
        if (irc)  
        {  
            char err[133];  
            UF_get_fail_message(irc, err);  
            ECHO("*** ERROR code %d at line %d in %s:\n",  
                irc, line, file);  
            ECHO("+++ %s\n", err);  
            ECHO("%s;\n", call);  
        }  
        return(irc);  
    }  
    #define WRITE_D(X) ECHO("%s = %d\n", #X, X);  
    /* qq3123197280 */  
    static int mask_for_cae_nodes(UF_UI_selection_p_t select, void *type)  
    {  
    //  These are new in NX7.5.1 - see uf_ui_type.h:  
        UF_UI_mask_t  
            mask = { UF_pseudo_object_type, UF_pseudo_CAE_subtype,  
                UF_pseudo_CAE_node};  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static int select_cae_nodes(char *prompt, tag_t **cae_nodes)  
    {  
        int  
            cnt,  
            resp;  
        UF_UNDO_mark_id_t  
            mark_id;  
        UF_CALL(UF_UNDO_set_mark(UF_UNDO_invisible, NULL, &mark_id));  
        UF_CALL(UF_UI_select_with_class_dialog("Select CAE Nodes", prompt,  
            UF_UI_SEL_SCOPE_WORK_PART, mask_for_cae_nodes, NULL,  
            &resp, &cnt, cae_nodes));  
    /*  See PR 6469859  
        The number of nodes can be very large so don't do this:  
        for (ii = 0; ii < cnt; ii++)  
            UF_CALL(UF_DISP_set_highlight((*cae_nodes)[ii], FALSE));  
        Even this can take forever:  
        UF_CALL(UF_PART_cleanup(UF_PART_cleanup_highlight));  
        This seems to work but the nodes cannot be selected anymore:  
        UF_DISP_regenerate_display();  
        This works great however both UNDO and regen are needed:  
    */  
        UF_CALL(UF_UNDO_undo_to_mark(mark_id, NULL));  
        UF_CALL(UF_UNDO_delete_mark(mark_id, NULL));  
        UF_DISP_regenerate_display();  
        return cnt;  
    }  
    static void do_it(void)  
    {  
        int  
            current_app,  
            n_nodes;  
        tag_t  
            *nodes;  
        UF_CALL(UF_ask_application_module(&current_app));  
        if (current_app !=  UF_APP_SFEM)  
        {  
            ECHO("%s\n", "Please start Advanced Simulation before using this app.");  
            return;  
        }  
        while ((n_nodes = select_cae_nodes("Demo", &nodes)) > 0)  
        {  
            ECHO("selected %d nodes\n", n_nodes);  
            UF_free(nodes);  
        }  
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

> 这段代码是一个NX的二次开发示例，主要功能是实现选择CAE节点。具体包括以下关键内容：
>
> 1. 定义了一个ECHO宏，用于在列表窗口和系统日志中输出信息。
> 2. 定义了一个UF_CALL宏，用于调用NX API函数，并处理错误。
> 3. 实现了一个mask_for_cae_nodes函数，用于设置选择掩码，以便只选择CAE节点。
> 4. 实现了一个select_cae_nodes函数，用于打开选择对话框，并调用mask_for_cae_nodes设置选择掩码，然后返回选择的节点数量和数组。
> 5. 实现了一个do_it函数，用于检查当前模块是否为高级仿真，然后循环调用select_cae_nodes选择节点，并打印节点数量。
> 6. 实现了ufusr函数，用于初始化NX API，调用do_it，然后终止NX API。
> 7. 实现了ufusr_ask_unload函数，用于卸载用户函数。
>
> 总的来说，这段代码通过NX的UI选择API实现了选择CAE节点的功能，并包含错误处理、日志输出等辅助功能。
>
