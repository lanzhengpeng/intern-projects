### 【2344】report selected groups info 报告选中的组信息

#### 代码

```cpp
    /*HEAD REPORT_SELECTED_GROUPS_INFO CCC UFUN */  
    #include <stdio.h>  
    #include <stdlib.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_disp.h>  
    #include <uf_group.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog是V18版本中新增的函数。 */  
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
    static int mask_for_groups(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask[1] = { { UF_group_type, 0, 0 } };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static int select_groups(char *prompt, tag_t **groups)  
    {  
        int  
            cnt,  
            ii,  
            n = 0,  
            resp,  
            subtype,  
            type;  
        tag_t  
            *grps,  
            *objs;  
        UF_CALL(UF_UI_select_with_class_dialog(prompt, "",  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY,  
            mask_for_groups, NULL, &resp, &cnt, &objs));  
        grps = *groups = malloc(cnt * sizeof(tag_t));  
        for (ii = 0; ii < cnt; ii++)  
        {  
            UF_CALL(UF_DISP_set_highlight(objs[ii], FALSE));  
            UF_CALL(UF_OBJ_ask_type_and_subtype(objs[ii], &type, &subtype));  
            if (type == UF_group_type) grps[n++] = objs[ii];  
        }  
        UF_free(objs);  
        return n;  
    }  
    static void report_group_info(tag_t object)  
    {  
        int  
            n;  
        char  
            obj_name[MAX_ENTITY_NAME_SIZE + 1];  
        tag_t  
            *members;  
        printf("group %d", object);  
        if (!UF_OBJ_ask_name(object, obj_name))  
            printf(", named %s, ", obj_name);  
        UF_CALL(UF_GROUP_ask_group_data(object, &members, &n));  
        printf(" has %d group members\n", n);  
        if (n > 0) UF_free(members);  
    }  
    static void do_it(void)  
    {  
        int  
            ii,  
            n;  
        tag_t  
            *groups;  
        while ((n = select_groups("Select groups", &groups)) > 0)  
        {  
            for (ii = 0; ii < n; ii++) report_group_info(groups[ii]);  
            free(groups);  
        }  
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

> 这是段NX二次开发代码，主要功能是选择并报告用户选择的组的信息。代码主要包含以下功能：
>
> 1. 包含必要的NX Open头文件，定义了错误报告函数report_error，用于输出错误信息。
> 2. 定义了mask_for_groups函数，设置选择掩码，只选择组类型对象。
> 3. 定义了select_groups函数，用于打开选择对话框，让用户选择组，并返回选择的组的标签数组。
> 4. 定义了report_group_info函数，用于报告一个组的信息，包括名称和成员数量。
> 5. 定义了do_it函数，循环调用select_groups和report_group_info，让用户选择组并报告信息，直到用户取消选择。
> 6. 定义了ufusr函数，初始化NX Open，调用do_it函数，然后终止NX Open。
> 7. 定义了ufusr_ask_unload函数，用于卸载用户函数。
>
> 总体来说，这段代码实现了选择并报告NX组信息的功能，通过选择对话框让用户选择组，然后输出组的名称和成员数量。
>
