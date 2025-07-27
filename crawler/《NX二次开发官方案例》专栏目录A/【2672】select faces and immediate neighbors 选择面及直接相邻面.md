### 【2672】select faces and immediate neighbors 选择面及直接相邻面

#### 代码

```cpp
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_modl.h>  
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
    static int collect_neighbors(int num_sel, tag_t *sel, int num_desel,  
        tag_t *desel, void *user_data, UF_UI_selection_p_t select)  
    {  
        int  
            ii, n;  
        tag_t  
            *to_add;  
        uf_list_p_t  
            adjac;  
        char  
            msg[132];  
        for (ii = 0; ii < num_sel; ii++)  
        {  
            UF_CALL(UF_MODL_ask_adjac_faces(sel[ii], &adjac));  
            n = make_an_array(&adjac, &to_add);  
            if (n > 0)  
            {  
                UF_CALL(UF_UI_add_to_sel_list(select, n, to_add, TRUE));  
                UF_free(to_add);  
            }  
        }  
    //  The status does always show the new correct count unless you do this:  
        UF_CALL(UF_UI_ask_sel_list_count(select, &n));  
        sprintf(msg, "Face selected - %d total", n);  
        UF_CALL(UF_UI_set_status(msg));  
        return UF_UI_CB_CONTINUE_DIALOG;  
    }  
    /* qq3123197280 */  
    static int mask_for_faces(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_solid_type, 0, UF_UI_SEL_FEATURE_ANY_FACE };  
        UF_CALL(UF_UI_set_sel_procs(select, NULL, collect_neighbors, NULL));  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static int select_faces_and_neighbors(char *prompt, tag_t **faces)  
    {  
        int  
            cnt,  
            ii,  
            resp;  
        UF_CALL(UF_UI_select_with_class_dialog("Select faces", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_faces, NULL, &resp,  
            &cnt, faces));  
        for (ii = 0; ii < cnt; ii++)  
            UF_CALL(UF_DISP_set_highlight((*faces)[ii], FALSE));  
        return cnt;  
    }  
    static void do_it(void)  
    {  
        int  
            n;  
        tag_t  
            *faces;  
        while ((n = select_faces_and_neighbors("Select faces", &faces)) > 0)  
            UF_free(faces);  
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

> 这段代码是NX的二次开发代码，主要功能是选择面及其邻居面。
>
> 1. 功能函数和错误处理：包含了UF、UF_UI、UF_MODL等NX开发库的头文件。定义了ECHO、UF_CALL等宏，用于打印错误信息。实现了make_an_array、collect_neighbors、mask_for_faces、select_faces_and_neighbors等函数，分别用于处理数组、收集邻居、设置选择掩码和选择面及其邻居。
> 2. 包含了UF、UF_UI、UF_MODL等NX开发库的头文件。
> 3. 定义了ECHO、UF_CALL等宏，用于打印错误信息。
> 4. 实现了make_an_array、collect_neighbors、mask_for_faces、select_faces_and_neighbors等函数，分别用于处理数组、收集邻居、设置选择掩码和选择面及其邻居。
> 5. 主要功能实现：使用UF_UI_select_with_class_dialog函数，弹出一个选择对话框，用户可以在其中选择面。mask_for_faces函数设置选择掩码，只选择实体类型中的面。collect_neighbors函数收集选择面的邻居面，并添加到选择列表中。select_faces_and_neighbors函数实现选择面及其邻居面的主要逻辑。
> 6. 使用UF_UI_select_with_class_dialog函数，弹出一个选择对话框，用户可以在其中选择面。
> 7. mask_for_faces函数设置选择掩码，只选择实体类型中的面。
> 8. collect_neighbors函数收集选择面的邻居面，并添加到选择列表中。
> 9. select_faces_and_neighbors函数实现选择面及其邻居面的主要逻辑。
> 10. 程序流程：ufusr函数是程序的入口，初始化NX环境后调用do_it函数。do_it函数循环调用select_faces_and_neighbors函数选择面及其邻居面，直到用户取消选择。ufusr_ask_unload函数用于卸载用户程序。
> 11. ufusr函数是程序的入口，初始化NX环境后调用do_it函数。
> 12. do_it函数循环调用select_faces_and_neighbors函数选择面及其邻居面，直到用户取消选择。
> 13. ufusr_ask_unload函数用于卸载用户程序。
> 14. 注意事项：使用了UF_call宏来包装NX函数调用，以统一处理错误。在适当的位置释放了内存。使用了UF_free释放NX分配的内存。使用了va_list、vsprintf等标准C库函数。
> 15. 使用了UF_call宏来包装NX函数调用，以统一处理错误。
> 16. 在适当的位置释放了内存。
> 17. 使用了UF_free释放NX分配的内存。
> 18. 使用了va_list、vsprintf等标准C库函数。
>
> 综上所述，这段代码实现了在NX中通过对话框选择面及其邻居面的功能，并且具有较好的错误处理和内存管理。
>
