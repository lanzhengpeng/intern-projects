### 【1731】perform subtraction operation on selected solids 执行选定实体的减法操作

#### 代码

```cpp
    /*HEAD PERFORM_SUBTRACTION_OPERATION_ON_SELECTED_SOLIDS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_modl.h>  
    #include <uf_disp.h>  
    #include <uf_obj.h>  
    #include <uf_undo.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog是V18版本新增的函数，用于打印系统日志消息到标准输出。 */  
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
        UF_CALL(UF_UI_select_with_single_dialog( prompt, "",  
            UF_UI_SEL_SCOPE_WORK_PART, mask_body_type, &solid, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    static void do_it(void)  
    {  
        tag_t  
            target,  
            tool;  
        UF_UNDO_mark_id_t  
            mark;  
        while (((target = select_a_solid("Select target solid")) != NULL_TAG) &&  
               ((tool = select_a_solid("Select tool solid")) != NULL_TAG))  
        {  
            UF_CALL(UF_UNDO_set_mark(UF_UNDO_visible, "Boolean Subtract", &mark));  
            if (UF_CALL(UF_MODL_operations(target, tool, UF_NEGATIVE)))  
            {  
                UF_CALL(UF_UNDO_undo_to_mark(mark, NULL));  
                UF_CALL(UF_UNDO_delete_mark(mark, NULL));  
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

> 这段代码是一个NX Open的UF函数，用于在NX中实现布尔减运算操作。
>
> 主要功能包括：
>
> 1. 使用UF_MODL_operations函数对两个实体进行布尔减运算。
> 2. 提供了两个选择实体的对话框，分别用于选择目标实体和工具实体。
> 3. 使用UF_UNDO_set_mark和UF_UNDO_undo_to_mark实现操作的可撤销。
> 4. 在选择实体时，通过自定义过滤函数只允许选择实体类型为UF_MODL_SOLID_BODY的实体。
> 5. 使用UF_DISP_set_highlight函数高亮显示选中的实体。
> 6. 在出错时，使用UF_print_syslog和UF_UI_write_listing_window打印错误信息。
> 7. 每次操作前，都会调用UF_UNDO_set_mark设置撤销标记，在操作失败时，调用UF_UNDO_undo_to_mark撤销到标记处。
> 8. 在函数开始和结束时，调用UF_initialize和UF_terminate初始化和终止NX Open API。
> 9. 实现了ufusr和ufusr_ask_unload标准函数。
>
> 总体来说，这段代码实现了在NX中执行布尔减运算操作，并提供了用户交互界面、错误处理和撤销功能。
>
