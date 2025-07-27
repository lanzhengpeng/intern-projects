### 【0699】create feature parameters dimension 创建特征参数尺寸

#### 代码

```cpp
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_drf.h>  
    #include <uf_draw.h>  
    #include <uf_view.h>  
    #include <uf_modl.h>  
    #include <uf_obj.h>  
    #include <uf_object_types.h>  
    #include <uf_part.h>  
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
    static tag_t select_drawing_member_view(char *prompt)  
    {  
        int  
            orig_cv,  
            resp;  
        tag_t  
            view_tag;  
        double  
            pos[3];  
        UF_CALL(UF_UI_ask_cursor_view(&orig_cv));  
        UF_CALL(UF_UI_set_cursor_view(0));  
        UF_CALL(UF_UI_specify_screen_position(prompt, NULL, NULL, pos,  
            &view_tag, &resp));  
        UF_CALL(UF_UI_set_cursor_view(orig_cv));  
        if (resp == UF_UI_PICK_RESPONSE)  
            return view_tag;  
        else  
            return NULL_TAG;  
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
                case 1:                     /* Back 译:Back */  
                    if (a == 0) return -1;  
                    a = a - 13;  
                    break;  
                case 2:                     /* Cancel 译:Cancel */  
                    return -1;  
                    break;  
                case 18:                    /* More... 译:More... */  
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
    #include <uf_undo.h>  
    static void set_undo_mark(char *label)  
    {  
        UF_UNDO_mark_id_t  
            mark_id;  
        UF_CALL(UF_UNDO_set_mark(UF_UNDO_visible, label, &mark_id));  
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
    static tag_t ask_next_feature(tag_t part, tag_t feat)  
    {  
        while (!UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_feature_type, &feat))  
            && (feat != NULL_TAG))  
            if (UF_OBJ_ask_status(feat) == UF_OBJ_ALIVE) return feat;  
        return NULL_TAG;  
    }  
    static int ask_all_features_of_same_name(tag_t match_feat, tag_t **feats)  
    {  
        tag_t  
            part,  
            feat = NULL_TAG;  
        char  
            *match_name,  
            *name;  
        uf_list_p_t  
            feat_list;  
        UF_CALL(UF_OBJ_ask_owning_part(match_feat, &part));  
        UF_CALL(UF_MODL_ask_feat_name(match_feat, &match_name));  
        UF_CALL(UF_MODL_create_list(&feat_list));  
        while ((feat = ask_next_feature(part, feat)) != NULL_TAG)  
        {  
            UF_CALL(UF_MODL_ask_feat_name(feat, &name));  
            if (!strcmp(name, match_name))  
                UF_CALL(UF_MODL_put_list_item(feat_list, feat));  
            UF_free(name);  
        }  
        UF_free(match_name);  
        return (make_an_array(&feat_list, feats));  
    }  
    static int ask_all_of_type(tag_t part, int type, tag_t **objects)  
    {  
        tag_t  
            object = NULL_TAG;  
        uf_list_p_t  
            object_list;  
        UF_CALL(UF_MODL_create_list(&object_list));  
        while (!UF_CALL(UF_OBJ_cycle_objs_in_part(part, type, &object)) &&  
                (object != NULL_TAG))  
            UF_CALL(UF_MODL_put_list_item(object_list, object));  
        return (make_an_array(&object_list, objects));  
    }  
    static int ask_whats_new(tag_t *old, int n_old, tag_t *now, int n_now,  
        tag_t **whats_new)  
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
        return (make_an_array(&list, whats_new));  
    }  
    static void set_highlight_object_array(int n, tag_t *objects, int flag)  
    {  
        int  
            ii;  
        for (ii = 0; ii < n; ii++)  
                UF_CALL(UF_DISP_set_highlight(objects[ii], flag));  
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
    static void show_each_callout_member(tag_t callout)  
    {  
        int  
            ii, jj,  
            n_members,  
            n_rows;  
        tag_t  
            *members;  
        char  
            msg[133];  
        UF_CALL(UF_DRF_ask_number_rows_in_callout(callout, &n_rows));  
        for (ii = 0; ii < n_rows; ii++)  
        {  
            UF_CALL(UF_DRF_ask_callout_row_members(callout, ii, &n_members,  
                &members));  
            for (jj = 0; jj < n_members; jj++)  
            {  
                UF_CALL(UF_DISP_set_highlight(members[jj], TRUE));  
                sprintf(msg, "Row %d - member %d", ii, jj);  
                if (ask_yes_or_no(msg, "Delete callout member?"))  
                    UF_CALL(UF_OBJ_delete_object(members[jj]));  
                else  
                    UF_CALL(UF_DISP_set_highlight(members[jj], FALSE));  
            }  
            if (n_members > 0) UF_free(members);  
        }  
    }  
    static void do_it(void)  
    {  
        int  
            n_before,  
            n_after,  
            n_created,  
            ii,  
            n_feats,  
            resp,  
            which;  
        tag_t  
            *after,  
            *before,  
            *created,  
            feat,  
            *feats,  
            *occs,  
            part = UF_PART_ask_display_part(),  
            view;  
        char  
            msg[133],  
            *templates[] = { "ansi", "ansi_mm", "iso_din", "jis" };  
        n_before = ask_all_of_type(part, UF_draft_callout_type, &before);  
        which = choose_one_item("Template", templates, 4);  
        if (((feat = select_a_feature("Feature Parameters")) != NULL_TAG) &&  
            ((view = select_drawing_member_view("Feature Parameters")) != NULL_TAG))  
        {  
            set_undo_mark("Inherit Feature Parameters");  
            UF_CALL(UF_DRF_set_annotation_template(templates[which]));  
            n_feats = ask_all_features_of_same_name(feat, &feats);  
            occs = UF_allocate_memory(n_feats * sizeof(tag_t), &resp);  
            UF_CALL(resp);  
            for (ii = 0; ii < n_feats; ii++) occs[ii] = NULL_TAG;  
            UF_CALL(UF_DRF_inherit_feature_data(n_feats, feats, occs, 1,  
                &view, UF_DRF_INHERIT_FEATURE_PARAMETERS));  
            UF_free(feats);  
            UF_free(occs);  
            n_after = ask_all_of_type(part, UF_draft_callout_type, &after);  
            n_created = ask_whats_new(before, n_before, after, n_after, &created);  
            if (n_before > 0) UF_free(before);  
            if (n_after > 0) UF_free(after);  
            if (n_created > 0)  
            {  
                set_highlight_object_array(n_created, created, TRUE);  
                sprintf(msg, "created %d callouts", n_created);  
                uc1601(msg, TRUE);  
                set_highlight_object_array(n_created, created, FALSE);  
                for (ii = 0; ii < n_created; ii++)  
                    show_each_callout_member(created[ii]);  
                UF_free(created);  
            }  
            else  
                uc1601("No callouts created", TRUE);  
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

> 这段代码是一个NX二次开发程序，主要用于在NX中创建基于特征的标注。以下是代码的主要功能：
>
> 1. 初始化NX系统，并在系统关闭时卸载程序。
> 2. 选择标注模板，如ANSI、ISO、JIS等。
> 3. 选择要提取标注数据的特征，以及放置标注的视图。
> 4. 对所有与选定特征同名的特征进行标注数据的继承，以生成标注。
> 5. 在生成标注后，高亮显示新创建的标注，并提示用户删除或保留。
> 6. 代码中包含了错误处理、内存分配、数组操作等辅助函数。
> 7. 使用了UF对象循环、列表操作等NX API。
> 8. 代码结构清晰，注释详细，可读性强。
> 9. 充分利用了NX提供的二次开发接口，实现了所需功能。
> 10. 总体来说，这是一段功能完整、代码质量较高的NX二次开发程序。
>
