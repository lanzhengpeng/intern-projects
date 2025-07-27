### 【2646】select curves 选择曲线

#### 代码

```cpp
    /*HEAD SELECT_CURVES CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_obj.h>  
    #include <uf_object_types.h>  
    #include <uf_disp.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，UF_print_syslog 是在 V18 版本中新增的功能。 */  
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
    static tag_t select_a_curve(char *prompt)  
    {  
        double  
            cp[3];  
        int  
            resp;  
        tag_t  
            curve,  
            view;  
        UF_UI_selection_options_t  
            opts;  
        UF_UI_mask_t  
            mask[5];  
        opts.num_mask_triples = 5;  
        opts.mask_triples = mask;  
        opts.scope = UF_UI_SEL_SCOPE_WORK_PART;  
        mask[0].object_type = UF_line_type;  
        mask[0].object_subtype = 0;  
        mask[0].solid_type = 0;  
        mask[1].object_type = UF_circle_type;  
        mask[1].object_subtype = 0;  
        mask[1].solid_type = 0;  
        mask[2].object_type = UF_conic_type;  
        mask[2].object_subtype = UF_all_subtype;  
        mask[2].solid_type = 0;  
        mask[3].object_type = UF_spline_type;  
        mask[3].object_subtype = 0;  
        mask[3].solid_type = 0;  
        mask[4].object_type = UF_point_type;  
        mask[4].object_subtype = 0;  
        mask[4].solid_type = 0;  
        UF_CALL(UF_UI_select_single(prompt, &opts, &resp, &curve, cp, &view));  
        if (resp == 4 || resp == 5)  
        {  
            UF_CALL(UF_DISP_set_highlight(curve, 0));  
            return curve;  
        }  
        else return NULL_TAG;  
    }  
    static void do_it(void)  
    {  
        tag_t  
            curve;  
        while ((curve = select_a_curve("Select curve")) != NULL_TAG)  
            UF_CALL(UF_OBJ_set_color(curve, UF_OBJ_BLUE));  
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

> 这段代码是一个NX Open C++ API的二次开发示例，其主要功能是选择曲线并设置曲线颜色。代码的核心功能包括：
>
> 1. 包含必要的头文件，如uf.h, uf_ui.h等。
> 2. 定义了一个错误报告函数report_error，用于打印API调用错误信息。
> 3. 定义了一个选择曲线的函数select_a_curve，它可以提示用户选择曲线，并返回曲线的tag。同时，它还设置了选择范围和过滤条件，只允许选择线、圆、圆锥曲线、样条曲线和点。
> 4. 定义了一个设置曲线颜色的函数do_it，它会循环调用select_a_curve选择曲线，并调用UF_OBJ_set_color设置曲线颜色为蓝色。
> 5. 定义了ufusr函数，它是二次开发的入口函数。在这个函数中，初始化API，调用do_it，然后终止API。
> 6. 定义了一个卸载函数ufusr_ask_unload，返回立即卸载。
>
> 总体来说，这段代码通过NX Open C++ API实现了简单的曲线选择和颜色设置功能，是一个很好的二次开发示例。
>
