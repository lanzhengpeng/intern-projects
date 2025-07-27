### 【0394】assign reference attributes to selected solids 将参考属性分配给选定的实体

#### 代码

```cpp
    /*HEAD ASSIGN_REFERENCE_ATTRIBUTES_TO_SELECTED_SOLIDS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_attr.h>  
    #include <uf_disp.h>  
    #include <uf_obj.h>  
    #include <uf_object_types.h>  
    #include <uf_assem.h>  
    #include <uf_part.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:根据文档中的提示，UF_print_syslog是V18版本中新增的函数。因此，文档翻译为：请注意，UF_print_syslog是V18版本中的新增函数。 */  
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
    /* 里海 */  
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
        UF_CALL(UF_UI_select_with_single_dialog("Select a body", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_bodies, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
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
    static void report_load_status(UF_PART_load_status_p_t status)  
    {  
        char  
            msg[133];  
        int  
            ii;  
        if (status->n_parts > 0)  
        {  
            UF_UI_open_listing_window();  
            UF_UI_write_listing_window("Load notes:\n");  
            for (ii = 0; ii < status->n_parts; ii++)  
            {  
                UF_UI_write_listing_window("  ");  
                UF_UI_write_listing_window(status->file_names[ii]);  
                UF_UI_write_listing_window(" - ");  
                UF_get_fail_message(status->statuses[ii], msg);  
                UF_UI_write_listing_window(msg);  
                UF_UI_write_listing_window("\n");  
            }  
            UF_UI_write_listing_window("\n");  
            UF_free(status->statuses);  
            UF_free_string_array(status->n_parts, status->file_names);  
        }  
    }  
    static void ensure_part_fully_loaded(tag_t part)  
    {  
        char  
            partname[MAX_FSPEC_SIZE+1];  
        UF_PART_load_status_t  
            error_status;  
        if (!UF_CALL(UF_PART_ask_part_name(part, partname))  
            && ((UF_PART_is_loaded(partname)) != 1))  
        {  
            UF_CALL(UF_PART_open_quiet(partname, &part, &error_status));  
            if (error_status.n_parts > 0) report_load_status(&error_status);  
        }  
    }  
    static void do_it(void)  
    {  
        tag_t  
            object,  
            owning_part,  
            part;  
        char  
            str[UF_ATTR_MAX_STRING_LEN+1] = { "" },  
            title[UF_ATTR_MAX_TITLE_LEN+1] = { "" };  
        UF_ATTR_value_t  
            value;  
        value.type = UF_ATTR_reference;  
    /*  put the space for the characters into the structure. 译:将字符的空格放入结构中。 */  
        value.value.reference = str;  
        while ((object = select_a_body("Assign attributes")) != NULL_TAG)  
            while ((prompt_for_text("Attribute title", title)) &&  
                   (prompt_for_text("Attribute value", value.value.reference)))  
            {  
                UF_CALL(UF_ATTR_assign(object, title, value));  
                if (UF_ASSEM_is_occurrence(object) &&  
                    !UF_OBJ_is_object_a_promotion(object) &&  
                    prompt_for_text("Prototype Attribute value",  
                        value.value.reference))  
                {  
                    object = UF_ASSEM_ask_prototype_of_occ(object);  
                    UF_CALL(UF_OBJ_ask_owning_part(object, &owning_part));  
                    ensure_part_fully_loaded(owning_part);  
                    UF_CALL(UF_ASSEM_set_work_part_quietly(owning_part, &part));  
                    UF_CALL(UF_ATTR_assign(object, title, value));  
                    UF_CALL(UF_ASSEM_set_work_part_quietly(part, &owning_part));  
                }  
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

> 这段代码是NX的二次开发代码，主要实现了以下功能：
>
> 1. 错误处理：使用宏UF_CALL封装了NX API调用，实现了错误处理机制，能够在调用失败时打印错误信息。
> 2. 选择体：使用select_a_body函数实现选择一个体的功能，并高亮显示选中的体。
> 3. 输入文本：使用prompt_for_text函数实现输入文本的功能，用于输入属性标题和值。
> 4. 分配属性：使用UF_ATTR_assign函数给选中的体分配属性，支持为模型和原型分配属性。
> 5. 确保部件加载：使用ensure_part_fully_loaded函数确保部件完全加载，以便可以分配属性。
> 6. 主函数：在do_it函数中，实现了选择体、输入属性、分配属性的主流程，支持为模型和原型分配属性。
> 7. 启动和退出：在ufusr函数中，实现了启动NX和调用do_it函数，并在退出时释放NX资源。
> 8. 卸载请求：在ufusr_ask_unload函数中，实现了立即卸载二次开发程序的请求。
>
> 总体来说，这段代码通过交互式选择体和输入属性，实现了为NX模型分配属性的功能，并提供了良好的错误处理和用户交互机制。
>
