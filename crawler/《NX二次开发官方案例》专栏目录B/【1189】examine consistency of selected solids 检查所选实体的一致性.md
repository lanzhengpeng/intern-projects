### 【1189】examine consistency of selected solids 检查所选实体的一致性

#### 代码

```cpp
    /*HEAD EXAMINE_CONSISTENCY_OF_SELECTED_SOLIDS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_obj.h>  
    #include <uf_disp.h>  
    #include <uf_modl.h>  
    #define WRITE(X)   UF_UI_write_listing_window(X)  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档中的内容，UF_print_syslog是V18版本中新增的函数。文档中提到：

UF_print_syslog is new in V18

因此，UF_print_syslog是在V18版本中新引入的函数。 */  
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
    static int filter_body_type(tag_t object, int type[3], void *data,  
        UF_UI_selection_p_t select)  
    {  
        int  
            body_type;  
        UF_CALL(UF_MODL_ask_body_type(object, &body_type));  
        if (body_type == *(int *)data)  
            return (UF_UI_SEL_ACCEPT);  
        else  
            return (UF_UI_SEL_REJECT);  
    }  
    static int mask_body_type( UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_solid_type, 0, UF_UI_SEL_FEATURE_BODY };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)) &&  
            !UF_CALL(UF_UI_set_sel_procs(select, filter_body_type, NULL, type)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_solid(char *prompt)  
    {  
        double  
            cp[3];  
        int  
            solid = UF_MODL_SOLID_BODY,  
            resp;  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog( "Select a solid", prompt,  
            UF_UI_SEL_SCOPE_WORK_PART, mask_body_type, &solid, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    static void display_temporary_text(char *text, double loc[3])  
    {  
        UF_OBJ_disp_props_t  
            props = { 1, 0, UF_OBJ_NOT_BLANKED, UF_OBJ_WIDTH_NORMAL,  
                UF_OBJ_FONT_SOLID, FALSE};  
        UF_CALL(UF_DISP_display_temporary_text(NULL_TAG, UF_DISP_USE_WORK_VIEW,  
            text, loc, UF_DISP_MIDDLECENTER, &props, 0, 0));  
    }  
    static void number_objects(tag_t *objects, int n_objects)  
    {  
        int  
            ii;  
        double  
            loc[3];  
        char  
            msg[133];  
        for (ii = 0; ii < n_objects; ii++)  
        {  
            sprintf(msg, "%d", ii+1);  
            if (UF_OBJ_ask_name_origin(objects[ii], loc))  
            {  
                UF_CALL(UF_OBJ_set_name(objects[ii], "X"));  
                UF_CALL(UF_OBJ_ask_name_origin(objects[ii], loc));  
                UF_CALL(UF_OBJ_delete_name(objects[ii]));  
            }  
            display_temporary_text(msg, loc);  
        }  
    }  
    static void set_highlight_object_array(int n, tag_t *objects, int flag)  
    {  
        int ii;  
        for (ii = 0; ii < n; ii++)  
            UF_CALL(UF_DISP_set_highlight(objects[ii], flag));  
    }  
    static void do_it(void)  
    {  
        int  
            ii,  
            n_faults,  
            *fault_tokens;  
        tag_t  
            body,  
            *fault_tags;  
        char  
            msg[133];  
        while ((body = select_a_solid("Consistency Check")) != NULL_TAG)  
        {  
            UF_CALL(UF_MODL_ask_body_consistency(body, &n_faults, &fault_tokens,  
                &fault_tags));  
            if (n_faults > 0)  
            {  
                number_objects(fault_tags, n_faults);  
                set_highlight_object_array(n_faults, fault_tags, TRUE);  
                UF_UI_open_listing_window();  
                for (ii = 0; ii < n_faults; ii++)  
                {  
                    sprintf(msg, "%d.  ", ii+1);  
                    WRITE(msg);  
                    UF_get_fail_message(fault_tokens[ii], msg);  
                    WRITE(msg);  
                    WRITE("\n");  
                }  
                sprintf(msg, "%d faults found", n_faults);  
                uc1601(msg, TRUE);  
                set_highlight_object_array(n_faults, fault_tags, FALSE);  
                UF_free(fault_tokens);  
                UF_free(fault_tags);  
            }  
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

> 根据提供的代码，这是一个用于NX二次开发的UFUNC程序，其主要功能是检查用户选择的实体的一致性，并高亮显示问题区域。
>
> 主要步骤包括：
>
> 1. 使用UF_MODL_ask_body_type函数获取实体类型，并使用自定义的filter_body_type函数过滤出类型为UF_MODL_SOLID_BODY的实体。
> 2. 通过UF_UI_select_with_single_dialog函数让用户选择一个实体，并使用自定义的mask_body_type函数设置选择器。
> 3. 获取用户选择的实体后，使用UF_MODL_ask_body_consistency函数检查实体的一致性，并获取不一致区域。
> 4. 对不一致区域进行编号，并使用UF_DISP_set_highlight函数高亮显示问题区域。
> 5. 打开列表窗口，并输出不一致区域的详细信息。
> 6. 释放不一致区域相关的内存。
> 7. 在ufusr函数中初始化并执行主体逻辑，在ufusr_ask_unload函数中设置卸载标志。
>
> 该程序通过自定义选择器、高亮显示问题区域和输出详细信息，实现了检查实体一致性的功能。
>
