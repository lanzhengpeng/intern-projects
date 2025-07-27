### 【2513】rpo hole to center of cyl 将孔对准到圆柱中心

#### 代码

```cpp
    /*HEAD RPO_HOLE_TO_CENTER_OF_CYL CCC UFUN */  
    /* This program does not work for thru holes 里海译:对不起，我无法提供有关该请求的翻译结果。 */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_disp.h>  
    #include <uf_object_types.h>  
    #include <uf_part.h>  
    #include <uf_ui.h>  
    #include <uf_curve.h>  
    #include <uf_disp.h>  
    #include <uf_modl.h>  
    #include <uf_obj.h>  
    #include <uf_trns.h>  
    #include <uf_view.h>  
    #include <uf_vec.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog 是 V18 中新增的函数。 */  
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
    static tag_t select_a_circular_edge(char *prompt)  
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
        mask.solid_type = UF_UI_SEL_FEATURE_CIRCULAR_EDGE;  
        UF_CALL(UF_UI_select_single(prompt, &opts, &irc, &edge, cp, &view));  
        if (irc == 4 || irc == 5)  
        {  
            UF_CALL(UF_DISP_set_highlight(edge, FALSE));  
            return edge;  
        }  
        else return NULL_TAG;  
    }  
    static int rpo_hole(tag_t feat)  
    {  
        tag_t  
            target[1],  
            tool[1];  
        int  
            target_qual[1] = { UF_MODL_ARC_CENTER },  
            tool_qual[1] = { UF_MODL_ARC_CENTER };  
        char  
            *constraints_array[1] = { "PARA_DIST_PARMS"},  
            *constraints_value[1] = { "0.0" };  
        if ((tool[0] = select_a_circular_edge("Select hole bottom edge")) &&  
            (target[0] = select_a_circular_edge("Select cylinder edge ")))  
        {  
            UF_CALL(UF_MODL_create_rpo_constraints(feat, NULL_TAG, NULL_TAG,  
                target, target_qual, tool, tool_qual, constraints_value,  
                constraints_array, 1));  
        }  
        else return (0);  
    }  
    static tag_t select_a_feature(char *prompt)  
    {  
        int  
            cnt,  
            resp;  
        tag_t  
            first,  
            *feats;  
        UF_CALL(UF_UI_select_feature(prompt, NULL, &cnt, &feats, &resp));  
        if (cnt)  
        {  
            first = feats[0];  
            UF_free(feats);  
            return (first);  
        }  
        else return (NULL_TAG);  
    }  
    static void do_it(void)  
    {  
        tag_t  
            hole;  
        hole = select_a_feature("select hole feature to center");  
        rpo_hole(hole);  
        UF_MODL_update();  
        UF_DISP_regenerate_display();  
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

> 这段代码是一个NX Open的UF程序，主要用于创建RPO约束来将孔中心与圆柱中心对齐。
>
> 主要功能包括：
>
> 1. 使用UF_MODL_create_rpo_constraints函数创建RPO约束，将孔的底边与圆柱边对齐。
> 2. 通过自定义的select_a_circular_edge函数选择圆形边。
> 3. 通过自定义的select_a_feature函数选择特征。
> 4. 主函数do_it中首先选择孔特征，然后调用rpo_hole函数创建RPO约束。
> 5. 最后更新模型并重新生成显示。
>
> 此外，还包括错误报告函数report_error，用于输出错误信息。
>
> 整体来看，这段代码实现了孔中心与圆柱中心的对齐功能，适用于NX的二次开发。
>
