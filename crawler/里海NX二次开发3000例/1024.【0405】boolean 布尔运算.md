### 【0405】boolean 布尔运算

#### 代码

```cpp
    /*HEAD BOOLEAN CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_disp.h>  
    #include <uf_modl.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:根据文档内容，UF_print_syslog 是 V18 新增的函数。 */  
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
    static UF_FEATURE_SIGN ask_sign(void)  
    {  
        int  
          choice;  
        char  
           options[3][38] = { "UNITE", "SUBTRACT", "INTERSECT"};  
        choice = uc1603("SELECT DESIRED OPTION",0,options,3);  
        if (choice == 5) return UF_POSITIVE;  
        if (choice == 6) return UF_NEGATIVE;  
        if (choice == 7) return UF_UNSIGNED;  
        return (UF_FEATURE_SIGN)0;  
    }  
    static void do_it(void)  
    {  
        tag_t  
            tool_tag,  
            trgt_tag;  
        UF_FEATURE_SIGN  
           sign;  
        while ((( trgt_tag = select_a_solid("select target body")) != NULL_TAG) &&  
               (( tool_tag = select_a_solid("select tool body")) != NULL_TAG) &&  
               (( sign = ask_sign()) != 0))  
            UF_CALL(UF_MODL_operations(trgt_tag, tool_tag, sign));  
    }  
    void ufusr(char *param, int *retcode, int paramLen)  
    {  
        if (!UF_CALL(UF_initialize()))  
        {  
            do_it();  
            UF_CALL(UF_terminate());  
        }  
    }  
    int ufusr_ask_unload(void)  
    {  
        return (UF_UNLOAD_IMMEDIATELY);  
    }

```

#### 代码解析

> 这段代码是NX的二次开发代码，主要功能如下：
>
> 1. 定义了一个错误报告函数report_error，用于打印出错误代码、错误信息和出错的函数调用。
> 2. 定义了一个过滤函数filter_body_type，用于在UF_UI选择过程中过滤出指定类型的体。
> 3. 定义了一个设置选择掩码的函数mask_body_type，用于设置选择过程只允许选择指定类型的体。
> 4. 定义了一个选择实体的函数select_a_solid，用于在提示下选择一个实体并返回其tag。
> 5. 定义了一个询问特征符号的函数ask_sign，用于在提示下选择特征的符号（正、负、无符号）。
> 6. 定义了主要函数do_it，循环执行以下操作：选择目标体、选择工具体、选择特征符号，然后调用UF_MODL_operations进行布尔运算。
> 7. 定义了ufusr函数，用于初始化和终止NX，并调用do_it执行主要功能。
> 8. 定义了ufusr_ask_unload函数，返回立即卸载的标志。
>
> 综上所述，这段代码实现了在NX中通过UI选择目标体、工具体和特征符号，然后进行布尔运算的功能。
>
