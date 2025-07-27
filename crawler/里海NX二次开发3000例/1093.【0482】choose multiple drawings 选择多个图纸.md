### 【0482】choose multiple drawings 选择多个图纸

#### 代码

```cpp
    #include <stdio.h>  
    #include <string.h>  
    #include <stdarg.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_modl.h>  
    #include <uf_obj.h>  
    #include <uf_draw.h>  
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
    static logical choose_items(char *prompt, char **choices, int n_choices,  
        int *chosen)  
    {  
        logical  
            more;  
        int  
            a = 0,  
            ii,  
            resp,  
            selected[14],  
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
            {  
                more = true;  
                strcpy(&options[13][0], "More...");  
            }  
            else  
                more = false;  
            switch (resp = uc1605(prompt, 0, options, z, selected))  
            {  
                case 1:                     /* Back 译:Back */  
                    if (a == 0) return false;  
                    a = a - 13;  
                    break;  
                case 2:                     /* Cancel 译:Cancel */  
                    return false;  
                    break;  
                default:                    /* OK 译:好的，以下是翻译：

这篇文章主要讨论了气候变化对农业生产的影响。随着全球气候变暖，许多地区的温度和降雨模式发生了变化，这直接影响到了农业生产。例如，一些地区可能会经历干旱，导致农作物减产；而另一些地区可能会遭受洪灾，同样导致农作物受损。此外，气候变化也导致了一些病虫害的增加，对农作物的生长产生了负面影响。因此，农民需要采取适应性措施，比如改变种植结构，以应对气候变化对农业的影响。 */  
                    if (more && (selected[13] == 1))  
                        z = z - 1;  
                    else  
                        more = false;  
                    for (ii = 0; ii < z; ii++)  
                        chosen[a+ii] = selected[ii];  
                    if (!more)  
                        return true;  
                    a = a + 13;  
                    break;  
            }  
        } while (TRUE);  
    }  
    static int allocate_memory(unsigned int nbytes, void **where)  
    {  
        int  
            resp;  
        *where = UF_allocate_memory(nbytes, &resp);  
        return resp;  
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
    static int choose_drawings(char *prompt, tag_t **drawings)  
    {  
        int  
            ii,  
            n_dwgs,  
            *flags;  
        tag_t  
            *dwgs;  
        uf_list_p_t  
            dwg_list;  
        char  
            **dwg_names;  
        UF_CALL(UF_DRAW_ask_drawings(&n_dwgs, &dwgs));  
        if (n_dwgs == 0)  
        {  
            ECHO("Part has no drawing sheets\n");  
            return 0;  
        }  
        UF_CALL(allocate_memory(n_dwgs * sizeof(int), (void **)&flags));  
        UF_CALL(allocate_string_array(n_dwgs, MAX_ENTITY_NAME_SIZE+1, &dwg_names));  
        for (ii = 0; ii < n_dwgs; ii++)  
        {  
            flags[ii] = 0;  
            UF_CALL(UF_OBJ_ask_name(dwgs[ii], dwg_names[ii]));  
        }  
        if (choose_items(prompt, dwg_names, n_dwgs, flags))  
        {  
            UF_CALL(UF_MODL_create_list(&dwg_list));  
            for (ii = 0; ii < n_dwgs; ii++)  
            {  
                if (flags[ii] == 1)  
                    UF_CALL(UF_MODL_put_list_item(dwg_list, dwgs[ii]));  
            }  
            UF_free(dwgs);  
            return make_an_array(&dwg_list, drawings);  
        }  
        UF_free(dwgs);  
        return 0;  
    }  
    static void do_it(void)  
    {  
        int  
            ii,  
            n_dwgs;  
        tag_t  
            *dwgs;  
        char  
            dwg_name[MAX_ENTITY_NAME_SIZE+1];  
        while ((n_dwgs = choose_drawings("Select Sheets", &dwgs)) > 0)  
        {  
            ECHO("Selected drawing sheets:\n");  
            for (ii = 0; ii < n_dwgs; ii++)  
            {  
                UF_CALL(UF_OBJ_ask_name(dwgs[ii], dwg_name));  
                ECHO("%s\n", dwg_name);  
            }  
            UF_free(dwgs);  
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

> 这段代码是一个NX的二次开发示例，主要实现了选择图纸的功能。代码的主要功能包括：
>
> 1. 初始化和结束：使用UF_initialize和UF_terminate进行NX环境的初始化和结束。
> 2. 错误处理：定义了ECHO宏来输出错误信息，以及report_error函数来处理UF函数返回的错误码。
> 3. 内存分配：提供allocate_memory和allocate_string_array函数用于内存分配。
> 4. 选择功能：提供choose_items函数，实现弹出窗口让用户选择项目，返回用户的选择结果。
> 5. 图纸列表：使用choose_drawings函数获取当前部件的图纸列表，并让用户选择。
> 6. 主函数：在do_it函数中循环调用choose_drawings，输出用户选择的图纸名称，直到用户不选择为止。
> 7. UF函数调用封装：使用UF_CALL宏封装UF函数调用，便于错误处理。
> 8. 程序卸载：提供了卸载函数，使程序可以立即卸载。
>
> 总体而言，这段代码实现了基本的图纸选择功能，可作为NX二次开发的一个简单示例。
>
