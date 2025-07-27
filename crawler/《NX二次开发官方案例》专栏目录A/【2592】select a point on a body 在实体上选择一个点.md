### 【2592】select a point on a body 在实体上选择一个点

#### 代码

```cpp
    /*HEAD SELECT_A_POINT_ON_A_BODY CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_modl.h>  
    #include <uf_mtx.h>  
    #include <uf_disp.h>  
    #include <uf_vec.h>  
    #include <uf_object_types.h>  
    #include <uf_assem.h>  
    #include <uf_obj.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档描述，UF_print_syslog是在V18版本中新增的函数。这个函数的功能是打印系统日志消息。 */  
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
    /* qq3123197280 */  
    static int mask_for_bodies(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_solid_type, 0, UF_UI_SEL_FEATURE_BODY };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_point_on_body(char *prompt, double loc[3], tag_t *face)  
    {  
        int  
            n,  
            resp;  
        double  
            cp[3],  
            identity[16],  
            vmx[9];  
        tag_t  
            body,  
            view;  
        char  
            vname[MAX_ENTITY_NAME_SIZE+1];  
        UF_MODL_ray_hit_point_info_t  
            *hit_list;  
        UF_OBJ_disp_props_t  
            attrib = { 1, UF_OBJ_WHITE, UF_OBJ_NOT_BLANKED, UF_OBJ_WIDTH_NORMAL,  
                UF_OBJ_FONT_SOLID, FALSE};  
        *face = NULL_TAG;  
        UF_CALL(UF_UI_select_with_single_dialog("Select point on a body", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_bodies, NULL, &resp,  
            &body, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED)  
        {  
            UF_CALL(UF_DISP_set_highlight(body, 0));  
            UF_CALL(UF_OBJ_ask_name(view, vname));  
            UF_CALL(uc6433(vname, vmx));  
            UF_MTX4_identity(identity);  
            UF_VEC3_copy(cp, loc);  
            UF_VEC3_negate(&vmx[6], vmx);  
            UF_CALL(UF_MODL_trace_a_ray(1, &body, cp, vmx, identity, 1, &n,  
                &hit_list));  
            if (n == 0)  
                UF_CALL(UF_MODL_trace_a_ray(1, &body, cp, &vmx[6], identity, 1, &n,  
                    &hit_list));  
            if (n > 0)  
            {  
                *face = hit_list[0].hit_face;  
            /*  The following command is a work around for PR 4771722 里海译:翻译如下：以下命令是针对PR 4771722的临时解决方案。 */  
                if (UF_ASSEM_is_occurrence(body))  
                    *face = UF_ASSEM_find_occurrence(  
                        UF_ASSEM_ask_part_occurrence(body), *face);  
                UF_VEC3_copy(hit_list[0].hit_point, loc);  
                UF_CALL(UF_DISP_display_temporary_point(view,  
                    UF_DISP_USE_VIEW_TAG, loc, &attrib, UF_DISP_ASTERISK));  
                UF_free(hit_list);  
            }  
            return body;  
        }  
        else return NULL_TAG;  
    }  
    static void do_it(void)  
    {  
        tag_t  
            body,  
            face;  
        double  
            loc[3];  
        while ((body = select_point_on_body("Select point on body", loc, &face))  
            != NULL_TAG)  
        {  
            UF_DISP_set_highlight(face, TRUE);  
            uc1601("face at selected point is highlighted", TRUE);  
            UF_DISP_set_highlight(face, FALSE);  
        }  
    }  
    /* qq3123197280 */  
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

> 这是段NX二次开发代码，其主要功能是让用户在NX装配体中选择一个零件上的点，并高亮显示该点所在的曲面。
>
> 主要步骤包括：
>
> 1. 包含必要的NX头文件。
> 2. 定义错误报告函数report_error，用于打印错误信息。
> 3. 定义mask_for_bodies函数，设置选择过滤条件，只允许选择实体零件。
> 4. 定义select_point_on_body函数，使用NX选择对话框让用户选择一个点，并获取该点所在的零件和曲面。
> 5. 定义do_it函数，循环调用select_point_on_body函数，获取用户选择的点，然后高亮显示该点所在的曲面。
> 6. 定义ufusr函数，初始化NX，调用do_it函数，然后终止NX。
> 7. 定义ufusr_ask_unload函数，设置卸载类型为立即卸载。
>
> 该代码通过NX选择对话框让用户在装配体中选择零件上的点，获取该点所在的曲面，然后高亮显示该曲面，实现了选择零件上的点并高亮显示对应曲面的功能。
>
