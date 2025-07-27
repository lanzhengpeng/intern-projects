### 【1027】delete group and keep group members 删除组并保留组成员

#### 代码

```cpp
    /*HEAD DELETE_GROUP_AND_KEEP_GROUP_MEMBERS CCC UFUN */  
    #include <stdio.h>  
    #include <stdlib.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_group.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，UF_print_syslog 是 V18 中的新增功能。 */  
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
        UF_CALL(UF_UI_select_with_single_dialog(prompt, "",  
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
        while ((group = select_a_group("Select a group")) != NULL_TAG)  
        {  
            /** disbands the group and deletes the group tag **/  
            UF_CALL(UF_GROUP_ungroup_all(group));   
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

> 这段NX二次开发代码的主要功能是删除一个组，同时保留组中的成员。以下是代码的详细功能介绍：
>
> 1. 错误报告函数：report_error用于输出错误信息，包括错误码、错误行号、文件名等。
> 2. 组选择函数：mask_for_groups用于设置选择过滤条件，只允许选择组类型的对象。
> 3. 选择组函数：select_a_group用于弹出一个对话框，让用户选择一个组。选中的组将被高亮显示，同时组中的所有成员也会被高亮显示。
> 4. 执行删除组函数：do_it中有一个循环，不断调用select_a_group让用户选择组，然后调用UF_GROUP_ungroup_all函数解散并删除选中的组，但保留组中的成员。
> 5. 入口函数：ufusr是程序的入口，首先调用UF_initialize初始化NX，然后调用do_it执行主体逻辑，最后调用UF_terminate终止NX。
> 6. 卸载函数：ufusr_ask_unload用于设置立即卸载本代码。
>
> 总的来说，这段代码通过二次开发实现了NX中删除组但保留组内成员的功能，通过循环选择并删除组，实现了批量删除组的效果。
>
