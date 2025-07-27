### 【1036】delete selected datum axis and verify status 删除选定的基准轴并验证状态

#### 代码

```cpp
    /*HEAD DELETE_SELECTED_DATUM_AXIS_AND_VERIFY_STATUS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_obj.h>  
    #include <uf_modl.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档，UF_print_syslog在V18版本中是新增的。 */  
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
    #define WRITE_D(X) (write_integer_to_listing_window(#X, X))  
    #define WRITENZ(X) if (X != 0) (write_integer_to_listing_window(#X, X))  
    static void write_integer_to_listing_window(char *title, int n)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %d\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    #define WRITE(X)   UF_UI_open_listing_window(); UF_UI_write_listing_window(X)  
    /* qq3123197280 */  
    static int mask_for_datum_axes(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_datum_axis_type, 0, 0 };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_datum_axis(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select a datum axis", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_datum_axes, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    static void report_object_status(tag_t object_id)  
    {  
        int  
            status = UF_OBJ_ask_status(object_id);  
        switch(status)  
        {  
            case UF_OBJ_DELETED: WRITE("UF_OBJ_DELETED ");  
                break;  
            case UF_OBJ_TEMPORARY: WRITE("UF_OBJ_TEMPORARY ");  
                break;  
            case UF_OBJ_CONDEMNED: WRITE("UF_OBJ_CONDEMNED ");  
                break;  
            case UF_OBJ_ALIVE: WRITE("UF_OBJ_ALIVE ");  
                break;  
            default:  
                break;  
        }  
        WRITE_D(status);  
    }  
    static void do_it(void)  
    {  
        int  
            status = 0;  
        tag_t  
            datum_axis = NULL_TAG;  
        while ((datum_axis = select_a_datum_axis("Select Datum Axis:")) != NULL_TAG)  
        {  
            UF_CALL( UF_OBJ_delete_object( datum_axis ));  
            status = UF_OBJ_ask_status( datum_axis );  
            report_object_status( status );  
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

> 这段代码是一个NX Open C++ UFUN函数，其主要功能是：
>
> 1. 提供错误报告函数report_error，用于在函数调用失败时输出错误信息。
> 2. 提供宏WRITE_D和WRITENZ，用于将整数值写入列表窗口。
> 3. 提供宏WRITE，用于将字符串写入列表窗口。
> 4. 提供函数mask_for_datum_axes，用于设置选择掩码，仅选择基准轴。
> 5. 提供函数select_a_datum_axis，用于选择一个基准轴并返回其tag。
> 6. 提供函数report_object_status，用于报告一个对象的状态。
> 7. 提供函数do_it，用于循环选择并删除基准轴，然后报告其状态。
> 8. 提供主函数ufusr，用于初始化和调用do_it。
> 9. 提供卸载函数ufusr_ask_unload，用于设置立即卸载。
>
> 该代码的主要逻辑是通过选择并删除基准轴，然后报告其状态，以验证删除操作是否成功。整体结构清晰，使用了UF_OBJ和UF_UI相关函数，是典型的NX二次开发代码。
>
