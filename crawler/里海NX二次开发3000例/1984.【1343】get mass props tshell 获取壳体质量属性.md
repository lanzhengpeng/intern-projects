### 【1343】get mass props tshell 获取壳体质量属性

#### 代码

```cpp
    /*HEAD GET_MASS_PROPS_TSHELL CCC UFUN */  
    #include <stdio.h>  
    #include <stdlib.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_modl.h>  
    #include <uf_object_types.h>  
    #include <uf_part.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog是V18版本中的新功能，主要用于打印系统日志。 */  
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
    static void do_it(void)  
    {  
        tag_t  
            body_tag[1];  
        double  
            acc_value[11] = {0.9, 0,0,0,0,0,0,0,0,0,0},  
            mass_props[47],  
            stats[13];  
        while ((body_tag[0] = select_a_sheet("pick a sheet")) != NULL_TAG)  
        {  
            UF_CALL(UF_MODL_ask_mass_props_3d(body_tag, 1, 2, 1, 0.2829, 2,  
                acc_value, mass_props, stats));  
            printf("Surface Area is : %f\n", mass_props[0]);  
        }  
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

> 这段NX二次开发代码的主要功能是从NX零件中选择钣金实体，并计算钣金实体的质量属性，包括表面积等。代码的关键步骤包括：
>
> 1. 定义错误报告函数report_error，用于在函数调用出错时打印错误信息。
> 2. 定义筛选函数filter_body_type，用于在UI选择时只允许选择钣金实体。
> 3. 定义遮罩函数mask_body_type，设置UI选择时只允许选择钣金实体。
> 4. 定义选择函数select_a_sheet，用于弹窗让用户选择一个钣金实体。
> 5. 定义主函数do_it，循环调用select_a_sheet选择钣金实体，然后计算钣金实体的质量属性，并打印表面积。
> 6. 定义ufusr函数，初始化NX，调用do_it，然后终止NX。
> 7. 定义卸载函数ufusr_ask_unload，用于卸载NX插件。
>
> 总体来说，该代码实现了在NX中计算钣金实体的质量属性的功能，主要包括错误处理、钣金实体选择、质量属性计算等步骤。
>
