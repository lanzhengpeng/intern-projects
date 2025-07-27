### 【0772】create multiple extruded sheets 创建多个挤压片

#### 代码

```cpp
    /*HEAD CREATE_MULTIPLE_EXTRUDED_SHEETS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_modl.h>  
    #include <uf_disp.h>  
    #include <uf_ui.h>  
    #include <uf_obj.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:UF_print_syslog是V18版本新增的函数。 */  
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
    static int mask_for_curves(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask[4] = { { UF_line_type, 0, 0 },  
                        { UF_circle_type, 0, 0 },  
                        { UF_conic_type, UF_all_subtype, 0 },  
                        { UF_spline_type, 0, 0 } };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 4, mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static int select_curves(char *prompt, tag_t **curves)  
    {  
        int  
            cnt,  
            ii,  
            resp;  
        UF_CALL(UF_UI_select_with_class_dialog("Select curves", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY,  
            mask_for_curves, NULL, &resp, &cnt, curves));  
        for (ii = 0; ii < cnt; ii++)  
            UF_CALL(UF_DISP_set_highlight((*curves)[ii], FALSE));  
        return cnt;  
    }  
    static logical specify_vector(char *prompt, double vec[3], double point[3])  
    {  
        int  
            mode = UF_UI_NO_DEFAULT,  
            resp;  
        UF_CALL(UF_UI_specify_vector(prompt, &mode, UF_UI_DISP_TEMP_VECTOR,  
            vec, point, &resp));  
        if (resp == UF_UI_OK) return TRUE;  
        else return FALSE;  
    }  
    static void set_feature_face_color(tag_t feat, int color)  
    {  
        uf_list_p_t  
            face_list,  
            temp;  
        UF_CALL(UF_MODL_ask_feat_faces(feat, &face_list));  
        for (temp = face_list; temp != NULL; temp = temp->next)  
            UF_CALL(UF_OBJ_set_color(temp->eid, color));  
        UF_CALL(UF_MODL_delete_list(&face_list));  
    }  
    static uf_list_p_t make_a_list(int count, tag_t *item_array)  
    {  
        int  
            ii;  
        uf_list_p_t  
            list;  
        UF_CALL(UF_MODL_create_list(&list));  
        for (ii = 0; ii < count; ii++)  
            UF_CALL(UF_MODL_put_list_item(list, item_array[ii]));  
        return (list);  
    }  
    static int allocate_memory(unsigned int nbytes, void **where)  
    {  
        int  
            resp;  
        *where = UF_allocate_memory(nbytes, &resp);  
        return resp;  
    }  
    static int make_an_array(uf_list_p_t *object_list, tag_t **objects)  
    {  
        int  
            ii,  
            n;  
        uf_list_p_t  
            temp;  
        UF_CALL(UF_MODL_ask_list_count(*object_list, &n));  
        UF_CALL(allocate_memory(n * sizeof(tag_t), (void **)objects));  
        for (ii = 0, temp = *object_list; ii < n; temp = temp->next, ii++)  
            (*objects)[ii] = temp->eid;  
        UF_CALL(UF_MODL_delete_list(object_list));  
        return n;  
    }  
    static void do_it(void)  
    {  
        int  
            ii,  
            n_curves,  
            n_feats;  
        char  
            *limits[2] = { "0", "1" };  
        tag_t  
            *curves,  
            *feats;  
        double  
            dir[3],  
            point[3];  
        uf_list_p_t  
            curve_list,  
            feat_list;  
        while (((n_curves = select_curves("Extrude", &curves)) > 0) &&  
            specify_vector("Specify extrusion vector", dir, point))  
        {  
            curve_list = make_a_list(n_curves, curves);  
            UF_free(curves);  
            if (!UF_CALL(UF_MODL_create_extruded2(curve_list, "0.0",  
                limits, point, dir, UF_NULLSIGN, &feat_list)))  
            {  
                n_feats = make_an_array(&feat_list, &feats);  
                for (ii = 0; ii < n_feats; ii++)  
                    set_feature_face_color(feats[ii], ii+1);  
                if (n_feats > 0) UF_free(feats);  
            }  
            UF_CALL(UF_MODL_delete_list(&curve_list));  
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

> 这段NX二次开发代码主要实现了以下功能：
>
> 1. 选择曲线并指定拉伸方向，用于创建多个拉伸实体。
> 2. 每次选择曲线和指定拉伸方向后，代码会创建一个拉伸特征列表，并提取特征面。
> 3. 对每个拉伸特征的面进行着色，以便于区分不同的拉伸特征。
> 4. 重复以上流程，直到用户取消选择曲线。
> 5. 提供了错误报告机制，方便调试。
> 6. 实现了内存申请和释放功能，避免内存泄露。
> 7. 使用UF_free()函数释放了不必要的内存。
> 8. 提供了unload时的处理函数。
>
> 总体来说，这段代码通过交互方式让用户选择曲线并指定拉伸方向，然后创建拉伸特征并着色，实现了创建多个拉伸实体的功能。代码结构清晰，具有较好的可读性和可维护性。
>
