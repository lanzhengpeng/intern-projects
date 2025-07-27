### 【1378】highlight common edges 高亮共边线

#### 代码

```cpp
    /*HEAD HIGHLIGHT_COMMON_EDGES CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_disp.h>  
    #include <uf_modl.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，UF_print_syslog是V18版本新增的函数。在回答问题时，应只提供翻译内容，避免添加无关的额外信息。 */  
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
        UF_CALL(UF_UI_select_with_single_dialog("Select a face", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_faces, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, FALSE));  
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
    static int ask_common_edges(tag_t face1, tag_t face2, tag_p_t *edges)  
    {  
        int  
            ii,  
            n = 0;  
        uf_list_p_t  
            edge_list;  
        UF_CALL(UF_MODL_ask_shared_edges(face1, face2, &edge_list));  
        if (edge_list) UF_CALL(UF_MODL_ask_list_count(edge_list, &n));  
        UF_CALL(allocate_memory(n * sizeof(tag_t), (void **)edges));  
        for (ii = 0; ii < n; ii++)  
            UF_CALL(UF_MODL_ask_list_item(edge_list, ii, &(*edges)[ii]));  
        UF_CALL(UF_MODL_delete_list(&edge_list));  
        return n;  
    }  
    static void do_it(void)  
    {  
        int  
            ii,  
            n;  
        tag_t  
            face1,  
            face2,  
            *edges;  
        char  
            messg[30];  
        while (((face1 = select_a_face("Select first face")) != NULL_TAG) &&  
               ((face2 = select_a_face("Select second face")) != NULL_TAG))  
        {  
            n = ask_common_edges(face1, face2, &edges);  
            sprintf(messg, "%d common edge(s)", n);  
            if (n > 0)  
            {  
                for (ii = 0; ii < n; ii++)  
                    UF_DISP_set_highlight(edges[ii], TRUE);  
                uc1601(messg, TRUE);  
                for (ii = 0; ii < n; ii++)  
                    UF_DISP_set_highlight(edges[ii], FALSE);  
                UF_free(edges);  
            }  
            else  
                uc1601(messg, TRUE);  
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

> 这段代码是用于NX软件的二次开发代码，其主要功能是选择两个面，并找出它们之间的共边边线。代码的关键步骤如下：
>
> 1. 定义错误报告函数：用于在函数调用失败时输出错误信息。
> 2. 定义选择面函数：使用UF_UI库提供的对话框，让用户选择一个面。
> 3. 定义内存分配函数：使用UF库的内存分配函数，用于动态分配内存。
> 4. 定义查询共边边线函数：使用UF_MODL库提供的函数，查询两个面之间的共边边线。
> 5. 定义主函数：在主函数中，循环调用选择面函数，让用户选择两个面，然后调用查询共边边线函数，找出共边边线，并高亮显示。
> 6. 定义ufusr函数：NX二次开发程序入口函数，初始化UF库，调用主函数，然后结束。
> 7. 定义卸载函数：返回立即卸载标志。
>
> 通过以上步骤，这段代码实现了NX软件中查询两个面之间共边边线并高亮显示的功能。
>
