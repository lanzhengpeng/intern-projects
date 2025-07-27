### 【0671】create dplane through line at angle to plane 创建通过线与平面成角度的基准面

#### 代码

```cpp
    /*HEAD CREATE_DPLANE_THROUGH_LINE_AT_ANGLE_TO_PLANE CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_modl.h>  
    #include <uf_object_types.h>  
    #define ECHO(X) { UF_UI_open_listing_window(); \  
        UF_UI_write_listing_window(X); \  
        UF_print_syslog(X, FALSE); }  
    #define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))  
    static int report_error( char *file, int line, char *call, int irc)  
    {  
        if (irc)  
        {  
            char err[133],  
                 msg[UF_UI_MAX_STRING_LEN];  
            sprintf(msg, "*** ERROR code %d at line %d in %s:\n+++ ",  
                irc, line, file);  
            UF_get_fail_message(irc, err);  
            ECHO(msg);  
            ECHO(err);  
            ECHO("\n");  
            ECHO(call);  
            ECHO(";\n");  
        }  
        return(irc);  
    }  
    /* 里海 */  
    static int mask_for_linear_edges_or_datum_axes(UF_UI_selection_p_t select,  
        void *type)  
    {  
        UF_UI_mask_t  
            mask[2] = { { UF_solid_type, 0, UF_UI_SEL_FEATURE_LINEAR_EDGE },  
                        { UF_datum_axis_type, 0, 0 }};  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 2, mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_linear_edge_or_datum_axis(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select edge/datum axis", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_linear_edges_or_datum_axes,  
            NULL, &resp, &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    /* 里海 */  
    static int mask_for_planar_object(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask[3] = { { UF_solid_type, 0, UF_UI_SEL_FEATURE_PLANAR_FACE },  
                        { UF_plane_type, 0, 0 },  
                        { UF_datum_plane_type, 0, 0 } };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 3, mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_planar_object(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select planar object", prompt,  
            UF_UI_SEL_SCOPE_WORK_PART_AND_OCC, mask_for_planar_object, NULL, &resp,  
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
        int  
            n,  
            point_select[3] = { UF_MODL_EDGE, 0, 0 };  
        tag_t  
            dplane_feats[2],  
            obj_eids[3] = { NULL_TAG, NULL_TAG, NULL_TAG };  
        if (((obj_eids[0] = select_a_linear_edge_or_datum_axis("Through"))  
            != NULL_TAG) &&  
            ((obj_eids[1] = select_a_planar_object("Angle to Plane"))  
            != NULL_TAG))  
            UF_CALL(UF_MODL_create_relative_dplane(2, obj_eids, point_select, 0,  
                NULL, "90", "0", &n, dplane_feats));  
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

> 这段代码是一个NX二次开发的UF函数，其主要功能是创建一个通过线并且与平面成一定角度的基准平面。
>
> 关键点如下：
>
> 1. 包含了必要的NX头文件。
> 2. 定义了ECHO和UF_CALL宏，用于打印日志和调用NX API。
> 3. 定义了report_error函数，用于报告API调用错误。
> 4. 定义了mask_for_linear_edges_or_datum_axes和select_a_linear_edge_or_datum_axis函数，用于选择一条线或轴。
> 5. 定义了mask_for_planar_object和select_a_planar_object函数，用于选择一个平面。
> 6. 定义了do_it函数，用于执行创建基准平面的操作。
> 7. ufusr函数是程序的入口，初始化NX环境，调用do_it函数，然后终止NX环境。
> 8. ufusr_ask_unload函数用于卸载UF函数。
>
> 总体来说，这段代码实现了通过线并且与平面成一定角度创建基准平面的功能，包含了错误处理和用户交互选择。
>
