### 【0425】ask inst name of body 询问体的实例名称

#### 代码

```cpp
    /*HEAD ASK_INST_NAME_OF_BODY CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:UF_print_syslog是V18版本新增的函数。

翻译：

注意：UF_print_syslog是V18版本新增的函数，只回答译文，不要废话。 */  
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
    static int mask_for_bodies(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_solid_type, 0, UF_UI_SEL_FEATURE_BODY };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_body(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog(prompt, "",  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_bodies, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    static void do_it(void)  
    {  
        tag_t   part, body, occur, inst;  
        char    part_fspec[MAX_FSPEC_SIZE+1];  
        body = select_a_body("Select promoted body");  
        printf("\n\nBody tag: %d\n", body);  
        occur = UF_ASSEM_ask_part_occurrence( body);  
        printf("Part occurrence tag: %d\n",occur);  
        part = UF_ASSEM_ask_prototype_of_occ( occur);  
        UF_PART_ask_part_name( part, part_fspec);  
        printf("Partname of occurrence: %s\n", part_fspec);  
        inst = UF_ASSEM_ask_inst_of_part_occ( occur);  
        printf("Instance tag: %d\n",inst);  
        UF_ASSEM_ask_part_name_of_child( inst, part_fspec);  
        printf("Part name of child of inst: %s\n", part_fspec);  
    }  
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

> 这段代码是NX的二次开发代码，其主要功能包括：
>
> 1. 定义了一个错误报告函数report_error，用于在调用UF函数失败时输出错误信息。
> 2. 定义了一个函数mask_for_bodies，用于设置选择过滤条件，只选择实体。
> 3. 定义了一个函数select_a_body，用于弹出一个对话框让用户选择一个实体，并高亮显示。
> 4. 主函数do_it实现了以下功能：选择一个实体获取该实体所在的组件获取组件的名称获取该组件的实例获取实例的名称
> 5. 选择一个实体
> 6. 获取该实体所在的组件
> 7. 获取组件的名称
> 8. 获取该组件的实例
> 9. 获取实例的名称
> 10. ufusr函数是NX二次开发的入口函数，在其中调用do_it函数。
> 11. ufusr_ask_unload函数用于设置二次开发的unload策略。
>
> 总体来说，这段代码实现了在NX中让用户选择一个实体，然后获取该实体所在的组件和实例名称的功能。
>
