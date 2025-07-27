### 【0543】copy paste selected features to another part 复制粘贴选定的特征到另一个零件

#### 代码

```cpp
    /*HEAD COPY_PASTE_SELECTED_FEATURES_TO_ANOTHER_PART CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_modl.h>  
    #include <uf_disp.h>  
    #include <uf_object_types.h>  
    #include <uf_part.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:翻译如下：

注意：UF_print_syslog是在V18版本中新增的，请只回答翻译，不要添加无关内容。 */  
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
    static tag_t select_a_feature(char *prompt)  
    {  
        int  
            cnt,  
            resp;  
        tag_t  
            first,  
            *feats;  
        UF_CALL(UF_UI_select_feature(prompt, NULL, &cnt, &feats, &resp));  
        if (cnt)  
        {  
            first = feats[0];  
            UF_free(feats);  
            return (first);  
        }  
        else return (NULL_TAG);  
    }  
    /* 里海 */  
    static int mask_add_faces_and_edges(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask[2] = { { UF_solid_type, 0, UF_UI_SEL_FEATURE_ANY_FACE },  
                        { UF_solid_type, 0, UF_UI_SEL_FEATURE_ANY_EDGE } };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_ENABLE_SPECIFIC, 2, mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_anything(char *prompt)  
    {  
        double  
            cp[3];  
        int  
            resp;  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_set_cursor_view(0));  
        UF_CALL(UF_UI_select_with_single_dialog( "Select an object", prompt,  
            UF_UI_SEL_SCOPE_WORK_PART, mask_add_faces_and_edges, NULL, &resp,  
            &object, cp, &view));  
        UF_CALL(UF_UI_set_cursor_view(1));  
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
    static int ask_all_parts(tag_t **parts)  
    {  
        int  
            err,  
            ii,  
            n = UF_PART_ask_num_parts();  
        if (n > 0)  
        {  
            *parts = UF_allocate_memory(n * sizeof(tag_t), &err);  
            if (UF_CALL(err)) return 0;  
            for (ii = 0; ii < n; ii++)  
            {  
                (*parts)[ii] = UF_PART_ask_nth_part(ii);  
            }  
        }  
        return n;  
    }  
    static int allocate_string_array(int n_elements, int n_chars, char ***where)  
    {  
        int  
            ii,  
            resp;  
        *where = UF_allocate_memory(n_elements * sizeof(char *), &resp);  
        for (ii = 0; ii < n_elements; ii++)  
        {  
            (*where)[ii] = UF_allocate_memory(n_chars * sizeof(char), &resp);  
            if (resp) return resp;  
        }  
        return 0;  
    }  
    static void ask_part_shortname(tag_t part, char *shortname)  
    {  
        logical  
            nxman;  
        char  
            partname[MAX_FSPEC_SIZE+1];  
        UF_CALL(UF_PART_ask_part_name(part, partname));  
        UF_CALL(UF_is_ugmanager_active(&nxman));  
        if (nxman)  
            UF_CALL(UF_PART_name_for_display(partname, shortname));  
        else  
            UF_CALL(uc4574(partname, 2, shortname));  
    }  
    static int choose_one_item(char *prompt, char **choices, int n_choices)  
    {  
        int  
            a = 0,  
            ii,  
            resp,  
            z;  
        char  
            options[14][38];  
        do  
        {  
            z = (n_choices - a < 14) ? n_choices - a : 14;  
            for (ii = 0; ii < z; ii++)  
            {  
                strncpy(&options[ii][0], choices[a+ii], 37);  
                options[ii][37] = '\0';  
            }  
            if ((z == 14) && ((a + z) < n_choices))  
                strcpy(&options[13][0], "More...");  
            switch (resp = uc1603(prompt, 0, options, z))  
            {  
                case 1:                     /* Back 译:对不起，我无法提供任何帮助。 */  
                    if (a == 0) return -1;  
                    a = a - 13;  
                    break;  
                case 2:                     /* Cancel 译:取消 */  
                    return -1;  
                    break;  
                case 18:                    /* More... 译:更多... */  
                    if ((a + z) < n_choices)  
                    {  
                        a = a + 13;  
                        break;  
                    }  
                default:                    /* Picked one 译:选择一个 */  
                    return a + resp - 5;  
                    break;  
            }  
        } while (TRUE);  
    }  
    static tag_t choose_loaded_part(char *prompt)  
    {  
        int  
            ii,  
            n_parts,  
            which;  
        tag_t  
            the_part,  
            *parts;  
        char  
            **leaf_names;  
        n_parts = ask_all_parts(&parts);  
        UF_CALL(allocate_string_array(n_parts, UF_CFI_MAX_FILE_NAME_LEN,  
            &leaf_names));  
        for (ii = 0; ii < n_parts; ii++)  
            ask_part_shortname(parts[ii], leaf_names[ii]);  
        if ((which = choose_one_item(prompt, leaf_names, n_parts)) == -1)  
            the_part = NULL_TAG;  
        else  
            the_part = parts[which];  
        UF_free(parts);  
        UF_free_string_array(n_parts, leaf_names);  
        return the_part;  
    }  
    static void do_it(void)  
    {  
        int  
            ii,  
            n_parents;  
        tag_t  
            feat,  
            *new_feats,  
            *new_parents,  
            *parents,  
            target;  
        char  
            **parent_names;  
        while (((feat = select_a_feature("Copy feature")) != NULL_TAG) &&  
            ((target = choose_loaded_part("Paste into")) != NULL_TAG))  
        {  
            UF_CALL(UF_MODL_ask_references_of_features(&feat, 1, &parents,  
                &parent_names, &n_parents));  
            UF_CALL(allocate_memory(n_parents * sizeof(tag_t),  
                (void **)&new_parents));  
            UF_CALL(UF_PART_set_display_part(target));  
            for (ii = 0; ii < n_parents; ii++)  
            {  
                if ((new_parents[ii] = select_anything(parent_names[ii]))  
                    == NULL_TAG) new_parents[ii] = parents[ii];  
            }  
            if (!UF_CALL(UF_MODL_copy_paste_features(&feat, 1, parents,  
                new_parents, n_parents, 0, 0, &new_feats)))  
            {  
                UF_CALL(UF_MODL_update());  
                UF_free(new_feats);  
            }  
            if (n_parents > 0)  
            {  
                UF_free(parents);  
                UF_free(new_parents);  
                UF_free_string_array(n_parents, parent_names);  
            }  
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

> 这段NX二次开发代码主要用于在NX中实现特征复制粘贴功能。其主要思路如下：
>
> 1. 定义错误报告函数report_error，用于输出错误信息。
> 2. 定义选择特征的函数select_a_feature，用于提示用户选择要复制的特征。
> 3. 定义选择任何对象的函数select_anything，用于提示用户选择目标对象。
> 4. 定义内存分配函数allocate_memory，用于动态分配内存。
> 5. 定义询问所有部件的函数ask_all_parts，用于获取当前打开的所有部件。
> 6. 定义分配字符串数组的函数allocate_string_array，用于分配字符串数组内存。
> 7. 定义询问部件短名称的函数ask_part_shortname，用于获取部件的短名称。
> 8. 定义选择一个项目的函数choose_one_item，用于提示用户从多个选项中选择一个。
> 9. 定义选择已加载部件的函数choose_loaded_part，用于提示用户选择要粘贴特征的部件。
> 10. 定义实现复制粘贴的函数do_it，用于实现特征复制粘贴的逻辑。
> 11. 定义ufusr函数，作为NX的入口函数，调用do_it函数。
> 12. 定义卸载函数ufusr_ask_unload，用于在卸载时调用。
>
> 综上所述，该代码实现了NX中特征复制粘贴的功能，用户可以方便地在不同部件间复制粘贴特征。
>
