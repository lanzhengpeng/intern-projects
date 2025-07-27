### 【2777】set member view anchor point 设置绘图成员视图的锚点

#### 代码

```cpp
    /* Sets the anchor point of a selected drawing member view *  
     * to a smart point created at a selected Datum Csys feature */  
    #include <stdio.h>  
    #include <string.h>  
    #include <stdlib.h>  
    #include <uf.h>  
    #include <uf_csys.h>  
    #include <uf_curve.h>  
    #include <uf_disp.h>  
    #include <uf_draw.h>  
    #include <uf_modl.h>  
    #include <uf_obj.h>  
    #include <uf_object_types.h>  
    #include <uf_so.h>  
    #include <uf_view.h>  
    #include <uf_ui.h>  
    #include <stdarg.h>  
    static void ECHO(char *format, ...)  
    {  
        char msg[UF_UI_MAX_STRING_BUFSIZE];  
        va_list args;  
        va_start(args, format);  
        vsnprintf_s(msg, sizeof(msg), _TRUNCATE, format, args);  
        va_end(args);  
        UF_UI_open_listing_window();  
        UF_UI_write_listing_window(msg);  
        UF_print_syslog(msg, FALSE);  
    }  
    #define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))  
    static int report_error(char *file, int line, char *call, int irc)  
    {  
        if (irc)  
        {  
            char err[133];  
            UF_get_fail_message(irc, err);  
            ECHO("*** ERROR code %d at line %d in %s:\n",  
                irc, line, file);  
            ECHO("+++ %s\n", err);  
            ECHO("%s;\n", call);  
        }  
        return(irc);  
    }  
    static int mask_for_views(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_view_type, UF_all_subtype, 0 };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
            UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_view_by_mask(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_set_cursor_view(0));  
        UF_CALL(UF_UI_select_with_single_dialog(prompt, prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_views, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    static tag_t select_a_feature(char *prompt)  
    {  
        int  
            cnt,  
            resp;  
        tag_t  
            first,  
            *feats;  
        char  
            *feature_type;  
        UF_CALL(UF_UI_select_feature(prompt, NULL, &cnt, &feats, &resp));  
        if (cnt)  
        {  
            first = feats[0];  
            UF_CALL(UF_MODL_ask_feat_type(first, &feature_type));  
            if (strcmp(feature_type, "DATUM_CSYS") != 0)  
            {  
                ECHO("First selected feature is not a DATUM_CSYS\n");  
                first = NULL_TAG;  
            }  
            UF_free(feats);  
            UF_free(feature_type);  
            return (first);  
        }  
        else return (NULL_TAG);  
    }  
    static void do_it(void)  
    {  
        char  
            view_name[UF_OBJ_NAME_BUFSIZE] = "",  
            view_dep_name[UF_OBJ_NAME_BUFSIZE] = "";  
        double  
            ap_coords[3] = { 0.0, 0.0, 0.0 };  
        tag_t  
            view_tag = NULL_TAG,  
            dcsys_tag = NULL_TAG,  
            ap_tag = NULL_TAG;  
        int  
            vwdepstatus;  
        view_tag = select_a_view_by_mask("Select member view");  
        if (view_tag == NULL_TAG) return;  
        UF_CALL(UF_OBJ_ask_name(view_tag, view_name));  
        ECHO("View Tag: %d\nView Name: %s\n", view_tag, view_name);  
        UF_CALL(UF_DRAW_ask_view_anchor(view_tag, &ap_tag));  
        ECHO(" Anchor Point Tag before: %d\n", ap_tag);  
        UF_CALL(UF_CURVE_ask_point_data(ap_tag, ap_coords));  
        ECHO("  Anchor Point: X=%f, Y=%f, Z=%f\n", ap_coords[0], ap_coords[1], ap_coords[2]);  
        UF_CALL(uc6409(ap_tag, &vwdepstatus, view_dep_name));  
        if (vwdepstatus)  
            ECHO("  View Dependent in: %s\n", view_dep_name);  
        if ((dcsys_tag=select_a_feature("Select a DATUM_CSYS as new anchor")) != NULL_TAG)  
        {  
            tag_t csys, origin, daxes[3], dplanes[3];  
            logical is_so;  
            // UF_MODL_ask_datum_csys_components returns void!  
            UF_MODL_ask_datum_csys_components(dcsys_tag, &csys, &origin, daxes, dplanes);  
            // only csys component is smart but we need a smart point as anchor  
            UF_CALL(UF_SO_is_so(csys, &is_so));  
            if (csys != NULL_TAG && is_so)  
            {  
                tag_t smart_point = NULL_TAG, scalars[3];  
                int retValue;  
                retValue = uc6476(1); // set dwg state to modeling view to create SO in MODELING  
                for (int i = 0; i < 3; i++)  
                    UF_CALL(UF_SO_create_scalar_double(csys, UF_SO_update_within_modeling, 0.0, &scalars[i]));  
                UF_CALL(UF_SO_create_point_3_scalars_csys(csys, csys, scalars, UF_SO_update_within_modeling, &smart_point));  
                retValue = uc6476(2); // set dwg state back to drawing view  
                UF_CALL(UF_DRAW_set_view_anchor(view_tag, smart_point));  
            }  
            UF_CALL(UF_DRAW_ask_view_anchor(view_tag, &ap_tag));  
            ECHO(" Anchor Point Tag after: %d\n", ap_tag);  
            UF_CALL(UF_CURVE_ask_point_data(ap_tag, ap_coords));  
            ECHO("  Anchor Point: X=%f, Y=%f, Z=%f\n", ap_coords[0], ap_coords[1], ap_coords[2]);  
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

> 这段代码用于设置NX绘图成员视图的锚点到指定的基准坐标系特征上。具体步骤如下：
>
> 1. 包含必要的NX Open API头文件。
> 2. 定义了一个错误报告函数report_error。
> 3. 定义了一个视图选择函数select_a_view_by_mask，用于通过选择对话框选择一个视图。
> 4. 定义了一个特征选择函数select_a_feature，用于选择一个基准坐标系特征。
> 5. 主函数do_it中，先调用select_a_view_by_mask选择一个绘图成员视图，并获取其名称和当前锚点信息。
> 6. 然后调用select_a_feature选择一个基准坐标系特征。
> 7. 获取基准坐标系特征中的CSYS组件，并检查是否为智能体。如果是，则创建一个智能点作为新锚点。
> 8. 最后，将绘图成员视图的锚点设置为新创建的智能点，并输出设置后的锚点信息。
> 9. ufusr函数初始化NX Open API，调用do_it函数，然后终止NX Open API。
> 10. ufusr_ask_unload函数用于在卸载时立即释放资源。
>
> 这段代码实现了在NX中设置绘图成员视图锚点的功能，使得视图锚点与指定的基准坐标系特征关联。
>
