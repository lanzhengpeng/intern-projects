### 【2781】set object translucency 设置对象透明度

#### 代码

```cpp
    /*HEAD SET_OBJECT_TRANSLUCENCY CCC UFUN */  
    /*New in NX 2.0.2 */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_obj.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档描述，UF_print_syslog是V18版本新增的函数。它的作用是打印系统日志。这个函数可以将日志信息输出到标准输出，同时也可以将其输出到系统日志中。这个函数的参数包括日志信息的优先级、日志信息的格式化字符串以及后续的参数。通过这个函数，用户可以将自定义的日志信息输出到系统日志中，以便进行调试和记录。总的来说，UF_print_syslog是一个方便的功能，用于记录系统日志信息。 */  
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
    static logical prompt_for_an_integer(char *prompt, char *item, int *number)  
    {  
        int  
            irc,  
            resp;  
        char  
            menu[1][16];  
        int  
            da[1];  
        strncpy(&menu[0][0], item, 15);  
        menu[0][15] = '\0';  
        da[0] = *number;  
        resp = uc1607(prompt, menu, 1, da, &irc);  
        if (resp == 3 || resp == 4)  
        {  
            *number = da[0];  
            return TRUE;  
        }  
        else return FALSE;  
    }  
    static void do_it(void)  
    {  
        tag_t  
            object;  
        UF_OBJ_translucency_t    
            translucency = 100;  
        while (((object = select_a_solid("Select solid")) != NULL_TAG) &&  
            prompt_for_an_integer("Translucency Percentage:", "Translucency",  
                &translucency))  
        UF_CALL(UF_OBJ_set_translucency( object, translucency ));  
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

> 这段代码是NX二次开发代码，主要用于设置NX对象透明度。主要功能包括：
>
> 1. 定义了错误报告函数report_error，用于在函数调用出错时打印错误信息。
> 2. 定义了过滤函数filter_body_type，用于过滤选择，只选择实体类型为参数指定类型的对象。
> 3. 定义了mask_body_type函数，用于设置选择过滤器，只选择实体类型。
> 4. 定义了select_a_solid函数，用于选择一个实体，并返回其tag。
> 5. 定义了prompt_for_an_integer函数，用于提示用户输入一个整数，并返回用户输入的值。
> 6. 定义了do_it函数，用于选择一个实体，提示用户输入透明度百分比，然后设置该实体的透明度。
> 7. 主函数ufusr初始化NX环境，调用do_it函数实现功能，最后终止NX环境。
> 8. ufusr_ask_unload函数返回立即卸载，表示该二次开发模块可以立即卸载。
>
> 总体来说，该代码实现了在NX中根据用户选择设置实体透明度的功能，用户可以交互式选择实体并设置其透明度百分比。
>
