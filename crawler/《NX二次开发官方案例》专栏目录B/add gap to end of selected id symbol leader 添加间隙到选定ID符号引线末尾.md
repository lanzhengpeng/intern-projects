### add gap to end of selected id symbol leader 添加间隙到选定ID符号引线末尾

#### 代码

```cpp
    /*HEAD ADD_GAP_TO_END_OF_SELECTED_ID_SYMBOL_LEADER CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_drf.h>  
    #include <uf_obj.h>  
    #include <uf_view.h>  
    #include <uf_undo.h>  
    #define ECHO(X) { UF_UI_open_listing_window(); \  
        UF_UI_write_listing_window(X); \  
        UF_print_syslog(X, FALSE); }  
    #define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))  
    static int report_error( char *file, int line, char *call, int irc)  
    {  
        if (irc)  
        {  
            char err[133],  
                 msg[UF_UI_MAX_STRING_LEN];  
            sprintf(msg, "*** ERROR code %d at line %d in %s:\n+++ ",  
                irc, line, file);  
            UF_get_fail_message(irc, err);  
            ECHO(msg);  
            ECHO(err);  
            ECHO("\n");  
            ECHO(call);  
            ECHO(";\n");  
        }  
        return(irc);  
    }  
    /* 里海 */  
    static int mask_for_id_symbols(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_drafting_entity_type, UF_draft_id_symbol_subtype, 0 };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_an_id_symbol(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select an ID symbol", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_id_symbols, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    static void build_util_filespec(char *short_name, int ftype, char *fspec)  
    {  
        char  
            *util_dir;  
        UF_CALL(UF_translate_variable("UGII_UTIL", &util_dir));  
        UF_CALL(uc4575(util_dir, ftype, short_name, fspec));  
    }  
    static void set_undo_mark(char *label)  
    {  
        UF_UNDO_mark_id_t  
            mark_id;  
        UF_CALL(UF_UNDO_set_mark(UF_UNDO_visible, label, &mark_id));  
    }  
    static void do_it(void)  
    {  
        int  
            n;  
        tag_t  
            id_symbol;  
        double  
            pt_coords[3] = { 0,0,0 };  
        char  
            sbf_name[UF_CFI_MAX_PATH_NAME_SIZE];  
        UF_DRF_object_assoc_data_t  
            *assoc_data;  
        UF_DRF_symbol_create_data_t  
            symbol_data = { "GAP25", 0, 0.25, 0.06, NULL_TAG };  
        build_util_filespec("ug_default", 95, sbf_name);  
        UF_CALL(UF_DRF_set_specified_sbf_file(sbf_name));  
        while ((id_symbol = select_an_id_symbol("Add gap")) != NULL_TAG)  
        {  
            UF_CALL(UF_DRF_ask_associativity_data(id_symbol, &n, &assoc_data));  
            if (n > 0)  
            {  
                set_undo_mark("Add leader gap");  
                UF_CALL(UF_VIEW_map_model_to_drawing(assoc_data[0].object_view,  
                    assoc_data[0].assoc_point, pt_coords));  
                UF_CALL(UF_CURVE_create_point(pt_coords, &symbol_data.anchor_tag));  
                UF_free(assoc_data);  
                UF_CALL(UF_DRF_add_symbol_to_object(&symbol_data, id_symbol));  
                UF_CALL(UF_OBJ_delete_object(symbol_data.anchor_tag));  
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

> 根据代码内容，这是一段NX的二次开发代码，主要功能是在所选ID符号的末端添加一个默认大小的间隙符号。
>
> 主要步骤如下：
>
> 1. 包含必要的NX API头文件。
> 2. 定义错误报告函数report_error，用于打印函数执行错误信息。
> 3. 定义mask_for_id_symbols函数，用于设置选择过滤器，仅选择ID符号。
> 4. 定义select_an_id_symbol函数，用于选择一个ID符号，并返回其tag。
> 5. 定义build_util_filespec函数，用于构建符号块文件路径。
> 6. 定义set_undo_mark函数，用于设置撤销标记。
> 7. 定义do_it函数，用于实现添加间隙符号的逻辑：循环选择ID符号。获取ID符号的关联数据。在关联数据点处设置撤销标记。将关联点映射到图纸，创建间隙符号的锚点。创建间隙符号，并添加到ID符号。删除间隙符号的锚点。
> 8. 循环选择ID符号。
> 9. 获取ID符号的关联数据。
> 10. 在关联数据点处设置撤销标记。
> 11. 将关联点映射到图纸，创建间隙符号的锚点。
> 12. 创建间隙符号，并添加到ID符号。
> 13. 删除间隙符号的锚点。
> 14. 定义ufusr函数，用于初始化NX API，执行do_it函数，然后终止NX API。
> 15. 定义ufusr_ask_unload函数，用于卸载用户自定义函数。
>
> 综上所述，这段代码实现了在所选ID符号的末端添加一个默认大小的间隙符号的功能。
>
