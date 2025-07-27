### 【1336】get distance between points 获取两点间距离

#### 代码

```cpp
    /*HEAD GET_DISTANCE_BETWEEN_POINTS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_curve.h>  
    #include <uf_vec.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，UF_print_syslog是在V18版本中新增的。 */  
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
    static tag_t select_a_point(char *prompt)  
    {  
        double  
            cp[3];  
        int  
            resp;  
        tag_t  
            point,  
            view;  
        UF_UI_selection_options_t  
            opts;  
        UF_UI_mask_t  
            mask;  
        opts.num_mask_triples = 1;  
        opts.mask_triples = &mask;  
        opts.scope = UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY;  
        mask.object_type = UF_point_type;  
        mask.object_subtype = 0;  
        mask.solid_type = 0;  
        UF_CALL(UF_UI_select_single(prompt, &opts, &resp, &point, cp, &view));  
        if (resp == 4 || resp == 5)  
        {  
            UF_CALL(UF_DISP_set_highlight(point, 0));  
            return point;  
        }  
        else return NULL_TAG;  
    }  
    static void do_it(void)  
    {  
        tag_t  
            point[2];  
        double  
            distance,  
            vec1[3],  
            vec2[3];  
        char  
            messg[133];  
        while ((point[0] = select_a_point("Select first point")) &&  
               (point[1] = select_a_point("Select second point")))  
        {  
            UF_CALL(UF_CURVE_ask_point_data(point[0], vec1));  
            UF_CALL(UF_CURVE_ask_point_data(point[1], vec2));  
            UF_VEC3_distance(vec1, vec2, &distance);  
            sprintf(messg, "The distance is %f", distance);  
            uc1601(messg, TRUE);  
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

> 这段代码是一个用于NX的二次开发示例，主要实现的功能是计算两个点之间的距离。
>
> 主要功能包括：
>
> 1. 定义了一个错误报告函数report_error，用于在函数调用失败时输出错误信息。
> 2. 定义了一个选择点的函数select_a_point，使用UF_UI接口让用户选择一个点，并返回点的tag。
> 3. 主函数do_it中，使用select_a_point函数让用户选择两个点，然后使用UF_CURVE接口获取这两个点的坐标，再使用UF_VEC3_distance函数计算两点之间的距离，并将结果显示在对话框中。
> 4. ufusr函数是NX调用的入口函数，初始化NX环境，调用do_it函数，然后终止NX环境。
> 5. ufusr_ask_unload函数用于处理插件卸载时的行为。
>
> 总体来说，这段代码通过调用NX提供的API，实现了让用户选择两个点，并计算两点之间距离的功能，展示了NX二次开发的基本流程。
>
