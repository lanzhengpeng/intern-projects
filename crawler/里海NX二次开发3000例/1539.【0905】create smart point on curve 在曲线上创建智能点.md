### 【0905】create smart point on curve 在曲线上创建智能点

#### 代码

```cpp
    /*HEAD CREATE_SMART_POINT_ON_CURVE CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_disp.h>  
    #include <uf_object_types.h>  
    #include <uf_so.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档描述，UF_print_syslog 是一个新功能，在 V18 版本中首次出现。该功能允许用户直接打印系统日志信息。 */  
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
    static tag_t select_a_curve(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog(prompt, "",  
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
            scalar,  
            smart_point;  
        double  
            parm = 0.5;  
        while (((curve = select_a_curve("Select curve for point")) != NULL_TAG)  
            && (prompt_for_a_number("Enter parameter value", "parm", &parm)))  
        {  
            UF_CALL(UF_SO_create_scalar_double(curve, UF_SO_update_after_modeling,  
                parm, &scalar));  
            UF_CALL(UF_SO_create_point_on_curve(curve, UF_SO_update_after_modeling,  
                curve, scalar, &smart_point));  
            UF_CALL(UF_SO_set_visibility_option(smart_point, UF_SO_visible));  
        }  
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

> 这段NX二次开发代码的主要功能是在曲线上创建智能点。具体步骤如下：
>
> 1. 包含所需的头文件，定义错误报告函数report_error，用于在出错时打印错误信息。
> 2. 定义mask_for_curves函数，设置选择掩码，只允许选择线、圆、锥曲线和样条线。
> 3. 定义select_a_curve函数，提示用户选择一条曲线，并返回曲线的tag。
> 4. 定义prompt_for_a_number函数，提示用户输入一个参数值，并返回输入的数值。
> 5. 定义do_it函数，循环提示用户选择曲线并输入参数值，然后在该参数位置创建一个智能点，并设置智能点为可见。
> 6. 定义ufusr函数，初始化NX环境，调用do_it函数执行主逻辑，然后终止NX环境。
> 7. 定义ufusr_ask_unload函数，用于卸载NXOpen库。
>
> 总的来说，这段代码的主要作用是在曲线上创建智能点，通过用户交互选择曲线和参数值，然后调用NX的UF函数来实现。代码结构清晰，功能明确，是一个典型的NX二次开发示例。
>
