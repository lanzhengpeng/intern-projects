### 【0832】create point on curve at parm 在曲线上指定参数处创建点

#### 代码

```cpp
    /*HEAD CREATE_POINT_ON_CURVE_AT_PARM CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_so.h>  
    #include <uf_disp.h>  
    #include <uf_view.h>  
    #include <uf_point.h>  
    #include <uf_curve.h>  
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
    #define WRITE(X)   UF_UI_open_listing_window(); UF_UI_write_listing_window(X)  
    /* 里海 */  
    static int mask_for_curves(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask[4] = { { UF_line_type, 0, 0 },  
                        { UF_circle_type, 0, 0 },  
                        { UF_conic_type, 0, 0 },  
                        { UF_spline_type, 0, 0 } };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 4, mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    extern tag_t select_a_curve(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select a curve", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_curves, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    static logical prompt_for_a_number(char *prompt, char *item, double *number)  
    {  
        int  
            irc,  
            resp;  
        char  
            menu[1][16];  
        double  
            da[1];  
        strncpy(&menu[0][0], item, 16);  
        menu[0][15] = '\0';  
        da[0] = *number;  
        resp = uc1609(prompt, menu, 1, da, &irc);  
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
            curve = NULL_TAG,  
            scalar = NULL_TAG,  
            object = NULL_TAG,  
            point_feature = NULL_TAG;  
        double  
            parm = 0.;  
        UF_VIEW_ask_work_view( &object );  
        if( NULL_TAG == object ) return;  
        if((curve=select_a_curve("Pick a CURVE:"))!=NULL_TAG &&  
            (prompt_for_a_number("Enter Parm 0.0 to 1.0:", "Parm:", &parm)))  
        {  
            UF_CALL( UF_SO_create_scalar_double(object,   
                     UF_SO_update_within_modeling, parm, &scalar));  
            UF_CALL( UF_POINT_create_on_curve( curve, scalar, & point_feature ));  
            UF_CALL(UF_SO_set_visibility_option(point_feature, UF_SO_visible));  
        }  
    }  
    /* 里海 */  
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

> 这段代码是一个NX的二次开发程序，其主要功能是：
>
> 1. 定义了错误报告函数report_error，用于在调用UF函数出错时输出错误信息。
> 2. 定义了创建曲线选择掩码的函数mask_for_curves，允许用户选择直线、圆、圆锥曲线或样条曲线。
> 3. 定义了选择曲线的函数select_a_curve，使用对话框让用户选择一条曲线，并高亮显示。
> 4. 定义了提示用户输入参数的函数prompt_for_a_number，用于获取用户输入的参数值。
> 5. 定义了主函数do_it，先获取当前工作视图，然后让用户选择一条曲线和一个参数值，在曲线上创建一个点特征。
> 6. 定义了ufusr函数，作为程序的入口，初始化环境后调用do_it函数，最后结束环境。
> 7. 定义了卸载函数ufusr_ask_unload，用于立即卸载程序。
>
> 总的来说，这段代码实现了在NX中根据用户选择的曲线和参数值，在该曲线上创建一个点的功能。
>
