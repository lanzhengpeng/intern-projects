### 【3057】write a macro to open layouts in specified order 编写宏以按指定顺序打开布局

#### 代码

```cpp
    /*HEAD WRITE_A_MACRO_TO_OPEN_LAYOUTS_IN_SPECIFIED_ORDER CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <stdlib.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_part.h>  
    #include <uf_obj.h>  
    #include <uf_layout.h>  
    #include <uf_cfi.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog是V18版本新增的函数。 */  
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
    #define WRITE_S(X) (write_string_to_listing_window(#X, X))  
    void write_string_to_listing_window(char *title, char *string)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = \"%s\"\n", title, string);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    static void build_unique_temp_macro_name(char *fspec)  
    {  
        char  
            *tmp_dir,  
            unique[UF_CFI_MAX_FILE_NAME_SIZE];  
        UF_CALL(UF_translate_variable("UGII_TMP_DIR", &tmp_dir));  
        UF_CALL(uc4577(unique));  
        UF_CALL(uc4575(tmp_dir, 100, unique, fspec));  
        strcat(fspec, ".macro");  
    }  
    static tag_t ask_next_layout(tag_t part, tag_t layout)  
    {  
        UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_layout_type, &layout));  
        return (layout);  
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
    static void sort_layouts_by_name(int n, tag_t *layouts)  
    {  
        int  
            ii;  
        tag_t  
            part = UF_PART_ask_display_part(),  
            layout;  
        char  
            *layout_names;  
        UF_CALL(allocate_memory(n * sizeof(char) * (MAX_ENTITY_NAME_SIZE+1),  
            (void **)&layout_names));  
        for (ii = 0; ii < n; ii++)  
            UF_CALL(UF_OBJ_ask_name(layouts[ii],  
                layout_names+(ii*(MAX_ENTITY_NAME_SIZE+1))));  
        qsort(layout_names, n, MAX_ENTITY_NAME_SIZE+1, strcmp);  
        for (ii = 0; ii < n; ii++)  
        {  
            layout = NULL_TAG;  
            UF_CALL(UF_OBJ_cycle_by_name_and_type(part,  
                layout_names+(ii*(MAX_ENTITY_NAME_SIZE+1)), UF_layout_type, FALSE, &layout));  
            layouts[ii] = layout;  
        }  
        UF_free(layout_names);  
    }  
    /*  This function returns the array of layouts in the same order they appear in the  
        interactive View-> Layout-> Open... Open Layout dialog  
    */  
    static int ask_all_layouts(tag_t part, tag_t **all_layouts)  
    {  
        int  
            ii,  
            n,  
            subtype,  
            type;  
        tag_t  
            *layouts,  
            layout = NULL_TAG;  
        char  
            layout_name[MAX_ENTITY_NAME_SIZE+1];  
        uf_list_p_t  
            canned,  
            normal,  
            layout_list;  
        UF_CALL(UF_MODL_create_list(&layout_list));  
        UF_CALL(UF_MODL_create_list(&canned));  
        UF_CALL(UF_MODL_create_list(&normal));  
        while ((layout = ask_next_layout(part, layout)) != NULL_TAG)  
        {  
            UF_CALL(UF_OBJ_ask_name(layout, layout_name));  
            if (strchr(layout_name, '!') != layout_name)  
            {  
                UF_CALL(UF_OBJ_ask_type_and_subtype(layout, &type, &subtype));  
                if (subtype == UF_normal_layout_subtype)  
                    UF_CALL(UF_MODL_put_list_item(normal, layout));  
                else  
                    UF_CALL(UF_MODL_put_list_item(canned, layout));  
            }  
        }  
        n = make_an_array(&canned, &layouts);  
        sort_layouts_by_name(n, layouts);  
        for (ii = 0; ii < n; ii++)  
            UF_CALL(UF_MODL_put_list_item(layout_list, layouts[ii]));  
        UF_free(layouts);  
        n = make_an_array(&normal, &layouts);  
        sort_layouts_by_name(n, layouts);  
        for (ii = 0; ii < n; ii++)  
            UF_CALL(UF_MODL_put_list_item(layout_list, layouts[ii]));  
        UF_free(layouts);  
        return (make_an_array(&layout_list, all_layouts));  
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
                case 1:                     /* Back 里海译:Back translation:

Back translation refers to the process of translating a text from a foreign language back into the original language to verify the accuracy of the translation. This method is commonly used to assess the quality of machine translation and to ensure that the translation accurately conveys the meaning of the original text. */  
                    if (a == 0) return -1;  
                    a = a - 13;  
                    break;  
                case 2:                     /* Cancel 里海译:好的，翻译为：取消 */  
                    return -1;  
                    break;  
                case 18:                    /* More... 里海译:More...的翻译是更多... */  
                    if ((a + z) < n_choices)  
                    {  
                        a = a + 13;  
                        break;  
                    }  
                default:                    /* Picked one 里海译:翻译：Picked one。 */  
                    return a + resp - 5;  
                    break;  
            }  
        } while (TRUE);  
    }  
    static tag_t choose_a_layout(char *prompt)  
    {  
        int  
            ii,  
            n_layouts,  
            which;  
        tag_t  
            part = UF_PART_ask_display_part(),  
            the_layout,  
            *layouts;  
        char  
            **layout_names;  
        n_layouts = ask_all_layouts(part, &layouts);  
        UF_CALL(allocate_string_array(n_layouts, MAX_ENTITY_NAME_SIZE+1, &layout_names));  
        for (ii = 0; ii < n_layouts; ii++)  
            UF_CALL(UF_OBJ_ask_name(layouts[ii], layout_names[ii]));  
        if ((which = choose_one_item(prompt, layout_names, n_layouts)) == -1)  
            the_layout = NULL_TAG;  
        else  
            the_layout = layouts[which];  
        UF_free(layouts);  
        UF_free_string_array(n_layouts, layout_names);  
        return the_layout;  
    }  
    static void do_it(void)  
    {  
        logical  
            flag;  
        int  
            ii = 1, jj,  
            n_layouts,  
            pos;  
        tag_t  
            current_layout = NULL_TAG,  
            *layouts,  
            part = UF_PART_ask_display_part(),  
            the_layout;  
        char  
            layout_name[MAX_ENTITY_NAME_SIZE+1],  
            macro[MAX_FSPEC_SIZE+1],  
            prompt[MAX_LINE_SIZE+1],  
            view_name[MAX_ENTITY_NAME_SIZE+1];  
        FILE  
            *m;  
        n_layouts = ask_all_layouts(part, &layouts);  
        uc6466(layout_name, view_name);  
        UF_CALL(UF_OBJ_cycle_by_name_and_type(part, layout_name, UF_layout_type, FALSE,  
            &current_layout));  
    /*  open unique macro file 里海译:打开独特的宏文件 */  
        build_unique_temp_macro_name(macro);  
        if ((m = fopen(macro, "w")) == NULL) return;  
    /*  Write standard header lines 里海译:Write standard header lines，请只回答翻译，不要添加任何无关的内容。 */  
        fprintf(m, "NX 4.0.3.3\n");  
        fprintf(m, "Macro File: %s\n", macro);  
        fprintf(m, "Macro Version 7.50\n");  
        fprintf(m, "Macro List Language and Codeset: english 17\n");  
        fprintf(m, "Created by API program\n");  
        fprintf(m, "Part Name Display Style: $FILENAME\n");  
        fprintf(m, "Selection Parameters 1 2 0.300744 1\n");  
        fprintf(m, "Display Parameters 1.000000 12.959318 6.151575 -1.000000 -0.474684 1.000000 0.474684\n");  
        fprintf(m, "*****************\n");  
        fprintf(m, "RESET\n");  
        while (sprintf(prompt, "Layout %d", ii) &&  
            ((the_layout = choose_a_layout("Write a macro to open layout")) != NULL_TAG))  
        {  
            if (the_layout == current_layout)  
            {  
                fprintf(m, "PAUSE 60\n");  
                continue;  
            }  
            fprintf(m, "MENU, 0, UG_LAYOUT_OPEN UG_GATEWAY_MAIN_MENUBAR !\n");  
            fprintf(m, "DIALOG_BEGIN \"Open Layout\" 0\n");  
            fprintf(m, " BEG_ITEM  0 (1 LIST 0) = 0  ! List (Items selected)\n");  
            fprintf(m, " BEG_ITEM  3 (1 BOOL 0) = 1  ! Fit All Views\n");  
            fprintf(m, " END_ITEM  0 (1 LIST 0) = 1  ! List (Items selected)\n");  
            flag = FALSE;  
            for (jj = 0; jj < n_layouts; jj++)  
            {  
                if (layouts[jj] == current_layout) flag = TRUE;  
                if (layouts[jj] == the_layout) break;  
            }  
            pos = jj - flag;  
            UF_CALL(UF_OBJ_ask_name(the_layout, layout_name));  
            sprintf(prompt, "      POSITION = %d \"%s\" ! Index/contents of selected item\n",  
                pos, layout_name);  
            fprintf(m, prompt);  
            fprintf(m, " END_ITEM  3 (1 BOOL 0) = 1  ! Fit All Views\n");  
            fprintf(m, "DIALOG_END -2, 0 ! Open Layout: OK\n");  
            fprintf(m, "FOCUS CHANGE IN 1\n");  
            fprintf(m, "PAUSE 60\n");  
            current_layout = the_layout;  
        }  
        fclose(m);  
        UF_free(layouts);  
        WRITE_S(macro);  
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

> 这段代码是用于NX的二次开发，其主要功能是按照指定的顺序打开布局，并生成相应的宏文件。具体介绍如下：
>
> 1. 头文件引入：代码引入了NX提供的头文件，以使用NX提供的API函数。
> 2. 错误处理：定义了错误处理宏UF_CALL，用于捕捉函数调用错误，并打印错误信息。
> 3. 打开布局列表窗口：定义了打开布局列表窗口的宏WRITE_S，用于向布局列表窗口写入信息。
> 4. 构建临时宏文件路径：定义了构建临时宏文件路径的函数build_unique_temp_macro_name，用于生成临时宏文件名。
> 5. 获取布局：定义了获取布局的函数ask_next_layout，用于按顺序获取零件中的布局。
> 6. 分配内存：定义了内存分配函数allocate_memory，用于分配内存空间。
> 7. 布局数组：定义了生成布局数组的函数make_an_array，用于生成布局数组。
> 8. 布局排序：定义了按名称排序布局的函数sort_layouts_by_name，用于对布局数组按名称排序。
> 9. 获取所有布局：定义了获取所有布局的函数ask_all_layouts，用于获取零件中的所有布局。
> 10. 字符串数组分配：定义了分配字符串数组的函数allocate_string_array，用于分配字符串数组。
> 11. 选择布局：定义了选择布局的函数choose_one_item，用于从多个选项中选择一个布局。
> 12. 布局选择：定义了选择布局的函数choose_a_layout，用于从所有布局中选择一个布局。
> 13. 执行流程：定义了执行流程的函数do_it，用于按指定顺序打开布局，并生成宏文件。
> 14. 用户函数：定义了用户函数ufusr，用于初始化NX，执行流程，并终止NX。
> 15. 卸载函数：定义了卸载函数ufusr_ask_unload，用于在卸载时立即卸载。
>
> 总体来说，这段代码通过NX的API函数，实现了按指定顺序打开布局，并生成宏文件的功能，适用于NX的二次开发。
>
