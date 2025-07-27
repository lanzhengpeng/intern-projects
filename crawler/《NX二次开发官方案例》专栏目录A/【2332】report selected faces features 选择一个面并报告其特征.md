### 【2332】report selected faces features 选择一个面并报告其特征

#### 代码

```cpp
    /*HEAD REPORT_SELECTED_FACES_FEATURES CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_disp.h>  
    #include <uf_obj.h>  
    #include <uf_object_types.h>  
    #include <uf_modl.h>  
    #include <uf_part.h>  
    #include <uf_assem.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据提供的信息，翻译内容为：

注意：UF_print_syslog是V18版本新增的。 */  
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
    static int mask_for_faces(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_solid_type, 0, UF_UI_SEL_FEATURE_ANY_FACE };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_face(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog(prompt, "",  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_faces, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, FALSE));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    static void report_load_status(UF_PART_load_status_p_t status)  
    {  
        char  
            msg[133];  
        int  
            ii;  
        if (status->n_parts > 0)  
        {  
            UF_UI_open_listing_window();  
            UF_UI_write_listing_window("Load notes:\n");  
            for (ii = 0; ii < status->n_parts; ii++)  
            {  
                UF_UI_write_listing_window("  ");  
                UF_UI_write_listing_window(status->file_names[ii]);  
                UF_UI_write_listing_window(" - ");  
                UF_get_fail_message(status->statuses[ii], msg);  
                UF_UI_write_listing_window(msg);  
                UF_UI_write_listing_window("\n");  
            }  
            UF_UI_write_listing_window("\n");  
            UF_free(status->statuses);  
            UF_free_string_array(status->n_parts, status->file_names);  
        }  
    }  
    static void ensure_part_fully_loaded(tag_t part)  
    {  
        char  
            partname[MAX_FSPEC_SIZE+1];  
        UF_PART_load_status_t  
            error_status;  
        if (!UF_CALL(UF_PART_ask_part_name(part, partname))  
            && ((UF_PART_is_loaded(partname)) != 1))  
        {  
            UF_CALL(UF_PART_open_quiet(partname, &part, &error_status));  
            if (error_status.n_parts > 0) report_load_status(&error_status);  
        }  
    }  
    static void report_face_features(tag_t face)  
    {  
        int  
            ii,  
            count;  
        uf_list_p_t  
            list;  
        char  
            *feat_name,  
            *feat_type;  
        tag_t  
            feat,  
            owning_part;  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        if (UF_ASSEM_is_occurrence(face) &&  
            !UF_OBJ_is_object_a_promotion(face))  
        {  
            face = UF_ASSEM_ask_prototype_of_occ(face);  
            UF_CALL(UF_OBJ_ask_owning_part(face, &owning_part));  
            ensure_part_fully_loaded(owning_part);  
        }  
        UF_CALL(UF_MODL_ask_face_feats(face, &list));  
        UF_CALL(UF_MODL_ask_list_count(list, &count));  
        sprintf(msg, "Face (%d) has %d features\n", face, count);  
        UF_CALL(UF_UI_write_listing_window(msg));  
        for (ii = 0; ii < count; ii++)  
        {  
            UF_CALL(UF_MODL_ask_list_item(list, ii, &feat));  
            UF_CALL(UF_MODL_ask_feat_name(feat, &feat_name));  
            UF_CALL(UF_MODL_ask_feat_type(feat, &feat_type));  
            UF_CALL(UF_MODL_ask_feat_type(feat, &feat_type));  
            sprintf(msg, "  %d.  %s (%d - %s)\n", ii+1, feat_name, feat, feat_type);  
            UF_CALL(UF_UI_write_listing_window(msg));  
            UF_free(feat_type);  
            UF_free(feat_name);  
        }  
        UF_CALL(UF_MODL_delete_list(&list));  
    }  
    static void do_it(void)  
    {  
        tag_t  
            face;  
        UF_CALL(UF_UI_open_listing_window());  
        while ((face = select_a_face("Select face")) != NULL_TAG)  
            report_face_features(face);  
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

> 这段NX二次开发代码的主要功能是选择一个面，并报告该面上包含的所有特征。
>
> 代码的主要逻辑包括以下几个方面：
>
> 1. 定义了一个错误报告函数report_error，用于输出错误信息。
> 2. 定义了一个函数mask_for_faces，用于设置选择过滤，只选择实体类型的面。
> 3. 定义了一个函数select_a_face，用于弹出一个选择对话框，让用户选择一个面。
> 4. 定义了一个函数report_load_status，用于报告加载部件时的状态信息。
> 5. 定义了一个函数ensure_part_fully_loaded，用于确保指定的部件完全加载。
> 6. 定义了一个函数report_face_features，用于报告指定面上包含的所有特征信息。
> 7. 定义了一个函数do_it，用于循环选择面并报告特征，直到用户取消选择。
> 8. 定义了ufusr函数作为程序的入口，初始化后执行do_it，然后终止。
> 9. 定义了ufusr_ask_unload函数，用于立即卸载该程序。
>
> 通过这个程序，用户可以方便地查看一个面上包含的所有特征信息，从而更好地理解模型的特征结构。
>
