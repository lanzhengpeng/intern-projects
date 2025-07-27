### 【1879】reparent broken wave links 修复断开的波形链接

#### 代码

```cpp
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_assem.h>  
    #include <uf_obj.h>  
    #include <uf_object_types.h>  
    #include <uf_wave.h>  
    #include <uf_part.h>  
    #include <uf_modl.h>  
    #include <uf_so.h>  
    #include <stdarg.h>  
    static void ECHO(char *format, ...)  
    {  
        char msg[UF_UI_MAX_STRING_LEN+1];  
        va_list args;  
        va_start(args, format);  
        vsprintf(msg, format, args);  
        va_end(args);  
        UF_UI_open_listing_window();  
        UF_UI_write_listing_window(msg);  
        UF_print_syslog(msg, FALSE);  
    }  
    #define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))  
    static int report_error( char *file, int line, char *call, int irc)  
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
    static tag_t ask_next_wave_linked_feature(tag_t part, tag_t feat)  
    {  
        char  
            *this_type;  
        while (!UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_feature_type, &feat))  
            && (feat != NULL_TAG))  
        {  
            if (UF_OBJ_ask_status(feat) == UF_OBJ_ALIVE)  
            {  
                UF_CALL(UF_MODL_ask_feat_type(feat, &this_type));  
                if (strstr(this_type, "LINK"))  
                {  
                    UF_free(this_type);  
                    return feat;  
                }  
                UF_free(this_type);  
            }  
        }  
        return NULL_TAG;  
    }  
    /* qq3123197280 */  
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
    static tag_t select_anything(char * title, char *prompt)  
    {  
        double  
            cp[3];  
        int  
            resp;  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_set_cursor_view(0));  
        UF_CALL(UF_UI_select_with_single_dialog( title, prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_add_faces_and_edges, NULL, &resp,  
            &object, cp, &view));  
        UF_CALL(UF_UI_set_cursor_view(1));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    static logical ask_yes_or_no(char *title, char *message)  
    {  
        int  
            resp;  
        UF_UI_message_buttons_t  
            buttons = { TRUE, FALSE, TRUE, "Yes", NULL, "No", 1, 0, 2 };  
        UF_CALL(UF_UI_message_dialog(title, UF_UI_MESSAGE_QUESTION, &message, 1,  
            TRUE, &buttons, &resp));  
        return (2 - resp);  
    }  
    static int ask_all_other_parts(tag_t except_part, tag_t **parts)  
    {  
        int  
            err,  
            ii, jj,  
            n = UF_PART_ask_num_parts();  
        tag_t  
            a_part;  
        if (n > 1)  
        {  
            *parts = UF_allocate_memory((n-1) * sizeof(tag_t), &err);  
            if (UF_CALL(err)) return 0;  
            for (ii = 0, jj = 0; ii < n; ii++)  
            {  
                a_part = UF_PART_ask_nth_part(ii);  
                if (a_part != except_part) (*parts)[jj++] = a_part;  
            }  
        }  
        return n  - 1;  
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
                case 1:                     /* Back 里海译:Back */  
                    if (a == 0) return -1;  
                    a = a - 13;  
                    break;  
                case 2:                     /* Cancel 里海译:取消 */  
                    return -1;  
                    break;  
                case 18:                    /* More... 里海译:More... */  
                    if ((a + z) < n_choices)  
                    {  
                        a = a + 13;  
                        break;  
                    }  
                default:                    /* Picked one 里海译:挑选了一个 */  
                    return a + resp - 5;  
                    break;  
            }  
        } while (TRUE);  
    }  
    static tag_t choose_another_loaded_part(char *prompt)  
    {  
        int  
            ii,  
            n_parts,  
            which;  
        tag_t  
            the_part,  
            this_part = UF_PART_ask_display_part(),  
            *parts;  
        char  
            **leaf_names;  
        n_parts = ask_all_other_parts(this_part, &parts);  
        if (n_parts == 0) return NULL_TAG;  
        if (n_parts == 1)  
        {  
            the_part = parts[0];  
            UF_free(parts);  
            return the_part;  
        }  
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
        logical  
            here,  
            is_broken;  
        tag_t  
            feat = NULL_TAG,  
            other_part,  
            part = UF_PART_ask_display_part(),  
            source,  
            xform;  
        char  
            *name,  
            prompt[133];  
        while ((feat = ask_next_wave_linked_feature(part, feat)) != NULL_TAG)  
        {  
            if (UF_CALL(UF_WAVE_is_link_broken(feat, &is_broken)) || !is_broken)  
                continue;  
            UF_CALL(UF_MODL_ask_feat_name(feat, &name));  
            sprintf(prompt, "for %s", name);  
            UF_free(name);  
            UF_CALL(UF_DISP_set_highlight(feat, TRUE));  
            here = ask_yes_or_no(prompt, "Is new source object in this part?");  
            if (!here)  
            {  
                other_part = choose_another_loaded_part("Which one?");  
                UF_CALL(UF_PART_set_display_part(other_part));  
            }  
            source = select_anything(prompt, "Select new source object");  
            if (!here) UF_CALL(UF_PART_set_display_part(part));  
            UF_CALL(UF_DISP_set_highlight(feat, FALSE));  
            if (source == NULL_TAG) continue;  
            if (UF_ASSEM_is_occurrence(source))  
            {  
                UF_CALL(UF_SO_create_xform_assy_ctxt(source,  
                    UF_ASSEM_ask_part_occurrence(source), NULL_TAG, &xform));  
                source = UF_ASSEM_ask_prototype_of_occ(source);  
            }  
            else  
                xform = NULL_TAG;  
            UF_CALL(UF_MODL_ask_feat_sysname(feat, &name));  
            if (strstr(name, "Linked Point"))  
                UF_CALL(UF_WAVE_set_linked_pt_point(feat, source, xform));  
            else  
                UF_CALL(UF_WAVE_set_link_data(feat, source, xform, TRUE));  
            UF_free(name);  
        }  
        UF_CALL(UF_MODL_update());  
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

> 这是NX Open C++ API编写的二次开发代码，主要功能是修复NX中损坏的链接特征。代码主要包括以下几个部分：
>
> 1. 错误报告函数：用于报告错误，包括文件名、行号、调用函数名和错误码。
> 2. 获取链接特征：通过循环遍历零件中的特征，找出类型包含"LINK"的特征。
> 3. 选择新源对象：提供一个对话框，让用户选择新的源对象来修复损坏的链接特征。
> 4. 切换当前零件：如果新源对象不在当前零件中，则切换到另一个零件，让用户选择新源对象。
> 5. 修复链接特征：根据特征类型，调用相应的函数来设置新源对象和变换矩阵，以修复损坏的链接特征。
> 6. 主函数：遍历所有损坏的链接特征，并调用函数来修复它们。
> 7. 卸载函数：返回立即卸载二次开发程序的标志。
>
> 总体来说，这段代码实现了在NX中批量修复损坏链接特征的功能，通过对话框让用户选择新源对象，从而实现自动化修复。
>
