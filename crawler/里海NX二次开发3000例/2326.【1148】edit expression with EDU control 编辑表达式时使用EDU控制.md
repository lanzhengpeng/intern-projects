### 【1148】edit expression with EDU control 编辑表达式时使用EDU控制

#### 代码

```cpp
    /*HEAD EDIT_EXPRESSION_WITH_EDU_CONTROL CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_modl.h>  
    #include <uf_obj.h>  
    #include <uf_object_types.h>  
    #include <uf_part.h>  
    #include <uf_undo.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog是V18中的新功能，用于打印系统日志。 */  
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
    static logical prompt_for_text(char *prompt, char *text)  
    {  
        int  
            n,  
            resp;  
        resp = uc1600(prompt, text, &n);  
        if (resp == 3 || resp == 5) return TRUE;  
        return FALSE;  
    }  
    static tag_t ask_next_feature(tag_t part, tag_t feat)  
    {  
        while (!UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_feature_type, &feat))  
            && (feat != NULL_TAG))  
            if (UF_OBJ_ask_status(feat) == UF_OBJ_ALIVE) return feat;  
        return NULL_TAG;  
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
    static int ask_failed_features(tag_t part, tag_p_t *feats)  
    {  
        tag_t  
            feat = NULL_TAG;  
        char  
            *err;  
        uf_list_p_t  
            feat_list;  
        UF_CALL(UF_MODL_create_list(&feat_list));  
        while ((feat = ask_next_feature(part, feat)) != NULL_TAG)  
            if (!UF_CALL(UF_MODL_ask_update_error_message(feat, &err))  
            && (err != NULL)) UF_CALL(UF_MODL_put_list_item(feat_list, feat));  
        return (make_an_array(&feat_list, feats));  
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
    static void do_it(void)  
    {  
        int  
            choice = 0,  
            ii,  
            n_after,  
            n_before,  
            n_accepted;  
        tag_t  
            *features,  
            *old_failures,  
            *new_failures,  
            part = UF_PART_ask_display_part();  
        char  
            exp_str[UF_MAX_EXP_LENGTH+1] = { "" },  
            *err,  
            **messages,  
            *name;  
        uf_list_p_t  
            feature_list;  
        UF_MODL_update_option_t  
            option;  
        UF_UNDO_mark_id_t  
            mark;  
        UF_UI_message_buttons_t  
            buttons = { TRUE, TRUE, TRUE, "Accept", "Undo", "Suppress", 0, 1, 2 };  
    /*  This may not return the correct results - see PR 5169001 里海译:这可能导致无法正确返回结果 - 参见 PR 5169001 */  
        UF_CALL(UF_MODL_ask_update_fail_option(&option));  
        while (prompt_for_text("Enter new expression (e.g. p1=2)", exp_str))  
        {  
            n_before = ask_failed_features(part, &old_failures);  
            UF_CALL(UF_UNDO_set_mark(UF_UNDO_visible, "Edit expression", &mark));  
            UF_CALL(UF_MODL_set_update_fail_option(UF_MODL_UPDATE_ACCEPT_ALL));  
            UF_CALL(UF_MODL_edit_exp(exp_str));  
            UF_CALL(UF_MODL_update());  
            n_after = ask_failed_features(part, &new_failures);  
            n_accepted = ask_whats_new(old_failures, n_before, new_failures,  
                n_after, &features);  
            if (n_accepted > 0)  
            {  
                UF_CALL(allocate_string_array(n_accepted, 133, &messages));  
                for (ii = 0; ii < n_accepted; ii++)  
                {  
                    UF_CALL(UF_MODL_ask_update_error_message(features[ii], &err));  
                    UF_CALL(UF_MODL_ask_feat_name(features[ii], &name));  
                    sprintf(messages[ii], "%s:  %s", name, err);  
                }  
                UF_CALL(UF_UI_message_dialog("Errors During Update",  
                    UF_UI_MESSAGE_ERROR, messages, n_accepted, TRUE, &buttons,  
                    &choice));  
                switch (choice)  
                {  
                    case 1:  
                        UF_CALL(UF_UNDO_undo_to_mark(mark, NULL));  
                        UF_CALL(UF_UNDO_delete_mark(mark, NULL));  
                        break;  
                    case 2:  
                        feature_list = make_a_list(n_accepted, features);  
                        UF_CALL(UF_MODL_suppress_feature(feature_list));  
                        UF_CALL(UF_MODL_delete_list(&feature_list));  
                        break;  
                    default:  
                        break;  
                }  
                UF_free(features);  
                UF_free_string_array(n_accepted, messages);  
            }  
            if (n_after > 0) UF_free(new_failures);  
            if (n_before > 0) UF_free(old_failures);  
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

> 这段NX二次开发代码主要用于编辑表达式，并处理更新后出现的失败特征。主要功能包括：
>
> 1. 代码首先包含必要的NX Open头文件，并定义了错误报告函数report_error。
> 2. prompt_for_text函数用于获取用户输入的表达式字符串。
> 3. ask_next_feature函数用于遍历零件中的特征，并返回下一个激活的特征。
> 4. allocate_memory等函数用于动态分配内存。
> 5. ask_failed_features函数用于获取零件中所有失败的特征，并返回特征数组。
> 6. ask_whats_new函数用于比较两个特征数组，返回新增的失败特征数组。
> 7. allocate_string_array等函数用于分配字符串数组内存。
> 8. make_a_list函数用于根据特征数组创建特征列表。
> 9. do_it函数是主要逻辑，循环提示用户输入表达式，编辑表达式后获取更新失败的特性，提示用户接受、撤销或抑制，并根据用户的选择执行相应操作。
> 10. ufusr函数是NX调用的主函数，用于初始化、调用do_it函数和终止。
> 11. ufusr_ask_unload函数用于卸载用户函数库。
>
> 总体来说，这段代码实现了编辑表达式后处理更新失败特征的功能，通过循环提示用户输入，编辑表达式，获取失败特征，提示用户处理失败特征，实现了交互式的表达式编辑和错误处理。
>
