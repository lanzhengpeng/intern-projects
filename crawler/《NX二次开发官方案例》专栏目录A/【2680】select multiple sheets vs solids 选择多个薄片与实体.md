### 【2680】select multiple sheets vs solids 选择多个薄片与实体

#### 代码

```cpp
    /*HEAD SELECT_MULTIPLE_SHEETS_VS_SOLIDS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_modl.h>  
    #define ECHO(X)    (printf("\t%s = %d\n", #X, X))  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog是V18版本中的新功能。 */  
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
    static int select_sheets(char *prompt, tag_t **sheets)  
    {  
        int  
            ii,  
            n,  
            sheet = UF_MODL_SHEET_BODY,  
            resp;  
        tag_t  
            *objs;  
        UF_CALL(UF_UI_select_with_class_dialog( prompt, "",  
            UF_UI_SEL_SCOPE_WORK_PART, mask_body_type, &sheet, &resp, &n, sheets));  
        objs = *sheets;  
        for (ii = 0; ii < n; ii++)  
            UF_CALL(UF_DISP_set_highlight(objs[ii], FALSE));  
        return n;  
    }  
    static int select_solids(char *prompt, tag_t **solids)  
    {  
        int  
            ii,  
            n,  
            solid = UF_MODL_SOLID_BODY,  
            resp;  
        tag_t  
            *objs;  
        UF_CALL(UF_UI_select_with_class_dialog( prompt, "",  
            UF_UI_SEL_SCOPE_WORK_PART, mask_body_type, &solid, &resp, &n, solids));  
        objs = *solids;  
        for (ii = 0; ii < n; ii++)  
            UF_CALL(UF_DISP_set_highlight(objs[ii], FALSE));  
        return n;  
    }  
    static void do_it(void)  
    {  
        int  
            n_sheets,  
            n_solids;  
        tag_t  
            *sheets,  
            *solids;  
        if ((n_sheets = select_sheets("Select sheet bodies", &sheets)) > 0)  
            UF_free(sheets);  
        if ((n_solids = select_solids("Select solid bodies", &solids)) > 0)  
            UF_free(solids);  
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

> 这段代码是一个NX的二次开发示例，主要功能是选择多个薄板(sheet)和实体(solid)。
>
> 主要思路如下：
>
> 1. 定义了一个错误报告函数report_error，用于在函数调用出错时打印错误信息。
> 2. 定义了一个过滤函数filter_body_type，用于在UF_UI_select_with_class_dialog中选择时过滤出特定类型的实体。
> 3. 定义了一个设置选择掩码的函数mask_body_type，用于设置在选择对话框中只显示特定类型的实体。
> 4. 定义了两个选择实体的函数select_sheets和select_solids，分别用于选择薄板和实体。这两个函数内部调用UF_UI_select_with_class_dialog进行选择，并设置对应的过滤条件。
> 5. 主函数do_it中调用select_sheets和select_solids函数，分别选择薄板和实体，并打印出选择结果的数量。
> 6. ufusr函数是NX的二次开发入口函数，初始化NX环境后调用do_it函数执行主体逻辑。
> 7. ufusr_ask_unload函数在NX关闭时调用，用于卸载二次开发。
>
> 这段代码通过NX的UI选择机制实现了对薄板和实体的选择功能，通过自定义过滤条件，用户可以灵活选择特定类型的实体。
>
