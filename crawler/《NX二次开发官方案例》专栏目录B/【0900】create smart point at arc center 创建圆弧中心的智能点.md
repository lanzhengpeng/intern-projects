### 【0900】create smart point at arc center 创建圆弧中心的智能点

#### 代码

```cpp
    /*HEAD CREATE_SMART_POINT_AT_ARC_CENTER CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_disp.h>  
    #include <uf_so.h>  
    #include <uf_obj.h>  
    #include <uf_assem.h>  
    #include <uf_view.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:UF_print_syslog 是 V18 中的新功能。 */  
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
    static int mask_for_arcs(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask[2] = { { UF_circle_type, 0, 0 },  
                        { UF_solid_type, 0, UF_UI_SEL_FEATURE_CIRCULAR_EDGE } };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 2, mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_an_arc(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog(prompt, "",  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_arcs, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    static void report_object_info(char *what, tag_t object)  
    {  
        int  
            status,  
            subtype,  
            type;  
        tag_t  
            owner;  
        char  
            name[UF_OBJ_NAME_LEN + 1];  
        UF_OBJ_disp_props_t  
            disp_props;  
        if (object != NULL_TAG)  
        {  
            UF_CALL(UF_OBJ_ask_type_and_subtype(object, &type, &subtype));  
            printf("%s = %d, type = %d, subtype = %d, ", what, object, type,  
                subtype);  
            if (UF_OBJ_is_object_a_promotion(object)) printf("promotion, ");  
            if (UF_ASSEM_is_occurrence(object)) printf("occurrence, ");  
            if (!UF_CALL(UF_OBJ_ask_owning_part(object, &owner)))  
                printf("owned by %d, ", owner);  
            if (!uc6409(object, &status, name) && (status == 1))  
                printf("view dependent in %s, ", name);  
            if (!UF_OBJ_ask_name(object, name)) printf("named %s, ", name);  
            status = UF_OBJ_ask_status(object);  
            printf("status %d\n", status);  
            UF_CALL(UF_OBJ_ask_display_properties(object, &disp_props));  
            printf("  layer = %d, color = %d, blank = %d, width = %d, "  
                "font = %d, highlight = %d\n", disp_props.layer,  
                disp_props.color, disp_props.blank_status, disp_props.line_width,  
                disp_props.font, disp_props.highlight_status);  
        }  
        else printf("%s = %d (NULL_TAG)\n", what, object);  
    }  
    static void do_it(void)  
    {  
        int  
            ii,  
            n_parents;  
        tag_t  
            arc,  
            *parents,  
            point;  
        char  
            name[MAX_ENTITY_NAME_SIZE+1];  
        while ((arc = select_an_arc("Select arc")) != NULL_TAG)  
        {  
            UF_CALL( UF_SO_create_point_conic_center( arc,  
                UF_SO_update_after_modeling, arc, &point));  
            UF_CALL( UF_SO_set_visibility_option( point, UF_SO_visible));  
            UF_CALL( UF_SO_ask_parents( point, UF_SO_ASK_WIREFRAME_PARENTS,  
                &n_parents, &parents));  
            for (ii = 0; ii < n_parents; ii++)  
            {  
                sprintf(name, "parents[%d]", ii);  
                report_object_info(name, parents[ii]);  
            }  
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

> 这段NX二次开发代码主要用于在选中弧的中心创建一个点，并设置该点的可见性。以下是该代码的主要功能：
>
> 1. 定义了错误报告函数report_error，用于输出错误信息。
> 2. 定义了选择弧的函数select_an_arc，用于在提示下选择弧。
> 3. 定义了报告实体信息的函数report_object_info，用于输出实体的各种信息。
> 4. 定义了主函数do_it，用于在选中的弧中心创建点，设置点的可见性，并输出点的父实体信息。
> 5. 定义了UF函数ufusr，用于初始化NX和执行do_it函数，以及结束NX。
> 6. 定义了卸载函数ufusr_ask_unload，用于立即卸载该UF。
>
> 该代码的主要逻辑是循环调用select_an_arc选择弧，然后在弧中心创建点，设置点可见，并输出点的父实体信息。通过UF函数实现了与NX的交互。
>
