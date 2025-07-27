### 【2243】report object preferences 报告对象首选项

#### 代码

```cpp
    #include <stdio.h>  
    #include <string.h>  
    #include <stdarg.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_drf.h>  
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
    /* qq3123197280 */  
    static int mask_for_dim_or_draft(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask[4] = { { UF_drafting_entity_type, UF_all_subtype, 0 },  
                        { UF_dimension_type, UF_all_subtype, 0 },  
                        { UF_tabular_note_type, UF_all_subtype, 0 },  
                        { UF_tol_feature_instance_type, 0, 0} };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 4, mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_dim_or_draft(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select dimension or drafting aid",  
            prompt, UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_dim_or_draft, NULL,  
            &resp, &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    static void report_object_preferences(tag_t object)  
    {  
        int  
            ii,  
            mpi[100];  
        double  
            mpr[70];  
        char  
            rad[27],  
            dia[27];  
        UF_CALL(UF_DRF_ask_object_preferences(object, mpi, mpr, rad, dia));  
        for (ii = 0; ii < 100; ii++)  
            ECHO("mpi[%d] = %d\n", ii, mpi[ii]);  
        for (ii = 0; ii < 70; ii++)  
            ECHO("mpr[%d] = %f\n", ii, mpr[ii]);  
        ECHO("radius_val = \"%s\"\n", rad);  
        ECHO("diameter_val = \"%s\"\n", dia);  
    }  
    static void do_it(void)  
    {  
        tag_t  
            object;  
        while ((object = select_a_dim_or_draft("Report Preferences")) != NULL_TAG)  
            report_object_preferences(object);  
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

> 这段代码是一个NX Open API的二次开发示例，主要功能是查询并打印NX对象（如尺寸、草图等）的参数设置。
>
> 代码主要包含以下几个部分：
>
> 1. 错误报告宏：通过宏UF_CALL来检查NX API函数的返回值，如果有错误则打印错误信息。
> 2. 查询掩码设置函数：mask_for_dim_or_draft函数用于设置查询掩码，只选择尺寸、草图、注释等对象。
> 3. 选择对话框函数：select_a_dim_or_draft函数用于弹出一个选择对话框，让用户选择一个对象，并返回所选对象的tag。
> 4. 打印参数设置函数：report_object_preferences函数用于查询并打印一个对象的参数设置，包括整数参数、实数参数、半径值、直径值等。
> 5. 主函数：do_it函数循环调用选择对话框，获取用户选择的对象，并打印其参数设置。
> 6. API初始化和终止：ufusr函数用于初始化和终止NX Open API。
> 7. 卸载函数：ufusr_ask_unload函数用于卸载二次开发应用程序。
>
> 总的来说，这段代码实现了通过二次开发来查询和打印NX对象的参数设置的功能。
>
