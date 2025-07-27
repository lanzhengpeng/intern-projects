### 【1335】get distance between bodies 获取两个体之间的距离

#### 代码

```cpp
    /*HEAD GET_DISTANCE_BETWEEN_BODIES CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_modl.h>  
    #include <uf_curve.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档描述，UF_print_syslog是在V18版本中新引入的函数。 */  
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
    static tag_t select_a_body(char *prompt)  
    {  
        double  
            cp[3];  
        int  
            resp;  
        tag_t  
            body,  
            view;  
        UF_UI_selection_options_t  
            opts;  
        UF_UI_mask_t  
            mask;  
        opts.num_mask_triples = 1;  
        opts.mask_triples = &mask;  
        opts.scope = UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY;  
        mask.object_type = UF_solid_type;  
        mask.object_subtype = 0;  
        mask.solid_type = UF_UI_SEL_FEATURE_BODY;  
        UF_CALL(UF_UI_select_single(prompt, &opts, &resp, &body, cp, &view));  
        if (resp == 4 || resp == 5)  
        {  
            UF_CALL(UF_DISP_set_highlight(body, FALSE));  
            return body;  
        }  
        else return NULL_TAG;  
    }  
    static void do_it(void)  
    {  
        tag_t  
            body1,  
            body2,  
            point;  
        double  
            dist,  
            p1[3],  
            p2[3],  
            zero[3] = { 0, 0, 0 };  
        while ((body1 = select_a_body("One")) &&  
                (body2 = select_a_body("Two")))  
        {  
            UF_CALL(UF_MODL_ask_minimum_dist(body1, body2, FALSE, zero, FALSE, zero,  
                &dist, p1, p2));  
            UF_CALL(UF_CURVE_create_point(p1, &point));  
            UF_CALL(UF_CURVE_create_point(p2, &point));  
            printf("The distance is %f\n", dist);  
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

> 这段NX二次开发代码的主要功能是计算两个NX实体之间的最小距离。
>
> 代码的主要逻辑如下：
>
> 1. 定义了一个错误报告函数report_error，用于在函数调用失败时输出错误信息。
> 2. 定义了一个选择实体的函数select_a_body，该函数提示用户选择一个实体，并返回其tag。
> 3. 定义了一个主函数do_it，该函数反复提示用户选择两个实体，并调用UF_MODL_ask_minimum_dist函数计算它们之间的最小距离。然后创建两个点来表示这两个距离最近的点，并输出距离值。
> 4. 定义了ufusr函数，该函数在NX启动时调用，初始化NX环境，执行do_it函数，然后终止NX环境。
> 5. 定义了ufusr_ask_unload函数，该函数在NX卸载时调用，返回立即卸载。
>
> 整体来看，这段代码通过UF_MODL_ask_minimum_dist函数实现了计算两个NX实体之间最小距离的核心功能，并通过用户交互来选择实体，最终输出距离结果。
>
