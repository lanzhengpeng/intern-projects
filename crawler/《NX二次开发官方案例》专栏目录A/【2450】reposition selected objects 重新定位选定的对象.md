### 【2450】reposition selected objects 重新定位选定的对象

#### 代码

```cpp
    /*HEAD REPOSITION_SELECTED_OBJECTS CCC UFUN */  
    /*  
        This program demonstrates a method of transforming objects by  
        exporting them to a dummy part and importing them to a new  
        location.  
        This can be used instead of uf5947 which will not work when  
        attempting to transform objects which when transformed  
        interactively cause this message to be raised:  
    Not all parents of highlighted parametric objects were selected for transform.  
    OK to continue without transforming those parametric objects.  
    Choose Transform Parents to transform those objects and their parents.  
        Another work around would be remove all the parameters from the objects  
        being transformed using UF_MODL_delete_object_parms.  
        Another work around would be to track down the parent objects and  
        add them to the array to be transformed using uf5947, but this is  
        not always easy.  
    */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_disp.h>  
    #include <uf_part.h>  
    #include <uf_cfi.h>  
    #include <uf_csys.h>  
    #include <uf_obj.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:翻译如下：

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
    static int select_objects(char *prompt, tag_t **objects)  
    {  
        int  
            cnt,  
            ii,  
            resp;  
        UF_CALL(UF_UI_select_with_class_dialog("Select Objects", prompt,  
            UF_UI_SEL_SCOPE_WORK_PART, NULL, NULL, &resp, &cnt, objects));  
        for (ii = 0; ii < cnt; ii++)  
            UF_CALL(UF_DISP_set_highlight((*objects)[ii], 0));  
        return cnt;  
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
    static void build_unique_temp_part_name(char *fspec)  
    {  
        char  
            *tmp_dir,  
            unique[UF_CFI_MAX_FILE_NAME_SIZE];  
        UF_CALL(UF_translate_variable("UGII_TMP_DIR", &tmp_dir));  
        UF_CALL(uc4577(unique));  
        UF_CALL(uc4575(tmp_dir, 2, unique, fspec));  
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
    static int ask_whats_new(tag_t *old, int n_old, tag_t *now, int n_now,  
        tag_t **new)  
    {  
        int  
            ii,  
            jj;  
        uf_list_p_t  
            list;  
        UF_CALL(UF_MODL_create_list(&list));  
        for (ii = 0; ii < n_now; ii++)  
        {  
            for (jj = 0; jj < n_old; jj++)  
                if (old[jj] == now[ii]) break;  
            if (jj == n_old) UF_CALL(UF_MODL_put_list_item(list, now[ii]));  
        }  
        return (make_an_array(&list, new));  
    }  
    static void rename_expression_without_prime(tag_t exp)  
    {  
        char  
            *lhs,  
            tmp_name[UF_MAX_EXP_LENGTH+1],  
            *rhs,  
            *string;  
        UF_CALL(UF_MODL_ask_exp_tag_string(exp, &string));  
        UF_CALL(UF_MODL_dissect_exp_string(string, &lhs, &rhs, &exp));  
        strcpy(tmp_name, lhs);  
        strcpy(strrchr(tmp_name, 39), ""); /* 39 = ' 里海译:39 = ', just answer the translation, no nonsense. */  
        UF_MODL_rename_exp(lhs, tmp_name);  
        UF_free(string);  
        UF_free(rhs);  
        UF_free(lhs);  
    }  
    static void rename_expressions_without_primes(tag_t *exps, int n_exps)  
    {  
        int ii;  
        for (ii = 0; ii < n_exps; ii++) rename_expression_without_prime(exps[ii]);  
    }  
    static void do_it(void)  
    {  
        int  
            n_after,  
            n_before,  
            n_objects,  
            n_new;  
        tag_t  
            *after,  
            *before,  
            d_csys,  
            group,  
            matrix,  
            *new,  
            *objects,  
            part = UF_PART_ask_display_part(),  
            r_csys,  
            wcs;  
        double  
            dest_csys[9],  
            dest_point[3];  
        char  
            fspec[UF_CFI_MAX_FILE_NAME_SIZE];  
        UF_PART_export_options_t  
            opts = { TRUE, UF_PART_maintain_all_params, UF_PART_copy_exp_deeply };  
        UF_import_part_modes_t  
            modes = { 1, 0, 0, 0, 0, FALSE, 0 };  
        while (  
            ((n_objects = select_objects("Reposition", &objects)) > 0) &&  
            ((r_csys = select_a_csys("Reference csys")) != NULL_TAG) &&  
            ((d_csys = select_a_csys("Destination csys")) != NULL_TAG))  
        {  
            UF_CALL(UF_CSYS_ask_wcs(&wcs));  
            UF_CALL(UF_CSYS_set_wcs(r_csys));  
            build_unique_temp_part_name(fspec);  
            UF_CALL(UF_PART_export_with_options(fspec, n_objects, objects, &opts));  
            UF_CALL(UF_CSYS_set_wcs(wcs));  
            UF_CALL(UF_MODL_ask_exps_of_part(part, &n_before, &before));  
            UF_CALL(UF_CSYS_ask_csys_info(d_csys, &matrix, dest_point));  
            UF_CALL(UF_CSYS_ask_matrix_values(matrix, dest_csys));  
            UF_CALL(UF_PART_import(fspec, &modes, dest_csys, dest_point, 1.0,  
                &group));  
            UF_CALL(uc4561(fspec, 2));  
            UF_CALL(UF_MODL_ask_exps_of_part(part, &n_after, &after));  
            UF_CALL(UF_OBJ_delete_array_of_objects(n_objects, objects, NULL));  
            n_new = ask_whats_new(before, n_before, after, n_after, &new);  
            rename_expressions_without_primes(new, n_new);  
            if (n_after > 0) UF_free(after);  
            if (n_before > 0) UF_free(before);  
            if (n_new > 0) UF_free(new);  
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

> 这段代码是一个NX二次开发程序，其主要功能是实现对NX中选定对象的重新定位。
>
> 具体来说，代码的主要流程包括：
>
> 1. 选择要重新定位的对象。
> 2. 选择参考坐标系。
> 3. 选择目标坐标系。
> 4. 将选定对象导出为临时文件。
> 5. 将临时文件导入到新的位置，基于目标坐标系。
> 6. 删除原始对象，保留新的对象。
> 7. 对新对象的表达式进行重命名，去除表达式中的单引号。
> 8. 重复以上步骤，直到用户选择不再继续。
>
> 该代码通过将对象导出后再导入的方式，绕过了直接变换对象可能遇到的问题，如参数化对象变换时需要变换其父对象等。通过导出导入，避免了这些问题，实现了对选定对象的重新定位。
>
> 代码中还包含了错误处理、内存分配、列表操作等辅助功能，以确保程序的健壮性。总体来说，这是一个比较完整的NX二次开发程序示例。
>
