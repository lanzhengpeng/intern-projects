### 【1186】evaluate midpt of edge 评估边的中点

#### 代码

```cpp
    /*HEAD EVALUATE_MIDPT_OF_EDGE CCC UFUN */  
    #include <uf_curve.h>  
    #include <uf_eval.h>  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，可以翻译为：

注意：UF_print_syslog是V18版本新增的，只需回答翻译，不要赘述。 */  
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
    static tag_t select_an_edge(char *prompt)  
    {  
        double  
            cp[3];  
        int  
            irc;  
        tag_t  
            edge,  
            view;  
        UF_UI_selection_options_t  
            opts;  
        UF_UI_mask_t  
            mask;  
        opts.num_mask_triples = 1;  
        opts.mask_triples = &mask;  
        opts.scope = UF_UI_SEL_SCOPE_WORK_PART;  
        mask.object_type = UF_solid_type;  
        mask.object_subtype = 0;  
        mask.solid_type = UF_UI_SEL_FEATURE_ANY_EDGE;  
        UF_CALL(UF_UI_select_single(prompt, &opts, &irc, &edge, cp, &view));  
        if (irc == 4 || irc == 5)  
        {  
            UF_CALL(UF_DISP_set_highlight(edge, FALSE));  
            return edge;  
        }  
        else return NULL_TAG;  
    }  
    static void do_it(void)  
    {  
        tag_t   curv1, pt1;  
        UF_EVAL_p_t evaluator;  
        double  mid_t, limits[2];  
        double  point[3], derivatives[3];  
        while ((curv1 = select_an_edge("Select edge")) != NULL_TAG)  
        {  
            UF_CALL( UF_EVAL_initialize( curv1, &evaluator));  
            UF_CALL( UF_EVAL_ask_limits( evaluator, limits));  
            printf("Parameter limits of selected edge: %f to %f\n",  
                    limits [ 0 ], limits [ 1 ] );  
            mid_t = ( limits [ 1 ] + limits [ 0 ] ) / 2.0;  
            printf("Average Parameter on selected edge: %f\n", mid_t);  
            UF_CALL( UF_EVAL_evaluate( evaluator, 0, mid_t, point, derivatives));  
            UF_CALL( UF_CURVE_create_point( point, &pt1));  
            UF_CALL( UF_EVAL_free( evaluator));  
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

> 这段代码是一个用于NX的二次开发示例，其主要功能是选择边线，并计算边线的中点坐标，然后在图形窗口中显示这个中点。
>
> 具体来说，代码的主要逻辑如下：
>
> 1. 包含NX提供的UF库的头文件，这些头文件包含了二次开发所需的接口函数。
> 2. 定义了一个错误报告函数report_error，用于在调用NX接口函数失败时输出错误信息。
> 3. 定义了一个选择边线的函数select_an_edge，用户会看到一个提示，然后可以在图形窗口中选择一条边线。选择后，会返回这条边线的tag。
> 4. 定义了一个主函数do_it，在这个函数中，会循环调用select_an_edge函数来选择边线。
> 5. 对每条选中的边线，会初始化一个evaluator，然后获取这条边线的参数范围limits。
> 6. 计算参数范围的中点mid_t。
> 7. 使用evaluator在mid_t参数处计算边线的坐标point和导数derivatives。
> 8. 使用UF_CURVE_create_point接口函数在图形窗口中显示这个中点。
> 9. ufusr是主函数，用于初始化和终止NX环境，并调用do_it函数。
> 10. ufusr_ask_unload用于设置二次开发模块卸载的标志。
>
> 综上，这段代码通过二次开发实现了选择边线并显示其参数中点坐标的功能，为NX的二次开发提供了示例。
>
