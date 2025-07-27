### 【1005】delete all drawings but selected 删除所有图纸，只保留选中的图纸

#### 代码

```cpp
    /*HEAD DELETE_ALL_DRAWINGS_BUT_SELECTED CCC UFUN */  
    /*  To create a dialog to use with this program:  
        - Select Application->User Interface Styler  
        - Change the dialog's Prefix Name to "DWGS"  
        - Specify the Constructor Callback as DWGS_"const_cb"  
        - Add a SINGLE List object  
        - Change the button Style to "OK and CANCEL"  
        - Save Dialog As "application/drawing_sel"  
    */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_styler.h>  
    #include <uf_draw.h>  
    #include <uf_object_types.h>  
    #include <uf_obj.h>  
    #include <uf_part.h>  
    #include <uf_modl.h>  
    #define DWGS_LIST_0              ("LIST_0")  
    #define DWGS_CB_COUNT ( 2 + 1 ) /* Add 1 for the terminator 里海译:加1用于终结符 */  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档，UF_print_syslog是V18版本新增的功能。翻译如下：

注意：UF_print_syslog是V18版本新增的功能。 */  
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
    static tag_t ask_next_drawing(tag_t part, tag_t drawing)  
    {  
        UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_drawing_type, &drawing));  
        return (drawing);  
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
    static int ask_all_other_drawings(tag_t part, tag_t **dwgs, tag_t compare_dwg)  
    {  
        tag_t  
            dwg = NULL_TAG;  
        uf_list_p_t  
            draw_list;  
        UF_CALL(UF_MODL_create_list(&draw_list));  
        while ((dwg = ask_next_drawing(part, dwg)) != NULL_TAG)  
        {  
            if(dwg != compare_dwg)  
                UF_CALL(UF_MODL_put_list_item(draw_list, dwg));  
        }  
        return make_an_array(&draw_list, dwgs);  
    }  
    static void delete_all_other_drawings(tag_t compare_dwg, tag_t part)  
    {  
        int  
            ii,  
            n_dwgs;  
        tag_t  
            *dwgs;  
        n_dwgs = ask_all_other_drawings(part, &dwgs, compare_dwg);  
        UF_CALL(UF_DRAW_set_display_state(1));  
        if (n_dwgs > 0)  
        {  
            for (ii = 0; ii < n_dwgs; ii++)  
                UF_DRAW_delete_drawing(dwgs[ii]);  
            UF_free(dwgs);  
        }  
    }  
    static tag_t ask_drawing_tag(char *dwg_name)  
    {  
        tag_t  
            dwg = NULL_TAG,  
            part = UF_PART_ask_display_part();  
        UF_CALL(UF_OBJ_cycle_by_name_and_type(part, dwg_name, UF_drawing_type,  
            FALSE, &dwg));  
        return dwg;  
    }  
    /* qq3123197280 */  
    static int DWGS_const_cb(int dialog_id, void *client_data,  
        UF_STYLER_item_value_type_p_t callback_data)  
    {  
        tag_t  
            drawing = NULL_TAG,  
            part = UF_PART_ask_display_part();  
        char  
            dname[MAX_ENTITY_NAME_SIZE+1];  
        UF_STYLER_item_value_type_t  
            set_value;  
        set_value.item_attr = UF_STYLER_LIST_INSERT;  
        set_value.item_id = DWGS_LIST_0;  
        set_value.subitem_index = UF_STYLER_NO_SUB_INDEX;  
        set_value.indicator = UF_STYLER_STRING_VALUE;  
        set_value.value.string = dname;  
        while ((drawing = ask_next_drawing(part, drawing)) != NULL_TAG)  
        {  
            UF_CALL(UF_OBJ_ask_name(drawing, dname));  
            UF_CALL(UF_STYLER_set_value(dialog_id, &set_value));  
        }  
        return (UF_UI_CB_CONTINUE_DIALOG);  
    }  
    /* qq3123197280 */  
    static int DWGS_ok_cb(int dialog_id, void *client_data,  
        UF_STYLER_item_value_type_p_t callback_data)  
    {  
        UF_STYLER_item_value_type_t  
            ask_value;  
        tag_t  
            sel_dwg = NULL_TAG,  
            part = UF_PART_ask_display_part();  
        ask_value.item_attr = UF_STYLER_VALUE;  
        ask_value.item_id = DWGS_LIST_0;  
        ask_value.indicator = UF_STYLER_STRING_VALUE;  
        UF_CALL(UF_STYLER_ask_value(dialog_id, &ask_value));  
        if (ask_value.count > 0)   
        {  
            printf("Selected drawing %s\n", ask_value.value.string);  
            if((sel_dwg = ask_drawing_tag(ask_value.value.string)) != NULL_TAG)  
            {  
                delete_all_other_drawings(sel_dwg, part);         
            }  
        }  
        if (ask_value.count > 0) UF_STYLER_free_value(&ask_value);  
        return (UF_UI_CB_EXIT_DIALOG);  
    }  
    static void do_it(void)  
    {  
        int  
            resp;  
        UF_STYLER_callback_info_t DWGS_cbs[DWGS_CB_COUNT] = {  
            {UF_STYLER_DIALOG_INDEX, UF_STYLER_CONSTRUCTOR_CB, 0, DWGS_const_cb},  
            {UF_STYLER_DIALOG_INDEX, UF_STYLER_OK_CB, 0, DWGS_ok_cb},  
            {UF_STYLER_NULL_OBJECT, UF_STYLER_NO_CB, 0, 0 } };  
        if (UF_PART_ask_num_parts() > 0)  
            UF_CALL(UF_STYLER_create_dialog("drawing_sel.dlg", DWGS_cbs,  
                DWGS_CB_COUNT, NULL, &resp));  
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

> 这段代码是一个NX的二次开发示例，主要功能是删除除选定图纸外的所有图纸。
>
> 主要步骤包括：
>
> 1. 定义了一些宏，用于获取图纸对象和对话框的ID。
> 2. 定义了一个错误报告函数report_error，用于打印错误信息。
> 3. 定义了一个函数ask_next_drawing，用于遍历获取部件下的所有图纸对象。
> 4. 定义了一个函数allocate_memory，用于分配内存。
> 5. 定义了一个函数make_an_array，用于将列表转换为数组。
> 6. 定义了一个函数ask_all_other_drawings，用于获取除选定图纸外的所有图纸对象。
> 7. 定义了一个函数delete_all_other_drawings，用于删除这些图纸对象。
> 8. 定义了一个函数ask_drawing_tag，用于根据图纸名称获取图纸对象。
> 9. 定义了两个对话框回调函数DWGS_const_cb和DWGS_ok_cb。前者用于在对话框中插入所有图纸名称，后者用于获取用户选定的图纸，并删除其他图纸。
> 10. 定义了一个函数do_it，用于创建并显示对话框。
> 11. 最后，定义了ufusr和ufusr_ask_unload两个函数，作为程序的入口和卸载函数。
>
> 这段代码展示了如何使用NX的二次开发接口，进行图纸对象的遍历、删除，以及对话框的交互。
>
