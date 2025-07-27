### 【0860】create relative datum plane thru midpoint of edge parallel to dplane 创建与基准面平行的边中点的相对基准面

#### 代码

```cpp
    /*HEAD CREATE_RELATIVE_DATUM_PLANE_THRU_MIDPOINT_OF_EDGE_PARALLEL_TO_DPLANE CCC UFUN */  
    #include <string.h>  
    #include <stdio.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_defs.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:UF_print_syslog在V18版本中是新增的。 */  
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
    #define WRITE_D(X) (write_integer_to_listing_window(#X, X))  
    #define WRITENZ(X) if (X != 0) (write_integer_to_listing_window(#X, X))  
    static void write_integer_to_listing_window(char *title, int n)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %d\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
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
    /* 里海 */  
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
    static void do_it(void)  
    {  
        int  
            num_dplanes = 0,  
            point_select[3] = { 0,0,0 },  
            which_plane = 1;  
        tag_t  
            dplanes[2] = { NULL_TAG, NULL_TAG },  
            obj_eids[3] = { NULL_TAG, NULL_TAG, NULL_TAG },  
            edge = NULL_TAG,  
            plane = NULL_TAG;  
        double  
            ref_point[3] = { 0.,0.,0. };  
        if( (( edge=select_an_edge("Select Edge:"))!= NULL_TAG) &&  
            ((plane=select_a_datum_plane("Select Datum Plane"))!= NULL_TAG)  
          )  
        {  
            obj_eids[0] = edge;  
            obj_eids[1] = plane;  
            point_select[0] = UF_MODL_MID_POINT;  
            UF_CALL(UF_MODL_create_relative_dplane(2, obj_eids, point_select,  
                which_plane, ref_point, "0.", "0.", &num_dplanes, dplanes));  
            WRITENZ( dplanes[0] );  
            WRITENZ( dplanes[1] );  
        }  
    }  
    /* 里海 */  
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

> 这段代码是用于NX的二次开发，其功能是创建一个相对于选定边中点且平行于指定基准平面的新基准面。以下是主要步骤和功能：
>
> 1. 包含必要的NX API头文件。
> 2. 定义错误报告函数report_error，用于在出错时打印错误信息。
> 3. 定义辅助函数mask_for_datum_planes，用于设置选择基准面时的过滤条件。
> 4. 定义函数select_a_datum_plane，用于选择一个基准面。
> 5. 定义辅助函数mask_for_edges，用于设置选择边时的过滤条件。
> 6. 定义函数select_an_edge，用于选择一个边。
> 7. 定义函数do_it，用于执行创建新基准面的操作。首先，它会调用select_an_edge和select_a_datum_plane分别选择一个边和一个基准面。然后，利用UF_MODL_create_relative_dplane API创建一个相对于选定边中点且平行于指定基准面的新基准面。最后，打印新基准面的tag。
> 8. 定义ufusr函数，它是NX二次开发的入口函数。在ufusr中，首先初始化NX，然后调用do_it执行创建基准面的操作，最后终止NX。
> 9. 定义ufusr_ask_unload函数，用于卸载二次开发程序。
>
> 总的来说，这段代码通过用户交互式选择一个边和一个基准面，然后在该边的中点位置创建一个与指定基准面平行的基准面，实现了NX的二次开发功能。
>
