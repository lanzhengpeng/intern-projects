### 【0897】create smart datum plane normal to curve at specified percentage 创建与曲线指定百分比处法线平行的智能基准面

#### 代码

```cpp
    /*HEAD CREATE_SMART_DATUM_PLANE_NORMAL_TO_CURVE_AT_SPECIFIED_PERCENTAGE CCC UFUN */  
    /*--------------------------------------------------------  
        Please note that when you are prompted for a number,  
        the percentage should be expressed as a decimal value.  
        For example, 25% should be entered as 0.25  
        Once the datum plane has been created,  
        you can edit the created expression and  
        see it move.  
    --------------------------------------------------------*/  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_modl.h>  
    #include <uf_so.h>  
    #include <uf_obj.h>  
    #include <uf_object_types.h>  
    #define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))  
    /* 里海 */  
    static int report_error( char *file, int line, char *call, int irc)  
    {  
        if (irc)  
        {  
            char err[133],  
                 msg[133];  
            sprintf(msg, "*** ERROR code %d at line %d in %s:\n+++ ",  
                irc, line, file);  
            UF_get_fail_message(irc, err);  
        /*  NOTE:  UF_print_syslog is new in V18 译:根据文档，UF_print_syslog 是 V18 版本中新增的函数。

该函数的作用是打印系统日志消息。

系统日志消息通常包括有关系统运行状况的信息，例如硬件状态、软件状态、网络连接等。

因此，UF_print_syslog 函数可以将这些系统日志消息输出到控制台或其他日志文件，以便用户可以查看系统运行情况。

总的来说，该函数提供了系统日志的打印功能，使得用户可以更好地了解系统的运行状态。 */  
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
    /* 里海 */  
    static int mask_for_curves(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask[5] = { { UF_line_type, 0, 0 },  
                        { UF_circle_type, 0, 0 },  
                        { UF_conic_type, 0, 0 },  
                        { UF_spline_type, 0, 0 },  
                        { UF_solid_type, 0, UF_UI_SEL_FEATURE_ANY_EDGE } };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 5, mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    /* 里海 */  
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
    /* 里海 */  
    static logical prompt_for_a_number(char *prompt, char *item, double *number)  
    {  
        int  
            irc,  
            resp;  
        char  
            menu[1][16];  
        double  
            da[1];  
        strcpy(&menu[0][0], item);  
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
            curve,  
            expTag1,  
            scalar_from_exp,  
            smart_dirr,  
            smart_point,  
            datum_plane;  
        double  
            expval = 0.;  
        char  
            expstr[30] = { "" };  
        while ((curve=select_a_curve("Select Curve:"))!=NULL_TAG &&  
                prompt_for_a_number("Percentage of Curve:",   
                "Percentage:", &expval ))   
        {  
            sprintf( expstr, "%f", expval );  
            UF_CALL(UF_MODL_create_exp_tag(expstr, &expTag1));  
            UF_CALL(UF_SO_create_scalar_exp( curve,UF_SO_update_within_modeling,   
                                                  expTag1 , &scalar_from_exp ));  
            UF_CALL(UF_SO_create_point_on_curve(curve,  
             UF_SO_update_within_modeling, curve, scalar_from_exp, &smart_point));  
            UF_CALL(UF_SO_set_visibility_option(smart_point, UF_SO_invisible));  
            UF_CALL(UF_SO_create_dirr_on_curve(curve,  
             UF_SO_update_within_modeling, curve, scalar_from_exp,  
                    UF_SO_dirr_on_curve_tangent, FALSE, &smart_dirr));  
            UF_CALL(UF_MODL_create_point_dirr_dplane( smart_point, smart_dirr,   
                                                              &datum_plane ));  
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

> 这段代码是用于在NX中创建智能数据平面，以指定曲线在特定百分比处的法线。主要功能如下：
>
> 1. 选择曲线：通过对话框提示用户选择曲线。
> 2. 提示百分比：通过对话框提示用户输入曲线的百分比。
> 3. 创建表达式：根据输入的百分比创建表达式。
> 4. 创建点：在曲线的指定百分比处创建一个点。
> 5. 创建方向：在曲线的指定百分比处创建一个切线方向。
> 6. 创建平面：以创建的点为原点，创建一个与曲线切线方向垂直的智能数据平面。
> 7. 错误处理：使用UF_CALL宏来调用NX API，并在出错时记录错误信息。
> 8. 初始化和终止：在ufusr函数中调用UF_initialize和UF_terminate来初始化和终止NX。
> 9. 卸载：在ufusr_ask_unload函数中返回立即卸载标志。
>
> 总体来说，这段代码通过二次开发实现了在NX中创建指定曲线在特定百分比处的法线平面，并使用对话框交互获取用户输入，实现了一个简单的二次开发示例。
>
