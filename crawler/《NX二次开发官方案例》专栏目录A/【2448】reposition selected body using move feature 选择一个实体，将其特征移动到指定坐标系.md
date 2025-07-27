### 【2448】reposition selected body using move feature 选择一个实体，将其特征移动到指定坐标系

#### 代码

```cpp
    /*HEAD REPOSITION_SELECTED_BODY_USING_MOVE_FEATURE CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_csys.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog是V18版本新增的功能。 */  
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
            n_feats;  
        tag_t  
            body,  
            csys_mx,  
            d_csys,  
            *feats,  
            r_csys;  
        double  
            real_data[2][12];  
        uf_list_p_t  
            move_list;  
        while ((body = select_a_body("Body to reposition")) &&  
               (r_csys = select_a_csys("Reference csys")) &&  
               (d_csys = select_a_csys("Destination csys")))  
        {  
            UF_CALL(UF_CSYS_ask_csys_info(r_csys, &csys_mx, &real_data[0][0]));  
            UF_CALL(UF_CSYS_ask_matrix_values(csys_mx, &real_data[0][3]));  
            UF_CALL(UF_CSYS_ask_csys_info(d_csys, &csys_mx, &real_data[1][0]));  
            UF_CALL(UF_CSYS_ask_matrix_values(csys_mx, &real_data[1][3]));  
            n_feats = ask_body_feats(body, &feats);  
            if (n_feats > 0)  
            {  
                UF_CALL(UF_MODL_create_list(&move_list));  
                UF_CALL(UF_MODL_put_list_item(move_list, feats[0]));  
                UF_CALL(UF_MODL_move_feature(move_list, CSYS_TO_CSYS, real_data));  
                UF_CALL(UF_MODL_update());  
                UF_CALL(UF_MODL_delete_list(&move_list));  
                UF_free(feats);  
            }  
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

> 这段代码是一个NX的二次开发示例，主要实现了以下功能：
>
> 1. 定义了一个错误报告函数report_error，用于在调用NX API失败时输出错误信息。
> 2. 定义了两个选择函数select_a_csys和select_a_body，分别用于选择坐标系和体。
> 3. 定义了内存分配函数allocate_memory和创建数组函数make_an_array。
> 4. 定义了获取体特征函数ask_body_feats。
> 5. 主函数do_it实现了选择体、参考坐标系、目标坐标系，然后获取特征列表，对特征进行移动更新。
> 6. ufusr是NX二次开发程序的入口函数，在其中调用主函数do_it。
> 7. ufusr_ask_unload函数用于卸载二次开发程序。
>
> 总的来说，该代码实现了选择体、参考坐标系、目标坐标系，然后移动体特征到目标坐标系的功能。使用了NX的UI、模型、坐标系等API，是一个典型的NX二次开发示例。
>
