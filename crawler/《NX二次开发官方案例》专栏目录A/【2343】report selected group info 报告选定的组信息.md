### 【2343】report selected group info 报告选定的组信息

#### 代码

```cpp
    #include <stdio.h>  
    #include <string.h>  
    #include <stdarg.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_obj.h>  
    #include <uf_object_types.h>  
    #include <uf_disp.h>  
    #include <uf_group.h>  
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
    static int mask_for_groups(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_group_type, 0, 0 };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_group(char *prompt)  
    {  
        int  
            ii,  
            member_count,  
            resp;  
        double  
            cp[3];  
        tag_t  
            *member_list,  
            group,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog(prompt, "",  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_groups, NULL, &resp,  
            &group, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(group, 0));  
            UF_CALL(UF_GROUP_ask_group_data(group, &member_list, &member_count));  
            for (ii = 0; ii < member_count; ++ii)  
               UF_CALL(UF_DISP_set_highlight(member_list[ii], 0));  
            if (member_count > 0) UF_free(member_list);  
            return group;  
        }  
        else return NULL_TAG;  
    }  
    static void report_group_info(tag_t object)  
    {  
        int  
            n;  
        char  
            obj_name[MAX_ENTITY_NAME_SIZE + 1];  
        tag_t  
            *members;  
        ECHO("group %d", object);  
        if (!UF_OBJ_ask_name(object, obj_name))  
            ECHO(", named %s, ", obj_name);  
        UF_CALL(UF_GROUP_ask_group_data(object, &members, &n));  
        ECHO(" has %d group members\n", n);  
        if (n > 0) UF_free(members);  
    }  
    static void do_it(void)  
    {  
        tag_t  
            group;  
        while ((group = select_a_group("Select a group")) != NULL_TAG)  
            report_group_info(group);  
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

> 这段代码是一个NX二次开发示例，其主要功能是选择并报告组信息。
>
> 主要功能包括：
>
> 1. 定义了ECHO宏，用于在列表窗口和系统日志中输出信息。
> 2. 定义了UF_CALL宏，用于执行UF函数并报告错误。
> 3. 定义了mask_for_groups函数，用于设置选择掩码，仅选择组类型的对象。
> 4. 定义了select_a_group函数，使用单选对话框让用户选择一个组对象，并返回该组对象的tag。
> 5. 定义了report_group_info函数，用于报告组对象的信息，包括组名和成员数量。
> 6. 定义了do_it函数，循环调用select_a_group获取组对象，然后调用report_group_info报告组信息。
> 7. 定义了ufusr函数，初始化后调用do_it，然后终止。
> 8. 定义了ufusr_ask_unload函数，返回立即卸载标志。
>
> 整体来说，这段代码实现了在NX中选择组对象并报告组信息的功能，展示了NX二次开发中常用的函数调用、错误处理、对话框使用等技巧。
>
