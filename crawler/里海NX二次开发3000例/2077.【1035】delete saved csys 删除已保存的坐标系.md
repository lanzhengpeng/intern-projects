### 【1035】delete saved csys 删除已保存的坐标系

#### 代码

```cpp
    /*HEAD DELETE_SAVED_CSYS CCC UFUN*/  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_obj.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档，UF_print_syslog 是 V18 的新功能，用于打印系统日志。 */  
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
    static int mask_for_csys(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_coordinate_system_type, 0, 0 };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
     static tag_t select_a_csys(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select a coordinate system",  
            prompt, UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_csys, NULL, &resp,  
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
        char  
            msg[MAX_LINE_SIZE+1];  
        tag_t  
            sel_csys = NULL_TAG;  
        sel_csys=select_a_csys("select saved csys");  
        sprintf(msg, "tag of csys: %d\n", sel_csys);  
        UF_CALL(UF_UI_open_listing_window());  
        UF_CALL(UF_UI_write_listing_window(msg));  
        UF_CALL(UF_OBJ_delete_object(sel_csys));  
        if (UF_OBJ_ask_status(sel_csys) == UF_OBJ_ALIVE)  
        {  
            sprintf(msg, "csys with tag: %d could not be deleted\n", sel_csys);  
            strcat(msg,"check, if it is a datum csys\n");  
            UF_CALL(UF_UI_write_listing_window(msg));  
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

> 这段代码是一个NX二次开发程序，用于删除用户选择的坐标系。
>
> 主要功能包括：
>
> 1. 包含必要的NX Open头文件，定义了UF_CALL宏来简化错误报告。
> 2. 定义了report_error函数，用于打印UF函数调用失败时的错误信息。
> 3. 定义了mask_for_csys函数，设置了选择坐标系时使用的选择掩码。
> 4. 定义了select_a_csys函数，使用选择对话框让用户选择一个坐标系，并返回其tag。
> 5. 定义了do_it函数，用于选择坐标系、打印信息，并调用UF_OBJ_delete_object函数来删除坐标系。如果删除失败，会检查坐标系是否为基准坐标系。
> 6. 定义了ufusr函数，作为程序的入口点，初始化NX Open，调用do_it函数，然后终止NX Open。
> 7. 定义了ufusr_ask_unload函数，返回立即卸载，表示程序运行后可以立即卸载。
>
> 总体来说，该程序实现了选择并删除坐标系的功能，具有一定的错误处理能力，可以作为一个NX二次开发示例程序。
>
