### 【0974】create variable radius blends 创建可变半径圆角

#### 代码

```cpp
    /*HEAD CREATE_VARIABLE_RADIUS_BLENDS CCC UFUN */  
    /* This sample program requires  *  
     * point objects to be created   *  
     * at the points on the edge     *  
     * where various radius values   *  
     * will be assigned              */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_curve.h>  
    #include <uf_modl.h>  
    #include <uf_ui.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，UF_print_syslog是V18版本新增的函数。 */  
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
    static int select_edges(char *prompt, tag_t **edges)  
    {  
        int  
            ii,  
            irc,  
            n;  
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
        UF_CALL(UF_UI_select_by_class(prompt, &opts, &irc, &n, edges));  
        for (ii=0; ii<n; ii++)  
            UF_CALL(UF_DISP_set_highlight(*(*edges + ii), FALSE));  
        return n;  
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
    static int vrb_routine(tag_t eid, double points[100][3], char radii[100][256],  
                           int smooth_overflow, int cliff_overflow,  
                           int notch_overflow, double vrb_tol,  
                           int *number_of_points)  
    {  
        int  
            num_char,  
            ii = 0;  
        tag_t  
            pt_tag;  
        UF_CALL(UF_DISP_set_highlight(eid, 1));  
        while((pt_tag = select_a_point("point on highlited edge")) != NULL_TAG)  
        {  
            UF_CALL(UF_CURVE_ask_point_data(pt_tag, &points[ii][0]));  
            uc1600("Enter radius value for point", radii[ii], &num_char);  
            ii ++;  
        }  
        UF_CALL(UF_DISP_set_highlight(eid, 0));  
        *number_of_points = ii;  
        return(0);  
    }  
    static uf_list_p_t make_a_list( int count, tag_t *item_array)  
    {  
        uf_list_p_t list;  
        int         ii;  
        UF_CALL( UF_MODL_create_list( &list));  
        for( ii = 0; ii < count; ii++)  
            UF_CALL( UF_MODL_put_list_item( list, item_array[ii]));  
        return( list);  
    }  
    static void do_it(void)  
    {  
        tag_t  
            feat_id,  
            *edges;  
        double  
            vrb_tol = 0.00001;  
        int  
            num_edges,  
            smooth_overflow = UF_MODL_BLEND_SMOOTH_OVERFLOW,  
            cliff_overflow  = UF_MODL_BLEND_CLIFF_OVERFLOW,  
            notch_overflow  = UF_MODL_BLEND_NOTCH_OVERFLOW;  
        uf_list_p_t  
            edge_list;  
        UF_CALL(UF_MODL_register_var_blend(vrb_routine));  
        while ((num_edges = select_edges("select edges to blend", &edges)) > 0)  
        {  
            edge_list = make_a_list(num_edges, edges);  
            UF_free(edges);  
            UF_CALL(UF_MODL_create_blend("0.009246", edge_list, smooth_overflow,  
                cliff_overflow, notch_overflow, vrb_tol, &feat_id));  
            UF_CALL(UF_MODL_delete_list(&edge_list));  
        }  
        UF_CALL(UF_MODL_unregister_var_blend());  
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

> 这段代码是用于NX的二次开发，其主要功能是创建一个变量半径混合的命令。具体步骤如下：
>
> 1. 定义了一个错误报告函数report_error，用于在调用NX API出错时报告错误。
> 2. 定义了一个选择边的函数select_edges，用于提示用户选择要混合的边，并返回边的数组。
> 3. 定义了一个选择点的函数select_a_point，用于提示用户选择一个点，并返回点的tag。
> 4. 定义了变量半径混合的回调函数vrb_routine，用于在每个选中的边上选择点，并设置每个点的半径值，然后返回点的坐标和半径值。
> 5. 定义了创建列表的函数make_a_list，用于创建边的列表。
> 6. 定义了主函数do_it，首先注册变量半径混合的回调函数，然后循环提示用户选择边，并为每条边创建变量半径混合特征，最后注销回调函数。
> 7. 定义了ufusr函数，用于初始化NX，执行do_it，然后终止NX。
> 8. 定义了ufusr_ask_unload函数，用于在卸载时立即卸载。
>
> 综上所述，这段代码的主要功能是创建一个NX命令，让用户可以创建变量半径混合特征。它通过回调函数的方式实现了NX API的扩展。
>
