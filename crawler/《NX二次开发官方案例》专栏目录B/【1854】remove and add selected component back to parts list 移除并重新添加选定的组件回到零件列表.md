### 【1854】remove and add selected component back to parts list 移除并重新添加选定的组件回到零件列表

#### 代码

```cpp
    /*HEAD REMOVE_AND_ADD_SELECTED_COMPONENT_BACK_TO_PARTS_LIST CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_obj.h>  
    #include <uf_part.h>  
    #include <uf_assem.h>  
    #include <uf_plist.h>  
    #include <uf_retiring.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))  
    /* qq3123197280 */  
    static int report_error( char *file, int line, char *call, int irc)  
    {  
        if (irc)  
        {  
            char err[133],  
                 msg[133];  
            sprintf(msg, "*** ERROR code %d at line %d in %s:\n+++ ",  
                irc, line, file);  
            UF_get_fail_message(irc, err);  
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
    static int mask_for_components(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_component_type, 0, 0 };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    /* qq3123197280 */  
    static tag_t select_a_component(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select a component", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_components, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    #define WRITE(X)   UF_UI_open_listing_window(); UF_UI_write_listing_window(X)  
    static void do_it(void)  
    {  
        tag_t  
            plist = NULL_TAG,  
            comp = NULL_TAG,  
            part = UF_PART_ask_display_part();  
        if( NULL_TAG == part ) return;  
        plist = UF_PLIST_ask_tag( );  
        if( NULL_TAG == plist ) return;  
        while((comp=select_a_component("Pick Component:"))!=NULL_TAG)  
        {  
            UF_CALL( UF_PLIST_remove_object( plist, TRUE, comp ));  
            UF_CALL( UF_PLIST_update( plist ) );  
            uc1601( "Component has been removed", TRUE );  
            UF_CALL( UF_PLIST_add_object( plist, TRUE, comp ));  
            UF_CALL( UF_PLIST_update( plist ) );  
            uc1601( "Component has been added", TRUE );  
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

> 这段代码是一个NX Open C++ API编写的NX二次开发应用程序，主要实现了从部件列表中移除和添加一个已选组件的功能。以下是代码的主要功能和流程：
>
> 1. 包含必要的NX Open C++ API头文件。
> 2. 定义了错误报告函数report_error，用于在API调用失败时输出错误信息。
> 3. 定义了组件选择掩码函数mask_for_components，只允许选择组件类型的对象。
> 4. 定义了组件选择函数select_a_component，使用单选对话框让用户选择一个组件，并返回组件的tag。
> 5. 定义了主函数do_it，获取当前显示的部件和部件列表，在循环中让用户选择一个组件，然后从部件列表中移除和添加该组件，并更新部件列表。
> 6. 定义了ufusr函数，是NX二次开发程序的入口函数，初始化API后调用do_it函数，最后终止API。
> 7. 定义了卸载函数ufusr_ask_unload，返回立即卸载标志。
> 8. 整个程序通过对话框让用户选择组件，实现了从部件列表中移除和添加该组件的功能。
>
