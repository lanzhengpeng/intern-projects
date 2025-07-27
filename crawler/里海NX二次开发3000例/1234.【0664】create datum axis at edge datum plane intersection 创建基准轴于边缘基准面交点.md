### 【0664】create datum axis at edge datum plane intersection 创建基准轴于边缘基准面交点

#### 代码

```cpp
    /*HEAD CREATE_DATUM_AXIS_AT_EDGE_DATUM_PLANE_INTERSECTION CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_so.h>  
    #include <uf_modl.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:根据文档描述，UF_print_syslog 是在 V18 版本中新增的功能。文档指出：

翻译：
UF_print_syslog 是 V18 新增的功能。

这里只回答了翻译，没有添加其他无关的废话。 */  
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
    static int mask_for_datum_planes(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_datum_plane_type, 0, 0 };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_datum_plane(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select a datum plane", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_datum_planes, NULL, &resp,  
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
            daxis,  
            direction,  
            dplane,  
            edge,  
            point;  
        if (((edge = select_an_edge("Create datum axis")) != NULL_TAG) &&  
            ((dplane = select_a_datum_plane("Create datum axis")) != NULL_TAG))  
        {  
            UF_CALL(UF_SO_create_point_surface_crv(edge,  
                UF_SO_update_within_modeling, dplane, edge, NULL_TAG, NULL_TAG,  
                &point));  
            UF_CALL(UF_SO_create_dirr_plane(edge, UF_SO_update_within_modeling,  
                dplane, FALSE, &direction));  
            UF_CALL(UF_MODL_create_point_dirr_daxis(point, direction, &daxis));  
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

> 这是段NX二次开发代码，主要功能是创建一个与边和基准面相交的基准轴。具体步骤如下：
>
> 1. 定义错误报告函数report_error，用于在函数调用失败时打印错误信息。
> 2. 定义mask_for_edges和mask_for_datum_planes函数，用于设置选择集的过滤条件，分别用于选择边和基准面。
> 3. 定义select_an_edge和select_a_datum_plane函数，用于选择一个边和一个基准面。
> 4. 定义do_it函数，首先调用UF_SO_create_point_surface_crv函数计算边和基准面的交点，然后调用UF_SO_create_dirr_plane函数计算边的方向矢量，最后调用UF_MODL_create_point_dirr_daxis函数根据交点和方向矢量创建基准轴。
> 5. 定义ufusr函数，在NX启动时调用do_it函数执行创建基准轴的功能。
> 6. 定义ufusr_ask_unload函数，在NX卸载时调用，返回立即卸载标志。
>
> 总的来说，这段代码实现了在NX中通过选择边和基准面来创建与之相交的基准轴的功能。
>
