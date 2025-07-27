### 【2632】select anything but a curve 选择任何非曲线

#### 代码

```cpp
    /*HEAD SELECT_ANYTHING_BUT_A_CURVE CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_disp.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，可以总结如下：

1. UF_print_syslog是V18版本中新增的函数。

2. 该函数用于打印系统日志信息。

3. 在调用该函数时，需要提供日志信息作为参数。

4. 该函数是用户自定义的函数。

5. 该函数属于V18版本新增的函数。

综上所述，UF_print_syslog是V18版本中新增的一个用于打印系统日志信息的用户自定义函数。 */  
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
    static int mask_for_no_curves(UF_UI_selection_p_t select, void *user_data)  
    {  
        UF_UI_mask_t  
            mask[5] = { { UF_line_type, 0, 0 },  
                        { UF_circle_type, 0, 0 },  
                        { UF_conic_type, UF_all_subtype, 0 },  
                        { UF_spline_type, 0, 0 },  
                        { UF_solid_type, 0, UF_UI_SEL_FEATURE_ANY_EDGE } };  
        if (UF_CALL(UF_UI_set_sel_mask(select,  
            UF_UI_SEL_MASK_ALL_AND_DISABLE_SPECIFIC, 5, mask)))  
            return (UF_UI_SEL_FAILURE);  
        else  
            return (UF_UI_SEL_SUCCESS);  
    }  
    static tag_t select_a_non_curve(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog(prompt, "",  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_no_curves, NULL, &resp,  
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
            item;  
        while ((item = select_a_non_curve("Select something")) != NULL_TAG)  
            ECHO(item);  
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

> 这段代码是一个用于NX Open C++ API的二次开发示例代码，主要功能是在NX中通过UI选择非曲线实体，并打印所选实体的tag号。
>
> 具体来说，代码主要包括以下几个部分：
>
> 1. 包含必要的头文件，定义了UF系统函数和UI函数的接口。
> 2. 定义了一个错误报告函数report_error，用于在函数调用失败时输出错误信息。
> 3. 定义了一个函数mask_for_no_curves，用于设置选择过滤条件，只选择非曲线实体，如直线、圆、圆锥、样条线等。
> 4. 定义了一个函数select_a_non_curve，用于通过UI对话框选择实体，并应用选择过滤条件。
> 5. 定义了一个函数do_it，用于循环调用select_a_non_curve函数，直到用户取消选择，并打印所选实体的tag号。
> 6. 定义了ufusr函数，这是二次开发程序的入口函数，初始化UF系统，调用do_it函数，然后终止UF系统。
> 7. 定义了ufusr_ask_unload函数，用于在卸载程序时立即卸载。
>
> 综上所述，这段代码实现了一个简单的NX Open二次开发示例，展示了如何通过UI选择非曲线实体，并打印所选实体的tag号。
>
