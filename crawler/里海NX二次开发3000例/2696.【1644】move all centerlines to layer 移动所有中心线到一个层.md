### 【1644】move all centerlines to layer 移动所有中心线到一个层

#### 代码

```cpp
    /*HEAD MOVE_ALL_CENTERLINES_TO_LAYER CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_assem.h>  
    #include <uf_layer.h>  
    #include <uf_obj.h>  
    #include <uf_modl.h>  
    #define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))  
    static int report_error( char *file, int line, char *call, int irc)  
    {  
        if (irc)  
        {  
            char err[133];  
            UF_get_fail_message(irc, err);  
            if (!UF_UI_open_listing_window())  
            {  
                UF_UI_write_listing_window(err);  
                UF_UI_write_listing_window("\n");  
                sprintf(err, "error %d at line %d in %s\n", irc, line, file);  
                UF_UI_write_listing_window(err);  
                UF_UI_write_listing_window(call);  
                UF_UI_write_listing_window(";\n\n");  
            }  
            else  
            {  
                fprintf(stderr, "%s\n", err);  
                fprintf(stderr, "error %d at line %d in %s\n", irc, line, file);  
                fprintf(stderr, "%s;\n\n", call);  
    #ifdef _USRDLL  
                uc1601(err, TRUE);  
                sprintf(err, "error %d at line %d in %s\n", irc, line, file);  
                uc1601(err, TRUE);  
    #endif  
            }  
        }  
        return(irc);  
    }  
    static tag_t ask_next_centerline(tag_t part, tag_t cntrline)  
    {  
        int  
            subtype,  
            type;  
        while (!UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_drafting_entity_type, &cntrline))  
            && (cntrline != NULL_TAG))  
        {  
            UF_CALL(UF_OBJ_ask_type_and_subtype(cntrline, &type, &subtype));  
            if( subtype == UF_draft_cntrline_subtype ||  
                subtype == UF_draft_linear_cntrln_subtype ||  
                subtype == UF_draft_full_cir_cntrln_subtype ||  
                subtype == UF_draft_part_cir_cntrln_subtype ||  
                subtype == UF_draft_full_blt_circle_subtype ||  
                subtype == UF_draft_part_blt_circle_subtype ||  
                subtype == UF_draft_cyl_cntrln_subtype ||  
                subtype == UF_draft_sym_cntrln_subtype ||  
                subtype == UF_draft_block_cntrln_subtype )  
            {  
                return cntrline;  
            }  
        }  
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
    static int ask_all_centerlines(tag_t part, tag_t **centerlines)  
    {  
        tag_t  
            line = NULL_TAG;  
        uf_list_p_t  
            cntr_list;  
        UF_CALL(UF_MODL_create_list(&cntr_list));  
        while ((line = ask_next_centerline(part, line)) != NULL_TAG)  
            UF_CALL(UF_MODL_put_list_item(cntr_list, line));  
        return (make_an_array(&cntr_list, centerlines));  
    }  
    static logical prompt_for_a_layer(char *prompt, char *item, int *number)  
    {  
        int  
            irc,  
            resp;  
        char  
            laymsg[100],  
            menu[1][16];  
        int  
            da[1];  
        strcpy(&menu[0][0], item);  
        da[0] = *number;  
        resp = uc1607(prompt, menu, 1, da, &irc);  
        if (resp == 3 || resp == 4)  
        {  
            *number = da[0];  
            if ((*number > UF_LAYER_MAX_LAYER) || (*number <= 0))  
            {  
                sprintf(laymsg, "Layers range from 1 to %d", UF_LAYER_MAX_LAYER);  
                uc1601(laymsg, TRUE);  
                return prompt_for_a_layer(prompt, item, number);  
            }  
            return TRUE;  
        }  
        else return FALSE;  
    }  
    static void do_it(void)  
    {  
        int  
            layer = 1,  
            n_objs,  
            ii;  
        tag_t  
            *objs,  
            part = UF_ASSEM_ask_work_part();  
        char  
            messg[133];  
        if( part == NULL_TAG )  
        {  
            sprintf(messg, "No work part!");  
            uc1601(messg,1);  
            return;  
        }  
        if(prompt_for_a_layer("Insert Target Layer", "Layer", &layer))  
        {  
            n_objs = ask_all_centerlines(part, &objs);  
            if (n_objs > 0)  
            {  
                for (ii = 0; ii < n_objs; ii++)  
                    UF_CALL(UF_OBJ_set_layer(objs[ii], layer));  
                UF_free(objs);  
                sprintf(messg, "Done.");  
                uc1601(messg,1);  
            }  
            else  
            {  
                sprintf(messg, "No centerlines found");  
                uc1601(messg,1);  
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

> 这段代码是一个NX Open C++ API的二次开发示例，主要实现了以下功能：
>
> 1. 错误报告机制：通过自定义函数report_error，实现了错误报告功能，方便调试。
> 2. 遍历获取中心线：通过自定义函数ask_next_centerline，实现了遍历工作部件中的所有中心线并返回下一个中心线的功能。
> 3. 内存分配：通过自定义函数allocate_memory，实现了内存分配功能，避免了内存泄露。
> 4. 创建对象数组：通过自定义函数make_an_array，实现了从对象列表创建对象数组的功能。
> 5. 查询所有中心线：通过自定义函数ask_all_centerlines，实现了查询工作部件中所有中心线并返回它们的数组的功能。
> 6. 提示输入层号：通过自定义函数prompt_for_a_layer，实现了提示用户输入目标层号的功能。
> 7. 移动中心线到指定层：在函数do_it中，实现了遍历查询到的中心线，并将它们移动到用户指定的层号。
> 8. 主函数：在ufusr函数中，完成了初始化、执行主体功能、以及终止的流程。
> 9. 卸载提示：在ufusr_ask_unload函数中，实现了卸载时的提示功能。
>
> 总的来说，这段代码实现了在NX中查询并移动所有中心线到指定层的功能，并提供了完善的错误处理机制。
>
