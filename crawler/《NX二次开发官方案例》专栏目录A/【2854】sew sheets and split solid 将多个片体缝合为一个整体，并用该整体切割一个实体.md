### 【2854】sew sheets and split solid 将多个片体缝合为一个整体，并用该整体切割一个实体

#### 代码

```cpp
    /*HEAD SEW_SHEETS_AND_SPLIT_SOLID CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_disp.h>  
    #include <uf_modl.h>  
    #include <uf_assem.h>  
    #include <uf_obj.h>  
    #include <uf_so.h>  
    #include <uf_wave.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:翻译如下：

注意：UF_print_syslog是V18版本新增的功能，请只回答翻译内容，不要添加其他无关内容。 */  
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
    static int filter_body_type(tag_t object, int type[3], void *data,  
        UF_UI_selection_p_t select)  
    {  
        int  
            body_type;  
        UF_CALL(UF_MODL_ask_body_type(object, &body_type));  
        if (body_type == *(int *)data)  
            return (UF_UI_SEL_ACCEPT);  
        else  
            return (UF_UI_SEL_REJECT);  
    }  
    static int mask_body_type( UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_solid_type, 0, UF_UI_SEL_FEATURE_BODY };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)) &&  
            !UF_CALL(UF_UI_set_sel_procs(select, filter_body_type, NULL, type)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static int select_sheets(char *prompt, tag_t **sheets)  
    {  
        int  
            ii,  
            n,  
            sheet = UF_MODL_SHEET_BODY,  
            resp;  
        UF_CALL(UF_UI_select_with_class_dialog( "Select sheet bodies", prompt,  
            UF_UI_SEL_SCOPE_WORK_PART, mask_body_type, &sheet, &resp, &n, sheets));  
        for (ii = 0; ii < n; ii++)  
            UF_CALL(UF_DISP_set_highlight((*sheets)[ii], FALSE));  
        return n;  
    }  
    static tag_t select_a_solid(char *prompt)  
    {  
        double  
            cp[3];  
        int  
            solid = UF_MODL_SOLID_BODY,  
            resp;  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog( "Select a solid", prompt,  
            UF_UI_SEL_SCOPE_WORK_PART, mask_body_type, &solid, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
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
    static tag_t ask_part_occ_of_prom_occ(tag_t prom_object)  
    {  
        tag_t  
            owning_part,  
            part_occ = prom_object,  
            proto;  
        proto = UF_ASSEM_ask_prototype_of_occ(prom_object);  
        UF_CALL(UF_OBJ_ask_owning_part(proto, &owning_part));  
        while (!UF_CALL(UF_ASSEM_ask_parent_component(part_occ, &part_occ))  
            && (UF_ASSEM_ask_prototype_of_occ(part_occ) != owning_part));  
        return part_occ;  
    }  
    static tag_t create_dumb_body_copy(tag_t body)  
    {  
        tag_t  
            feat,  
            part_occ,  
            proto,  
            xform,  
            dumb_body;  
        uf_list_p_t  
            list;  
        if (UF_ASSEM_is_occurrence(body))  
            proto = UF_ASSEM_ask_prototype_of_occ(body);  
        else  
            proto = body;  
        if (UF_OBJ_is_object_a_promotion(proto))  
            part_occ = ask_part_occ_of_prom_occ(body);  
        else  
            part_occ = UF_ASSEM_ask_part_occurrence(body);  
        if (part_occ == NULL_TAG)  
            UF_CALL(UF_WAVE_create_linked_body(proto,NULL_TAG,body,FALSE,&feat));  
        else  
        {  
            UF_CALL(UF_SO_create_xform_assy_ctxt(body, part_occ, NULL_TAG, &xform));  
            UF_CALL(UF_WAVE_create_linked_body(proto, xform, body, FALSE, &feat));  
        }  
        UF_CALL(UF_MODL_ask_feat_body(feat, &dumb_body));  
        if (dumb_body != NULL_TAG)  
        {  
            list = make_a_list(1, &dumb_body);  
            UF_CALL(UF_MODL_delete_object_parms(list));  
            UF_CALL(UF_MODL_delete_list(&list));  
        }  
        return dumb_body;  
    }  
    static void do_it(void)  
    {  
        int  
            n_bodies,  
            n_sheets;  
        tag_t  
            *bodies,  
            sew = NULL_TAG,  
            sheet,  
            *sheets,  
            solid;  
        double  
            tol;  
        uf_list_p_t  
            disjoint_list = NULL;  
        UF_MODL_ask_distance_tolerance(&tol);  
        while (((n_sheets = select_sheets("Select sheets to sew", &sheets)) > 1)  
            && ((solid = select_a_solid("Select solid body to split")) != NULL_TAG))  
        {  
            UF_CALL(UF_MODL_create_sew(0, 1, sheets, n_sheets-1, &sheets[1], tol,  
                0, &disjoint_list, &sew));  
            if (disjoint_list != NULL)  
                UF_CALL(UF_MODL_delete_list(&disjoint_list));  
            UF_CALL(UF_MODL_ask_feat_body(sew, &sheet));  
        /*  UF_MODL_split_body deletes the cutting body, so make a copy first 里海译:UF_MODL_split_body会删除切割体，所以请先复制一份。 */  
            sheet = create_dumb_body_copy(sheet);  
            UF_CALL(UF_MODL_split_body(1, &solid, sheet, &n_bodies, &bodies));  
            if (n_bodies > 0) UF_free(bodies);  
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

> 这段代码是一个NX二次开发示例，主要功能是缝合多个面片并切割实体。代码的关键点如下：
>
> 1. 错误处理：使用UF_CALL宏来封装NX API调用，并在出错时打印错误信息。
> 2. 选择面片：使用自定义的filter_body_type函数来选择面片，并通过UF_UI_select_with_class_dialog对话框实现。
> 3. 选择实体：使用自定义的filter_body_type函数来选择实体，并通过UF_UI_select_with_single_dialog对话框实现。
> 4. 创建缝合特征：使用UF_MODL_create_sew函数创建缝合特征，将多个面片缝合为一个整体。
> 5. 创建切割体：使用UF_WAVE_create_linked_body函数创建切割体的副本，避免切割时删除原始面片。
> 6. 切割实体：使用UF_MODL_split_body函数用切割体切割实体，产生多个新实体。
> 7. 主函数：在ufusr函数中初始化NX系统，执行主逻辑，然后终止NX系统。
> 8. 卸载函数：定义了卸载函数，返回立即卸载标志。
>
> 总体来说，这段代码实现了选择面片、创建缝合特征、选择实体、创建切割体、切割实体等功能，并进行了错误处理和对话框交互。
>
