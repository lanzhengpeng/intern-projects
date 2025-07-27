### 【2217】report name of selected named group 选择命名组的报告名称

#### 代码

```cpp
    /*HEAD REPORT_NAME_OF_SELECTED_NAMED_GROUP CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_disp.h>  
    #include <uf_group.h>  
    #include <uf_obj.h>  
    #include <uf_modl.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))  
    /* qq3123197280 */  
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

注意：UF_print_syslog 是 V18 中的新增功能，请仅提供翻译，不要添加其他无关内容。 */  
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
    #define WRITE(X)   UF_UI_open_listing_window(); UF_UI_write_listing_window(X)  
    #define WRITE_S(X) (write_string_to_listing_window(#X, X))  
    /* qq3123197280 */  
    void write_string_to_listing_window(char *title, char *string)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = \"%s\"\n", title, string);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    /* qq3123197280 */  
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
    /* qq3123197280 */  
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
        UF_CALL(UF_UI_select_with_single_dialog("Select a group", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_groups, NULL, &resp,  
            &group, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(group, 0));  
            UF_CALL(UF_GROUP_ask_group_data(group, &member_list, &member_count));  
            for (ii = 0; ii < member_count; ii++)  
               UF_CALL(UF_DISP_set_highlight(member_list[ii], 0));  
            if (member_count > 0) UF_free(member_list);  
            return group;  
        }  
        else return NULL_TAG;  
    }  
    static void do_it(void)  
    {  
        tag_t  
            group = NULL_TAG;  
       char   
            groupname[30+1] = { "" };  
        while ((group = select_a_group("Select a group")) != NULL_TAG)  
        {  
            UF_CALL( UF_OBJ_ask_name( group, groupname ) );  
            WRITE_S( groupname );  
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

> 这段代码是用于NX软件的二次开发代码，其功能是选择并打印组名。
>
> 主要特点包括：
>
> 1. 使用UF函数进行NX操作，如UF_DISP_set_highlight用于设置高亮，UF_GROUP_ask_group_data用于获取组成员，UF_OBJ_ask_name用于获取对象名称等。
> 2. 定义了错误报告函数report_error，用于打印错误信息。
> 3. 定义了写列表窗口函数write_string_to_listing_window，用于向列表窗口输出信息。
> 4. 定义了组选择函数select_a_group，用于让用户选择一个组，并返回组标签。
> 5. 定义了主函数do_it，循环调用select_a_group函数选择组，然后打印组名。
> 6. 定义了ufusr函数作为NX二次开发程序的入口点。
> 7. 定义了ufusr_ask_unload函数，用于在程序卸载时立即执行。
>
> 综上所述，这段代码实现了选择组并打印组名的功能，采用了NX提供的UF函数进行操作，并包含了错误处理和用户交互等功能。
>
