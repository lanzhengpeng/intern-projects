### 【1488】isotrim face 使用等参数线修剪实体面

#### 代码

```cpp
    /*HEAD ISOTRIM_FACE CCC UFUN */  
    #include <stdio.h>  
    #include <stdlib.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_modl.h>  
    #include <uf_object_types.h>  
    #include <uf_ui.h>  
    #define ECHO(X)    (printf("%s = %d\n", #X, X))  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据上下文，UF_print_syslog是V18版本新增的函数，仅用于打印系统日志。 */  
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
    static tag_t select_a_sheet(char *prompt)  
    {  
        double  
            cp[3];  
        int  
            sheet = UF_MODL_SHEET_BODY,  
            resp;  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog( prompt, "",  
            UF_UI_SEL_SCOPE_WORK_PART, mask_body_type, &sheet, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    static tag_t isotrim_face(tag_t object, double trim_param[4])  
    {  
        tag_t  
            new_object = object;  
        if (!UF_CALL(UF_MODL_isotrim_face(&new_object,trim_param)))  
            return(new_object);  
        else  
            return(NULL_TAG);  
    }  
    static void do_it(void)  
    {  
        tag_t  
            first_face,  
            sheet;  
        double  
            trim_param[4] = { -0.1, 1.1, -0.1, 1.1 };  
        while ((sheet = select_a_sheet("Select a sheet body")) != NULL_TAG)  
        {  
            ECHO(sheet);  
            first_face = isotrim_face(sheet, trim_param);  
            ECHO(first_face);  
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

> 这段代码是NX二次开发代码，主要实现了以下几个功能：
>
> 1. 错误报告函数：通过定义report_error函数，实现了对UF函数调用错误的捕捉和报告功能。
> 2. 选择体类型过滤函数：定义了filter_body_type函数，用于在选择对话框中根据给定的体类型过滤选择。
> 3. 设置选择掩码函数：通过mask_body_type函数，实现了在选择对话框中只显示和选择特定类型的体。
> 4. 选择面体函数：通过select_a_sheet函数，实现了弹出一个选择对话框，让用户选择一个面体。
> 5. 等参数裁剪面函数：定义了isotrim_face函数，用于对选中的面体进行等参数裁剪操作。
> 6. 执行主函数：在do_it函数中，循环调用select_a_sheet选择面体，然后调用isotrim_face进行等参数裁剪操作。
> 7. UF初始化和终止：在ufusr函数中实现了对NX的初始化和终止调用。
> 8. 卸载函数：定义了ufusr_ask_unload函数，实现了在代码卸载时的处理。
>
> 总体来说，这段代码实现了选择面体并对其进行等参数裁剪的功能，具有较好的错误处理和用户交互。
>
