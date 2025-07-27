### 【1720】orient wcs to existing csys 将工作坐标系对齐到现有坐标系

#### 代码

```cpp
    /*HEAD ORIENT_WCS_TO_EXISTING_CSYS CCC UFUN */  
    #include <stdio.h>  
    #include <stdlib.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_csys.h>  
    #include <uf_disp.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据您提供的句子，我将其翻译为中文如下：

UF_print_syslog是V18版本新增的功能，请只回答翻译，不要添加其他无关内容。 */  
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
        UF_CALL(UF_UI_select_with_single_dialog(prompt, "",  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_csys, NULL, &resp,  
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
        tag_t  
            exist_csys;  
        while ((exist_csys = select_a_csys("Select CSYS for WCS")) != NULL_TAG)  
            UF_CALL(UF_CSYS_set_wcs(exist_csys));  
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

> 根据提供的代码，这是一段用于NX软件的二次开发代码，其主要功能是实现世界坐标系(WCS)的设置。
>
> 主要步骤包括：
>
> 1. 包含所需的头文件，包括UF函数库、UI函数库等。
> 2. 定义一个错误报告函数report_error，用于在函数执行出错时打印错误信息。
> 3. 定义一个选择坐标系的面具函数mask_for_csys，用于在用户选择坐标系时过滤只选择坐标系类型。
> 4. 定义一个选择坐标系的函数select_a_csys，用于弹出对话框让用户选择一个坐标系。
> 5. 定义一个设置WCS的函数do_it，循环调用select_a_csys函数让用户选择坐标系，直到用户选择"确定"，然后调用UF_CSYS_set_wcs函数将用户选择的坐标系设置为WCS。
> 6. 定义ufusr函数，这是NX二次开发程序的入口函数，初始化UF环境，调用do_it函数实现功能，然后终止UF环境。
> 7. 定义一个询问卸载函数ufusr_ask_unload，返回立即卸载。
>
> 总体来说，这段代码通过UF函数库实现了让用户选择坐标系并设置为WCS的功能，实现了NX软件坐标系设置的基本需求。
>
