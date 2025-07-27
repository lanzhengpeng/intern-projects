### 【3104】demo using handles 一个简单的交互式NX Open C++程序

#### 代码

```cpp
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_disp.h>  
    #include <uf_curve.h>  
    #include <uf_obj.h>  
    #include <uf_undo.h>  
    #include <uf_modl.h>  
    #include <uf_assem.h>  
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
    #define WRITENZ(X) if (X != 0) ECHO("%s = %d\n", #X, X);  
    static logical pick_one_of_two_choices(char *prompt, char *option_one,  
        char *option_two, int *choice)  
    {  
        int  
            resp;  
        char  
            options[2][38];  
        strncpy(&options[0][0], option_one, 37);  
        strncpy(&options[1][0], option_two, 37);  
        options[0][37] = '\0';  
        options[1][37] = '\0';  
        resp = uc1603(prompt, *choice, options, 2);  
        if ((resp > 4) && (resp < 19))  
        {  
            *choice = resp - 4;  /* returns 1 or 2 里海译:根据文档的内容，可以翻译为：

返回1或2。 */  
            return TRUE;  
        }  
        else return FALSE;  
    }  
    static logical prompt_for_text_file_name(char *prompt, char *fspec)  
    {  
        int  
            resp;  
        char  
            filter[MAX_FSPEC_SIZE+1] = { "*.txt" };  
        if (!UF_CALL(UF_UI_create_filebox(prompt, "Text File Name", filter, "",  
            fspec, &resp)) && (resp != UF_UI_CANCEL))  
        {  
            if (!strstr(fspec, ".txt")) strcat(fspec, ".txt");  
            WRITENZ(uc4573(fspec, 4, fspec));  
            return TRUE;  
        }  
        else return FALSE;  
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
    static int select_things(char *prompt, tag_t **objects)  
    {  
        int  
            cnt,  
            ii,  
            resp;  
        UF_CALL(UF_UI_set_cursor_view(0));  
        UF_CALL(UF_UI_select_with_class_dialog( "Select Objects", prompt,  
            UF_UI_SEL_SCOPE_WORK_PART, mask_add_faces_and_edges, NULL, &resp,  
            &cnt, objects));  
        UF_CALL(UF_UI_set_cursor_view(1));  
        for (ii = 0; ii < cnt; ii++)  
            UF_CALL(UF_DISP_set_highlight((*objects)[ii], 0));  
        return cnt;  
    }  
    static int allocate_memory(unsigned int nbytes, void **where)  
    {  
        int  
            resp;  
        *where = UF_allocate_memory(nbytes, &resp);  
        return resp;  
    }  
    static int ask_data_from_text_file(char *fspec, char ***data)  
    {  
        int  
            ii = 0,  
            n_lines = 0;  
        char  
            a_line[MAX_LINE_SIZE+1];  
        FILE  
            *the_file;  
        if ((the_file = fopen(fspec, "r")) != NULL)  
        {  
            while ((fgets(a_line, MAX_LINE_SIZE, the_file)) != NULL) n_lines++;  
            rewind(the_file);  
            UF_CALL(allocate_memory(n_lines * sizeof(char *), (void **)data));  
            for (ii = 0; ii < n_lines; ii++)  
            {  
                fgets(a_line, MAX_LINE_SIZE, the_file);  
            /*  strip off the carriage return 里海译:去掉回车 */  
                a_line[strlen(a_line) - 1] = '\0';  
                UF_CALL(allocate_memory((unsigned int)(strlen(a_line) + 1),  
                    (void **)&(*data)[ii]));  
                strcpy((*data)[ii], a_line);  
            }  
            fclose(the_file);  
        }  
        return n_lines;  
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
    static int find_tags_from_handles(unsigned int n_handles, char **handles,  
        tag_t **objects)  
    {  
        int  
            ii;  
        tag_t  
            object;  
        uf_list_p_t  
            object_list;  
        UF_CALL(UF_MODL_create_list(&object_list));  
        for (ii = 0; ii < n_handles; ii++)  
        {  
            if ((object = UF_TAG_ask_tag_of_handle(handles[ii])) != NULL_TAG)  
                UF_CALL(UF_MODL_put_list_item(object_list, object));  
        }  
        return make_an_array(&object_list, objects);  
    }  
    static void set_highlight_object_array(int n, tag_t *objects, int flag)  
    {  
        int  
            ii,  
            subtype,  
            type;  
        for (ii = 0; ii < n; ii++)  
        {  
            UF_CALL(UF_OBJ_ask_type_and_subtype(objects[ii], &type, &subtype));  
            if (type == UF_occ_instance_type)  
                UF_CALL(UF_DISP_set_highlight(UF_ASSEM_ask_part_occ_of_inst(  
                    NULL_TAG, objects[ii]), flag));  
            else  
                UF_CALL(UF_DISP_set_highlight(objects[ii], flag));  
        }  
    }  
    static void do_it(void)  
    {  
        int  
            chan,  
            ii,  
            no, nh,  
            which = 1;  
        tag_t  
            *objects;  
        char  
            *handle,  
            **handles,  
            tfn[MAX_FSPEC_SIZE+1] = { "" };  
        while (pick_one_of_two_choices("Demo handle usage",  
            "Write object handles to file",  
            "Find objects from handles in file", &which))  
        {  
            if (which == 1)  
            {  
                if (!prompt_for_text_file_name("Save handles", tfn)) continue;  
                if ((no = select_things("Save handles", &objects)) == 0) continue;  
                chan = uc4504(tfn, 3, 4);  
                if (chan < 0)  
                {  
                    ECHO("Error opening %s\n", tfn);  
                    UF_CALL(chan);  
                    continue;  
                }  
                for (ii = 0; ii < no; ii++)  
                {  
                    handle = UF_TAG_ask_handle_of_tag(objects[ii]);  
                    WRITENZ(uc4524(chan, handle));  
                    UF_free(handle);  
                }  
                UF_CALL(uc4540(chan, 0));  
                ECHO("%d handles written to %s\n", no, tfn);  
                UF_free(objects);  
            }  
            else  
            {  
                if (!prompt_for_text_file_name("Read handles", tfn)) continue;  
                if ((nh = ask_data_from_text_file(tfn, &handles)) == 0) continue;  
                ECHO("%d handles read from %s\n", nh, tfn);  
                if ((no = find_tags_from_handles(nh, handles, &objects)) > 0)  
                {  
                    ECHO("%d tags found using handles\n", no);  
                    set_highlight_object_array(no, objects, TRUE);  
                    uc1601("Found objects highlighted", TRUE);  
                    set_highlight_object_array(no, objects, FALSE);  
                    UF_free(objects);  
                }  
                UF_free_string_array(nh, handles);  
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

> 这段代码是一个NX Open的二次开发示例，主要实现了以下功能：
>
> 1. 提供了错误报告函数report_error，用于在函数调用失败时打印错误信息。
> 2. 定义了ECHO宏，用于在列表窗口和系统日志中输出信息。
> 3. 实现了函数pick_one_of_two_choices，用于在两个选项中选择一个。
> 4. 实现了函数prompt_for_text_file_name，用于提示用户输入文本文件名。
> 5. 定义了函数mask_add_faces_and_edges，用于在对话框中添加面和边的选择掩码。
> 6. 实现了函数select_things，用于提示用户选择对象，并返回选中的对象数组。
> 7. 实现了函数allocate_memory，用于分配内存。
> 8. 实现了函数ask_data_from_text_file，用于从文本文件中读取数据。
> 9. 实现了函数make_an_array，用于将链表转换为数组。
> 10. 实现了函数find_tags_from_handles，用于根据句柄查找对象标签。
> 11. 实现了函数set_highlight_object_array，用于设置对象的高亮显示。
> 12. 实现了函数do_it，用于执行程序的主逻辑，包括将对象句柄写入文件、从文件中读取句柄查找对象等功能。
> 13. 实现了ufusr函数，用于程序的入口点。
> 14. 实现了ufusr_ask_unload函数，用于卸载程序。
>
> 总的来说，这段代码通过NX Open API实现了NX中的对象选择、句柄操作、文件读写等功能，并提供了交互式的用户界面。
>
