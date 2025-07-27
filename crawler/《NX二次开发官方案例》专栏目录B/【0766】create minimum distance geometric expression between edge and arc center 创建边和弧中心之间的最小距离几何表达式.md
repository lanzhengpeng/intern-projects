### 【0766】create minimum distance geometric expression between edge and arc center 创建边和弧中心之间的最小距离几何表达式

#### 代码

```cpp
    /*HEAD CREATE_MINIMUM_DISTANCE_GEOMETRIC_EXPRESSION_BETWEEN_EDGE_AND_ARC_CENTER CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_gexp.h>  
    #include <uf_so.h>  
    #include <uf_assem.h>  
    #include <uf_modl.h>  
    #include <uf_obj.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:翻译：UF_print_syslog是V18版本新增的功能。 */  
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
    #define WRITE_F(X) (write_double_to_listing_window(#X, X))  
    static void write_double_to_listing_window(char *title, double n)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %f\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    /* 里海 */  
    static int mask_for_edges(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_solid_type, 0, UF_UI_SEL_FEATURE_ANY_EDGE };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_an_edge(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select an edge", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_edges, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, FALSE));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    /* 里海 */  
    static int mask_for_circular_edges(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_solid_type, 0, UF_UI_SEL_FEATURE_CIRCULAR_EDGE };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_circular_edge(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select circular edge", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_circular_edges, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    static tag_t ask_object_xform(tag_t object)  
    {  
        tag_t  
            base_tag,  
            part_occ,  
            prom_part,  
            part_proto,  
            proto,  
            xform = NULL_TAG;  
        if (!UF_ASSEM_is_occurrence(object)) return xform;  
        proto = UF_ASSEM_ask_prototype_of_occ(object);  
        if (UF_OBJ_is_object_a_promotion(proto))  
        {  
            base_tag = proto;  
            while (base_tag != NULL_TAG)  
            {  
                UF_CALL(UF_MODL_prom_map_object_down(proto, &base_tag));  
                if (base_tag != NULL_TAG) proto = base_tag;  
            }  
            UF_CALL(UF_OBJ_ask_owning_part(proto, &prom_part));  
            part_occ = UF_ASSEM_ask_part_occurrence(object);  
            part_proto = UF_ASSEM_ask_prototype_of_occ(part_occ);  
            while (prom_part != part_proto)  
            {  
                UF_CALL(UF_ASSEM_ask_parent_component(part_occ, &part_occ));  
                part_proto = UF_ASSEM_ask_prototype_of_occ(part_occ);  
            }  
            UF_CALL(UF_SO_create_xform_assy_ctxt(object, part_occ, NULL_TAG,  
                &xform));  
        }  
        else  
            UF_CALL(UF_SO_create_xform_assy_ctxt(object,  
                UF_ASSEM_ask_part_occurrence(object), NULL_TAG, &xform));  
        return xform;  
    }  
    static void do_it(void)  
    {  
        tag_t  
            arc,  
            edge,  
            exp,  
            feat,  
            point,  
            xform1,  
            xform2;  
        double  
            distance;  
        if (((edge = select_an_edge("Distance expression")) != NULL_TAG) &&  
            ((arc = select_a_circular_edge("Distance expression")) != NULL_TAG))  
        {  
            xform1 = ask_object_xform(edge);  
            if (UF_ASSEM_is_occurrence(edge))  
                edge = UF_ASSEM_ask_prototype_of_occ(edge);  
            xform2 = ask_object_xform(arc);  
            if (UF_ASSEM_is_occurrence(arc))  
                UF_CALL(UF_SO_create_curve_extract(arc,  
                    UF_SO_update_after_parent_body,  
                    UF_ASSEM_ask_prototype_of_occ(arc), 0, UF_all_subtype, xform2,  
                    &arc));  
            UF_CALL(UF_SO_create_point_conic_center(arc,  
                UF_SO_update_after_parent_body, arc, &point));  
            UF_CALL(UF_GEXP_create_distance(edge, xform1, point, NULL_TAG,  
                &feat, &exp));  
            UF_CALL(UF_MODL_ask_exp_tag_value(exp, &distance));  
            WRITE_F(distance);  
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

> 这是段NX二次开发代码，其主要功能是创建一个几何表达式，用于计算直线边与圆弧中心之间的最小距离。
>
> 代码主要流程如下：
>
> 1. 定义了错误处理函数report_error，用于输出函数调用错误。
> 2. 定义了函数mask_for_edges，用于设置选择边界的过滤条件。
> 3. 定义了函数select_an_edge，用于选择一个边界。
> 4. 定义了函数mask_for_circular_edges，用于设置选择圆弧边的过滤条件。
> 5. 定义了函数select_a_circular_edge，用于选择一个圆弧边。
> 6. 定义了函数ask_object_xform，用于获取对象在装配中的变换矩阵。
> 7. 定义了函数do_it，实现了主要功能：
>
> 1. 定义了ufusr函数，作为用户自定义函数的入口。
> 2. 定义了ufusr_ask_unload函数，用于卸载用户自定义函数。
>
> 通过该代码，用户可以方便地创建一个表达式，以计算直线边界与圆弧中心之间的距离。
>
