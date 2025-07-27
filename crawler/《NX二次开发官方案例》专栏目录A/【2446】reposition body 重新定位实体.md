### 【2446】reposition body 重新定位实体

#### 代码

```cpp
    /*HEAD REPOSITION_BODY CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_csys.h>  
    #include <uf_modl.h>  
    #include <uf_trns.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog is new in V18 */  
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
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %d\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    /* qq3123197280 */  
    static int mask_for_csys(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_coordinate_system_type, 0, 0 };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_csys(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select a coordinate system",  
            prompt, UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_csys, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    static void matrix_csys2csys(tag_t ref_csys, tag_t dest_csys, double mx[12])  
    {  
        double  
            csys[9],  
            orig[3],  
            tx1[12],  
            tx2[12],  
            tx3[12],  
            tx4[12],  
            v[3];  
        int  
            ii;  
        tag_t  
            csys_mx;  
        UF_CALL(UF_CSYS_ask_csys_info(ref_csys, &csys_mx, orig));  
        UF_CALL(UF_CSYS_ask_matrix_values(csys_mx, csys));  
        tx1[0] = csys[0];  
        tx1[1] = csys[1];  
        tx1[2] = csys[2];  
        tx1[3] = 0;  
        tx1[4] = csys[3];  
        tx1[5] = csys[4];  
        tx1[6] = csys[5];  
        tx1[7] = 0;  
        tx1[8] = csys[6];  
        tx1[9] = csys[7];  
        tx1[10] = csys[8];  
        tx1[11] = 0;  
    /*  set up to translate from reference csys back to absolute 里海译:根据参考坐标系将坐标转换回绝对坐标系。 */  
        for (ii=0; ii<3; ii++) v[ii] = -orig[ii];  
        FTN(uf5943)(v, tx2);  
    /*  combine this with the rotation matrix from the reference csys 里海译:将此与参考坐标系的旋转矩阵结合起来。 */  
        FTN(uf5942)(tx2, tx1, tx3);  
        UF_CALL(UF_CSYS_ask_csys_info(dest_csys, &csys_mx, orig));  
        UF_CALL(UF_CSYS_ask_matrix_values(csys_mx, csys));  
    /*  Invert the rotation from the destination matrix 里海译:翻转目标矩阵的旋转 */  
        tx2[0] = csys[0];  
        tx2[1] = csys[3];  
        tx2[2] = csys[6];  
        tx2[3] = 0;  
        tx2[4] = csys[1];  
        tx2[5] = csys[4];  
        tx2[6] = csys[7];  
        tx2[7] = 0;  
        tx2[8] = csys[2];  
        tx2[9] = csys[5];  
        tx2[10] = csys[8];  
        tx2[11] = 0;  
    /*  set up to translate from abs to the destination csys 里海译:根据原文，翻译为“设置从ABS到目标坐标系Csys的转换”。 */  
        FTN(uf5943)(orig, tx1);  
    /*  combine this with the inverted rotation csys above 里海译:结合上面的反转旋转坐标系。 */  
        FTN(uf5942)(tx2, tx1, tx4);  
    /*  and the one from the reference csys 里海译:将 "and the one from the reference csys" 翻译为 "以及来自参考坐标系的那个"。 */  
        FTN(uf5942)(tx3, tx4, mx);  
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
    static tag_t select_a_body(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select a body", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_bodies, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
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
    static int ask_body_feats(tag_t body, tag_p_t *body_feats)  
    {  
        int  
            n;  
        uf_list_p_t  
            feat_list;  
        if (!UF_CALL(UF_MODL_ask_body_feats(body, &feat_list)))  
        {  
            n = make_an_array(&feat_list, body_feats);  
            return n;  
        }  
        else return 0;  
    }  
    static void do_it(void)  
    {  
        int  
            ii,  
            irc,  
            move = 1,  
            n_feats,  
            n_objects,  
            n_parents,  
            original = 0,  
            off = 2;  
        tag_t  
            body,  
            d_csys,  
            *feats,  
            *objects,  
            *parents,  
            r_csys;  
        double  
            mx[12];  
        char  
            **parent_names;  
        uf_list_p_t  
            object_list;  
        while ((body = select_a_body("Body to reposition")) &&  
               (r_csys = select_a_csys("Reference csys")) &&  
               (d_csys = select_a_csys("Destination csys")))  
        {  
            UF_CALL(UF_MODL_create_list(&object_list));  
            UF_CALL(UF_MODL_put_list_item(object_list, body));  
            n_feats = ask_body_feats(body, &feats);  
            if (n_feats > 0)  
            {  
                UF_CALL(UF_MODL_ask_references_of_features(feats, n_feats,  
                    &parents, &parent_names, &n_parents));  
                if (n_parents > 0)  
                {  
                    for (ii = 0; ii < n_parents; ii++)  
                        UF_CALL(UF_MODL_put_list_item(object_list, parents[ii]));  
                    UF_free(parents);  
                    UF_free_string_array(n_parents, parent_names);  
                }  
                UF_free(feats);  
            }  
            n_objects = make_an_array(&object_list, &objects);  
            matrix_csys2csys(r_csys, d_csys, mx);  
            FTN(uf5947)(mx, objects, &n_objects, &move, &original, &off, NULL,  
                NULL, &irc);  
            WRITENZ(irc);  
            UF_free(objects);  
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

> 这段代码是一个NX Open C++ API的二次开发代码，主要功能是重新定位一个或多个体的位置。具体步骤包括：
>
> 1. 提示用户选择一个体(Body)和一个参考坐标系(Reference csys)，然后选择一个目标坐标系(Destination csys)。
> 2. 查询体的特征列表，并获取特征的父对象，将体和父对象添加到列表中。
> 3. 计算从参考坐标系到目标坐标系的变换矩阵。
> 4. 应用变换矩阵，重新定位列表中的所有对象。
> 5. 重复以上步骤，直到用户选择取消。
> 6. 使用UF_print_syslog函数打印错误信息。
> 7. 在ufusr函数中初始化和终止NX Open。
> 8. 提供一个unload函数，以便卸载用户函数。
>
> 该代码的关键在于计算坐标系之间的变换矩阵，并应用该矩阵来移动对象。这需要使用NX Open的坐标系、矩阵运算和特征查询相关的API。
>
